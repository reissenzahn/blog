

Pass by value

By default, non-pointer arguments in C++ are passed by value. When an argument is passed by value, the argument’s value is copied into the value of the corresponding function parameter.

Consider the following snippet:

#include <iostream>

void foo(int y)
{
    std::cout << "y = " << y << '\n';
}

int main()
{
    foo(5); // first call

    int x{ 6 };
    foo(x); // second call
    foo(x+1); // third call

    return 0;
}
In the first call to foo(), the argument is the literal 5. When foo() is called, variable y is created, and the value of 5 is copied into y. Variable y is then destroyed when foo() ends.

In the second call to foo(), the argument is the variable x. x is evaluated to produce the value 6. When foo() is called for the second time, variable y is created again, and the value of 6 is copied into y. Variable y is then destroyed when foo() ends.

In the third call to foo(), the argument is the expression x+1. x+1 is evaluated to produce the value 7, which is passed to variable y. Variable y is once again destroyed when foo() ends.

Thus, this program prints:

y = 5
y = 6
y = 7
Because a copy of the argument is passed to the function, the original argument can not be modified by the function. This is shown in the following example:

#include <iostream>

void foo(int y)
{
    std::cout << "y = " << y << '\n';

    y = 6;

    std::cout << "y = " << y << '\n';
} // y is destroyed here

int main()
{
    int x{ 5 };
    std::cout << "x = " << x << '\n';

    foo(x);

    std::cout << "x = " << x << '\n';
    return 0;
}
This snippet outputs:

x = 5
y = 5
y = 6
x = 5
At the start of main, x is 5. When foo() is called, the value of x (5) is passed to foo’s parameter y. Inside foo(), y is assigned the value of 6, and then destroyed. The value of x is unchanged, even though y was changed.

Function parameters passed by value can also be made const. This will enlist the compiler’s help in ensuring the function doesn’t try to change the parameter’s value.

Pros and cons of pass by value

Advantages of passing by value:

Arguments passed by value can be variables (e.g. x), literals (e.g. 6), expressions (e.g. x+1), structs & classes, and enumerators. In other words, just about anything.
Arguments are never changed by the function being called, which prevents side effects.
Disadvantages of passing by value:

Copying structs and classes can incur a significant performance penalty, especially if the function is called many times.
When to use pass by value:

When passing fundamental data types and enumerators, and the function does not need to change the argument.
When not to use pass by value:

When passing structs or classes (including std::array, std::vector, and std::string).
In most cases, pass by value is the best way to accept parameters of fundamental types when the function does not need to change the argument. Pass by value is flexible and safe, and in the case of fundamental types, efficient.


---


While pass by value is suitable in many cases, it has a couple of limitations. First, when passing a large struct or class to a function, pass by value will make a copy of the argument into the function parameter. In many cases, this is a needless performance hit, as the original argument would have sufficed. Second, when passing arguments by value, the only way to return a value back to the caller is via the function’s return value. While this is often suitable, there are cases where it would be more clear and efficient to have the function modify the argument passed in. Pass by reference solves both of these issues.

Pass by reference

To pass a variable by reference, we simply declare the function parameters as references rather than as normal variables:

void addTwo(int& ref) // ref is a reference variable
{
    ref += 2;
}
When the function is called, ref will become a reference to the argument. Since a reference to a variable is treated exactly the same as the variable itself, any changes made to the reference are passed through to the argument!

The following example shows this in action:

void addTwo(int& ref)
{
   ref += 2;
}

int main()
{
    int value{ 5 };

    cout << "value = " << value << '\n';
    addTwo(value);
    cout << "value = " << value << '\n';
    return 0;
}
This program is the same as the one we used for the pass by value example, except addTwo’s parameter is now a reference instead of a normal variable. When we call addTwo(value), ref becomes a reference to main’s value variable. This snippet produces the output:

value = 5
value = 7
As you can see, the function changed the value of the argument from 5 to 7!

