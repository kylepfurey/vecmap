// .h
// Hash Map
// by Kyle Furey

#ifndef VECMAP_MAP_H
#define VECMAP_MAP_H

#include <vecmap/allocator.h>
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

typedef size_t (*hasher_t)(const void *element);
typedef bool (*equater_t)(const void *left, const void *right);

typedef struct {
    size_t key_size;
    size_t value_size;
    size_t count;
    size_t capacity;
    pair_t *pairs;
    hasher_t hasher;
    equater_t equater;
    deleter_t value_deleter;
    const allocator_t *allocator;
} map_t;

#ifdef __cplusplus
extern "C" {
#endif

bool map_new(map_t *self, size_t key_size, size_t value_size, hasher_t hasher, equater_t equater, deleter_t value_deleter, const allocator_t *allocator);
bool map_copy(map_t *self, const map_t *copied);
size_t map_count(const map_t *self);
size_t map_capacity(const map_t *self);
bool map_empty(const map_t *self);
void *map_find(map_t *self, const void *key);
const void *map_find_const(const map_t *self, const void *key);
bool map_contains(const map_t *self, const void *key);
bool map_resize(map_t *self, size_t capacity);
void *map_insert(map_t *self, const void *key);
bool map_erase(map_t *self, const void *key);
void map_clear(map_t *self);
bool map_foreach(map_t *self, bool (*action)(const void *key, void *value, void *data), void *data);
bool map_foreach_const(const map_t *self, bool (*action)(const void *key, const void *value, void *data), void *data);
void map_free(map_t *self);

#ifdef __cplusplus
}
#endif

#endif // VECMAP_MAP_H
