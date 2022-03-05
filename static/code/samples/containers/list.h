#ifndef LIST_H
#define LIST_H

struct list_node {
  void *data;
  struct list_node *prev;
  struct list_node *next;
};

struct list {
  int size;
  struct list_node *head;
  struct list_node *tail;
  void (*destroy)(void *data);
};

// int (*match)(const void *key1, const void *key2);


void list_init(struct list *this, void (*destroy)(void *data)) {
  this->size = 0;
  this->head = NULL;
  this->tail = NULL;
  this->destroy = destroy;
}

void list_destroy(struct list *this) {
  while (list-)
}

void dlist_destroy(DList *list) {

void               *data;

while (dlist_size(list) > 0) {

   if (dlist_remove(list, dlist_tail(list), (void **)&data) == 0 && list->
      destroy != NULL) {

      /***********************************************************************
      *                                                                      *
      *  Call a user-defined function to free dynamically allocated data.    *
      *                                                                      *
      ***********************************************************************/

      list->destroy(data);

   }

}

memset(list, 0, sizeof(DList));

return;

}

int list_insert_next(struct list *this, struct list_node *node, void *data) {
  if (node == NULL && this->size != 0) {
    return -1;
  }

  struct list_node *new_node = malloc(sizeof(struct list_node));
  if (new_node == NULL) {
    return -1;
  }

  new_node->data = data;

  if (this->size == 0) {
    new_node->prev = NULL;
    new_node->next = NULL;
    this->head = new_node;
    this->tail = new_node;
  } else {
    new_node->next = node->next;
    new_node->prev = node;

    if (node->next == NULL) {
      this->tail = new_node;
    } else {
      new_node->next->prev = new_node;
    }

    node->next = new_node;
  } 

  this->size++;

  return 0; 
}

int list_insert_prev(struct list *this, struct list_node *node, void *data) {
  if (node == NULL && this->size != 0) {
    return -1;
  }


}

if ((new_element = (DListElmt *)malloc(sizeof(DListElmt))) == NULL)
   return -1;

/*****************************************************************************
*                                                                            *
*  Insert the new element into the list.                                     *
*                                                                            *
*****************************************************************************/

new_element->data = (void *)data;

if (dlist_size(list) == 0) {

   /**************************************************************************
   *                                                                         *
   *  Handle insertion when the list is empty.                               *
   *                                                                         *
   **************************************************************************/

   list->head = new_element;
   list->head->prev = NULL;
   list->head->next = NULL;
   list->tail = new_element;

   }


else {

   /**************************************************************************
   *                                                                         *
   *  Handle insertion when the list is not empty.                           *
   *                                                                         *
   **************************************************************************/

   new_element->next = element; 
   new_element->prev = element->prev;

   if (element->prev == NULL)
      list->head = new_element;
   else
      element->prev->next = new_element;

   element->prev = new_element;

}


/*****************************************************************************
*                                                                            *
*  Adjust the size of the list to account for the new element.               *
*                                                                            *
*****************************************************************************/

list->size++;

return 0;

}

/*****************************************************************************
*                                                                            *
*  ----------------------------- dlist_remove -----------------------------  *
*                                                                            *
*****************************************************************************/

int dlist_remove(DList *list, DListElmt *element, void **data) {

/*****************************************************************************
*                                                                            *
*  Do not allow a NULL element or removal from an empty list.                *
*                                                                            *
*****************************************************************************/

if (element == NULL || dlist_size(list) == 0)
   return -1;

/*****************************************************************************
*                                                                            *
*  Remove the element from the list.                                         *
*                                                                            *
*****************************************************************************/

*data = element->data;

if (element == list->head) {

   /**************************************************************************
   *                                                                         *
   *  Handle removal from the head of the list.                              *
   *                                                                         *
   **************************************************************************/

   list->head = element->next;

   if (list->head == NULL)
      list->tail = NULL;
   else
      element->next->prev = NULL;

   }

else {

   /**************************************************************************
   *                                                                         *
   *  Handle removal from other than the head of the list.                   *
   *                                                                         *
   **************************************************************************/

   element->prev->next = element->next;

   if (element->next == NULL)
      list->tail = element->prev;
   else
      element->next->prev = element->prev;

}

/*****************************************************************************
*                                                                            *
*  Free the storage allocated by the abstract data type.                     *
*                                                                            *
*****************************************************************************/

free(element);

/*****************************************************************************
*                                                                            *
*  Adjust the size of the list to account for the removed element.           *
*                                                                            *
*****************************************************************************/

list->size--;

return 0;

}

#endif