Returning multiple values via out parameters

Sometimes we need a function to return multiple values. However, functions can only have one return value. One way to return multiple values is using reference parameters:

#include <iostream>
#include <cmath>    // for std::sin() and std::cos()

void getSinCos(double degrees, double& sinOut, double& cosOut)
{
    // sin() and cos() take radians, not degrees, so we need to convert
    constexpr double pi { 3.14159265358979323846 }; // the value of pi
    double radians{ degrees * pi / 180.0 };
    sinOut = std::sin(radians);
    cosOut = std::cos(radians);
}

int main()
{
    double sin{ };
    double cos{ };

    // getSinCos will return the sin and cos in variables sin and cos
    getSinCos(30.0, sin, cos);

    std::cout << "The sin is " << sin << '\n';
    std::cout << "The cos is " << cos << '\n';
    return 0;
}
This function takes one parameter (by value) as input, and “returns” two parameters (by reference) as output. Parameters that are only used for returning values back to the caller are called out parameters. We’ve named these out parameters with the suffix “out” to denote that they’re out parameters. This helps remind the caller that the initial value passed to these parameters doesn’t matter, and that we should expect them to be rewritten. By convention, output parameters are typically the rightmost parameters.

Let’s explore how this works in more detail. First, the main function creates local variables sin and cos. Those are passed into function getSinCos() by reference (rather than by value). This means function getSinCos() has access to the actual sin and cos variables, not just copies. getSinCos() accordingly assigns new values to sin and cos (through references sinOut and cosOut respectively), which overwrites the old values in sin and cos. Main then prints these updated values.

If sin and cos had been passed by value instead of reference, getSinCos() would have changed copies of sin and cos, leading to any changes being discarded at the end of the function. But because sin and cos were passed by reference, any changes made to sin or cos (through the references) are persisted beyond the function. We can therefore use this mechanism to return values back to the caller.

This method, while functional, has a few minor downsides. First, the caller must pass in arguments to hold the updated outputs even if it doesn’t intend to use them. More importantly, the syntax is a bit unnatural, with both the input and output parameters being put together in the function call. It’s not obvious from the caller’s end that sin and cos are out parameters and will be changed. This is probably the most dangerous part of this method (as it can lead to mistakes being made). Some programmers and companies feel this is a big enough problem to advise avoiding output parameters altogether, or using pass by address for out parameters instead (which has a clearer syntax indicating whether a parameter is modifiable or not).

Personally, we recommend avoiding out parameters altogether if possible. If you do use them, naming out parameters (and output arguments) with an “out” suffix (or prefix) can help make it clear that the value might be modified.

Limitations of pass by reference

Non-const references can only reference non-const l-values (e.g. non-const variables), so a reference parameter cannot accept an argument that is a const l-value or an r-value (e.g. literals and the results of expressions).

Pass by const reference

As mentioned in the introduction, one of the major disadvantages of pass by value is that all arguments passed by value are copied into the function parameters. When the arguments are large structs or classes, this can take a lot of time. References provide a way to avoid this penalty. When an argument is passed by reference, a reference is created to the actual argument (which takes minimal time) and no copying of values takes place. This allows us to pass large structs and classes with a minimum performance penalty.

However, this also opens us up to potential trouble. References allow the function to change the value of the argument, which is undesirable when we want an argument be read-only. If we know that a function should not change the value of an argument, but don’t want to pass by value, the best solution is to pass by const reference.

You already know that a const reference is a reference that does not allow the variable being referenced to be changed through the reference. Consequently, if we use a const reference as a parameter, we guarantee to the caller that the function will not change the argument!

The following function will produce a compiler error:

void foo(const std::string& x) // x is a const reference
{
    x = "hello";  // compile error: a const reference cannot have its value changed!
}
Using const is useful for several reasons:

