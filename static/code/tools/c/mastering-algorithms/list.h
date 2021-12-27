#ifndef LIST_H
#define LIST_H

struct list_elem {
  void *data;
  struct list_elem *next;
};

struct list {
  int size;

  int (*match)(const void *key1, const void *key2);
  void (*destroy)(void *data);

  struct list_elem *head;
  struct list_elem *tail;
};


void list_init(struct list *this, void (*destroy)(void *data));

void list_destroy(struct list *this);

int list_ins_next(struct list *this, struct list_elem *elem, const void *data);

int list_rem_next(struct list *this, struct list_elem *elem, void **data);


#define list_size(list) ((list)->size)

#define list_head(list) ((list)->head)

#define list_tail(list) ((list)->tail)

#define list_is_head(list, elem) ((elem) == (list)->head ? 1 : 0)

#define list_is_tail(elem) ((elem)->next == NULL ? 1 : 0)

#define list_data(elem) ((elem)->data)

#define list_next(elem) ((elem)->next)

#endif