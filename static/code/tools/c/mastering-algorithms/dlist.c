#include <stdlib.h>
#include <string.h>

#include "dlist.h"

void dlist_init(struct dlist *this, void (*destroy)(void *data)) {
  this->size = 0;
  this->destroy = destroy;
  this->head = NULL;
  this->tail = NULL;

  return;
}

void dlist_destroy(struct dlist *this) {
  void *data;

  while (dlist_size(this) > 0) {
    if (dlist_rem(this, dlist_tail(this), (void **) &data) == 0 && this->destroy != NULL) {
      this->destroy(data);
    }
  }

  memset(this, 0, sizeof(struct dlist));

  return;
}

int dlist_ins_next(struct dlist *this, struct dlist_elem *elem, const void *data) {
  struct dlist_elem *new_elem;

  // do not allow NULL element unless the list is empty
  if (elem == NULL && dlist_size(this) != 0) {
    return -1;
  }

  new_elem = (struct dlist_elem *) malloc(sizeof(struct dlist_elem));

  if (new_elem == NULL) {
    return -1;
  }

  new_elem->data = (void *) data;

  if (dlist_size(this) == 0) {
    this->head = new_elem;
    this->head->prev = NULL;
    this->head->next = NULL;
    this->tail = new_elem;
  } else {
    new_elem->next = elem->next;
    new_elem->prev = elem;

    if (elem->next == NULL) {
      this->tail = new_elem;
    } else {
      elem->next->prev = new_elem;
    }

    elem->next = new_elem;
  }

  this->size++;

  return 0;
}

int dlist_ins_prev(struct dlist *this, struct dlist_elem *elem, const void *data) {
  struct dlist_elem *new_elem;

  if (elem == NULL && dlist_size(this) != 0) {
    return -1;
  }

  new_elem = (struct dlist_elem *) malloc(sizeof(struct dlist_elem));

  if (new_elem == NULL) {
    return -1;
  }

  new_elem->data = (void *) data;

  if (dlist_size(this) == 0) {
    this->head = new_elem;
    this->head->prev = NULL;
    this->head->next = NULL;
    this->tail = new_elem;
  } else {
    new_elem->next = elem;
    new_elem->prev = elem->prev;

    if (elem->prev == NULL) {
      this->head = new_elem;
    } else {
      elem->prev->next = new_elem;
    }

    elem->prev = new_elem;
  }

  this->size++;

  return 0;
}

int dlist_rem(struct dlist *this, struct dlist_elem *elem, void **data) {
  if (elem == NULL || dlist_size(this) == 0) {
    return -1;
  }

  *data = elem->data;

  if (elem == this->head) {
    this->head = elem->next;

    if (this->head == NULL) {
      this->tail = NULL;
    } else {
      elem->next->prev = NULL;
    }
  } else {
    elem->prev->next = elem->next;

    if (elem->next == NULL) {
      this->tail = elem->prev;
    } else {
      elem->next->prev = elem->prev;
    }
  }

  free(elem);

  this->size--;

  return 0;
}
