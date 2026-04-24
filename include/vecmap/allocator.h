// .h
// Allocator Interface
// by Kyle Furey

#ifndef VECMAP_ALLOCATOR_H
#define VECMAP_ALLOCATOR_H

#include <stddef.h>

typedef struct {
    void *context;
    void *(*malloc)(void *context, size_t size);
    void *(*calloc)(void *context, size_t n, size_t size);
    void *(*realloc)(void *context, void *ptr, size_t size);
    void (*free)(void *context, void *ptr);
} allocator_t;

typedef void (*deleter_t)(void *element);

#ifdef __cplusplus
extern "C" {
#endif

extern const allocator_t stdalloc;

void *allocator_malloc(const allocator_t *self, size_t size);
void *allocator_calloc(const allocator_t *self, size_t n, size_t size);
void *allocator_realloc(const allocator_t *self, void *ptr, size_t size);
void allocator_free(const allocator_t *self, void *ptr);
void *allocator_clone(const allocator_t *self, const void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif // VECMAP_ALLOCATOR_H
