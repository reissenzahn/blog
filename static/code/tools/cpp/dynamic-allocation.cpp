#include <iostream>


// the new[] operator is used to dynamically allocate arrays

// dynamically allocating arrays allows us to choose the array size at runtime


int size{};
// std::cin >> size;
size = 10;

int* arr{ new int[size]{} };

std::cout << "I just allocated an array of integers of length " << length << '\n';

array[0] = 5; // set element 0 to value 5

delete[] array; // use array delete to deallocate array




Because we are allocating an array, C++ knows that it should use the array version of new instead of the scalar version of new. Essentially, the new[] operator is called, even though the [] isn’t placed next to the new keyword.

The length of dynamically allocated arrays has to be a type that’s convertible to std::size_t. In practice, using an int length is fine, since int will convert to std::size_t.



Dynamically deleting arrays

When deleting a dynamically allocated array, we have to use the array version of delete, which is delete[].

In lesson 10.10 -- Pointers and arrays, you learned that a fixed array holds the memory address of the first array element. You also learned that a fixed array can decay into a pointer that points to the first element of the array. In this decayed form, the length of the fixed array is not available (and therefore neither is the size of the array via sizeof()), but otherwise there is little difference.

A dynamic array starts its life as a pointer that points to the first element of the array. Consequently, it has the same limitations in that it doesn’t know its length or size. A dynamic array functions identically to a decayed fixed array, with the exception that the programmer is responsible for deallocating the dynamic array via the delete[] keyword.


Initializing dynamically allocated arrays

If you want to initialize a dynamically allocated array to 0, the syntax is quite simple:

int* array{ new int[length]{} };
Prior to C++11, there was no easy way to initialize a dynamic array to a non-zero value (initializer lists only worked for fixed arrays). This means you had to loop through the array and assign element values explicitly.

int* array = new int[5];
array[0] = 9;
array[1] = 7;
array[2] = 5;
array[3] = 3;
array[4] = 1;
Super annoying!

However, starting with C++11, it’s now possible to initialize dynamic arrays using initializer lists!

int fixedArray[5] = { 9, 7, 5, 3, 1 }; // initialize a fixed array before C++11
int* array{ new int[5]{ 9, 7, 5, 3, 1 } }; // initialize a dynamic array since C++11
// To prevent writing the type twice, we can use auto. This is often done for types with long names.
auto* array{ new int[5]{ 9, 7, 5, 3, 1 } };
Note that this syntax has no operator= between the array length and the initializer list.

For consistency, fixed arrays can also be initialized using uniform initialization:

int fixedArray[]{ 9, 7, 5, 3, 1 }; // initialize a fixed array in C++11
char fixedArray[]{ "Hello, world!" }; // initialize a fixed array in C++11
Explicitly stating the size of the array is optional.




