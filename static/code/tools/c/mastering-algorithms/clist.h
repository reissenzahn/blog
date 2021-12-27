#ifndef CLIST_H
#define CLIST_H

struct clist_elem {
  void *data;
  struct clist_elem *next;
};

struct clist {
  int size;

  int (*match)(const void *key1, const void *key2);
  void (*destroy)(void *data);

  struct clist_elem *head;
};

void clist_init(struct clist *this, void (*destroy)(void *data));

void clist_destroy(struct clist *this);

int clist_ins_next(struct clist *this, struct clist_elem *elem, const void *data);

int clist_rem_next(struct clist *this, struct clist_elem elem, void **data);

#define clist_size(this) ((this)->size)

#define clist_head(this) ((this)->head)

#define clist_data(elem) ((elem)->data)

#define clist_next(elem) ((elem)->next)

#endif