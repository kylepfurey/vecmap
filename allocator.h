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

bool allocator_new(allocator_t *self, size_t size);
void *allocator_borrow(allocator_t *self, size_t size);
void *allocator_move(allocator_t *self, void *memory, size_t size);
void allocator_return(allocator_t *self, void *memory);
void allocator_free(allocator_t *self);

#ifdef __cplusplus
}
#endif

#endif // ALLOCATOR_H
