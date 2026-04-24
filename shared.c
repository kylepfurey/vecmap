// .c
// Reference Counting
// by Kyle Furey

#include "shared.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

bool shared_new(shared_t *self, size_t size, void (*deleter)(void *)) {
    assert(self != NULL);
    assert(size > 0);
    rc_t *control = (rc_t *) malloc(sizeof(rc_t));
    if (control == NULL) {
        return false;
    }
    void *data = malloc(size);
    if (data == NULL) {
        free(control);
        return false;
    }
    memset(data, 0, size);
    *control = (rc_t) {
        data,
        1,
        0,
        deleter,
    };
    *self = (shared_t) {
        control,
    };
    return true;
}

void shared_copy(shared_t *self, shared_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(copied->control != NULL);
    assert(copied->control->data != NULL);
    assert(copied->control->count > 0);
    ++copied->control->count;
    *self = *copied;
}

size_t shared_count(const shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->count > 0);
    return self->control->count;
}

size_t shared_weak_count(const shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->count > 0);
    return self->control->weak_count;
}

void *shared_get(shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->count > 0);
    return self->control->data;
}

const void *shared_get_const(const shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->count > 0);
    return self->control->data;
}

bool shared_reset(shared_t *self, size_t size, void (*deleter)(void *)) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->count > 0);
    assert(size > 0);
    void *data = malloc(size);
    if (data == NULL) {
        return false;
    }
    memset(data, 0, size);
    if (self->control->deleter != NULL) {
        self->control->deleter(self->control->data);
    }
    free(self->control->data);
    self->control->data = data;
    self->control->deleter = deleter;
    return true;
}

void shared_free(shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->count > 0);
    --self->control->count;
    if (self->control->count == 0) {
        if (self->control->deleter != NULL) {
            self->control->deleter(self->control->data);
        }
        free(self->control->data);
        self->control->data = NULL;
        if (self->control->weak_count == 0) {
            free(self->control);
            self->control = NULL;
        }
    }
    *self = (shared_t){0};
}

void weak_new(weak_t *self, shared_t *shared) {
    assert(self != NULL);
    assert(shared != NULL);
    assert(shared->control != NULL);
    assert(shared->control->data != NULL);
    assert(shared->control->count > 0);
    ++shared->control->weak_count;
    *self = (weak_t){
        shared->control,
    };
}

void weak_copy(weak_t *self, weak_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(copied->control != NULL);
    assert(copied->control->weak_count > 0);
    ++copied->control->weak_count;
    *self = *copied;
}

bool weak_upgrade(weak_t *self, shared_t *shared) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->weak_count > 0);
    assert(shared != NULL);
    if (self->control->count > 0) {
        ++self->control->count;
        *shared = (shared_t) {
            self->control,
        };
        return true;
    }
    return false;
}

void weak_free(weak_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->weak_count > 0);
    --self->control->weak_count;
    if (self->control->count == 0 && self->control->weak_count == 0) {
        free(self->control);
        self->control = NULL;
    }
    *self = (weak_t){0};
}
