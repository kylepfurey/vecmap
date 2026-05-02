// .c
// String Builder
// by Kyle Furey

#include "stringbuilder.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

bool string_new(string_t *self, const char *str) {
    assert(self != NULL);
    assert(str != NULL);
    size_t length = strlen(str);
    size_t capacity = (length + 1) > STRING_DEFAULT_CAP ? (length + 1) : STRING_DEFAULT_CAP;
    char *buffer = (char *) calloc(capacity, 1);
    if (buffer == NULL) {
        return false;
    }
    memcpy(buffer, str, length);
    *self = (string_t){
        length,
        capacity,
        buffer,
    };
    return true;
}

bool string_copy(string_t *self, const string_t *copied) {
    assert(self != NULL);
    assert(copied != NULL);
    assert(copied->capacity > 0);
    assert(copied->length < copied->capacity);
    assert(copied->buffer != NULL);
    assert(copied->buffer[copied->length] == '\0');
    size_t length = copied->length;
    size_t capacity = copied->capacity;
    char *buffer = (char *) calloc(capacity, 1);
    if (buffer == NULL) {
        return false;
    }
    memcpy(buffer, copied->buffer, length);
    *self = (string_t){
        length,
        capacity,
        buffer,
    };
    return true;
}

size_t string_length(const string_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    return self->length;
}

size_t string_capacity(const string_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    return self->capacity;
}

bool string_empty(const string_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    return self->length == 0;
}

char string_get(const string_t *self, size_t index) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(index < self->length);
    return self->buffer[index];
}

const char *string_set(string_t *self, size_t index, char data) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(index < self->length);
    if (data == '\0') {
        memset(self->buffer + index, 0, self->length - index);
        self->length = index;
    } else {
        self->buffer[index] = data;
    }
    return self->buffer;
}

const char *string_cstr(const string_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    return self->buffer;
}

const char *string_substr(string_t *self, size_t index, size_t length) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(index <= self->length);
    if (index + length > self->length) {
        length = self->length - index;
    }
    memmove(self->buffer, self->buffer + index, length);
    memset(self->buffer + length, 0, self->length - length);
    self->length = length;
    return self->buffer;
}

int string_compare(const string_t *self, const char *str) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(str != NULL);
    return strcmp(self->buffer, str);
}

bool string_resize(string_t *self, size_t length) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    size_t capacity = length + 1;
    char *buffer = realloc(self->buffer, capacity);
    if (buffer == NULL) {
        return false;
    }
    if (capacity > self->capacity) {
        memset(buffer + self->capacity, 0, capacity - self->capacity);
    }
    if (self->length > length) {
        memset(buffer + length, 0, self->length - length);
        self->length = length;
    }
    self->buffer = buffer;
    self->capacity = capacity;
    return true;
}

const char *string_insert(string_t *self, size_t index, const char *str) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(index <= self->length);
    assert(str != NULL);
    size_t length = strlen(str);
    size_t new_length = self->length + length;
    if (self->buffer == str) { 
        while (new_length >= self->capacity) {
            if (!string_resize(self, (self->capacity * STRING_EXPANSION) - 1)) {
                return NULL;
            }
        }
        str = self->buffer;
    } else {   
        while (new_length >= self->capacity) {
            if (!string_resize(self, (self->capacity * STRING_EXPANSION) - 1)) {
                return NULL;
            }
        }
    }
    memmove(self->buffer + index + length, self->buffer + index, self->length - index);
    memcpy(self->buffer + index, str, length);
    self->length = new_length;
    return self->buffer;
}

const char *string_erase(string_t *self, size_t index, size_t length) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(index < self->length);
    if (index + length > self->length) {
        length = self->length - index;
    }
    size_t new_length = self->length - length;
    memmove(self->buffer + index, self->buffer + index + length, self->length - index - length);
    memset(self->buffer + new_length, 0, length);
    self->length = new_length;
    return self->buffer;
}

const char *string_append(string_t *self, const char *str) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(str != NULL);
    return string_insert(self, self->length, str);
}

const char *string_prepend(string_t *self, const char *str) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(str != NULL);
    return string_insert(self, 0, str);
}

const char *string_format(string_t *self, ...) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    va_list args;
    va_start(args, self);
    int minimum = vsnprintf(NULL, 0, self->buffer, args);
    va_end(args);
    if (minimum < 0) {
        return NULL;
    }
    size_t length = (size_t) minimum;
    char *format = (char *) malloc(self->length + 1);
    if (format == NULL) {
        return NULL;
    }
    memcpy(format, self->buffer, self->length + 1);
    while (length >= self->capacity) {
        if (!string_resize(self, (self->capacity * STRING_EXPANSION) - 1)) {
            free(format);
            return NULL;
        }
    }
    va_start(args, self);
    vsnprintf(self->buffer, length + 1, format, args);
    va_end(args);
    self->length = length;
    free(format);
    return self->buffer;
}

size_t string_parse(const string_t *self, const char *format, ...) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(format != NULL);
    va_list args;
    va_start(args, format);
    int count = vsscanf(self->buffer, format, args);
    va_end(args);
    return count < 0 ? 0 : (size_t) count;
}

bool string_find_first(const string_t *self, const char *str, size_t *index) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(str != NULL);
    size_t length = strlen(str);
    if (length == 0 || length > self->length) {
        return false;
    }
    for (size_t i = 0; i <= self->length - length; ++i) {
        if (memcmp(self->buffer + i, str, length) == 0) {
            if (index != NULL) {
                *index = i;
            }
            return true;
        }
    }
    return false;
}

