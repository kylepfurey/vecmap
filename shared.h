// .h
// Reference Counting
// by Kyle Furey

#ifndef SHARED_H
#define SHARED_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void *data;
    size_t count;
    size_t weak_count;
    void (*deleter)(void *);
} control_t;

typedef struct {
    control_t *control;
} shared_t;

typedef struct {
    control_t *control;
} weak_t;

#ifdef __cplusplus
extern "C" {
#endif

bool shared_new(shared_t *self, size_t size, void (*deleter)(void *));
void shared_copy(shared_t *self, shared_t *copied);
size_t shared_count(const shared_t *self);
size_t shared_weak_count(const shared_t *self);
void *shared_get(shared_t *self);
const void *shared_get_const(const shared_t *self);
bool shared_reset(shared_t *self, size_t size, void (*deleter)(void *));
void shared_free(shared_t *self);

void weak_new(weak_t *self, shared_t *shared);
void weak_copy(weak_t *self, weak_t *copied);
bool weak_upgrade(weak_t *self, shared_t *shared);
void weak_free(weak_t *self);

#ifdef __cplusplus
}
#endif

#endif // SHARED_H
