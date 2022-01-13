

The compiler tool chain is a collection of three elements that run one after the other to convert source code into a program:

1. The preprocessor performs basic source code manipulation. When the preprocessor finishes processing a source file, it produces a single translation unit. Each translation unit is then passed to the compiler for further processing.

2. The compiler reads a translation unit and generates an object file. Object files contain an intermediate format called object code. These files contain data and instructions in an intermediate format. Compilers work on one translation unit at a time so each translation unit corresponds to a single object file.

3. The linker generates programs from object files. Linkers are also responsible for finding the libraries you’ve included within your
source code.

---

l-values and r-values

In C++, variables are a type of l-value. An l-value is a value that has an address (in memory). Since all variables have addresses, all variables are l-values. When we do an assignment, the left hand side of the assignment operator must be an l-value. When an l-value has a value assigned to it, the current value at that memory address is overwritten.

The opposite of l-values are r-values. An r-value is an expression that is not an l-value. Examples of r-values are literals and non-l-value expressions (e.g. 2 + x).

The key takeaway is that on the left side of the assignment, you must have something that represents a memory address (such as a variable). Everything on the right side of the assignment will be evaluated to produce a value.

const variables are considered non-modifiable l-values.

---

Objects are abstractions with state and behavior.
The collection of behaviors and states describing an object is called its
type.

Named objects are called variables.

Object initialization establishes an object’s initial state, such as setting its value.

Conditional statements allow you to make decisions in your programs. These
decisions rest on Boolean expressions, which evaluate to true or false.

Sometimes, you’ll want a group of statements to run rather than a
single statement. Such a group is called a compound statement. To declare a
compound statement, simply wrap the group of statements in braces { }.

Functions are blocks of code that accept any number of input objects called parameters or arguments and can return output objects to their callers.

Parameters represent the objects passed into a function.
A set of braces following that list contains the function’s body. This is a
compound statement that contains the function’s logic. Within this logic, the
function might decide to return a value to the function’s caller. 
Functions
that return values will have one or more return statements.


int step_function(int ux) {
 int result = 0; v
 if (x < 0) {
 result = -1; w
 } else if (x > 0) {
 result = 1; x
 }
 return result; y
}

for zero, and 1 for positive values
The step_function takes a single argument x u. The result variable is
declared and initialized to 0 v. Next, the if statement sets result to -1 w
if x is less than 0. If x is greater than 0, the if statement sets result to 1 x.
Finally, result is returned to the caller y.
18 Chapter 1
Calling Functions
To call (or invoke) a function, you use the name of the desired function,
parentheses, and a comma-separated list of the required parameters. The
compiler reads files from top to bottom, so the function’s declaration must
appear before its point of first use.
Consider the program in Listing 1-7, which uses the step_function.
int step_function(int x) {
 --snip--
}
int main() {
 int value1 = step_function(100); // value1 is 1
 int value2 = step_function(0); // value2 is 0
 int value3 = step_function(-10); // value3 is -1
}
Listing 1-7: A program using the step_function. (This program produces no output.)
Listing 1-7 calls step_function three times with different arguments and
assigns the results to the variables value1, value2, and value3.
Wouldn’t it be nice if you could print these values? Fortunately, you can
use the printf function to build output from different variables. The trick is
to use printf format specifiers.


printf Format Specifiers

#include <cstdio> u

int step_function(int x) { v
 --snip--
}

Finally, you print the returned values by invoking printf. Each invocation starts with a format string, like "Num1: %d, Step: %d\n" z. There are two
%d format specifiers embedded in each format string. Per the requirements
of printf, there are two parameters following the format string, num1 and
result1, that correspond to these two format specifiers.
Up and Running 21

---

Since we’re going to talk a lot about initialization in the next few lessons, let’s first recap the types of initialization that C++ supports: direct initialization, uniform initialization or copy initialization.

#include <cassert>
#include <iostream>

class Fraction
{
private:
    int m_numerator{};
    int m_denominator{};

public:
    // Default constructor
    Fraction(int numerator=0, int denominator=1)
        : m_numerator{numerator}, m_denominator{denominator}
    {
        assert(denominator != 0);
    }

    friend std::ostream& operator<<(std::ostream& out, const Fraction& f1);
};

std::ostream& operator<<(std::ostream& out, const Fraction& f1)
{
	out << f1.m_numerator << '/' << f1.m_denominator;
	return out;
}

int x(5); // Direct initialize an integer
Fraction fiveThirds(5, 3); // Direct initialize a Fraction, calls Fraction(int, int) constructor

int x { 5 }; // Uniform initialization of an integer
Fraction fiveThirds {5, 3}; // Uniform initialization of a Fraction, calls Fraction(int, int) constructor


int x = 6; // Copy initialize an integer
Fraction six = Fraction(6); // Copy initialize a Fraction, will call Fraction(6, 1)
Fraction seven = 7; // Copy initialize a Fraction.  The compiler will try to find a way to convert 7 to a Fraction, which will invoke the Fraction(7, 1) constructor.
