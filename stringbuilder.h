// .h
// String Builder
// by Kyle Furey

#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include <stddef.h>
#include <stdbool.h>

#define STRING_DEFAULT_CAP 8
#define STRING_EXPANSION 2

typedef struct {
    size_t length;
    size_t capacity;
    char *buffer;
} string_t;

#ifdef __cplusplus
extern "C" {
#endif

bool string_new(string_t *self, const char *str);
bool string_copy(string_t *self, const string_t *copied);
size_t string_length(const string_t *self);
size_t string_capacity(const string_t *self);
bool string_empty(const string_t *self);
char string_get(const string_t *self, size_t index);
const char *string_set(string_t *self, size_t index, char data);
const char *string_cstr(const string_t *self);
const char *string_substr(string_t *self, size_t index, size_t length);
int string_compare(const string_t *self, const char *str);
bool string_resize(string_t *self, size_t length);
const char *string_insert(string_t *self, size_t index, const char *str);
const char *string_erase(string_t *self, size_t index, size_t length);
const char *string_append(string_t *self, const char *str);
const char *string_prepend(string_t *self, const char *str);
const char *string_format(string_t *self, ...);
size_t string_parse(const string_t *self, const char *format, ...);
bool string_find_first(const string_t *self, const char *str, size_t *index);
bool string_find_last(const string_t *self, const char *str, size_t *index);
bool string_contains(const string_t *self, const char *str);
size_t string_count(const string_t *self, const char *str);
const char *string_replace_first(string_t *self, const char *find, const char *replace);
const char *string_replace_last(string_t *self, const char *find, const char *replace);
const char *string_replace_all(string_t *self, const char *find, const char *replace);
const char *string_reverse(string_t *self);
const char *string_lower(string_t *self);
const char *string_upper(string_t *self);
const char *string_trim(string_t *self, bool resize);
void string_clear(string_t *self);
const char *string_map(string_t *self, char(*transform)(char));
void string_foreach(const string_t *self, void(*action)(char element, void *data), void *data);
void string_free(string_t *self);

#ifdef __cplusplus
}
#endif

#endif // STRINGBUILDER_H
