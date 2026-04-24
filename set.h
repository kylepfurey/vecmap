// .h
// Binary Tree
// by Kyle Furey

#ifndef SET_H
#define SET_H

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
} tree_t;

typedef struct {
    size_t element_size;
    size_t count;
    tree_t *root;
    comparison_t (*comparer)(const void *left, const void *right);
    void (*deleter)(void *);
} set_t;

#ifdef __cplusplus
extern "C" {
#endif

void set_new(set_t *self, size_t element_size, comparison_t (*comparer)(const void *left, const void *right), void (*deleter)(void *));
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
void set_foreach(const set_t *self, void (*action)(const void *element, void *data), void *data);
void set_free(set_t *self);

#ifdef __cplusplus
}
#endif

#endif // SET_H
