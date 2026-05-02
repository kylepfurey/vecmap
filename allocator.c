// .c
// Arena Allocator
// by Kyle Furey

#include "allocator.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

bool allocator_contains(const allocator *self, const void *ptr) {
    assert(self != NULL);
    assert(self->buffer != NULL);
    assert(self->size > 0);
    assert(ptr != NULL);
    return (ptr >= self->buffer) && (ptr < (((const uint8_t *) self->buffer) + self->size));
}

void allocator_new(allocator_t *self, void *buffer, size_t size) {
}

void *allocator_malloc(allocator_t *self, size_t size) {
}

void *allocator_calloc(allocator_t *self, size_t n, size_t size) {
}

void *allocator_realloc(allocator_t *self, void *ptr, size_t size) {
}

void allocator_free(allocator_t *self, void *ptr) {
}

void allocator_clear(allocator_t *self) {
}
