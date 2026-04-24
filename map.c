// .c
// Hash Map
// by Kyle Furey

#include "map.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

size_t map_hash_default(const void *data, size_t size) {
    assert(data != NULL);
    size_t hash = 2166136261u;
    const uint8_t *bytes = (const uint8_t *) data;
    for (size_t i = 0; i < size; ++i) {
        hash ^= bytes[i];
        hash *= 16777619u;
    }
    return hash;
}

bool map_equate_default(const void *a, const void *b, size_t size) {
    assert(a != NULL);
    assert(b != NULL);
    const uint8_t *a_bytes = (const uint8_t *) a;
    const uint8_t *b_bytes = (const uint8_t *) b;
    for (size_t i = 0; i < size; ++i) {
        if (a_bytes[i] != b_bytes[i]) {
            return false;
        }
    }
    return true;
}

bool map_new(map_t *self, size_t key_size, size_t value_size, size_t (*hasher)(const void *), bool (*equater)(const void *, const void *), void (*value_deleter)(void *)) {
    assert(self != NULL);
    assert(key_size > 0);
    assert(value_size > 0);
    pair_t *pairs = (pair_t *) calloc(MAP_DEFAULT_CAP, sizeof(pair_t));
    if (pairs == NULL) {
        return false;
    }
    for (size_t i = 0; i < MAP_DEFAULT_CAP; ++i) {
        pairs[i].state = PAIR_STATE_EMPTY;
    }
    *self = (map_t){
        key_size,
        value_size,
        0,
        MAP_DEFAULT_CAP,
        pairs,
        hasher,
        equater,
        value_deleter,
    };
    return true;
}

bool map_copy(map_t *self, const map_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(copied->key_size > 0);
    assert(copied->value_size > 0);
    assert(copied->capacity > 0);
    assert(copied->count <= copied->capacity);
    assert(copied->pairs != NULL);
    pair_t *pairs = (pair_t *) malloc(copied->capacity * sizeof(pair_t));
    if (pairs == NULL) {
        return false;
    }
    for (size_t i = 0; i < copied->capacity; ++i) {
        pairs[i].state = copied->pairs[i].state;
        if (pairs[i].state != PAIR_STATE_FULL) {
            pairs[i].key = NULL;
            pairs[i].value = NULL;
            continue;
        }
        pairs[i].key = malloc(copied->key_size);
        if (pairs[i].key == NULL) {
            for (size_t j = 0; j < i; ++j) {
                if (pairs[j].state != PAIR_STATE_FULL) {
                    continue;
                }
                free(pairs[j].key);
                free(pairs[j].value);
            }
            free(pairs);
            return false;
        }
        memcpy(pairs[i].key, copied->pairs[i].key, copied->key_size);
        pairs[i].value = malloc(copied->value_size);
        if (pairs[i].value == NULL) {
            free(pairs[i].key);
            for (size_t j = 0; j < i; ++j) {
                if (pairs[j].state != PAIR_STATE_FULL) {
                    continue;
                }
                free(pairs[j].key);
                free(pairs[j].value);
            }
            free(pairs);
            return false;
        }
        memcpy(pairs[i].value, copied->pairs[i].value, copied->value_size);
    }
    *self = (map_t){
        copied->key_size,
        copied->value_size,
        copied->count,
        copied->capacity,
        pairs,
        copied->hasher,
        copied->equater,
        copied->value_deleter,
    };
    return true;
}

size_t map_count(const map_t *self) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    return self->count;
}

size_t map_capacity(const map_t *self) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    return self->capacity;
}

bool map_empty(const map_t *self) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    return self->count == 0;
}

void *map_find(map_t *self, const void *key) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    assert(key != NULL);
    size_t hash;
    if (self->hasher != NULL) {
        hash = self->hasher(key);
    } else {
        hash = map_hash_default(key, self->key_size);
    }
    for (size_t i = 0; i < self->capacity; ++i) {
        pair_t *pair = self->pairs + (hash % self->capacity);
        switch (pair->state) {
            case PAIR_STATE_EMPTY:
                return NULL;
            case PAIR_STATE_FULL:
                if (self->equater != NULL) {
                    if (self->equater(key, pair->key)) {
                        return pair->value;
                    }
                } else {
                    if (map_equate_default(key, pair->key, self->key_size)) {
                        return pair->value;
                    }
                }
                break;
            case PAIR_STATE_SKIP:
            default:
                break;
        }
        ++hash;
    }
    return NULL;
}

