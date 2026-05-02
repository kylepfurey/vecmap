// .c
// Arena Allocator
// by Kyle Furey

#include "allocator.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

bool allocator_contains(const allocator_t *self, const void *ptr) {
    assert(self != NULL);
    assert(self->buffer != NULL);
    assert(self->size > 0);
    assert(ptr != NULL);
    return (ptr >= ((const block_t *) self->buffer + 1)) && (ptr < ((const uint8_t *) self->buffer + self->size));
}

void allocator_new(allocator_t *self, void *buffer, size_t size) {
    assert(self != NULL);
    assert(buffer != NULL);
    assert(size > sizeof(block_t));
    memset(buffer, 0, size);
    block_t *free = (block_t *) buffer;
    free->size = size - sizeof(block_t);
    *self = (allocator_t){
        buffer,
        size,
        free,
    };
}

void *allocator_malloc(allocator_t *self, size_t size) {
    assert(self != NULL);
    assert(self->buffer != NULL);
    assert(self->size > 0);
    assert(size > 0);
    size = (size + ALLOCATOR_ALIGN - 1) & ~(ALLOCATOR_ALIGN - 1);
    block_t *block = self->free;
    if (block == NULL || size > block->size) {
        return NULL;
    }
    // TODO
    return block + 1;
}

void *allocator_calloc(allocator_t *self, size_t n, size_t size) {
    assert(self != NULL);
    assert(self->buffer != NULL);
    assert(self->size > 0);
    assert(n > 0);
    assert(size > 0);
    if (n > SIZE_MAX / size) {
        return NULL;
    }
    return allocator_malloc(self, n * size);
}

void *allocator_realloc(allocator_t *self, void *ptr, size_t size) {
    assert(self != NULL);
    assert(self->buffer != NULL);
    assert(self->size > 0);
    assert(ptr == NULL || allocator_contains(self, ptr));
    assert(size > 0);
    if (ptr == NULL) {
        return allocator_malloc(self, size);
    }
    size = (size + ALLOCATOR_ALIGN - 1) & ~(ALLOCATOR_ALIGN - 1);
    block_t *block = (block_t *) ptr - 1;
    if (block->size > size) {
        // TODO
    } else if (block->size < size) {
        // TODO
    }
    return ptr;
}

void allocator_free(allocator_t *self, void *ptr) {
    assert(self != NULL);
    assert(self->buffer != NULL);
    assert(self->size > 0);
    assert(ptr == NULL || allocator_contains(self, ptr));
    if (ptr == NULL) {
        return;
    }
    block_t *block = (block_t *) ptr - 1;
    // TODO
}

void allocator_clear(allocator_t *self) {
    assert(self != NULL);
    assert(self->buffer != NULL);
    assert(self->size > 0);
    memset(self->buffer, 0, self->size);
    block_t *free = (block_t *) self->buffer;
    free->size = self->size - sizeof(block_t);
    self->free = free;
}
