#include <iostream>

// function overloading allows us to create multiple functions with the same name so long as each function has different parameters (or the functions can be otherwise differentiated)

// each function sharing a name (in the same scope) is called an overloaded function
int add(int x, int y)
{
  return x + y;
}

double add(double x, double y)
{
  return x + y;
}


// functions can be overloaded so long as each overloaded function can be differentiated by the compiler

// when a function call is made to function that has been overloaded, the compiler will try to match the function call to the appropriate overload based on the arguments used in the function call. This is called overload resolution.

int main()
{
    std::cout << add(1, 2); // calls add(int, int)
    std::cout << '\n';
    std::cout << add(1.2, 3.4); // calls add(double, double)

    return 0;
}

// If an overloaded function is not differentiated, or if a function call to an overloaded function can not be resolved to an overloaded function, then a compile error will result.


---


// The easiest way to differentiate an overload function is to ensure each overloaded function has a different set (number and/or type) of parameters.

// Note that a function’s return type is not used to differentiate overloaded functions



int add(int x, int y)
{
    return x + y;
}

int add(int x, int y, int z)
{
    return x + y + z;
}

Because type aliases (or typedefs) are not distinct types, overloaded functions using type aliases are not distinct from overloads using the aliased type

typedef int height_t; // typedef
using age_t = int; // type alias

void print(int value);
void print(age_t value); // not differentiated from print(int)
void print(height_t value); // not differentiated from print(int)

For parameters passed by value, the const qualifier is also not considered. Therefore, the following functions are not considered to be differentiated:

void print(int);
void print(const int); // not differentiated from print(int)

ellipsis parameters are considered to be a unique type of parameter

void foo(int x, int y);
void foo(int x, ...); // differentiated from foo(int, int)


---


However, having a set of differentiated overloaded functions is only half of the picture. When any function call is made, the compiler must also ensure that a matching function declaration can be found.

the process of matching function calls to a specific overloaded function is called overload resolution

When a function call is made to an overloaded function, the compiler steps through a sequence of rules to determine which (if any) of the overloaded functions is the best match.

At each step, the compiler applies a bunch of different type conversions to the argument(s) in the function call. For each conversion applied, the compiler checks if any of the overloaded functions are now a match. After all the different type conversions have been applied and checked for matches, the step is done. The result will be one of three possible outcomes:

- No matching functions were found. The compiler moves to the next step in the sequence.
- A single matching function was found. This function is considered to be the best match. The matching process is now complete, and subsequent steps are not executed.
- More than one matching function was found. The compiler will issue an ambiguous match compile error. We’ll discuss this case further in a bit.


Step 1) The compiler tries to find an exact match. This happens in two phases. First, the compiler will see if there is an overloaded function where the type of the arguments in the function call exactly matches the type of the parameters in the overloaded functions.

Second, the compiler will apply a number of trivial conversions to the arguments in the function call. The trivial conversions are a set of specific conversion rules that will modify types (without modifying the value) for purposes of finding a match. For example, a non-const type can be trivially converted to a const type:

void print(const int)
{
}

void print(double)
{
}

int main()
{
    int x { 0 };
    print(x); // x trivially converted to const int

    return 0;
}
In the above example, we’ve called print(x), where x is an int. The compiler will trivially convert x from an int into a const int, which then matches print(const int).

Matches made via the trivial conversions are considered exact matches.

Step 2) If no exact match is found, the compiler tries to find a match by applying numeric promotion to the argument(s). If, after numeric promotion, a match is found, the function call is resolved.

void print(int)
{
}

void print(double)
{
}

int main()
{
    print('a'); // promoted to match print(int)
    print(true); // promoted to match print(int)
    print(4.5f); // promoted to match print(double)

    return 0;
}

Step 3) If no match is found via numeric promotion, the compiler tries to find a match by applying numeric conversions to the arguments.

For example:

#include <string> // for std::string

void print(double)
{
}

void print(std::string)
{
}

int main()
{
    print('a'); // 'a' converted to match print(double)

    return 0;
}

In this case, because there is no print(char) (exact match), and no print(int) (promotion match), the 'a' is numerically converted to a double and matched with print(double).

Matches made by applying numeric promotions take precedence over any matches made by applying numeric conversions.

Step 4) If no match is found via numeric conversion, the compiler tries to find a match through any user-defined conversions.

// We haven't covered classes yet, so don't worry if this doesn't make sense
class X // this defines a new type called X
{
public:
    operator int() { return 0; } // Here's a user-defined conversion from X to int
};

void print(int)
{
}

void print(double)
{
}

int main()
{
    X x; // Here, we're creating an object of type X (named x)
    print(x); // x is converted to type int using the user-defined conversion from X to int

    return 0;
}
In this example, the compiler will first check whether an exact match to print(X) exists. We haven’t defined one. Next the compiler will check whether x can be numerically promoted, which it can’t. The compiler will then check if x can be numerically converted, which it also can’t. Finally, the compiler will then look for any user-defined conversions. Because we’ve defined a user-defined conversion from X to int, the compiler will convert X to an int to match print(int).

