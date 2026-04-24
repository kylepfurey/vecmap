// .h
// Doubly Linked List
// by Kyle Furey

#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct node {
    void *data;
    struct node *previous;
    struct node *next;
    void *owner;
} node_t;

typedef struct {
    size_t element_size;
    size_t count;
    node_t *head;
    node_t *tail;
    void (*deleter)(void *);
} list_t;

#ifdef __cplusplus
extern "C" {
#endif

void *node_data(node_t *self);
const void *node_data_const(const node_t *self);
node_t *node_previous(node_t *self);
const node_t *node_previous_const(const node_t *self);
node_t *node_next(node_t *self);
const node_t *node_next_const(const node_t *self);

void list_new(list_t *self, size_t element_size, void (*deleter)(void *));
bool list_copy(list_t *self, const list_t *copied);
size_t list_count(const list_t *self);
bool list_empty(const list_t *self);
node_t *list_front(list_t *self);
const node_t *list_front_const(const list_t *self);
node_t *list_back(list_t *self);
const node_t *list_back_const(const list_t *self);
node_t *list_get(list_t *self, size_t index);
const node_t *list_get_const(const list_t *self, size_t index);
bool list_contains(const list_t *self, const node_t *node);
void *list_insert_before(list_t *self, node_t *next);
void *list_insert_after(list_t *self, node_t *previous);
void *list_insert_where(list_t *self, bool (*predicate)(const node_t *next, void *data), void *data);
void list_erase(list_t *self, node_t *node);
void *list_push_front(list_t *self);
void *list_push_back(list_t *self);
void list_pop_front(list_t *self);
void list_pop_back(list_t *self);
void list_clear(list_t *self);
void list_foreach(list_t *self, void (*action)(void *element, void *data), void *data);
void list_foreach_const(const list_t *self, void (*action)(const void *element, void *data), void *data);
void list_free(list_t *self);

#ifdef __cplusplus
}
#endif

#endif // LIST_H
