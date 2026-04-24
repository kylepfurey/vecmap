// .c
// Reference Counting Pointers
// by Kyle Furey

#include <vecmap/shared.h>
#include <string.h>
#include <assert.h>

bool shared_new(shared_t *self, size_t size, deleter_t deleter, const allocator_t *allocator) {
    assert(self != NULL);
    assert(size > 0);
    allocator = allocator == NULL ? &stdalloc : allocator;
    rc_t *control = (rc_t *) allocator_malloc(allocator, sizeof(rc_t));
    if (control == NULL) {
        return false;
    }
    void *data = allocator_malloc(allocator, size);
    if (data == NULL) {
        allocator_free(allocator, control);
        return false;
    }
    memset(data, 0, size);
    *control = (rc_t){
        data,
        1,
        0,
        deleter,
        allocator,
    };
    *self = (shared_t){
        control,
    };
    return true;
}

void shared_copy(shared_t *self, shared_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(self != copied);
    assert(copied->control != NULL);
    assert(copied->control->data != NULL);
    assert(copied->control->shared_count > 0);
    ++copied->control->shared_count;
    *self = *copied;
}

size_t shared_count_shared(const shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->shared_count > 0);
    return self->control->shared_count;
}

size_t shared_count_weak(const shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->shared_count > 0);
    return self->control->weak_count;
}

void *shared_get(shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->shared_count > 0);
    return self->control->data;
}

const void *shared_get_const(const shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->shared_count > 0);
    return self->control->data;
}

void shared_free(shared_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->shared_count > 0);
    --self->control->shared_count;
    if (self->control->shared_count == 0) {
        if (self->control->deleter != NULL) {
            self->control->deleter(self->control->data);
        }
        allocator_free(self->control->allocator, self->control->data);
        self->control->data = NULL;
        if (self->control->weak_count == 0) {
            allocator_free(self->control->allocator, self->control);
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
    assert(shared->control->shared_count > 0);
    ++shared->control->weak_count;
    *self = (weak_t){
        shared->control,
    };
}

void weak_copy(weak_t *self, weak_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(self != copied);
    assert(copied->control != NULL);
    assert(copied->control->weak_count > 0);
    ++copied->control->weak_count;
    *self = *copied;
}

size_t weak_count_shared(const weak_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->shared_count > 0);
    return self->control->shared_count;
}

size_t weak_count_weak(const weak_t *self) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->data != NULL);
    assert(self->control->shared_count > 0);
    return self->control->weak_count;
}

bool weak_upgrade(weak_t *self, shared_t *shared) {
    assert(self != NULL);
    assert(self->control != NULL);
    assert(self->control->weak_count > 0);
    assert(shared != NULL);
    if (self->control->shared_count > 0) {
        ++self->control->shared_count;
        *shared = (shared_t){
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
    if (self->control->shared_count == 0 && self->control->weak_count == 0) {
        allocator_free(self->control->allocator, self->control);
        self->control = NULL;
    }
    *self = (weak_t){0};
}