const void *map_find_const(const map_t *self, const void *key) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    assert(key != NULL);
    size_t hash;
    if (self->hasher != NULL) {
        hash = self->hasher(key);
    } else {
        hash = map_hash_default(key, self->key_size);
    }
    for (size_t i = 0; i < self->capacity; ++i) {
        const pair_t *pair = self->pairs + (hash % self->capacity);
        switch (pair->state) {
            case PAIR_STATE_EMPTY:
                return NULL;
            case PAIR_STATE_FULL:
                if (self->equater != NULL) {
                    if (self->equater(key, pair->key)) {
                        return pair->value;
                    }
                } else {
                    if (map_equate_default(key, pair->key, self->key_size)) {
                        return pair->value;
                    }
                }
                break;
            case PAIR_STATE_SKIP:
            default:
                break;
        }
        ++hash;
    }
    return NULL;
}

bool map_contains(const map_t *self, const void *key) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    assert(key != NULL);
    return map_find_const(self, key) != NULL;
}

bool map_rehash(map_t *self, size_t capacity) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    assert(capacity > 0);
    capacity = capacity < self->count ? self->count : capacity;
    if (capacity > SIZE_MAX / sizeof(pair_t)) {
        return false;
    }
    if (capacity == self->capacity) {
        return true;
    }
    pair_t *pairs = (pair_t *) calloc(capacity, sizeof(pair_t));
    if (pairs == NULL) {
        return false;
    }
    for (size_t i = 0; i < capacity; ++i) {
        pairs[i].state = PAIR_STATE_EMPTY;
    }
    size_t count = self->count;
    for (size_t i = 0; i < self->capacity && count > 0; ++i) {
        pair_t *old_pair = self->pairs + i;
        if (old_pair->state != PAIR_STATE_FULL) {
            continue;
        }
        size_t hash;
        if (self->hasher != NULL) {
            hash = self->hasher(old_pair->key);
        } else {
            hash = map_hash_default(old_pair->key, self->key_size);
        }
        for (size_t j = 0; j < capacity; ++j) {
            pair_t *new_pair = pairs + (hash % capacity);
            if (new_pair->state == PAIR_STATE_FULL) {
                ++hash;
                continue;
            }
            new_pair->state = PAIR_STATE_FULL;
            new_pair->key = old_pair->key;
            new_pair->value = old_pair->value;
            break;
        }
        --count;
    }
    free(self->pairs);
    self->pairs = pairs;
    self->capacity = capacity;
    return true;
}

void *map_insert(map_t *self, const void *key) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    assert(key != NULL);
    if (self->count * MAP_LOAD_FACTOR_DEN >= self->capacity * MAP_LOAD_FACTOR_NUM) {
        if (self->capacity > SIZE_MAX / MAP_EXPANSION) {
            return NULL;
        }
        if (!map_rehash(self, self->capacity * MAP_EXPANSION)) {
            return NULL;
        }
    }
    size_t hash;
    if (self->hasher != NULL) {
        hash = self->hasher(key);
    } else {
        hash = map_hash_default(key, self->key_size);
    }
    for (size_t i = 0; i < self->capacity; ++i) {
        pair_t *pair = self->pairs + (hash % self->capacity);
        void *new_key;
        void *new_value;
        switch (pair->state) {
            case PAIR_STATE_EMPTY:
            case PAIR_STATE_SKIP:
                new_key = malloc(self->key_size);
                if (new_key == NULL) {
                    return NULL;
                }
                new_value = malloc(self->value_size);
                if (new_value == NULL) {
                    free(new_key);
                    return NULL;
                }
                memcpy(new_key, key, self->key_size);
                memset(new_value, 0, self->value_size);
                pair->state = PAIR_STATE_FULL;
                pair->key = new_key;
                pair->value = new_value;
                ++self->count;
                return new_value;
            case PAIR_STATE_FULL:
                if (self->equater != NULL) {
                    if (self->equater(key, pair->key)) {
                        memcpy(pair->key, key, self->key_size);
                        if (self->value_deleter != NULL) {
                            self->value_deleter(pair->value);
                        }
                        memset(pair->value, 0, self->value_size);
                        return pair->value;
                    }
                } else {
                    if (map_equate_default(key, pair->key, self->key_size)) {
                        memcpy(pair->key, key, self->key_size);
                        if (self->value_deleter != NULL) {
                            self->value_deleter(pair->value);
                        }
                        memset(pair->value, 0, self->value_size);
                        return pair->value;
                    }
                }
                break;
            default:
                break;
        }
        ++hash;
    }
    return NULL;
}

