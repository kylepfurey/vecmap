// .c
// Allocator Interface
// by Kyle Furey

#include <vecmap/allocator.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void *std_malloc(void *context, size_t size) {
    (void) context;
    return malloc(size);
}

static void *std_calloc(void *context, size_t n, size_t size) {
    (void) context;
    return calloc(n, size);
}

static void *std_realloc(void *context, void *ptr, size_t size) {
    (void) context;
    return realloc(ptr, size);
}

static void std_free(void *context, void *ptr) {
    (void) context;
    free(ptr);
}

const allocator_t stdalloc = {
    NULL,
    std_malloc,
    std_calloc,
    std_realloc,
    std_free,
};

void *allocator_malloc(const allocator_t *self, size_t size) {
    assert(self != NULL);
    assert(self->malloc != NULL);
    return self->malloc(self->context, size);
}

void *allocator_calloc(const allocator_t *self, size_t n, size_t size) {
    assert(self != NULL);
    assert(self->calloc != NULL);
    return self->calloc(self->context, n, size);
}

void *allocator_realloc(const allocator_t *self, void *ptr, size_t size) {
    assert(self != NULL);
    assert(self->realloc != NULL);
    return self->realloc(self->context, ptr, size);
}

void allocator_free(const allocator_t *self, void *ptr) {
    assert(self != NULL);
    assert(self->free != NULL);
    self->free(self->context, ptr);
}

void *allocator_clone(const allocator_t *self, const void *ptr, size_t size) {
    assert(self != NULL);
    assert(self->malloc != NULL);
    assert(ptr != NULL || size == 0);
    void *data = self->malloc(self->context, size);
    if (data == NULL) {
        return NULL;
    }
    memcpy(data, ptr, size);
    return data;
}
