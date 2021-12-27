#ifndef DLIST_H
#define DLIST_H

struct dlist_elem {
  void *data;
  struct dlist_elem *prev;
  struct dlist_elem *next;
};

struct dlist {
  int size;

  int (*match)(const void *key1, const void *key2);
  void (*destroy)(void *data);

  struct dlist_elem *head;
  struct dlist_elem *tail;
};


void dlist_init(struct dlist *this, void (*destroy)(void *data));

void dlist_destroy(struct dlist *this);

int dlist_ins_next(struct dlist *this, struct dlist_elem *elem, const void *data);

int dlist_ins_prev(struct dlist *this, struct dlist_elem *elem, const void *data);

int dlist_rem(struct dlist *this, struct dlist_elem *elem, void **data);


#define dlist_size(this) ((this)->size)

#define dlist_head(this) ((this)->head)

#define dlist_tail(this) ((this)->tail)

#define dlist_is_head(elem) ((elem)->prev == NULL ? 1 : 0)

#define dlist_is_tail(elem) ((elem)->next == NULL ? 1 : 0)

#define dlist_data(elem) ((elem)->data)

#define dlist_next(elem) ((elem)->next)

#define dlist_prev(elem) ((elem)->prev)

#endif