#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// there are several ways of passing the address of a string to a function depending on how the string is declared

// in the parameter list we declare the parameter as a pointer to a char
size_t length(char *str) {
  size_t l = 0;

  while (*(str++)) {
    l++;
  }

  return l;
}

// notice we could just as well have declared the string parameter using array notation
size_t length2(char str[]) {
  size_t l = 0;

  while (*(str++)) {
    l++;
  }

  return l;
}

// passing a pointer to a string as a constrant char is useful to prevent the passed string from being modified
size_t length3(const char *str) {
  size_t l = 0;

  while (*(str++)) {
    l++;
  }

  // this will cause an error at compile-time
  // *str = '#';

  return l;
}

int main(int argc, char *argv[]) {
  char arr[] = "hello";
  char *ptr = (char *) malloc(strlen("hello") + 1);
  strcpy(ptr, "hello");

  // to use an array as an argument we can either use the name of the array or explicitly provide a reference to the first element of the array with the address-of operator
  printf("%d\n", length(arr));      // 5
  printf("%d\n", length(&arr[0]));  // 5

  // to invoke the function with the pointer we simply use its name
  printf("%d\n", length(ptr));  // 5
}


// TODO: passing literal and using that to dynamically allocate

// void initializePerson(Person *person, const char* fn,
//  const char* ln, const char* title, uint age) {
//  person->firstName = (char*) malloc(strlen(fn) + 1);
//  strcpy(person->firstName, fn);
//  person->lastName = (char*) malloc(strlen(ln) + 1);
//  strcpy(person->lastName, ln);
//  person->title = (char*) malloc(strlen(title) + 1);
//  strcpy(person->title, title);
//  person->age = age;
// }

// void processPerson() {
//  Person person;
//  initializePerson(&person, "Peter", "Underwood", "Manager", 36);
//  ...
// }
