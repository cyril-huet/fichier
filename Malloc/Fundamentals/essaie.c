#include <unistd.h>
#include <string.h>
#include <err.h>
#include "malloc.h"

const size_t HSIZE = sizeof(header);
const size_t YES = 1;
const size_t NO = 0;

void* get_data(header* h)
{
    return (void*)((char*)h + HSIZE);
}

header* get_header_from_data(void* data)
{
    return (header*)((char*)data - HSIZE);
}

size_t get_total(header* h)
{
    return h->size + HSIZE;
}

header* get_sentinel()
{
    static header* sentinel = NULL;
    if (!sentinel)
        sentinel = sbrk(0); // Set initial program break as the sentinel
    return sentinel;
}

void init_heap()
{
    header* sentinel = get_sentinel();
    sbrk(HSIZE); // Allocate space for the sentinel
    sentinel->prev = NULL;
    sentinel->next = NULL;
    sentinel->size = 0;
    sentinel->free = NO;
}

header* expand_heap(header* last_header, size_t size)
{
    header* current = sbrk(0); // Current program break, start of new block
    size_t total_size = size + HSIZE;

    if (sbrk(total_size) == (void*)-1) // Expand heap by total size (header + data)
        return NULL; // Allocation failed

    // Initialize the new header
    current->prev = last_header;
    current->next = NULL;
    current->size = size;
    current->free = NO;

    // Update the last header's next pointer if it exists
    if (last_header != NULL)
        last_header->next = current;

    return current;
}

header* find_free_chunk(size_t size)
{
    header* current = get_sentinel()->next; // Start after the sentinel

    while (current != NULL)
    {
        if (current->free == YES && current->size >= size)
            return current; // Found a free chunk large enough
        current = current->next;
    }

    return get_sentinel()->prev; // Return the last header if no suitable free chunk
}

void* my_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    // Align size to the next multiple of 8 if necessary
    if (size % 8 != 0)
        size = (size + 7) & ~7;

    header* chunk = find_free_chunk(size);

    // If no free chunk found, expand the heap
    if (chunk == get_sentinel()->prev)
        chunk = expand_heap(chunk, size);

    if (chunk == NULL)
        return NULL; // Allocation failed

    chunk->size = size; // Set actual requested size
    chunk->free = NO; // Mark as used

    return get_data(chunk); // Return pointer to data section
}

void* my_calloc(size_t nmemb, size_t size)
{
    size_t total_size;
    if (__builtin_mul_overflow(nmemb, size, &total_size))
        return NULL; // Overflow detected

    void* ptr = my_malloc(total_size);
    if (ptr)
        memset(ptr, 0, total_size); // Initialize memory to zero
    return ptr;
}

void my_free(void* ptr)
{
    if (!ptr)
        return;

    header* h = get_header_from_data(ptr);
    h->free = YES;
}

void* my_realloc(void* ptr, size_t size)
{
    if (!ptr)
        return my_malloc(size);

    if (size == 0)
    {
        my_free(ptr);
        return NULL;
    }

    header* h = get_header_from_data(ptr);
    if (h->size >= size)
    {
        h->size = size;
        return ptr;
    }

    // Allocate a new block and copy data
    void* new_ptr = my_malloc(size);
    if (!new_ptr)
        return NULL;

    memcpy(new_ptr, ptr, h->size); // Copy old data to new block
    my_free(ptr);

    return new_ptr;
}
