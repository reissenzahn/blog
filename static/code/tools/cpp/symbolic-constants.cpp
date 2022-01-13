#include <iostream>

void display(const int i) {
  std::cout << i;
}

int main(int argc, char *argv[]) {
  // constant variables are called symbolic constants (as opposed to literal constants which are just values that have no name)

  // constant variables must be initialized when they are defined after which the value cannot be changed
  const double pi { 3.14159 };

  // the const may appear on either side of the type
  double const e { 2.71828 };

  // constant variables can be initialized from other variables
  int val;
  std::cin >> val;
  const int input { val };


  // const can also be used with function parameters to ensure that the value of the parameter is not changed inside the function; the value of the argument in the function call will be used as the initializer
  display(input);


  // runtime constants are initialized with values that can only be resolved at runtime while compile-time constants are initialized with values that can be determined at compile-time

  // the constexpr keyword ensures that a constant must be a compile-time constant
  constexpr double gravity { 9.0 + 0.8 };


  // any variable that should not be modifiable after initialization and whose initializer is known at compile-time should be declared as constexpr

  // any variable that should not be modifiable after initialization and whose initializer is not known at compile-time should be declared as const

  return 0;
}
