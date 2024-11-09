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
    {
        // Initialize the sentinel when it's the first call.
        sentinel = sbrk(0); // Current program break
    }
    return sentinel;
}

void init_heap()
{
    header* sentinel = get_sentinel();
    // The current break represents the start of the heap.
    sentinel->prev = NULL;
    sentinel->next = NULL;
    sentinel->size = 0;
    sentinel->free = NO; // The sentinel is not free.
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
}


header* find_free_chunk(size_t size)
{
    header* current = get_sentinel()->next;
    while (current)
    {
        if (current->free == YES && current->size >= size)
            return current;
        current = current->next;
    }
    return NULL;
}

void* my_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    // Align the size to a multiple of 8.
    if (size % 8 != 0)
        size = (size + 7) & ~7;

    header* free_chunk = find_free_chunk(size);
    if (!free_chunk)
    {
        header* last_header = get_sentinel()->prev;
        free_chunk = expand_heap(last_header, size);
        if (!free_chunk)
            return NULL;
    }

    free_chunk->free = NO;
    return get_data(free_chunk);
}


void* my_calloc(size_t nmemb, size_t size)
{
    size_t total_size;
    if (__builtin_mul_overflow(nmemb, size, &total_size))
        return NULL; // Overflow occurred

    void* ptr = my_malloc(total_size);
    if (ptr)
        memset(ptr, 0, total_size);

    return ptr;
}


void my_free(void* ptr)
{
    if (!ptr)
        return;

    header* h = get_header_from_data(ptr);
    h->free = YES;

    // Optional: Coalesce free chunks here.
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

    void* new_ptr = my_malloc(size);
    if (!new_ptr)
        return NULL;

    memcpy(new_ptr, ptr, h->size);
    my_free(ptr);
    return new_ptr;
}

