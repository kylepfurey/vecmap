// .h
// Binary Tree Set
// by Kyle Furey

#ifndef VECMAP_SET_H
#define VECMAP_SET_H

#include <vecmap/allocator.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum {
    COMPARISON_LESS_THAN = -1,
    COMPARISON_EQUAL_TO = 0,
    COMPARISON_GREATER_THAN = 1,
} comparison_t;

typedef struct tree {
    void *data;
    struct tree *left;
    struct tree *right;
    void *owner;
} tree_t;

typedef comparison_t (*comparer_t)(const void *left, const void *right);

typedef struct {
    size_t element_size;
    size_t count;
    tree_t *root;
    comparer_t comparer;
    deleter_t deleter;
    const allocator_t *allocator;
} set_t;

#ifdef __cplusplus
extern "C" {
#endif

bool set_new(set_t *self, size_t element_size, comparer_t comparer, deleter_t deleter, const allocator_t *allocator);
bool set_copy(set_t *self, const set_t *copied);
size_t set_count(const set_t *self);
bool set_empty(const set_t *self);
const void *set_least(const set_t *self);
const void *set_greatest(const set_t *self);
const void *set_find(const set_t *self, const void *element);
bool set_contains(const set_t *self, const void *element);
const void *set_insert(set_t *self, const void *element);
bool set_erase(set_t *self, const void *element);
void set_clear(set_t *self);
bool set_foreach(const set_t *self, bool (*action)(const void *element, void *data), void *data);
void set_free(set_t *self);

#ifdef __cplusplus
}
#endif

#endif // VECMAP_SET_H
