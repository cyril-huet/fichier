#include <unistd.h>
#include <string.h>
#include <err.h>
#include "malloc.h"

const size_t HSIZE = sizeof(header);
const size_t YES = 1;
const size_t NO = 0;

void* get_data(header* h)
{
    // TODO
    return (void*)((char*)h + HSIZE);
}

header* get_header_from_data(void* data)
{
    // TODO
    return (header*)((char*)data - HSIZE);
}

size_t get_total(header* h)
{
    // TODO
     return HSIZE + h->size;
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
    static header* sentinel = NULL;
    if (sentinel == NULL) {
        sentinel = sbrk(HSIZE);
        if (sentinel == (void*)-1) {
            err(1, "Failed to initialize sentinel");
        }
        sentinel->prev = NULL;
        sentinel->next = sbrk(0);
        sentinel->size = 0;
        sentinel->free = NO;
    }
    return sentinel;
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
    get_sentinel();
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
     header* new_header = sbrk(HSIZE + size);
    if (new_header == (void*)-1) {
        return NULL;
    }

    new_header->prev = last_header;
    new_header->next = sbrk(0);
    new_header->size = size;
    new_header->free = NO;

    if (last_header != NULL) {
        last_header->next = new_header;
    }

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
    while (current != NULL) {
        if (current->free == YES && current->size >= size) {
            return current;
        }
        if (current->next == sbrk(0)) {
            break;
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
    if (size == 0) {
        return NULL;
    }

    if (size % 8 != 0) {
        size += 8 - (size % 8);
    }

    header* chunk = find_free_chunk(size);
    if (chunk == NULL || chunk->size < size) {
        chunk = expand_heap(chunk, size);
        if (chunk == NULL) {
            return NULL;
        }
    }

    if (chunk->size > size + HSIZE) {
        header* new_chunk = (header*)((char*)chunk + HSIZE + size);
        new_chunk->size = chunk->size - size - HSIZE;
        new_chunk->prev = chunk;
        new_chunk->next = chunk->next;
        new_chunk->free = YES;

        chunk->next = new_chunk;
        chunk->size = size;
    }

    chunk->free = NO;
    return get_data(chunk);
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
    size_t total_size;
    if (__builtin_mul_overflow(nmemb, size, &total_size)) {
        return NULL;
    }

    void* ptr = my_malloc(total_size);
    if (ptr == NULL) {
        return NULL;
    }

    memset(ptr, 0, total_size);
    return ptr;
}

void my_free(void* ptr)
{
    // TODO:
    // - If "ptr" is NULL, no operation is performed.
    // - Get the header.
    // - Mark the chunk as free.
    if (ptr == NULL) {
        return;
    }

    header* chunk = get_header_from_data(ptr);
    chunk->free = YES;

    if (chunk->next != sbrk(0) && chunk->next->free == YES) {
        chunk->size += HSIZE + chunk->next->size;
        chunk->next = chunk->next->next;
        if (chunk->next) {
            chunk->next->prev = chunk;
        }
    }

    if (chunk->prev && chunk->prev->free == YES) {
        chunk->prev->size += HSIZE + chunk->size;
        chunk->prev->next = chunk->next;
        if (chunk->next) {
            chunk->next->prev = chunk->prev;
        }
        chunk = chunk->prev;
    }

    if (chunk->next == sbrk(0)) {
        if (sbrk(-get_total(chunk)) == (void*)-1) {
            err(1, "sbrk failed to release memory");
        }
        if (chunk->prev) {
            chunk->prev->next = sbrk(0);
        }
    }
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
    if (ptr == NULL) {
        return my_malloc(size);
    }

    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    header* old_chunk = get_header_from_data(ptr);
    if (old_chunk->size >= size) {
        old_chunk->size = size;
        return ptr;
    }

    void* new_ptr = my_malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }

    memcpy(new_ptr, ptr, old_chunk->size);
    my_free(ptr);
    return new_ptr;
}