It enlists the compilers help in ensuring values that shouldn’t be changed aren’t changed (the compiler will throw an error if you try, like in the above example).
It tells the programmer that the function won’t change the value of the argument. This can help with debugging.
You can’t pass a const argument to a non-const reference parameter. Using const parameters ensures you can pass both non-const and const arguments to the function.
Const references can accept any type of argument, including non-const l-values, const l-values, and r-values.
Best practice

When passing an argument by reference, always use a const reference unless you need to change the value of the argument.

A reminder

Non-const references cannot bind to r-values. A function with a non-const reference parameter cannot be called with literals or temporaries.

#include <string>

void foo(std::string& text) {}

int main()
{
  std::string text{ "hello" };

  foo(text); // ok
  foo(text + " world"); // illegal, non-const references can't bind to r-values.

  return 0;
}
References to pointers

It’s possible to pass a pointer by reference, and have the function change the address of the pointer entirely:

#include <iostream>

void foo(int*& ptr) // pass pointer by reference
{
	ptr = nullptr; // this changes the actual ptr argument passed in, not a copy
}

int main()
{
	int x{ 5 };
	int* ptr{ &x };
	std::cout << "ptr is: " << (ptr ? "non-null" : "null") << '\n'; // prints non-null
	foo(ptr);
	std::cout << "ptr is: " << (ptr ? "non-null" : "null") << '\n'; // prints null

	return 0;
}
(We’ll show another example of this in the next lesson)

As a reminder, you can pass a C-style array by reference. This is useful if you need the ability for the function to change the array (e.g. for a sort function) or you need access to the array’s type information of a fixed array (to do sizeof() or a for-each loop). However, note that in order for this to work, you explicitly need to define the array size in the parameter:

#include <iostream>

