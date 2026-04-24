// .c
// Stack Allocator
// by Kyle Furey

#include <vecmap/stack.h>
#include <string.h>
#include <assert.h>

bool stack_new(stack_t *self, size_t capacity, size_t alignment, const allocator_t *allocator) {
    assert(self != NULL);
    assert(capacity > 0);
    assert(alignment > 0);
    allocator = allocator == NULL ? &stdalloc : allocator;
    capacity = ALIGN_UP(capacity, alignment);
    uint8_t *buffer = (uint8_t *) allocator_calloc(allocator, 1, capacity);
    if (buffer == NULL) {
        return false;
    }
    *self = (stack_t){
        0,
        capacity,
        alignment,
        buffer,
        allocator,
    };
    return true;
}

bool stack_copy(stack_t *self, const stack_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(self != copied);
    assert(copied->capacity > 0);
    assert(copied->alignment > 0);
    assert(copied->offset <= copied->capacity);
    assert(IS_ALIGNED(copied->offset, copied->alignment));
    assert(IS_ALIGNED(copied->capacity, copied->alignment));
    assert(copied->buffer != NULL);
    uint8_t *buffer = (uint8_t *) allocator_malloc(copied->allocator, copied->capacity);
    if (buffer == NULL) {
        return false;
    }
    memcpy(buffer, copied->buffer, copied->capacity);
    *self = (stack_t){
        copied->offset,
        copied->capacity,
        copied->alignment,
        buffer,
        copied->allocator,
    };
    return true;
}

size_t stack_offset(const stack_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    return self->offset;
}

size_t stack_capacity(const stack_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    return self->capacity;
}

bool stack_empty(const stack_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    return self->offset == 0;
}

void *stack_bottom(stack_t *self, size_t offset) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    assert(offset <= self->offset);
    return (void *) (self->buffer + offset);
}

const void *stack_bottom_const(const stack_t *self, size_t offset) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    assert(offset <= self->offset);
    return (const void *) (self->buffer + offset);
}

void *stack_top(stack_t *self, size_t offset) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    assert(offset > 0);
    assert(offset <= self->offset);
    return (void *) (self->buffer + self->offset - offset);
}

const void *stack_top_const(const stack_t *self, size_t offset) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    assert(offset > 0);
    assert(offset <= self->offset);
    return (const void *) (self->buffer + self->offset - offset);
}

void *stack_push(stack_t *self, size_t size) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    assert(size > 0);
    size = ALIGN_UP(size, self->alignment);
    if (size > self->capacity - self->offset) {
        return NULL;
    }
    void *data = (void *) (self->buffer + self->offset);
    self->offset += size;
    return data;
}

void stack_pop(stack_t *self, size_t size) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    assert(size > 0);
    size = ALIGN_UP(size, self->alignment);
    assert(size <= self->offset);
    self->offset -= size;
    memset(self->buffer + self->offset, 0, size);
}

void stack_clear(stack_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    memset(self->buffer, 0, self->offset);
    self->offset = 0;
}

void stack_free(stack_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->alignment > 0);
    assert(self->offset <= self->capacity);
    assert(IS_ALIGNED(self->offset, self->alignment));
    assert(IS_ALIGNED(self->capacity, self->alignment));
    assert(self->buffer != NULL);
    allocator_free(self->allocator, self->buffer);
    *self = (stack_t){0};
}
