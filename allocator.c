// .c
// Arena Allocator
// by Kyle Furey

#include "allocator.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

bool allocator_new(allocator_t *self, size_t size) {
    assert(self != NULL);
    assert(size > 0);
    // TODO
}

void *allocator_borrow(allocator_t *self, size_t size) {
    assert(self != NULL);
    if (size == 0) {
        return NULL;
    }
    // TODO
}

void *allocator_move(allocator_t *self, void *memory, size_t size) {
    assert(self != NULL);
    if (memory == NULL) {
        return allocator_borrow(self, size);
    }
    if (size == 0) {
        return NULL;
    }
    // TODO
}

void allocator_return(allocator_t *self, void *memory) {
    assert(self != NULL);
    if (memory == NULL) {
        return;
    }
    // TODO
}

void allocator_free(allocator_t *self) {
    assert(self != NULL);
    // TODO
}