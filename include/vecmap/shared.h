// .h
// Reference Counting Pointers
// by Kyle Furey

#ifndef VECMAP_SHARED_H
#define VECMAP_SHARED_H

#include <vecmap/allocator.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void *data;
    size_t shared_count;
    size_t weak_count;
    deleter_t deleter;
    const allocator_t *allocator;
} rc_t;

typedef struct {
    rc_t *control;
} shared_t;

typedef struct {
    rc_t *control;
} weak_t;

#ifdef __cplusplus
extern "C" {
#endif

bool shared_new(shared_t *self, size_t size, deleter_t deleter, const allocator_t *allocator);
void shared_copy(shared_t *self, shared_t *copied);
size_t shared_count_shared(const shared_t *self);
size_t shared_count_weak(const shared_t *self);
void *shared_get(shared_t *self);
const void *shared_get_const(const shared_t *self);
void shared_free(shared_t *self);
void weak_new(weak_t *self, shared_t *shared);
void weak_copy(weak_t *self, weak_t *copied);
size_t weak_count_shared(const weak_t *self);
size_t weak_count_weak(const weak_t *self);
bool weak_upgrade(weak_t *self, shared_t *shared);
void weak_free(weak_t *self);

#ifdef __cplusplus
}
#endif

#endif // VECMAP_SHARED_H
