
// COPYING MEMORY

#include <stdio.h>
#include <string.h>

struct node {
  int value;
  struct node *next;
};

int main() {
  struct node n1, n2;

  n1.value = 1;
  n1.next = NULL;

  n2 = n1;
  printf("%d, %p\n", n2.value, n2.next);

  // memcpy(&n2, &n1, sizeof(struct node));
  // printf("%d, %p\n", n2.value, n2.next);

}

