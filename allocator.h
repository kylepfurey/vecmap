// .h
// Arena Allocator
// by Kyle Furey

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <stdbool.h>

#define ALLOCATOR_ALIGNMENT (sizeof(void *))

typedef struct {
} allocator_t;

#ifdef __cplusplus
extern "C" {
#endif

void allocator_new(allocator_t *self, void *buffer, size_t size);
void *allocator_malloc(allocator_t *self, size_t size);
void *allocator_calloc(allocator_t *self, size_t n, size_t size);
void *allocator_realloc(allocator_t *self, void *ptr, size_t size);
void allocator_free(allocator_t *self, void *ptr);

#ifdef __cplusplus
}
#endif

#endif // ALLOCATOR_H
