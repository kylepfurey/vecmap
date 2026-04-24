// .h
// Stack Allocator
// by Kyle Furey

#ifndef VECMAP_STACK_H
#define VECMAP_STACK_H

#include <vecmap/allocator.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define ALIGN_UP(size, alignment) ((((size) + (alignment) - 1) / (alignment)) * (alignment))
#define IS_ALIGNED(size, alignment) (((size) % (alignment)) == 0)

typedef struct {
    size_t offset;
    size_t capacity;
    size_t alignment;
    uint8_t *buffer;
    const allocator_t *allocator;
} stack_t;

#ifdef __cplusplus
extern "C" {
#endif

bool stack_new(stack_t *self, size_t capacity, size_t alignment, const allocator_t *allocator);
bool stack_copy(stack_t *self, const stack_t *copied);
size_t stack_offset(const stack_t *self);
size_t stack_capacity(const stack_t *self);
bool stack_empty(const stack_t *self);
void *stack_bottom(stack_t *self, size_t offset);
const void *stack_bottom_const(const stack_t *self, size_t offset);
void *stack_top(stack_t *self, size_t offset);
const void *stack_top_const(const stack_t *self, size_t offset);
void *stack_push(stack_t *self, size_t size);
void stack_pop(stack_t *self, size_t size);
void stack_clear(stack_t *self);
void stack_free(stack_t *self);

#ifdef __cplusplus
}
#endif

#endif // VECMAP_STACK_H