bool map_erase(map_t *self, const void *key) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    assert(key != NULL);
    if (self->count == 0) {
        return false;
    }
    size_t hash;
    if (self->hasher != NULL) {
        hash = self->hasher(key);
    } else {
        hash = map_hash_default(key, self->key_size);
    }
    for (size_t i = 0; i < self->capacity; ++i) {
        pair_t *pair = self->pairs + (hash % self->capacity);
        switch (pair->state) {
            case PAIR_STATE_EMPTY:
                return false;
            case PAIR_STATE_FULL:
                if (self->equater != NULL) {
                    if (self->equater(key, pair->key)) {
                        pair->state = PAIR_STATE_SKIP;
                        free(pair->key);
                        pair->key = NULL;
                        if (self->value_deleter != NULL) {
                            self->value_deleter(pair->value);
                        }
                        free(pair->value);
                        pair->value = NULL;
                        --self->count;
                        return true;
                    }
                } else {
                    if (map_equate_default(key, pair->key, self->key_size)) {
                        pair->state = PAIR_STATE_SKIP;
                        free(pair->key);
                        pair->key = NULL;
                        if (self->value_deleter != NULL) {
                            self->value_deleter(pair->value);
                        }
                        free(pair->value);
                        pair->value = NULL;
                        --self->count;
                        return true;
                    }
                }
                break;
            case PAIR_STATE_SKIP:
            default:
                break;
        }
        ++hash;
    }
    return false;
}

void map_clear(map_t *self) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    for (size_t i = 0; i < self->capacity; ++i) {
        if (self->pairs[i].state == PAIR_STATE_FULL) {
            free(self->pairs[i].key);
            self->pairs[i].key = NULL;
            if (self->value_deleter != NULL) {
                self->value_deleter(self->pairs[i].value);
            }
            free(self->pairs[i].value);
            self->pairs[i].value = NULL;
        }
        self->pairs[i].state = PAIR_STATE_EMPTY;
    }
    self->count = 0;
}

void map_foreach(map_t *self, void (*action)(const void *key, void *value, void *data), void *data) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    assert(action != NULL);
    size_t count = self->count;
    for (size_t i = 0; i < self->capacity && count > 0; ++i) {
        if (self->pairs[i].state == PAIR_STATE_FULL) {
            action(self->pairs[i].key, self->pairs[i].value, data);
            --count;
        }
    }
}

void map_foreach_const(const map_t *self, void (*action)(const void *key, const void *value, void *data), void *data) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    assert(action != NULL);
    size_t count = self->count;
    for (size_t i = 0; i < self->capacity && count > 0; ++i) {
        if (self->pairs[i].state == PAIR_STATE_FULL) {
            action(self->pairs[i].key, self->pairs[i].value, data);
            --count;
        }
    }
}

void map_free(map_t *self) {
    assert(self != NULL);
    assert(self->key_size > 0);
    assert(self->value_size > 0);
    assert(self->capacity > 0);
    assert(self->count <= self->capacity);
    assert(self->pairs != NULL);
    map_clear(self);
    free(self->pairs);
    *self = (map_t){0};
}