// Note: You need to specify the array size in the function declaration
void printElements(int (&arr)[4])
{
  int length{ sizeof(arr) / sizeof(arr[0]) }; // we can now do this since the array won't decay

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
This means this only works with fixed arrays of one particular length. If you want this to work with fixed arrays of any length, you can make the array length a template parameter (covered in a later chapter).

Pros and cons of pass by reference

Advantages of passing by reference:

References allow a function to change the value of the argument, which is sometimes useful. Otherwise, const references can be used to guarantee the function won’t change the argument.
Because a copy of the argument is not made, pass by reference is fast, even when used with large structs or classes.
References can be used to return multiple values from a function (via out parameters).
References must be initialized, so there’s no worry about null values.
Disadvantages of passing by reference:

Because a non-const reference cannot be initialized with a const l-value or an r-value (e.g. a literal or an expression), arguments to non-const reference parameters must be normal variables.
It can be hard to tell whether an argument passed by non-const reference is meant to be input, output, or both. Judicious use of const and a naming suffix for out variables can help.
It’s impossible to tell from the function call whether the argument may change. An argument passed by value and passed by reference looks the same. We can only tell whether an argument is passed by value or reference by looking at the function declaration. This can lead to situations where the programmer does not realize a function will change the value of the argument.
When to use pass by reference:

When passing structs or classes (use const if read-only).
When you need the function to modify an argument.
When you need access to the type information of a fixed array.
When not to use pass by reference:

When passing fundamental types that don’t need to be modified (use pass by value).
Best practice

Use pass by (const) reference instead of pass by value for structs and classes and other expensive-to-copy types.


---


There is one more way to pass variables to functions, and that is by address. Passing an argument by address involves passing the address of the argument variable rather than the argument variable itself. Because the argument is an address, the function parameter must be a pointer. The function can then dereference the pointer to access or change the value being pointed to.

Here is an example of a function that takes a parameter passed by address:

#include <iostream>

void foo(int* ptr)
{
    *ptr = 6;
}

int main()
{
    int value{ 5 };

    std::cout << "value = " << value << '\n';
    foo(&value);
    std::cout << "value = " << value << '\n';
    return 0;
}
The above snippet prints:

value = 5
value = 6
As you can see, the function foo() changed the value of the argument (variable value) through pointer parameter ptr.

Pass by address is typically used with pointers, which most often are used to point to built-in arrays. For example, the following function will print all the values in an array:

void printArray(int* array, int length)
{
    for (int index{ 0 }; index < length; ++index)
    {
        std::cout << array[index] << ' ';
    }
}
Here is an example program that calls this function:

int main()
{
    int array[6]{ 6, 5, 4, 3, 2, 1 }; // remember, arrays decay into pointers
    printArray(array, 6); // so array evaluates to a pointer to the first element of the array here, no & needed
}
This program prints the following:

6 5 4 3 2 1
Remember that fixed arrays decay into pointers when passed to a function, so we have to pass the length as a separate parameter.

It is always a good idea to ensure parameters passed by address are not null pointers before dereferencing them. Dereferencing a null pointer will typically cause the program to crash. Here is our printArray() function with a null pointer check:

void printArray(int* array, int length)
{
    // if user passed in a null pointer for array, bail out early!
    if (!array)
        return;

    for (int index{ 0 }; index < length; ++index)
        std::cout << array[index] << ' ';
}

int main()
{
    int array[6]{ 6, 5, 4, 3, 2, 1 };
    printArray(array, 6);
}
Passing by const address

Because printArray() doesn’t modify any of its arguments, it’s good form to make the array parameter const:

void printArray(const int* array, int length)
{
    // if user passed in a null pointer for array, bail out early!
    if (!array)
        return;

    for (int index{ 0 }; index < length; ++index)
        std::cout << array[index] << ' ';
}

int main()
{
    int array[6]{ 6, 5, 4, 3, 2, 1 };
    printArray(array, 6);
}
This allows us to tell at a glance that printArray() won’t modify the array argument passed in, and will ensure we don’t do so by accident.

Addresses are actually passed by value

When you pass a pointer to a function, the pointer’s value (the address it points to) is copied from the argument to the function’s parameter. In other words, it’s passed by value! If you change the function parameter’s value, you are only changing a copy. Consequently, the original pointer argument will not be changed.

Here’s a sample program that illustrates this.

#include <iostream>

void setToNull(int* tempPtr)
{
    // we're making tempPtr point at something else, not changing the value that tempPtr points to.
    tempPtr = nullptr; // use 0 instead if not C++11
}

int main()
{
    // First we set ptr to the address of five, which means *ptr = 5
    int five{ 5 };
    int* ptr{ &five };

    // This will print 5
    std::cout << *ptr;

    // tempPtr will receive a copy of ptr
    setToNull(ptr);

    // ptr is still set to the address of five!

    // This will print 5
    if (ptr)
        std::cout << *ptr;
    else
        std::cout << " ptr is null";

    return 0;
}
tempPtr receives a copy of the address that ptr is holding. Even though we change tempPtr to point at something else (nullptr), this does not change the value that ptr points to. Consequently, this program prints:

55
Note that even though the address itself is passed by value, you can still dereference that address to change the argument’s value. This is a common point of confusion, so let’s clarify:

When passing an argument by address, the function parameter variable receives a copy of the address from the argument. At this point, the function parameter and the argument both point to the same value.
If the function parameter is then dereferenced to change the value being pointed to, that will impact the value the argument is pointing to, since both the function parameter and argument are pointing to the same value!
If the function parameter is assigned a different address, that will not impact the argument, since the function parameter is a copy, and changing the copy won’t impact the original. After changing the function parameter’s address, the function parameter and argument will point to different values, so dereferencing the parameter and changing the value will no longer affect the value pointed to by the argument.
The following program illustrates the point:

#include <iostream>

void setToSix(int* tempPtr)
{
    *tempPtr = 6; // we're changing the value that tempPtr (and ptr) points to
}

int main()
{
    // First we set ptr to the address of five, which means *ptr = 5
    int five{ 5 };
    int* ptr{ &five };

    // This will print 5
    std::cout << *ptr;

    // tempPtr will receive a copy of ptr
    setToSix(ptr);

    // tempPtr changed the value being pointed to to 6, so ptr is now pointing to the value 6

    // This will print 6
    if (ptr)
        std::cout << *ptr;
    else
        std::cout << " ptr is null";

    return 0;
}
This prints:

56
Passing addresses by reference

The next logical question is, “What if we want to change the address an argument points to from within the function?”. Turns out, this is surprisingly easy. You can simply pass the address by reference. The syntax for doing a reference to a pointer is a little strange (and easy to get backwards). However, if you do get it backwards, the compiler will give you an error.

The following program illustrates using a reference to a pointer:

#include <iostream>

// tempPtr is now a reference to a pointer, so any changes made to tempPtr will change the argument as well!
void setToNull(int*& tempPtr)
{
    tempPtr = nullptr; // use 0 instead if not C++11
}

int main()
{
    // First we set ptr to the address of five, which means *ptr = 5
    int five{ 5 };
    int* ptr{ &five };

    // This will print 5
    std::cout << *ptr;

    // tempPtr is set as a reference to ptr
    setToNull(ptr);

    // ptr has now been changed to nullptr!

    if (ptr)
        std::cout << *ptr;
    else
        std::cout << " ptr is null";

    return 0;
}
When we run the program again with this version of the function, we get:

5 ptr is null
Which shows that calling setToNull() did indeed change the value of ptr from &five to nullptr!

There is only pass by value

Now that you understand the basic differences between passing by reference, address, and value, let’s get reductionist for a moment. :)

