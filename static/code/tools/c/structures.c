#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// structures are collections of one or more variables called fields grouped together under a single name
struct point {
  int x;
  int y;
  char *name;
};

int main(void) {
  // the struct keyword is included when refering to a struct type
  struct point p1;
  struct point* ptr = &p1;

  // initialization can be performed with an initializer or designated initializer
  struct point p2 = {1, 2};
  struct point p3 = {.y=2, .x=1};


  // the dot operator is used to access the fields of a structure
  p1.x = 1;
  (*ptr).y = 2;
  printf("(%d, %d)\n", p1.x, (*ptr).y);  // (1, 2)

  // the arrow operator is used to dereference a pointer to a struct and then access a field
  (&p1)->x = 3;
  ptr->y = 4;
  printf("(%d, %d)\n", (&p1)->x, ptr->y);  // (3, 4)


  // a structure declaration can be used to create an anonymous structure type
  struct {
    int x;
    int y;
  } p4;

  p4.x = 5;
  p4.y = 6;
  printf("(%d, %d)\n", p4.x, p4.y);  // (5, 6)


  // assigning one struct variable to another performs a memory copy
  p2 = p1;
  p1.x = 2;

  printf("(%d, %d)\n", p1.x, p1.y);  // (2, 4)
  printf("(%d, %d)\n", p2.x, p2.y);  // (3, 4)


  // we must remember to free any pointers in a dynamically allocated structure before freeing the structure
  struct point *p5 = (struct point *) malloc(sizeof(struct point));

  p5->x = 0;
  p5->y = 0;
  p5->name = (char *) malloc(strlen("origin") + 1);
  strcpy(p5->name, "origin");

  free(p5->name);
  free(p5);

  return 0;
}
