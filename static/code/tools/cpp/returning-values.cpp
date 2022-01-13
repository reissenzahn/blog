



Return by value

// returning a value by value results in a copy of the value being returned to the caller

// you can return by value literals (e.g. 5), variables (e.g. x), or expressions (e.g. x+1)


// Another advantage of return by value is that you can return variables (or expressions) that involve local variables declared within the function without having to worry about scoping issues. Because the variables are evaluated before the function returns, and a copy of the value is returned to the caller, there are no problems when the function’s variable goes out of scope at the end of the function.

int doubleValue(int x)
{
    int value{ x * 2 };
    return value; // A copy of value will be returned here
} // value goes out of scope here


Return by value is the most appropriate when returning variables that were declared inside the function, or for returning function arguments that were passed by value. However, like pass by value, return by value is slow for structs and large classes.


When to use return by value:

When returning variables that were declared inside the function
When returning function arguments that were passed by value


When not to use return by value:

When returning a built-in array or pointer (use return by address)
When returning a large struct or class (use return by reference)


Return by address

// returning by address involves returning the address of a variable to the caller. Similar to pass by address, return by address can only return the address of a variable, not a literal or an expression (which don’t have addresses). Because return by address just copies an address from the function to the caller, return by address is fast.

However, return by address has one additional downside that return by value doesn’t -- if you try to return the address of a variable local to the function, your program will exhibit undefined behavior. Consider the following example:

int* doubleValue(int x)
{
    int value{ x * 2 };
    return &value; // return value by address here
} // value destroyed here
As you can see here, value is destroyed just after its address is returned to the caller. The end result is that the caller ends up with the address of non-allocated memory (a dangling pointer), which will cause problems if used. This is a common mistake that new programmers make. Many newer compilers will give a warning (not an error) if the programmer tries to return a local variable by address -- however, there are quite a few ways to trick the compiler into letting you do something illegal without generating a warning, so the burden is on the programmer to ensure the pointer they are returning will point to a valid variable after the function returns.

Return by address was often used to return dynamically allocated memory to the caller:

int* allocateArray(int size)
{
    return new int[size];
}

int main()
{
    int* array{ allocateArray(25) };

    // do stuff with array

    delete[] array;
    return 0;
}
This works because dynamically allocated memory is not destroyed at the end of the block in which it is allocated, so that memory will still exist when the address is returned back to the caller. Keeping track of manual allocations can be difficult. Separating the allocation and deletion into different functions makes it even harder to understand who’s responsible for deleting the resource or if the resource needs to be deleted at all. Smart pointers (covered later) and types that clean up after themselves should be used instead of manual allocations.

When to use return by address:

When returning dynamically allocated memory and you can’t use a type that handles allocations for you
When returning function arguments that were passed by address
When not to use return by address:

When returning variables that were declared inside the function or parameters that were passed by value (use return by value)
When returning a large struct or class that was passed by reference (use return by reference)





// returning a variable by reference results in a reference to the variable being passed ack to the caller

// the caller can then use this reference to continue modifying the variable

// you should not return a reference to a local variable as it will go out of scope at the end of the function and you will end up returning a dangling reference


int& doubleValue(int x)
{
    int value{ x * 2 };
    return value; // return a reference to value here
} // value is destroyed here


// return by reference is typically used to return arguments passed by reference to the function back to the caller

#include <array>
#include <iostream>

// Returns a reference to the index element of array
int& getElement(std::array<int, 25>& array, int index)
{
    // we know that array[index] will not be destroyed when we return to the caller (since the caller passed in the array in the first place!)
    // so it's okay to return it by reference
    return array[index];
}

int main()
{
    std::array<int, 25> array;

    // Set the element of array with index 10 to the value 5
    getElement(array, 10) = 5;

    std::cout << array[10] << '\n';

    return 0;
}
This prints:

5
When we call getElement(array, 10), getElement() returns a reference to the array element with index 10. main() then uses this reference to assign that element the value 5.

Although this is somewhat of a contrived example (because you can access array[10] directly), once you learn about classes you will find a lot more uses for returning values by reference.

When to use return by reference:

When returning a reference parameter
When returning a member of an object that was passed into the function by reference or address
When returning a large struct or class that will not be destroyed at the end of the function (e.g. one that was passed in by reference)
When not to use return by reference:

When returning variables that were declared inside the function or parameters that were passed by value (use return by value)
When returning a built-in array or pointer value (use return by address)
Mixing return references and values

Although a function may return a value or a reference, the caller may or may not assign the result to a variable or reference accordingly. Let’s look at what happens when we mix value and reference types.

int returnByValue()
{
    return 5;
}

