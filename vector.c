// .c
// Dynamic Array
// by Kyle Furey

#include "vector.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

bool vector_new(vector_t *self, size_t element_size, void (*deleter)(void *)) {
    assert(self != NULL);
    assert(element_size > 0);
    void *array = calloc(VECTOR_DEFAULT_CAP, element_size);
    if (array == NULL) {
        return false;
    }
    *self = (vector_t){
        element_size,
        0,
        VECTOR_DEFAULT_CAP,
        array,
        deleter,
    };
    return true;
}

bool vector_copy(vector_t *self, const vector_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(copied->element_size > 0);
    assert(copied->capacity > 0);
    assert(copied->count <= copied->capacity);
    assert(copied->array != NULL);
    void *array = calloc(copied->capacity, copied->element_size);
    if (array == NULL) {
        return false;
    }
    memcpy(array, copied->array, copied->count * copied->element_size);
    *self = (vector_t){
        copied->element_size,
        copied->count,
        copied->capacity,
        array,
        copied->deleter,
    };
    return true;
}

size_t vector_count(const vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    return self->count;
}

size_t vector_capacity(const vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    return self->capacity;
}

bool vector_empty(const vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    return self->count == 0;
}

void *vector_get(vector_t *self, size_t index) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    assert(index < self->count);
    return ((uint8_t *) self->array) + (index * self->element_size);
}

const void *vector_get_const(const vector_t *self, size_t index) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    assert(index < self->count);
    return ((const uint8_t *) self->array) + (index * self->element_size);
}

void *vector_array(vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    return self->array;
}

const void *vector_array_const(const vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    return self->array;
}

bool vector_resize(vector_t *self, size_t capacity) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    assert(capacity > 0);
    if (capacity > SIZE_MAX / self->element_size) {
        return false;
    }
    void *deleted = NULL;
    if (self->deleter != NULL && self->count > capacity) {
        deleted = malloc((self->count - capacity) * self->element_size);
        if (deleted == NULL) {
            return false;
        }
        memcpy(
            deleted,
            ((uint8_t *) self->array) + (capacity * self->element_size),
            (self->count - capacity) * self->element_size
        );
    }
    void *array = realloc(self->array, capacity * self->element_size);
    if (array == NULL) {
        free(deleted);
        return false;
    }
    if (capacity > self->capacity) {
        memset(
            ((uint8_t *) array) + (self->capacity * self->element_size),
            0,
            (capacity - self->capacity) * self->element_size
        );
    }
    if (deleted != NULL) {
        for (size_t i = 0; i < self->count - capacity; ++i) {
            self->deleter(((uint8_t *) deleted) + (i * self->element_size));
        }
        free(deleted);
        self->count = capacity;
    }
    self->array = array;
    self->capacity = capacity;
    return true;
}

void *vector_insert(vector_t *self, size_t index) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    assert(index <= self->count);
    if (self->count == self->capacity) {
        if (self->capacity > SIZE_MAX / VECTOR_EXPANSION) {
            return NULL;
        }
        if (!vector_resize(self, self->capacity * VECTOR_EXPANSION)) {
            return NULL;
        }
    }
    ++self->count;
    void *dest = ((uint8_t *) self->array) + ((index + 1) * self->element_size);
    void *src = vector_get(self, index);
    memmove(dest, src, (self->count - index - 1) * self->element_size);
    void *data = vector_get(self, index);
    memset(data, 0, self->element_size);
    return data;
}

void vector_erase(vector_t *self, size_t index) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    assert(index < self->count);
    void *dest = vector_get(self, index);
    if (self->deleter != NULL) {
        self->deleter(dest);
    }
    if (index + 1 < self->count) {
        void *src = ((uint8_t *) self->array) + ((index + 1) * self->element_size);
        memmove(dest, src, (self->count - index - 1) * self->element_size);
    }
    memset(vector_get(self, self->count - 1), 0, self->element_size);
    --self->count;
}

void *vector_push(vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    if (self->count == self->capacity) {
        if (self->capacity > SIZE_MAX / VECTOR_EXPANSION) {
            return NULL;
        }
        if (!vector_resize(self, self->capacity * VECTOR_EXPANSION)) {
            return NULL;
        }
    }
    ++self->count;
    void *data = vector_get(self, self->count - 1);
    memset(data, 0, self->element_size);
    return data;
}

void vector_pop(vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    assert(self->count > 0);
    void *data = vector_get(self, self->count - 1);
    if (self->deleter != NULL) {
        self->deleter(data);
    }
    memset(data, 0, self->element_size);
    --self->count;
}

bool vector_reverse(vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    void *buffer = malloc(self->element_size);
    if (buffer == NULL) {
        return false;
    }
    for (size_t i = 0; i < self->count / 2; ++i) {
        void *a = vector_get(self, i);
        void *b = vector_get(self, self->count - i - 1);
        memcpy(buffer, a, self->element_size);
        memcpy(a, b, self->element_size);
        memcpy(b, buffer, self->element_size);
    }
    free(buffer);
    return true;
}

void vector_clear(vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    if (self->deleter != NULL) {
        for (size_t i = 0; i < self->count; ++i) {
            self->deleter(vector_get(self, i));
        }
    }
    memset(self->array, 0, self->count * self->element_size);
    self->count = 0;
}

void vector_foreach(vector_t *self, void (*action)(void *element, void *data), void *data) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    assert(action != NULL);
    for (size_t i = 0; i < self->count; ++i) {
        action(vector_get(self, i), data);
    }
}

void vector_foreach_const(const vector_t *self, void (*action)(const void *element, void *data), void *data) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    assert(action != NULL);
    for (size_t i = 0; i < self->count; ++i) {
        action(vector_get_const(self, i), data);
    }
}

void vector_free(vector_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->array != NULL);
    vector_clear(self);
    free(self->array);
    *self = (vector_t){0};
}
