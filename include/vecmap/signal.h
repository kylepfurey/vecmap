// .h
// Multicast Signal
// by Kyle Furey

#ifndef VECMAP_SIGNAL_H
#define VECMAP_SIGNAL_H

#include <vecmap/allocator.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define SIGNAL_DEFAULT_CAP 8
#define SIGNAL_EXPANSION 2
#define NULL_HANDLE SIZE_MAX

typedef void (*action_t)(void *element, void *data);

typedef struct {
    void *element;
    action_t action;
} binding_t;

typedef size_t handle_t;

typedef struct {
    size_t count;
    size_t capacity;
    handle_t next;
    binding_t *bindings;
    const allocator_t *allocator;
} signal_t;

#ifdef __cplusplus
extern "C" {
#endif

bool signal_new(signal_t *self, const allocator_t *allocator);
bool signal_copy(signal_t *self, const signal_t *copied);
size_t signal_count(const signal_t *self);
size_t signal_capacity(const signal_t *self);
bool signal_resize(signal_t *self, size_t capacity);
handle_t signal_bind(signal_t *self, void *element, action_t action);
void signal_unbind(signal_t *self, handle_t handle);
bool signal_invoke(signal_t *self, void *data);
void signal_clear(signal_t *self);
void signal_free(signal_t *self);

#ifdef __cplusplus
}
#endif

#endif // VECMAP_SIGNAL_H