int& returnByReference()
{
     static int x{ 5 }; // static ensures x isn't destroyed when the function ends
     return x;
}

int main()
{
    int giana{ returnByReference() }; // case A -- ok, treated as return by value
    int& ref{ returnByValue() }; // case B -- compile error since the value is an r-value, and an r-value can't bind to a non-const reference
    const int& cref{ returnByValue() }; // case C -- ok, the lifetime of the return value is extended to the lifetime of cref

    return 0;
}
In case A, we’re assigning a reference return value to a non-reference variable. Because giana isn’t a reference, the return value is copied into giana, as if returnByReference() had returned by value.

In case B, we’re trying to initialize reference ref with the copy of the return value returned by returnByValue(). However, because the value being returned doesn’t have an address (it’s an r-value), this will cause a compile error.

In case C, we’re trying to initialize const reference cref with the copy of the return value returned by returnByValue(). Because const references can bind to r-values, there’s no problem here. Normally, r-values expire at the end of the expression in which they are created -- however, when bound to a const reference, the lifetime of the r-value (in this case, the return value of the function) is extended to match the lifetime of the reference (in this case, cref)

Lifetime extension doesn’t save dangling references

Consider the following program:

const int& returnByReference()
{
     return 5;
}

int main()
{
    const int& ref { returnByReference() }; // runtime error
}
In the above program, returnByReference() is returning a const reference to a value that will go out of scope when the function ends. This is normally a no-no, as it will result in a dangling reference. However, we also know that initializing a const reference with an r-value can extend the lifetime of that value. So which takes precedence here? Does 5 go out of scope first, or does ref extend the lifetime of 5?

The answer is that 5 goes out of scope at the end of the function (resulting in the function returning a dangling reference), and ref is bound to that dangling reference.

However, the following does work as expected:

int returnByValue()
{
     return 5;
}

int main()
{
    const int& ref { returnByValue() }; // ok, we're extending the lifetime of the copy passed back to main
}
In this case, the literal value 5 is first copied back into the scope of the caller (main), and then ref extends the lifetime of that copy.

Returning multiple values

C++ doesn’t contain a direct method for passing multiple values back to the caller. While you can sometimes restructure your code in such a way that you can pass back each data item separately (e.g. instead of having a single function return two values, have two functions each return a single value), this can be cumbersome and unintuitive.

Fortunately, there are several indirect methods that can be used.

As covered in lesson 11.3 -- Passing arguments by reference, out parameters provide one method for passing multiple bits of data back to the caller. We don’t recommend this method.

A second method involves using a data-only struct:

#include <iostream>

struct S
{
	int m_x;
	double m_y;
};

S returnStruct()
{
	S s;
	s.m_x = 5;
	s.m_y = 6.7;
	return s;
}

int main()
{
	S s{ returnStruct() };
	std::cout << s.m_x << ' ' << s.m_y << '\n';

	return 0;
}
A third way is to use std::tuple. A tuple is a sequence of elements that may be different types, where the type of each element must be explicitly specified.

Here’s an example that returns a tuple, and uses std::get to get the nth element of the tuple (counting from 0):

#include <tuple>
#include <iostream>

std::tuple<int, double> returnTuple() // return a tuple that contains an int and a double
{
	return { 5, 6.7 };
}

int main()
{
	std::tuple s{ returnTuple() }; // get our tuple
	std::cout << std::get<0>(s) << ' ' << std::get<1>(s) << '\n'; // use std::get<n> to get the nth element of the tuple (counting from 0)

	return 0;
}
This works identically to the prior example.

You can also use std::tie to unpack the tuple into predefined variables, like so:

#include <tuple>
#include <iostream>

std::tuple<int, double> returnTuple() // return a tuple that contains an int and a double
{
	return { 5, 6.7 };
}

int main()
{
	int a;
	double b;
	std::tie(a, b) = returnTuple(); // put elements of tuple in variables a and b
	std::cout << a << ' ' << b << '\n';

	return 0;
}

As of C++17, a structured binding declaration can be used to simplify splitting multiple returned values into separate variables:

#include <tuple>
#include <iostream>

std::tuple<int, double> returnTuple() // return a tuple that contains an int and a double
{
	return { 5, 6.7 };
}

int main()
{
	auto [a, b]{ returnTuple() }; // used structured binding declaration to put results of tuple in variables a and b
	std::cout << a << ' ' << b << '\n';

	return 0;
}
Using a struct is a better option than a tuple if you’re using the struct in multiple places. However, for cases where you’re just packaging up these values to return and there would be no reuse from defining a new struct, a tuple is a bit cleaner since it doesn’t introduce a new user-defined data type.


