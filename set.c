// .c
// Binary Tree
// by Kyle Furey

#include "set.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

bool tree_copy(const tree_t *self, set_t *set) {
    if (self == NULL) {
        return true;
    }
    assert(self->data != NULL);
    if (!tree_copy(self->left, set)) {
        return false;
    }
    if (!tree_copy(self->right, set)) {
        return false;
    }
    return set_insert(set, self->data) != NULL;
}

void tree_clear(tree_t *self, void(*deleter)(void *)) {
    if (self == NULL) {
        return;
    }
    assert(self->data != NULL);
    tree_clear(self->left, deleter);
    if (deleter != NULL) {
        deleter(self->data);
    }
    free(self->data);
    self->data = NULL;
    tree_t *right = self->right;
    free(self);
    tree_clear(right, deleter);
}

void tree_foreach(const tree_t *self, void (*action)(const void *element, void *data), void *data) {
    assert(action != NULL);
    if (self == NULL) {
        return;
    }
    assert(self->data != NULL);
    tree_foreach(self->left, action, data);
    action(self->data, data);
    tree_foreach(self->right, action, data);
}

void set_new(set_t *self, size_t element_size, comparison_t (*comparer)(const void *left, const void *right), void (*deleter)(void *)) {
    assert(self != NULL);
    assert(element_size > 0);
    *self = (set_t){
        element_size,
        0,
        NULL,
        comparer,
        deleter,
    };
}

bool set_copy(set_t *self, const set_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(copied->element_size > 0);
    assert((copied->count == 0) == (copied->root == NULL));
    set_new(self, copied->element_size, copied->comparer, copied->deleter);
    if (!tree_copy(copied->root, self)) {
        set_clear(self);
        return false;
    }
    return true;
}

size_t set_count(const set_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    return self->count;
}

bool set_empty(const set_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    return self->count == 0;
}

const void *set_least(const set_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    const tree_t *current = self->root;
    if (current == NULL) {
        return NULL;
    }
    while (current->left != NULL) {
        current = current->left;
    }
    return current->data;
}

const void *set_greatest(const set_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    const tree_t *current = self->root;
    if (current == NULL) {
        return NULL;
    }
    while (current->right != NULL) {
        current = current->right;
    }
    return current->data;
}

const void *set_find(const set_t *self, const void *element) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    assert(element != NULL);
    const tree_t *current = self->root;
    while (current != NULL) {
        int result;
        if (self->comparer != NULL) {
            result = (int) self->comparer(element, current->data);
        } else {
            result = memcmp(element, current->data, self->element_size);
        }
        if (result == 0) {
            return current->data;
        } else if (result < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

bool set_contains(const set_t *self, const void *element) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    return set_find(self, element) != NULL;
}

const void *set_insert(set_t *self, const void *element) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    assert(element != NULL);
    tree_t **current = &self->root;
    while (*current != NULL) {
        int result;
        if (self->comparer != NULL) {
            result = (int) self->comparer(element, (*current)->data);
        } else {
            result = memcmp(element, (*current)->data, self->element_size);
        }
        if (result == 0) {
            if (self->deleter != NULL) {
                self->deleter((*current)->data);
            }
            memcpy((*current)->data, element, self->element_size);
            return (*current)->data;
        }
        current = (result < 0) ? (&(*current)->left) : (&(*current)->right);
    }
    tree_t *tree = malloc(sizeof(tree_t));
    if (tree == NULL) {
        return NULL;
    }
    void *data = malloc(self->element_size);
    if (data == NULL) {
        free(tree);
        return NULL;
    }
    memcpy(data, element, self->element_size);
    *tree = (tree_t){
        data,
        NULL,
        NULL,
    };
    *current = tree;
    self->count++;
    return tree->data;
}

bool set_erase(set_t *self, const void *element) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    assert(element != NULL);
    tree_t **current = &self->root;
    while (*current != NULL) {
        int result;
        if (self->comparer != NULL) {
            result = (int) self->comparer(element, (*current)->data);
        } else {
            result = memcmp(element, (*current)->data, self->element_size);
        }
        if (result == 0) {
            break;
        }
        current = (result < 0) ? (&(*current)->left) : (&(*current)->right);
    }
    if (*current == NULL) {
        return false;
    }
    tree_t *target = *current;
    if (target->left == NULL) {
        *current = target->right;
    } else if (target->right == NULL) {
        *current = target->left;
    } else {
        tree_t **successor = &target->right;
        while ((*successor)->left != NULL) {
            successor = &(*successor)->left;
        }
        void *swap = target->data;
        target->data = (*successor)->data;
        (*successor)->data = swap;
        target = *successor;
        *successor = target->right;
    }
    if (self->deleter != NULL) {
        self->deleter(target->data);
    }
    free(target->data);
    target->data = NULL;
    free(target);
    self->count--;
    return true;
}

void set_clear(set_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    tree_clear(self->root, self->deleter);
    self->count = 0;
    self->root = NULL;
}

void set_foreach(const set_t *self, void (*action)(const void *element, void *data), void *data) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    assert(action != NULL);
    tree_foreach(self->root, action, data);
}

void set_free(set_t *self) {
    assert(self != NULL);
    assert(self->element_size > 0);
    assert((self->count == 0) == (self->root == NULL));
    set_clear(self);
    *self = (set_t){0};
}
