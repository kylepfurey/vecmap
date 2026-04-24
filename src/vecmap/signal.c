// .c
// Multicast Signal
// by Kyle Furey

#include <vecmap/signal.h>
#include <string.h>
#include <assert.h>

bool signal_new(signal_t *self, const allocator_t *allocator) {
    assert(self != NULL);
    allocator = allocator == NULL ? &stdalloc : allocator;
    binding_t *bindings = (binding_t *) allocator_calloc(allocator, SIGNAL_DEFAULT_CAP, sizeof(binding_t));
    if (bindings == NULL) {
        return false;
    }
    *self = (signal_t){
        0,
        SIGNAL_DEFAULT_CAP,
        0,
        bindings,
        allocator,
    };
    return true;
}

bool signal_copy(signal_t *self, const signal_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(self != copied);
    assert(copied->capacity > 0);
    assert(copied->count <= copied->capacity);
    assert(copied->next <= copied->capacity);
    assert(copied->bindings != NULL);
    binding_t *bindings = (binding_t *) allocator_malloc(copied->allocator, copied->capacity * sizeof(binding_t));
    if (bindings == NULL) {
        return false;
    }
    memcpy(bindings, copied->bindings, copied->capacity * sizeof(binding_t));
    *self = (signal_t){
        copied->count,
        copied->capacity,
        copied->next,
        bindings,
        copied->allocator,
    };
    return true;
}

size_t signal_count(const signal_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->next <= self->capacity);
    assert(self->bindings != NULL);
    return self->count;
}

size_t signal_capacity(const signal_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->next <= self->capacity);
    assert(self->bindings != NULL);
    return self->capacity;
}

bool signal_resize(signal_t *self, size_t capacity) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->next <= self->capacity);
    assert(self->bindings != NULL);
    assert(capacity > 0);
    size_t min = 0;
    for (size_t i = 0; i < self->capacity; ++i) {
        if (self->bindings[i].action != NULL) {
            min = i + 1;
        }
    }
    capacity = capacity < min ? min : capacity;
    if (capacity > SIZE_MAX / sizeof(binding_t)) {
        return false;
    }
    binding_t *bindings = (binding_t *) allocator_realloc(self->allocator, self->bindings, capacity * sizeof(binding_t));
    if (bindings == NULL) {
        return false;
    }
    if (capacity > self->capacity) {
        memset(
            bindings + self->capacity,
            0,
            (capacity - self->capacity) * sizeof(binding_t)
        );
    }
    self->bindings = bindings;
    self->capacity = capacity;
    return true;
}

handle_t signal_bind(signal_t *self, void *element, action_t action) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->next <= self->capacity);
    assert(self->bindings != NULL);
    assert(action != NULL);
    handle_t handle = self->next;
    if (handle == self->capacity) {
        if (self->capacity > SIZE_MAX / SIGNAL_EXPANSION) {
            return NULL_HANDLE;
        }
        if (!signal_resize(self, self->capacity * SIGNAL_EXPANSION)) {
            return NULL_HANDLE;
        }
    }
    self->bindings[handle].element = element;
    self->bindings[handle].action = action;
    ++self->count;
    while (self->next < self->capacity && self->bindings[self->next].action != NULL) {
        ++self->next;
    }
    return handle;
}

void signal_unbind(signal_t *self, handle_t handle) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->next <= self->capacity);
    assert(self->bindings != NULL);
    assert(handle < self->capacity);
    assert(self->bindings[handle].action != NULL);
    self->bindings[handle].element = NULL;
    self->bindings[handle].action = NULL;
    --self->count;
    if (handle < self->next) {
        self->next = handle;
    }
}

bool signal_invoke(signal_t *self, void *data) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->next <= self->capacity);
    assert(self->bindings != NULL);
    size_t capacity = self->capacity;
    binding_t *bindings = (binding_t *) allocator_malloc(self->allocator, capacity * sizeof(binding_t));
    if (bindings == NULL) {
        return false;
    }
    memcpy(bindings, self->bindings, self->capacity * sizeof(binding_t));
    size_t count = self->count;
    for (size_t i = 0; i < capacity && count > 0; ++i) {
        if (bindings[i].action != NULL) {
            bindings[i].action(bindings[i].element, data);
            --count;
        }
    }
    allocator_free(self->allocator, bindings);
    return true;
}

void signal_clear(signal_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->next <= self->capacity);
    assert(self->bindings != NULL);
    memset(self->bindings, 0, self->capacity * sizeof(binding_t));
    self->count = 0;
    self->next = 0;
}

void signal_free(signal_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->next <= self->capacity);
    assert(self->bindings != NULL);
    allocator_free(self->allocator, self->bindings);
    *self = (signal_t){0};
}
