// .c
// Doubly Linked List
// by Kyle Furey

#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void *node_data(node_t *self) {
    assert(self != NULL);
    assert(self->data != NULL);
    assert(self->owner != NULL);
    return self->data;
}

const void *node_data_const(const node_t *self) {
    assert(self != NULL);
    assert(self->data != NULL);
    assert(self->owner != NULL);
    return self->data;
}

node_t *node_previous(node_t *self) {
    assert(self != NULL);
    assert(self->data != NULL);
    assert(self->owner != NULL);
    return self->previous;
}

const node_t *node_previous_const(const node_t *self) {
    assert(self != NULL);
    assert(self->data != NULL);
    assert(self->owner != NULL);
    return self->previous;
}

node_t *node_next(node_t *self) {
    assert(self != NULL);
    assert(self->data != NULL);
    assert(self->owner != NULL);
    return self->next;
}

const node_t *node_next_const(const node_t *self) {
    assert(self != NULL);
    assert(self->data != NULL);
    assert(self->owner != NULL);
    return self->next;
}

list_t *node_owner(node_t *self) {
    assert(self != NULL);
    assert(self->data != NULL);
    assert(self->owner != NULL);
    return self->owner;
}

const list_t *node_owner_const(const node_t *self) {
    assert(self != NULL);
    assert(self->data != NULL);
    assert(self->owner != NULL);
    return self->owner;
}

void list_new(list_t *self, size_t element_size, void (*deleter)(void *)) {
    assert(self != NULL);
    assert(element_size > 0);
    *self = (list_t){
        element_size,
        0,
        NULL,
        NULL,
        deleter,
    };
}

bool list_copy(list_t *self, const list_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(copied->element_size > 0);
    assert((copied->head == NULL) == (copied->tail == NULL));
    assert((copied->count == 0) == (copied->head == NULL));
    list_new(self, copied->element_size, copied->deleter);
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

node_t *list_get(list_t *self, size_t index) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(index < self->count);
    node_t *current;
    if (index <= self->count / 2) {
        current = self->head;
        while (index-- > 0) {
            current = current->next;
        }
    } else {
        current = self->tail;
        while (index++ < self->count - 1) {
            current = current->previous;
        }
    }
    return current;
}

const node_t *list_get_const(const list_t *self, size_t index) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(index < self->count);
    const node_t *current;
    if (index <= self->count / 2) {
        current = self->head;
        while (index-- > 0) {
            current = current->next;
        }
    } else {
        current = self->tail;
        while (index++ < self->count - 1) {
            current = current->previous;
        }
    }
    return current;
}

bool list_contains(const list_t *self, const node_t *node) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(node != NULL);
    return node->owner == self;
}

void *list_insert_before(list_t *self, node_t *next) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(next == NULL || next->owner == self);
    node_t *node = (node_t *) malloc(sizeof(node_t));
    if (node == NULL) {
        return NULL;
    }
    node->data = malloc(self->element_size);
    if (node->data == NULL) {
        free(node);
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
    node_t *node = (node_t *) malloc(sizeof(node_t));
    if (node == NULL) {
        return NULL;
    }
    node->data = malloc(self->element_size);
    if (node->data == NULL) {
        free(node);
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

void *list_insert_where(list_t *self, bool (*predicate)(const node_t *next, void *data), void *data) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(predicate != NULL);
    node_t *current = self->head;
    while (current != NULL) {
        if (predicate(current, data)) {
            break;
        }
        current = current->next;
    }
    return list_insert_before(self, current);
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
    free(node->data);
    *node = (node_t){0};
    free(node);
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
        free(current->data);
        node_t *deleted = current;
        current = current->next;
        *deleted = (node_t){0};
        free(deleted);
    }
    self->head = NULL;
    self->tail = NULL;
    self->count = 0;
}

void list_foreach(list_t *self, void (*action)(void *element, void *data), void *data) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(action != NULL);
    node_t *current = self->head;
    while (current != NULL) {
        action(current->data, data);
        current = current->next;
    }
}

void list_foreach_const(const list_t *self, void (*action)(const void *element, void *data), void *data) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    assert(action != NULL);
    const node_t *current = self->head;
    while (current != NULL) {
        action(current->data, data);
        current = current->next;
    }
}

void list_free(list_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->head == NULL) == (self->tail == NULL));
    assert((self->count == 0) == (self->head == NULL));
    list_clear(self);
    *self = (list_t){0};
}
