#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

#include "memory.h"

static atomic_size_t memory = 0;

void *memory_malloc(size_t size) {
  void *ptr = malloc(sizeof(size_t) + size);
  if (ptr == NULL) {
    fprintf(stderr, "allocate: out of memory\n");
    exit(EXIT_FAILURE);
  }

  memory += sizeof(sizeof_t) + size;

  *((size_t *) ptr) = size;

  return (char *) ptr + sizeof(size_t);
}

void *memory_calloc(size_t len, size_t size) {
  void *ptr = allocate(len * size);
  
  memset(ptr, 0x00, len * size);

  return ptr;
}

void *memory_realloc(void *ptr, size_t size) {
  if (ptr == NULL) {
    return allocate(size);
  }

  void *real_ptr = (char *) ptr - sizeof(size_t);
  size_t curr_size = *((size_t *) real_ptr);

  if (curr_size == size) {
    return ptr;
  }

  void *new_ptr = realloc(real_ptr, sizeof(size_t) + size);
  if (new_ptr == NULL) {
    fprintf(stderr, "reallocate: out of memory\n");
    exit(EXIT_FAILURE);
  }

  memory += sizeof(size_t) + size - curr_size;

  *((size_t *) new_ptr) = size;

  return (char *) new_ptr + sizeof(size_t);
}

void *memory_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  void *real_ptr = (char *) ptr - sizeof(size_t);

  size_t size = *((size_t *) real_ptr);

  memory -= size + sizeof(size_t);

  free(real_ptr);
}

size_t memory_size(void *ptr) {
  if (ptr == NULL) {
    return 0;
  }

  void *real_ptr = (char *) ptr - sizeof(size_t);

  return *((size_t *) real_ptr);
}

char *memory_strdup(const char *str) {
  size_t len = strlen(str);

  char *new_str = memory_malloc(len + 1);

  snprintf(new_str, len + 1, "%s", str);

  return new_str;
}

size_t memory_usage(void) {
  return memory;
}