After applying a user-defined conversion, the compiler may apply additional implicit promotions or conversions to find a match. So if our user-defined conversion had been to type char instead of int, the compiler would have used the user-defined conversion to char and then promoted the result into an int to match.

Related content

We discuss how to create user-defined conversions for class types (by overloading the typecast operators) in lesson 13.11 -- Overloading typecasts.

For advanced readers

The constructor of a class also acts as a user-defined conversion from other types to that class type, and can be used during this step to find matching functions.

Step 5) If no match is found via user-defined conversion, the compiler will look for a matching function that uses ellipsis.

Related content

We cover ellipses in lesson 11.12 -- Ellipsis (and why to avoid them).

Step 6) If no matches have been found by this point, the compiler gives up and will issue a compile error about not being able to find a matching function.

Ambiguous matches

With non-overloaded functions, each function call will either resolve to a function, or no match will be found and the compiler will issue a compile error:

void foo()
{
}

int main()
{
     foo(); // okay: match found
     goo(); // compile error: no match found

     return 0;
}
With overloaded functions, there is a third possible outcome: an ambiguous match may be found. An ambiguous match occurs when the compiler finds two or more functions that can be made to match in the same step. When this occurs, the compiler will stop matching and issue a compile error stating that it has found an ambiguous function call.

Since every overloaded function must be differentiated in order to compile, you might be wondering how it is possible that a function call could result in more than one match. Let’s take a look at an example that illustrates this:

void print(int x)
{
}

void print(double d)
{
}

int main()
{
    print(5l); // 5l is type long

    return 0;
}
Since literal 5l is of type long, the compiler will first look to see if it can find an exact match for print(long), but it will not find one. Next, the compiler will try numeric promotion, but values of type long can’t be promoted, so there is no match here either.

Following that, the compiler will try to find a match by applying numeric conversions to the long argument. In the process of checking all the numeric conversion rules, the compiler will find two potential matches. If the long argument is numerically converted into an int, then the function call will match print(int). If the long argument is instead converted into a double, then it will match print(double) instead. Since two possible matches via numeric conversion have been found, the function call is considered ambiguous.

On Visual Studio 2019, this results in the following error message:

error C2668: 'print': ambiguous call to overloaded function
message : could be 'void print(double)'
message : or       'void print(int)'
message : while trying to match the argument list '(long)'
Key insight

If the compiler finds multiple matches in a given step, an ambiguous function call will result. This means no match from a given step is considered to be better than any other match from the same step.

Here’s another example that yields ambiguous matches:

void print(unsigned int x)
{
}
void print(float y)
{
}

int main()
{
    print(0); // int can be numerically converted to unsigned int or to float
    print(3.14159); // double can be numerically converted to unsigned int or to float

    return 0;
}
Although you might expect 0 to resolve to print(unsigned int) and 3.14159 to resolve to print(float), both of these calls result in an ambiguous match. The int value 0 can be numerically converted to either an unsigned int or a float, so either overload matches equally well, and the result is an ambiguous function call.

The same applies for the conversion of a double to either a float or unsigned int. Both are numeric conversions, so either overload matches equally well, and the result is again ambiguous.

Resolving ambiguous matches

Because ambiguous matches are a compile-time error, an ambiguous match needs to be disambiguated before your program will compile. There are a few ways to resolve ambiguous matches:

Often, the best way is simply to define a new overloaded function that takes parameters of exactly the type you are trying to call the function with. Then C++ will be able to find an exact match for the function call.
Alternatively, explicitly cast the ambiguous argument(s) to match the type of the function you want to call. For example, to have print(0) match print(unsigned int) in the above example, you would do this:
int x{ 0 };
print(static_cast<unsigned int>(x)); // will call print(unsigned int)
If your argument is a literal, you can use the literal suffix to ensure your literal is interpreted as the correct type:
print(0u); // will call print(unsigned int) since 'u' suffix is unsigned int, so this is now an exact match
The list of the most used suffixes can be found in lesson 4.14 -- Literal constants.

Matching for functions with multiple arguments

If there are multiple arguments, the compiler applies the matching rules to each argument in turn. The function chosen is the one for which each argument matches at least as well as all the other functions, with at least one argument matching better than all the other functions. In other words, the function chosen must provide a better match than all the other candidate functions for at least one parameter, and no worse for all of the other parameters.

In the case that such a function is found, it is clearly and unambiguously the best choice. If no such function can be found, the call will be considered ambiguous (or a non-match).

For example:

#include <iostream>

void print(char c, int x)
{
	std::cout << 'a';
}

void print(char c, double x)
{
	std::cout << 'b';
}

void print(char c, float x)
{
	std::cout << 'c';
}

int main()
{
	print('x', 'a');
}
In the above program, all functions match the first argument exactly. However, the top function matches the second parameter via promotion, whereas the other functions require a conversion. Therefore, print(char, int) is unambiguously the best match.


