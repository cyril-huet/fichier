#include <unistd.h>
#include <string.h>
#include <err.h>
#include "malloc.h"

const size_t HSIZE = sizeof(header);
const size_t YES = 1;
const size_t NO = 0;

void* get_data(header* h)
{
    if (h == NULL)
    {
        return NULL;
    }
    return (void*)((char*)h + HSIZE);
}

header* get_header_from_data(void* data)
{
    if (data == NULL)
    {
        return NULL;
    }
    if(HSIZE > 0)
    {
        return (header*)((char*)data - HSIZE);
    }
    else
    {
        return (header*)((char*)data - HSIZE);
    }
}

size_t get_total(header* h)
{
    size_t res = HSIZE + h->size;
    return res;
}

header* get_sentinel()
{
    // TODO:
    // - Define and initialize a static variable to NULL.
    // - If this variable is NULL,
    //   it is the first call to the function.
    //   So, set it to the initial address of the heap
    //   (the initial value of the program break).
    // - If this variable is not NULL,
    //   it contains the initial value of the heap,
    //   which is also the address of the sentinel.
    //   So, just return it.
    static header* res = NULL;
    if(res == NULL)
    {
        res = (header*)sbrk(0);
    }
    return res;
}

void init_heap()
{
    // TODO:
    // - Get the address of the sentinel
    // - Allocate the memory for the sentinel
    //   (obviously do not use malloc).
    // - Initialize the sentinel with the following values:
    //   - prev = NULL
    //   - next = current program break
    //   - size = 0
    //   - free = No
    header* res = get_sentinel();
    sbrk(HSIZE);
    res->prev = NULL;
    res->next = sbrk(0);
    res->size = 0;
    res->free = NO;
}

header* expand_heap(header* last_header, size_t size)
{
    // TODO:
    // - Get the current program break.
    // - Expand the heap
    //   (Allocate the memory for the
    //    new header and the data section.)
    //   If an error occurs, return NULL.
    // - Initialize the next and prev fields of the new header.
    // - Update the previous header.
    // - Return the new header.
    void* current_break = sbrk(0);
    if(sbrk(HSIZE + size) == (void*)-1)
    {
        return NULL;
    }
    header* new_header = (header*)current_break;
    new_header->prev = last_header;
    new_header->next = current_break + HSIZE + size;
    last_header->next = new_header;
    return new_header;

}

header* find_free_chunk(size_t size)
{
    // TODO:
    // - Iterate over the chunks and return
    //   the first chunk (the address of the header)
    //   that is free and large enough to store "size" bytes
    //   (compare the total number of bytes with "size").
    // - If no chunk can be found,
    //   return the address of the last header.
    header* current = get_sentinel();
    while(current->next != NULL)
    {
        if(current->free == YES && get_total(current) >= size)
        {
            return current;
        }
        current = current->next;
    }
    return current;

}

void* my_malloc(size_t size)
{
    // TODO:
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
    if(size == 0)
    {
        return NULL;
    }
    if(size % 8 != 0)
    {
        size = size + (8 - size % 8);
    }
    header* free_chunk = find_free_chunk(size);
    if(free_chunk->next == NULL)
    {
        free_chunk = expand_heap(free_chunk, size);
    }
    free_chunk->size = size;
    free_chunk->free = NO;
    return get_data(free_chunk);
    
}

void* my_calloc(size_t nmemb, size_t size)
{
    // TODO:
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
    // TODO:
    // - If "ptr" is NULL, no operation is performed.
    // - Get the header.
    // - Mark the chunk as free.
}

void* my_realloc(void* ptr, size_t size)
{
    // TODO:
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
