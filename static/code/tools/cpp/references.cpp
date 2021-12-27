#include <iostream>

void increment(int& i) {
  i++;
}

int main() {

  // a reference is an alias that is typically implemented to hold the address of an object
  int i1 = 1;
  int& r1 {i1};

  // you access a reference with exactly the same syntax as the name of an object
  std::cout << r1 << '\n';  // 1

  // a reference always refers to the object to which it was initialized and cannot be changed after initialization
  r1 = 2;
  std::cout << i1 << '\n';  // 2

  // there is no null reference so we may assume that a reference refers to an object
  // int& r2;  // error


  // no operator operates on a reference; instead the operator gets applied to whatever the reference refers to
  r1++;  // i1++

  // thus we cannot have a pointer to a reference and will instead get a pointer to the object denoted by the reference
  int* p1 = &r1;

  // we also cannot defined an array of references


  // when initialization a const reference an implicit type conversion is applied (if necessary) then the resulting value is placed in a temporary variable of the type and finally this variable is used as the value of the initializer
  const int& r2 {1};


  // the main use of reference is for specifying arguments and return values for functions

  // a reference can be used to specify a function argument so that the function can change the value of an object passed to it
  int i2 = 2;
  increment(i2);



  // there are three kinds of references:

  // 1. lvalue references: refer to objects whose value we want to change
  // 2. const references: refer to objects whose value we do not want to change
  // 3. rvalue references: refer to objects whose value we do not need to preserve after we have used it


  return 0;
}


