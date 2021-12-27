#include <stdlib.h>
#include <string.h>

#include "list.h"

void list_init(struct list *this, void (*destroy)(void *data)) {
  this->size = 0;
  this->destroy = destroy;
  this->head = NULL;
  this->tail = NULL;

  return;
}

void list_destroy(struct list *this) {
  void *data;

  while (list_size(this) > 0) {
    if (list_rem_next(this, NULL, (void **) &data) == 0 && this->destroy != NULL) {
      this->destroy(data);
    }
  }

  memset(this, 0, sizeof(struct list));

  return;
}

int list_ins_next(struct list *this, struct list_elem elem, const void *data) {
  struct list_elem new_elem;

  new_elem = (struct list_elem *) malloc(sizeof(struct list_elem));
  
  if (new_elem == NULL) {
    return -1;
  }
  
  new_elem->data = (void *) data;

  if (elem == NULL) {
    new_elem->next = this->head;
    this->head = new_elem;

    if (list_size(this) == 0) {
      this->tail = new_elem;
    }
  } else {
    new_elem->next = elem->next;
    elem->next = new_elem;

    if (elem->next == NULL) {
      this->tail = new_elem;
    }
  }

  this->size++;
}

int list_rem_next(struct list *this, struct list_elem *elem, void **data) {
  struct list_elem *old_elem;

  if (list_size(this) == 0) {
    return -1;
  }

  if (elem == NULL) {
    *data = this->head->data;
    old_elem = this->head;
    this->head = this->head->next;

    if (list_size(this) == 1) {
      this->tail = NULL;
    }
  } else {
    if (elem->next == NULL) {
      return -1;
    }

    *data = elem->next->data;
    old_elem = elem->next;
    elem->next 

    if (elem->next == NULL) {
      this->tail = elem;
    }
  }

  free(old_elem);

  this->size--;

  return 0;
}
