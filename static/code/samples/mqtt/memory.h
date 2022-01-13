#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

void *memory_malloc(size_t size);

void *memory_calloc(size_t len, size_t size);

void *memory_realloc(void *ptr, size_t size);

void *memory_free(void *ptr);

size_t memory_size(void *ptr);

char *memory_strdup(const char *str);

size_t memory_usage(void);

#endif
