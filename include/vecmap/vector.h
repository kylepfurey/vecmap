// .h
// Dynamic Array
// by Kyle Furey

#ifndef VECMAP_VECTOR_H
#define VECMAP_VECTOR_H

#include <vecmap/allocator.h>
#include <stddef.h>
#include <stdbool.h>

#define VECTOR_DEFAULT_CAP 8
#define VECTOR_EXPANSION 2

typedef struct {
    size_t element_size;
    size_t count;
    size_t capacity;
    void *array;
    deleter_t deleter;
    const allocator_t *allocator;
} vector_t;

#ifdef __cplusplus
extern "C" {
#endif

bool vector_new(vector_t *self, size_t element_size, deleter_t deleter, const allocator_t *allocator);
bool vector_copy(vector_t *self, const vector_t *copied);
size_t vector_count(const vector_t *self);
size_t vector_capacity(const vector_t *self);
bool vector_empty(const vector_t *self);
void *vector_get(vector_t *self, size_t index);
const void *vector_get_const(const vector_t *self, size_t index);
void *vector_array(vector_t *self);
const void *vector_array_const(const vector_t *self);
bool vector_resize(vector_t *self, size_t capacity);
void *vector_insert(vector_t *self, size_t index);
void vector_erase(vector_t *self, size_t index);
void *vector_push(vector_t *self);
void vector_pop(vector_t *self);
void vector_clear(vector_t *self);
bool vector_foreach(vector_t *self, bool (*action)(void *element, void *data), void *data);
bool vector_foreach_const(const vector_t *self, bool (*action)(const void *element, void *data), void *data);
void vector_free(vector_t *self);

#ifdef __cplusplus
}
#endif

#endif // VECMAP_VECTOR_H
