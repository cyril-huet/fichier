#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include "malloc.h"

const size_t HSIZE = sizeof(header);
const size_t YES = 1;
const size_t NO = 0;

void* get_data(header* h) {
    return (void*)((long)h + HSIZE);
}

header* get_header_from_data(void* data) {
    return (header*)((long)data - (long)HSIZE);
}

size_t get_total(header* h) {
    return (long)h->next - (long)h - HSIZE;
}

header* get_sentinel() {
    static header* sentinel = NULL;
    if (sentinel == NULL) {
        sentinel = sbrk(HSIZE);  // Allocate space for sentinel header
        sentinel->prev = NULL;
        sentinel->next = NULL;   // No chunks allocated yet
        sentinel->size = 0;
        sentinel->free = NO;
    }
    return sentinel;
}

void init_heap() {
    get_sentinel();  // Ensure the sentinel is initialized
}

header* expand_heap(header* last_header, size_t size) {
    size_t total_size = size + HSIZE;
    header* current = sbrk(total_size);
    if (current == (void*) -1) {
        return NULL;  // sbrk failed
    }

    // Set the new chunk's fields
    current->prev = last_header;
    current->next = sbrk(0);  // Program break after new allocation
    current->size = size;
    current->free = NO;

    // Update the previous header to link to the new one
    if (last_header != NULL) {
        last_header->next = current;
    } else {
        // If there's no previous header (first allocation), link sentinel
        get_sentinel()->next = current;
    }

    return current;
}

header* find_free_chunk(size_t size) {
    header* current = get_sentinel();
    while (current && current != sbrk(0)) {
        if (current->free == YES && get_total(current) >= size) {
            return current;
        }
        current = current->next;
    }
    return current;  // Return last header if no free chunk is found
}

void* my_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    size_t aligned_size = size;
    while (aligned_size % 8 != 0) {
        aligned_size++;
    }

    header* chunk = find_free_chunk(aligned_size);
    if (chunk == sbrk(0)) {
        chunk = expand_heap(chunk->prev, aligned_size);
    }

    if (chunk == NULL) {
        return NULL;
    }

    chunk->size = size;  // Use the original requested size
    chunk->free = NO;
    return get_data(chunk);
}

void* my_calloc(size_t nmemb, size_t size) {
    size_t total_size;
    if (__builtin_mul_overflow(nmemb, size, &total_size)) {
        return NULL;
    }

    void* allocated_memory = my_malloc(total_size);
    if (allocated_memory != NULL) {
        memset(allocated_memory, 0, total_size);
    }
    return allocated_memory;
}

void my_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    header* h = get_header_from_data(ptr);
    h->free = YES;
}

void* my_realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return my_malloc(size);
    }
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    header* h = get_header_from_data(ptr);
    size_t current_size = get_total(h);

    if (current_size >= size) {
        h->size = size;
        return ptr;
    }

    void* new_memory = my_malloc(size);
    if (new_memory == NULL) {
        return NULL;
    }

    memcpy(new_memory, ptr, current_size);
    my_free(ptr);
    return new_memory;
}