bool string_find_last(const string_t *self, const char *str, size_t *index) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(str != NULL);
    size_t length = strlen(str);
    if (length == 0 || length > self->length) {
        return false;
    }
    for (size_t i = self->length - length; ; --i) {
        if (memcmp(self->buffer + i, str, length) == 0) {
            if (index != NULL) {
                *index = i;
            }
            return true;
        }
        if (i == 0) {
            break;
        }
    }
    return false;
}

bool string_contains(const string_t *self, const char *str) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(str != NULL);
    return string_find_first(self, str, NULL);
}

size_t string_count(const string_t *self, const char *str) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(str != NULL);
    size_t length = strlen(str);
    if (length == 0 || length > self->length) {
        return 0;
    }
    size_t count = 0;
    for (size_t i = 0; i <= self->length - length; ++i) {
        if (memcmp(self->buffer + i, str, length) == 0) {
            ++count;
            i += length - 1;
        }
    }
    return count;
}

const char *string_replace_first(string_t *self, const char *find, const char *replace) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(find != NULL);
    assert(replace != NULL);
    size_t index;
    if (!string_find_first(self, find, &index)) {
        return self->buffer;
    }
    size_t copy_length = self->length;
    char *copy = (char *) malloc(self->length + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, self->buffer, self->length + 1);
    string_erase(self, index, strlen(find));
    if (!string_insert(self, index, replace)) {
        free(self->buffer);
        self->buffer = copy;
        self->length = copy_length;
        self->capacity = copy_length + 1;
        return NULL;
    }
    free(copy);
    return self->buffer;
}

const char *string_replace_last(string_t *self, const char *find, const char *replace) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(find != NULL);
    assert(replace != NULL);
    size_t index;
    if (!string_find_last(self, find, &index)) {
        return self->buffer;
    }
    size_t copy_length = self->length;
    char *copy = (char *) malloc(self->length + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, self->buffer, self->length + 1);
    string_erase(self, index, strlen(find));
    if (!string_insert(self, index, replace)) {
        free(self->buffer);
        self->buffer = copy;
        self->length = copy_length;
        self->capacity = copy_length + 1;
        return NULL;
    }
    free(copy);
    return self->buffer;
}

const char *string_replace_all(string_t *self, const char *find, const char *replace) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(find != NULL);
    assert(replace != NULL);
    size_t find_length = strlen(find);
    if (find_length == 0) {
        return self->buffer;
    }
    size_t copy_length = self->length;
    char *copy = (char *) malloc(self->length + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, self->buffer, self->length + 1);
    size_t replace_length = strlen(replace);
    size_t i = 0;
    while (i + find_length <= self->length) {
        if (memcmp(self->buffer + i, find, find_length) == 0) {
            string_erase(self, i, find_length);
            if (!string_insert(self, i, replace)) {
                free(self->buffer);
                self->buffer = copy;
                self->length = copy_length;
                self->capacity = copy_length + 1;
                return NULL;
            }
            i += replace_length;
        } else {
            ++i;
        }
    }
    free(copy);
    return self->buffer;
}

const char *string_reverse(string_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    for (size_t i = 0; i < self->length / 2; ++i) {
        char swap = self->buffer[i];
        self->buffer[i] = self->buffer[self->length - i - 1];
        self->buffer[self->length - i - 1] = swap;
    }
    return self->buffer;
}

const char *string_lower(string_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    for (size_t i = 0; i < self->length; ++i) {
        self->buffer[i] = (char) tolower((unsigned char) self->buffer[i]);
    }
    return self->buffer;
}

const char *string_upper(string_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    for (size_t i = 0; i < self->length; ++i) {
        self->buffer[i] = (char) toupper((unsigned char) self->buffer[i]);
    }
    return self->buffer;
}

const char *string_trim(string_t *self, bool resize) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    if (self->length == 0) {
        return self->buffer;
    }
    size_t start;
    size_t end;
    for (start = 0; start < self->length; ++start) {
        if (!(bool) isspace((unsigned char) self->buffer[start])) {
            break;
        }
    }
    for (end = self->length - 1; end > start; --end) {
        if (!(bool) isspace((unsigned char) self->buffer[end])) {
            break;
        }
    }
    string_substr(self, start, end - start + 1);
    if (resize) {
        string_resize(self, self->length);
    }
    return self->buffer;
}

void string_clear(string_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    memset(self->buffer, 0, self->length);
    self->length = 0;
}

const char *string_map(string_t *self, char(*transform)(char)) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(transform != NULL);
    for (size_t i = 0; i < self->length; ++i) {
        self->buffer[i] = transform(self->buffer[i]);
        if (self->buffer[i] == '\0') {
            memset(self->buffer + i + 1, 0, self->length - i - 1);
            self->length = i;
            break;
        }
    }
    return self->buffer;
}

void string_foreach(const string_t *self, void(*action)(char element, void *data), void *data) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    assert(action != NULL);
    for (size_t i = 0; i < self->length; ++i) {
        action(self->buffer[i], data);
    }
}

void string_free(string_t *self) {
    assert(self != NULL);
    assert(self->capacity > 0);
    assert(self->length < self->capacity);
    assert(self->buffer != NULL);
    assert(self->buffer[self->length] == '\0');
    free(self->buffer);
    *self = (string_t){0};
}
