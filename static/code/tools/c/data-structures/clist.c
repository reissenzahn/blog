#include <stdlib.h>
#include <string.h>

#include "clist.h"

void clist_init(struct clist *this, void (*destroy)(void *data)) {
  this->size = 0;
  this->destroy = destroy;
  this->head = NULL;

  return;
}

void clist_destroy(struct clist *this) {
  void *data;

  while (clist_size(this) > 0) {
    if (clist_rem_next(this, this->head, (void **) &data) == 0 && this->destroy != NULL) {
      this->destroy(data);
    }
  }
  
  memset(this, 0, sizeof(struct clist));

  return;
}

int clist_ins_next(struct clist *this, struct clist_elem *elem, const void *data) {
  struct clist_elem *new_elem;

  new_elem = (struct list_elem *) malloc(sizeof(struct clist_elem));

  if (new_elem == NULL) {
    return -1;
  }

  new_elem->data = (void *) data;

  if (clist_size(this) == 0) {
    new_elem->next = new_elem;
    this->head = new_elem;
  } else {
    new_elem->next = elem->next;
    elem->next = new_elem;
  }

  this->size++;
  
  return 0;
}

int clist_rem_next(struct clist *this, struct clist_elem elem, void **data) {
  struct clist_elem *old_elem;

  if (clist_size(this) == 0) {
    return -1;
  }

  *data = elem->next->data;

  if (elem->next == elem) {
    old_elem = elem->next;
    this->head = NULL;
  } else {
    old_elem = elem->next;
    elem->next = elem->next->next;

    if (old_elem == clist_head(this)) {
      this->head = old_elem->next;
    }
  }

  free(old_elem);

  this->size--;

  return 0;
}