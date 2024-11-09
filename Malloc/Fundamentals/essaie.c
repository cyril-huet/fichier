#include <unistd.h>
#include <string.h>
#include <err.h>
#include "malloc.h"

const size_t HSIZE = sizeof(header);
const size_t YES = 1;
const size_t NO = 0;

void* get_data(header* h)
{
	void* rtn = (void*)((long)h+HSIZE);
	return rtn;
}

header* get_header_from_data(void* data)
{
	header* rtn = (header*)((long)data - (long)HSIZE);
	return rtn;
}

size_t get_total(header* h)
{
	return (long)h->next - (long)h - HSIZE;
}

header* get_sentinel()
{
    static header* rtn = NULL;

    if (rtn == NULL)
	    rtn = sbrk(0);

    return rtn;

    // - Define and initialize a static variable to NULL.
    // - If this variable is NULL,
    //   it is the first call to the function.
    //   So, set it to the initial address of the heap
    //   (the initial value of the program break).
    // - If this variable is not NULL,
    //   it contains the initial value of the heap,
    //   which is also the address of the sentinel.
    //   So, just return it.
}

void init_heap()
{
	header* sentinel = get_sentinel();
	sbrk(HSIZE);
	sentinel->prev = NULL;
	sentinel->next = sbrk(0);
	sentinel->size = 0;
	sentinel->free = NO;


    // - Get the address of the sentinel
    // - Allocate the memory for the sentinel
    //   (obviously do not use malloc).
    // - Initialize the sentinel with the following values:
    //   - prev = NULL
    //   - next = current program break
    //   - size = 0
    //   - free = No
}

header* expand_heap(header* last_header, size_t size)
{
    size_t total_size = size + HSIZE;
    header* new_header = (header*)sbrk(total_size); // Expands the heap
    if (new_header == (void*)-1)
        return NULL; // Allocation failed

    new_header->prev = last_header;
    new_header->next = NULL;
    new_header->size = size;
    new_header->free = NO;

    if (last_header)
        last_header->next = new_header;

    return new_header;


    // - Get the current program break.
    // - Expand the heap
    //   (Allocate the memory for the
    //    new header and the data section.)
    //   If an error occurs, return NULL.
    // - Initialize the next and prev fields of the new header.
    // - Update the previous header.
    // - Return the new header.
}

header* find_free_chunk(size_t size)
{
	header* current = get_sentinel();

	if(current == NULL)
		return NULL;

	while(current != sbrk(0))
	{
		if(current->free == YES && get_total(current) >= size)
			return current;
		else
		{
			current = current->next;
		}
	}

	return current;

    // - Iterate over the chunks and return
    //   the first chunk (the address of the header)
    //   that is free and large enough to store "size" bytes
    //   (compare the total number of bytes with "size").
    // - If no chunk can be found,
    //   return the address of the last header.
}

void* my_malloc(size_t size)
{
	if(size == 0)
		return NULL;

	size_t taille = size;

	while(size % 8 != 0)
		size += 1;

	header* chunk = find_free_chunk(size);

	if(chunk == sbrk(0))
		chunk = expand_heap(chunk->prev, size);

	if(chunk == NULL)
		return NULL;

	chunk->size = taille;
	chunk->free = NO;

	return get_data(chunk);


    // - If "size" is 0, return NULL.
    // - "size" must be a multiple of eight.
    //   If it is not, work out the next multiple of eight
    //   and use this value to find a free chunk large enough.
    // - Get the first free chunk large enough
    //   (or the last chunk).
    // - If no chunk can be found, expand the heap.
    // - Do not forget to update the "size" and the "free" fields.
    //   Be careful, the "size" field of the header
    //   must be initialized with the "size" parameter
    //   (not with the next multiple of eight).
    // - Return the address of the data section.
}

void* my_calloc(size_t nmemb, size_t size)
{
	size_t mul;
	if(__builtin_mul_overflow(nmemb, size, &mul))
		return NULL;

    void* rtn = my_malloc(nmemb * size);

    if(rtn == NULL)
	    return NULL;

    memset(rtn, 0, nmemb * size);

    return rtn;

    // - Work out the size in bytes.
    //   To do so, you have to multiply "nmemb" and "size".
    //   Use __builtin_mul_overflow() to detect an overflow.
    //   If an overflow occurs, return NULL.
    // - Call my_malloc() and return NULL if an error occurs.
    // - Fill the memory space with zeros (use memset(3)).
    // - Return the address of the data section.
}

void my_free(void* ptr)
{
	if(ptr == NULL)
		return;

	header* h = get_header_from_data(ptr);
	h->free = YES;

    // - If "ptr" is NULL, no operation is performed.
    // - Get the header.
    // - Mark the chunk as free.
}

void* my_realloc(void* ptr, size_t size)
{
    if(ptr == NULL)
	    return malloc(size);
    if(size == 0 && ptr != NULL)
    {
	    my_free(ptr);
	    return NULL;
    }

    header* h = get_header_from_data(ptr);
    size_t current = get_total(h);

    if(current >= size)
    {
	    h->size = size;
	    return ptr;
    }

    void* new = my_malloc(size);
    if(new == NULL)
	    return NULL;

    memcpy(new, ptr, current);

    my_free(ptr);

    return new;


    // - If "ptr" is NULL, realloc() is equivalent to malloc().
    // - If "size" is 0 (and "ptr" is not NULL),
    //   realloc() is equivalent to free() and return NULL.
    // - Get the header.
    // - If this chunk is large enough, just update the size.
    // - Otherwise:
    //   - Allocate a new memory space with my_malloc()
    //     (return NULL if an error occurs).
    //   - Copy the bytes from the previous memory
    //     space to the new one (use memcpy(3)).
    //   - Free the previous memory space.
    //   - Return the address of the new memory space.
}