In the lesson 10.16 -- Reference variables, we briefly mentioned that references are typically implemented by the compiler as pointers. This means that behind the scenes, pass by reference is essentially just a pass by address (with access to the reference doing an implicit dereference).

And just above, we showed that pass by address is actually just passing an address by value!

Therefore, we can conclude that C++ really passes everything by value! The properties of pass by address (and reference) come solely from the fact that we can dereference the passed address to change the argument, which we can not do with a normal value parameter!

Pass by address makes modifiable parameters explicit

Consider the following example:

int foo1(int x); // pass by value
int foo2(int& x); // pass by reference
int foo3(int* x); // pass by address

int i {};

foo1(i);  // can't modify i
foo2(i);  // can modify i
foo3(&i); // can modify i
It’s not obvious from the call to foo2() that the function can modify variable i, is it?

For this reason, some guides recommend passing all modifiable arguments by address, so that it’s more obvious from an existing function call that an argument could be modified.

However, this comes with its own set of downsides: the caller might think they can pass in nullptr when they aren’t supposed to, and you now have to rigorously check for null pointers.

We lean towards the recommendation of passing non-optional modifiable parameters by reference. Even better, avoid modifiable parameters altogether.

Pros and cons of pass by address

Advantages of passing by address:

Pass by address allows a function to change the value of the argument, which is sometimes useful. Otherwise, const can be used to guarantee the function won’t change the argument. (However, if you want to do this with a non-pointer, you should use pass by reference instead).
Because a copy of the argument is not made, it is fast, even when used with large structs or classes.
We can return multiple values from a function via out parameters.
Disadvantages of passing by address:

Because literals (excepting C-style string literals) and expressions do not have addresses, pointer arguments must be normal variables.
All values must be checked to see whether they are null. Trying to dereference a null value will result in a crash. It is easy to forget to do this.
Because dereferencing a pointer is slower than accessing a value directly, accessing arguments passed by address is slower than accessing arguments passed by value.
When to use pass by address:

When passing built-in arrays (if you’re okay with the fact that they’ll decay into a pointer).
When passing a pointer and nullptr is a valid argument logically.
When not to use pass by address:

When passing a pointer and nullptr is not a valid argument logically (use pass by reference).
When passing structs or classes (use pass by reference).
When passing fundamental types (use pass by value).
As you can see, pass by address and pass by reference have almost identical advantages and disadvantages. Because pass by reference is generally safer than pass by address, pass by reference should be preferred in most cases.
