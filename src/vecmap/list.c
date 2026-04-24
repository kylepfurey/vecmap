// .c
// Doubly Linked List
// by Kyle Furey

#include <vecmap/list.h>
#include <string.h>
#include <assert.h>

bool list_new(list_t *self, size_t element_size, deleter_t deleter, const allocator_t *allocator) {
    assert(self != NULL);
    assert(element_size > 0);
    allocator = allocator == NULL ? &stdalloc : allocator;
    *self = (list_t){
        element_size,
        0,
        NULL,
        NULL,
        deleter,
        allocator,
    };
    return true;
}

bool list_copy(list_t *self, const list_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(self != copied);
    assert(copied->element_size > 0);
    assert((copied->head == NULL) == (copied->tail == NULL));
    assert((copied->count == 0) == (copied->head == NULL));
    list_new(self, copied->element_size, copied->deleter, copied->allocator);
    node_t *current = copied->head;
    while (current != NULL) {
        void *data = list_push_back(self);
        if (data == NULL) {
            list_clear(self);
            return false;
        }
        memcpy(data, current->data, copied->element_size);
        current = current->next;
    }
    return true;
}

size_t list_count(const list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    return self->count;
}

bool list_empty(const list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    return self->count == 0;
}

node_t *list_front(list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    return self->head;
}

const node_t *list_front_const(const list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    return self->head;
}

node_t *list_back(list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    return self->tail;
}

const node_t *list_back_const(const list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    return self->tail;
}

void *list_insert_before(list_t *self, node_t *next) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(next == NULL || next->owner == self);
    node_t *node = (node_t *) allocator_malloc(self->allocator, sizeof(node_t));
    if (node == NULL) {
        return NULL;
    }
    node->data = allocator_malloc(self->allocator, self->element_size);
    if (node->data == NULL) {
        allocator_free(self->allocator, node);
        return NULL;
    }
    memset(node->data, 0, self->element_size);
    node->owner = self;
    node->next = next;
    if (next == NULL) {
        node->previous = self->tail;
        if (self->tail != NULL) {
            self->tail->next = node;
        }
        self->tail = node;
        if (self->count == 0) {
            self->head = node;
        }
    } else {
        node->previous = next->previous;
        if (next->previous != NULL) {
            next->previous->next = node;
        }
        next->previous = node;
        if (next == self->head) {
            self->head = node;
        }
    }
    ++self->count;
    return node->data;
}

void *list_insert_after(list_t *self, node_t *previous) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(previous != NULL);
    assert(previous->owner == self);
    node_t *node = (node_t *) allocator_malloc(self->allocator, sizeof(node_t));
    if (node == NULL) {
        return NULL;
    }
    node->data = allocator_malloc(self->allocator, self->element_size);
    if (node->data == NULL) {
        allocator_free(self->allocator, node);
        return NULL;
    }
    memset(node->data, 0, self->element_size);
    node->owner = self;
    node->previous = previous;
    node->next = previous->next;
    if (previous->next != NULL) {
        previous->next->previous = node;
    }
    previous->next = node;
    if (previous == self->tail) {
        self->tail = node;
    }
    ++self->count;
    return node->data;
}

void list_erase(list_t *self, node_t *node) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(self->count > 0);
    assert(node != NULL);
    assert(node->owner == self);
    if (node == self->head) {
        self->head = node->next;
    }
    if (node == self->tail) {
        self->tail = node->previous;
    }
    if (node->previous != NULL) {
        node->previous->next = node->next;
    }
    if (node->next != NULL) {
        node->next->previous = node->previous;
    }
    --self->count;
    if (self->deleter != NULL) {
        self->deleter(node->data);
    }
    allocator_free(self->allocator, node->data);
    *node = (node_t){0};
    allocator_free(self->allocator, node);
}

void *list_push_front(list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    return list_insert_before(self, self->head);
}

void *list_push_back(list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    return list_insert_before(self, NULL);
}

void list_pop_front(list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(self->count > 0);
    list_erase(self, self->head);
}

void list_pop_back(list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(self->count > 0);
    list_erase(self, self->tail);
}

void list_clear(list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    node_t *current = self->head;
    while (current != NULL) {
        if (self->deleter != NULL) {
            self->deleter(current->data);
        }
        allocator_free(self->allocator, current->data);
        node_t *deleted = current;
        current = current->next;
        *deleted = (node_t){0};
        allocator_free(self->allocator, deleted);
    }
    self->head = NULL;
    self->tail = NULL;
    self->count = 0;
}

bool list_foreach(list_t *self, bool (*action)(void *element, void *data), void *data) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(action != NULL);
    node_t *current = self->head;
    while (current != NULL) {
        if (!action(current->data, data)) {
            return false;
        }
        current = current->next;
    }
    return true;
}

bool list_foreach_const(const list_t *self, bool (*action)(const void *element, void *data), void *data) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(action != NULL);
    const node_t *current = self->head;
    while (current != NULL) {
        if (!action(current->data, data)) {
            return false;
        }
        current = current->next;
    }
    return true;
}

void list_free(list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    list_clear(self);
    *self = (list_t){0};
}
