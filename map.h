// .h
// Hash Map
// by Kyle Furey

#ifndef MAP_H
#define MAP_H

#include <stddef.h>
#include <stdbool.h>

#define MAP_DEFAULT_CAP 8
#define MAP_EXPANSION 2
#define MAP_LOAD_FACTOR_NUM 1
#define MAP_LOAD_FACTOR_DEN 2

typedef enum {
    PAIR_STATE_EMPTY = 0,
    PAIR_STATE_FULL = 1,
    PAIR_STATE_SKIP = 2,
} pair_state_t;

typedef struct {
    pair_state_t state;
    void *key;
    void *value;
} pair_t;

typedef struct {
    size_t key_size;
    size_t value_size;
    size_t count;
    size_t capacity;
    pair_t *pairs;
    size_t (*hasher)(const void *);
    bool (*equater)(const void *, const void *);
    void (*value_deleter)(void *);
} map_t;

#ifdef __cplusplus
extern "C" {
#endif

bool map_new(map_t *self, size_t key_size, size_t value_size, size_t (*hasher)(const void *), bool (*equater)(const void *, const void *), void (*value_deleter)(void *));
bool map_copy(map_t *self, const map_t *copied);
size_t map_count(const map_t *self);
size_t map_capacity(const map_t *self);
bool map_empty(const map_t *self);
void *map_find(map_t *self, const void *key);
const void *map_find_const(const map_t *self, const void *key);
bool map_contains(const map_t *self, const void *key);
bool map_rehash(map_t *self, size_t capacity);
void *map_insert(map_t *self, const void *key);
bool map_erase(map_t *self, const void *key);
void map_clear(map_t *self);
void map_foreach(map_t *self, void (*action)(const void *key, void *value, void *data), void *data);
void map_foreach_const(const map_t *self, void (*action)(const void *key, const void *value, void *data), void *data);
void map_free(map_t *self);

#ifdef __cplusplus
}
#endif

#endif // MAP_H
