// .c
// Arena Allocator
// by Kyle Furey

#include "allocator.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

bool allocator_contains(const allocator_t *self, const void *ptr) {
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
