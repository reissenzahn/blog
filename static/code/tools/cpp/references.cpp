#include <iostream>

int main(int argc, char *argv[]) {
  // a reference acts as an alias to another object or value
  int val {1};
  int& ref {val};

  // a reference generally acts identically to the value it is referencing
  std::cout << val << std::endl;  // 
  std::cout << ref << std::endl;  // 

  ref++;

  std::cout << val << std::endl;  // 
  std::cout << ref << std::endl;  // 

  // using the address-of operator on a reference returns the address of the value being references
  std::cout << &val << std::endl;  // 
  std::cout << &ref << std::endl;  // 


}





// references must be initialized when created and there is no such thing as a null reference
int& invalid;  // invalid, needs to reference something

References to non-const values can only be initialized with non-const l-values. They can not be initialized with const l-values or r-values.

int x{ 5 };
int& ref1{ x }; // okay, x is an non-const l-value

const int y{ 7 };
int& ref2{ y }; // not okay, y is a const l-value

int& ref3{ 6 }; // not okay, 6 is an r-value

Note that in the middle case, you can’t initialize a non-const reference with a const object -- otherwise you’d be able to change the value of the const object through the reference, which would violate the const-ness of the object.


References can not be reassigned

// a reference can not be changed to reference another variable after it has been initialized

int value1{ 5 };
int value2{ 6 };

int& ref{ value1 }; // okay, ref is now an alias for value1
ref = value2; // assigns 6 (the value of value2) to value1




References as function parameters


// a function that uses a reference parameter is able to modify the argument passed in

// a reference parameter acts as an alias for the argument and so no copy of the argument is made into the parameter
void increment(int& ref) {
  ref++;
}

When argument n is passed to the function, the function parameter ref is set as a reference to argument n. This allows the function to change the value of n through ref! Note that n does not need to be a reference itself.







References to non-const values can only be initialized with non-const l-values.

// references are most often used as function parameters when we either want to modify the value of the argument or when we want to avoid making an expensive copy of the argument


// WIP

Using references to pass C-style arrays to functions

One of the most annoying issues with C-style arrays is that in most cases they decay to pointers when evaluated. However, if a C-style array is passed by reference, this decaying does not happen.

#include <iostream>
#include <iterator>

// Note: You need to specify the array size in the function declaration
void printElements(int (&arr)[4])
{
  int length{ static_cast<int>(std::size(arr)) }; // we can now do this since the array won't decay

  for (int i{ 0 }; i < length; ++i)
  {
    std::cout << arr[i] << '\n';
  }
}

int main()
{
    int arr[]{ 99, 20, 14, 80 };

    printElements(arr);

    return 0;
}



---

// a reference to a const value (often called a const reference)
const int apples{ 5 };
const int& ref{ apples };

// references to const values can be initialized with non-const l-values, const l-values and r-values
int x{ 5 };
const int& ref1{ x };

const int y{ 7 };
const int& ref2{ y };

const int& ref3{ 6 };

// a reference to a const value can reference a non-const variable though, when accessed through a reference to a const value, the value is considered const even if the original variable is not
int apples{ 5 };
const int& ref{ apples };

apples = 6;
ref = 7;  // illegal


References to r-values extend the lifetime of the referenced value

Normally r-values have expression scope, meaning the values are destroyed at the end of the expression in which they are created.

std::cout << 2 + 3 << '\n'; // 2 + 3 evaluates to r-value 5, which is destroyed at the end of this statement
However, when a reference to a const value is initialized with an r-value, the lifetime of the r-value is extended to match the lifetime of the reference.

int somefcn()
{
    const int& ref{ 2 + 3 }; // normally the result of 2+3 has expression scope and is destroyed at the end of this statement
    // but because the result is now bound to a reference to a const value...
    std::cout << ref << '\n'; // we can use it here
} // and the lifetime of the r-value is extended to here, when the const reference dies
Const references as function parameters

References used as function parameters can also be const. This allows us to access the argument without making a copy of it, while guaranteeing that the function will not change the value being referenced.

// ref is a const reference to the argument passed in, not a copy
void changeN(const int& ref)
{
	ref = 6; // not allowed, ref is const
}
References to const values are particularly useful as function parameters because of their versatility. A const reference parameter allows you to pass in a non-const l-value argument, a const l-value argument, a literal, or the result of an expression:

#include <iostream>

void printIt(const int& x)
{
    std::cout << x;
}

int main()
{
    int a{ 1 };
    printIt(a); // non-const l-value

    const int b{ 2 };
    printIt(b); // const l-value

    printIt(3); // literal r-value

    printIt(2+b); // expression r-value

    return 0;
}
The above prints

1234
To avoid making unnecessary, potentially expensive copies, variables that are not pointers or fundamental data types (int, double, etc…) should be generally passed by (const) reference. Fundamental data types should be passed by value, unless the function needs to change them. There are a few exceptions to this rule, namely types that are so small that it’s faster for the CPU to copy them than having to perform an extra indirection for a reference.



// https://www.learncpp.com/cpp-tutorial/references/