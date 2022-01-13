


Exception handling provides a mechanism to decouple handling of errors or other exceptional circumstances from the typical control flow of your code


// a throw statement is used to signal that an exception has occurred

// a value of any data type can be used to signal that an exception has occurred though typically this value will be an error code, a description of the problem or a custom exception class

throw -1; // throw a literal integer value
throw "Can not take square root of negative number"; // throw a literal C-style (const char*) string
throw MyException("Fatal Error"); // Throw an object of class MyException


// a try block acts as an observer, looking for any exceptions that are thrown by any of the statements within the try block

// a catch block handles exceptions for a single data type

try {
  throw -1;
} catch (int x) {
  std::cerr << "We caught an int exception with value" << x << '\n';
}


// a try block detects any exceptions that are thrown by statements within the try block and routes them to the appropriate catch block for handling

// a try block must have at least one catch block immediately following it and may have multiple catch blocks listed in sequence

// once an exception has been caught by the try block and routed to a catch block for handling, the exception is considered handled and execution will resume as normal after the catch block

// catch parameters work just like function parameters, with the parameter being available within the subsequent catch block

// exceptions of fundamental types can be caught by value, but exceptions of non-fundamental types should be caught by const reference to avoid making an unnecessary copy

// if the parameter is not going to be used in the catch block, the variable name can be omitted:

catch (double)
{
    // Handle exception of type double here
    std::cerr << "We caught an exception of type double" << '\n';
}


#include <iostream>
#include <string>

int main()
{
    try
    {
        // Statements that may throw exceptions you want to handle go here
        throw -1; // here's a trivial example
    }
    catch (int x)
    {
        // Any exceptions of type int thrown within the above try block get sent here
        std::cerr << "We caught an int exception with value: " << x << '\n';
    }
    catch (double) // no variable name since we don't use the exception itself in the catch block below
    {
        // Any exceptions of type double thrown within the above try block get sent here
        std::cerr << "We caught an exception of type double" << '\n';
    }
    catch (const std::string& str) // catch classes by const reference
    {
        // Any exceptions of type std::string thrown within the above try block get sent here
        std::cerr << "We caught an exception of type std::string" << '\n';
    }

    std::cout << "Continuing on our merry way\n";

    return 0;
}


A throw statement was used to raise an exception with the value -1, which is of type int. The throw statement was then caught by the enclosing try block, and routed to the appropriate catch block that handles exceptions of type int. This catch block printed the appropriate error message.

Once the exception was handled, the program continued as normal after the catch blocks, printing “Continuing on our merry way”.

Recapping exception handling

Exception handling is actually quite simple, and the following two paragraphs cover most of what you need to remember about it:

When an exception is raised (using throw), execution of the program immediately jumps to the nearest enclosing try block (propagating up the stack if necessary to find an enclosing try block -- we’ll discuss this in more detail next lesson). If any of the catch handlers attached to the try block handle that type of exception, that handler is executed and the exception is considered handled.

If no appropriate catch handlers exist, execution of the program propagates to the next enclosing try block. If no appropriate catch handlers can be found before the end of the program, the program will fail with an exception error.

Note that the compiler will not perform implicit conversions or promotions when matching exceptions with catch blocks! For example, a char exception will not match with an int catch block. An int exception will not match a float catch block. However, casts from a derived class to one of its parent classes will be performed.

That’s really all there is to it. The rest of this chapter will be dedicated to showing examples of these principles at work.

Exceptions are handled immediately

Here’s a short program that demonstrates how exceptions are handled immediately:

#include <iostream>

int main()
{
    try
    {
        throw 4.5; // throw exception of type double
        std::cout << "This never prints\n";
    }
    catch(double x) // handle exception of type double
    {
        std::cerr << "We caught a double of value: " << x << '\n';
    }

    return 0;
}
This program is about as simple as it gets. Here’s what happens: the throw statement is the first statement that gets executed -- this causes an exception of type double to be raised. Execution immediately moves to the nearest enclosing try block, which is the only try block in this program. The catch handlers are then checked to see if any handler matches. Our exception is of type double, so we’re looking for a catch handler of type double. We have one, so it executes.

Consequently, the result of this program is as follows:

We caught a double of value: 4.5
Note that “This never prints” is never printed, because the exception caused the execution path to jump immediately to the exception handler for doubles.

A more realistic example

Let’s take a look at an example that’s not quite so academic:

#include <cmath> // for sqrt() function
#include <iostream>

int main()
{
    std::cout << "Enter a number: ";
    double x {};
    std::cin >> x;

    try // Look for exceptions that occur within try block and route to attached catch block(s)
    {
        // If the user entered a negative number, this is an error condition
        if (x < 0.0)
            throw "Can not take sqrt of negative number"; // throw exception of type const char*

        // Otherwise, print the answer
        std::cout << "The sqrt of " << x << " is " << std::sqrt(x) << '\n';
    }
    catch (const char* exception) // catch exceptions of type const char*
    {
        std::cerr << "Error: " << exception << '\n';
    }
}
In this code, the user is asked to enter a number. If they enter a positive number, the if statement does not execute, no exception is thrown, and the square root of the number is printed. Because no exception is thrown in this case, the code inside the catch block never executes. The result is something like this:

Enter a number: 9
The sqrt of 9 is 3
If the user enters a negative number, we throw an exception of type const char*. Because we’re within a try block and a matching exception handler is found, control immediately transfers to the const char* exception handler. The result is:

Enter a number: -4
Error: Can not take sqrt of negative number
By now, you should be getting the basic idea behind exceptions. In the next lesson, we’ll do quite a few more examples to show how flexible exceptions are.

What catch blocks typically do

If an exception is routed to a catch block, it is considered “handled” even if the catch block is empty. However, typically you’ll want your catch blocks to do something useful. There are three common things that catch blocks do when they catch an exception:

First, catch blocks may print an error (either to the console, or a log file).

Second, catch blocks may return a value or error code back to the caller.

Third, a catch block may throw another exception. Because the catch block is outside of the try block, the newly thrown exception in this case is not handled by the preceding try block -- it’s handled by the next enclosing try block.


---

One of the most useful properties of exception handling is that the throw statements do NOT have to be placed directly inside a try block due to the way exceptions propagate up the stack when thrown.

#include <cmath> // for sqrt() function
#include <iostream>

// A modular square root function
double mySqrt(double x)
{
    // If the user entered a negative number, this is an error condition
    if (x < 0.0)
        throw "Can not take sqrt of negative number"; // throw exception of type const char*

    return std::sqrt(x);
}

int main()
{
    std::cout << "Enter a number: ";
    double x {};
    std::cin >> x;

    try // Look for exceptions that occur within try block and route to attached catch block(s)
    {
        double d = mySqrt(x);
        std::cout << "The sqrt of " << x << " is " << d << '\n';
    }
    catch (const char* exception) // catch exceptions of type const char*
    {
        std::cerr << "Error: " << exception << std::endl;
    }

    return 0;
}

In this program, we’ve taken the code that checks for an exception and calculates the square root and put it inside a modular function called mySqrt(). We’ve then called this mySqrt() function from inside a try block. Let’s verify that it still works as expected:

Enter a number: -4
Error: Can not take sqrt of negative number
It does!

Let’s revisit for a moment what happens when an exception is raised. First, the program looks to see if the exception can be handled immediately (which means it was thrown inside a try block). If not, the current function is terminated, and the program checks to see if the function’s caller will handle the exception. If not, it terminates the caller and checks the caller’s caller. Each function is terminated in sequence until a handler for the exception is found, or until main() is terminated without the exception being handled. This process is called unwinding the stack (see the lesson on the stack and the heap if you need a refresher on what the call stack is).

Now, let’s take a detailed look at how that applies to this program when an exception is raised from within mySqrt(). First, the program checks to see if the exception was thrown from within a try block inside the function. In this case, it was not. Then, the stack begins to unwind. First, mySqrt() terminates, and control returns to main(). The program now checks to see if we’re inside a try block. We are, and there’s a const char* handler, so the exception is handled by the try block within main().

To summarize, mySqrt() raised the exception, but the try/catch block in main() was the one who captured and handled the exception. Or, put another way, try blocks catch exceptions not only from statements within the try block, but also from functions that are called within the try block.

The most interesting part of the above program is that the mySqrt() function can throw an exception, but this exception is not immediately inside of a try block! This essentially means mySqrt is willing to say, “Hey, there’s a problem!”, but is unwilling to handle the problem itself. It is, in essence, delegating the responsibility for handling the exception to its caller (the equivalent of how using a return code passes the responsibility of handling an error back to a function’s caller).

At this point, some of you are probably wondering why it’s a good idea to pass errors back to the caller. Why not just make MySqrt() handle its own error? The problem is that different applications may want to handle errors in different ways. A console application may want to print a text message. A windows application may want to pop up an error dialog. In one application, this may be a fatal error, and in another application it may not be. By passing the error back up the stack, each application can handle an error from mySqrt() in a way that is the most context appropriate for it! Ultimately, this keeps mySqrt() as modular as possible, and the error handling can be placed in the less-modular parts of the code.

Another stack unwinding example

Here’s another example showing stack unwinding in practice, using a larger stack. Although this program is long, it’s pretty simple: main() calls first(), first() calls second(), second() calls third(), third() calls last(), and last() throws an exception.

#include <iostream>

void last() // called by third()
{
    std::cout << "Start last\n";
    std::cout << "last throwing int exception\n";
    throw -1;
    std::cout << "End last\n";

}

void third() // called by second()
{
    std::cout << "Start third\n";
    last();
    std::cout << "End third\n";
}

void second() // called by first()
{
    std::cout << "Start second\n";
    try
    {
        third();
    }
    catch(double)
    {
         std::cerr << "second caught double exception\n";
    }
    std::cout << "End second\n";
}

void first() // called by main()
{
    std::cout << "Start first\n";
    try
    {
        second();
    }
    catch (int)
    {
         std::cerr << "first caught int exception\n";
    }
    catch (double)
    {
         std::cerr << "first caught double exception\n";
    }
    std::cout << "End first\n";
}

int main()
{
    std::cout << "Start main\n";
    try
    {
        first();
    }
    catch (int)
    {
         std::cerr << "main caught int exception\n";
    }
    std::cout << "End main\n";

    return 0;
}
Take a look at this program in more detail, and see if you can figure out what gets printed and what doesn’t when it is run. The answer follows:

Start main
Start first
Start second
Start third
Start last
last throwing int exception
first caught int exception
End first
End main
Let’s examine what happens in this case. The printing of all the “Start” statements is straightforward and doesn’t warrant further explanation. Function last() prints “last throwing int exception” and then throws an int exception. This is where things start to get interesting.

Because last() doesn’t handle the exception itself, the stack begins to unwind. Function last() terminates immediately and control returns to the caller, which is third().

Function third() doesn’t handle any exceptions, so it terminates immediately and control returns to second().

Function second() has a try block, and the call to third() is within it, so the program attempts to match the exception with an appropriate catch block. However, there are no handlers for exceptions of type int here, so second() terminates immediately and control returns to first(). Note that the integer exception is not implicitly converted to match the catch block handling a double.

Function first() also has a try block, and the call to second() is within it, so the program looks to see if there is a catch handler for int exceptions. There is! Consequently, first() handles the exception, and prints “first caught int exception”.

Because the exception has now been handled, control continues normally at the end of the catch block within first(). This means first() prints “End first” and then terminates normally.

Control returns to main(). Although main() has an exception handler for int, our exception has already been handled by first(), so the catch block within main() does not get executed. main() simply prints “End main” and then terminates normally.

There are quite a few interesting principles illustrated by this program:

First, the immediate caller of a function that throws an exception doesn’t have to handle the exception if it doesn’t want to. In this case, third() didn’t handle the exception thrown by last(). It delegated that responsibility to one of its callers up the stack.

Second, if a try block doesn’t have a catch handler for the type of exception being thrown, stack unwinding occurs just as if there were no try block at all. In this case, second() didn’t handle the exception either because it didn’t have the right kind of catch block.

Third, once an exception is handled, control flow proceeds as normal starting from the end of the catch blocks. This was demonstrated by first() handling the error and then terminating normally. By the time the program got back to main(), the exception had been thrown and handled already -- main() had no idea there even was an exception at all!

As you can see, stack unwinding provides us with some very useful behavior -- if a function does not want to handle an exception, it doesn’t have to. The exception will propagate up the stack until it finds someone who will! This allows us to decide where in the call stack is the most appropriate place to handle any errors that may occur.

In the next lesson, we’ll take a look at what happens when you don’t capture an exception, and a method to prevent that from happening.





















---

all functions are classified as either non-throwing (do not throw exceptions) or potentially throwing (may throw an exception)

looking at a typical function declaration, it is not possible to determine whether a function might throw an exception or not

the noexcept specifier defines a function as non-throwing

void doSomething() noexcept; // this function is non-throwing

Note that noexcept doesn’t actually prevent the function from throwing exceptions or calling other functions that are potentially throwing. Rather, when an exception is thrown, if an exception exits a noexcept function, std::terminate will be called. And note that if std::terminate is called from inside a noexcept function, stack unwinding may or may not occur (depending on implementation and optimizations), which means your objects may or may not be destructed properly prior to termination.

Much like functions that differ only in their return values can not be overloaded, functions differing only in their exception specification can not be overloaded.

The noexcept specifier with a Boolean parameter

The noexcept specifier has an optional Boolean parameter. noexcept(true) is equivalent to noexcept, meaning the function is non-throwing. noexcept(false) means the function is potentially throwing. These parameters are typically only used in template functions, so that a template function can be dynamically created as non-throwing or potentially throwing based on some parameterized value.

Which functions are non-throwing and potentially-throwing

Functions that are non-throwing by default:

default constructors
copy constructors
move constructors
destructors
copy assignment operators
move assignment operators
However, if any of the listed functions call (explicitly or implicitly) another function which is potentially throwing, then the listed function will be treated as potentially throwing as well. For example, if a class has a data member with a potentially throwing constructor, then the class’s constructors will be treated as potentially throwing as well. As another example, if a copy assignment operator calls a potentially throwing assignment operator, then the copy assignment will be potentially throwing as well.

Best practice

If you want any of the above listed functions to be non-throwing, explicitly tag them as noexcept (even though they are defaulted that way), to ensure they don’t inadvertently become potentially throwing.

The following are potentially throwing by default:

Normal functions
User-defined constructors
Some operators, such as new
The noexcept operator

The noexcept operator can be used inside functions. It takes an expression as an argument, and returns true or false if the compiler thinks it will throw an exception or not. The noexcept operator is checked statically at compile-time, and doesn’t actually evaluate the input expression.

void foo() {throw -1;}
void boo() {};
void goo() noexcept {};
struct S{};

constexpr bool b1{ noexcept(5 + 3) }; // true; ints are non-throwing
constexpr bool b2{ noexcept(foo()) }; // false; foo() throws an exception
constexpr bool b3{ noexcept(boo()) }; // false; boo() is implicitly noexcept(false)
constexpr bool b4{ noexcept(goo()) }; // true; goo() is explicitly noexcept(true)
constexpr bool b5{ noexcept(S{}) };   // true; a struct's default constructor is noexcept by default
The noexcept operator can be used to conditionally execute code depending on whether it is potentially throwing or not. This is required to fulfill certain exception safety guarantees, which we’ll talk about in the next section.

Exception safety guarantees

An exception safety guarantee is a contractual guideline about how functions or classes will behave in the event an exception occurs. There are four levels of exception safety:

No guarantee -- There are no guarantees about what will happen if an exception is thrown (e.g. a class may be left in an unusable state)
Basic guarantee -- If an exception is thrown, no memory will be leaked and the object is still usable, but the program may be left in a modified state.
Strong guarantee -- If an exception is thrown, no memory will be leaked and the program state will not be changed. This means the function must either completely succeed or have no side effects if it fails. This is easy if the failure happens before anything is modified in the first place, but can also be achieved by rolling back any changes so the program is returned to the pre-failure state.
No throw / No fail -- The function will always succeed (no-fail) or fail without throwing an exception (no-throw).
Let’s look at the no-throw/no-fail guarantees is more detail:

The no-throw guarantee: if a function fails, then it won’t throw an exception. Instead, it will return an error code or ignore the problem. No-throw guarantees are required during stack unwinding when an exception is already being handled; for example, all destructors should have a no-throw guarantee (as should any functions those destructors call). Examples of code that should be no-throw:

destructors and memory deallocation/cleanup functions
functions that higher-level no-throw functions need to call
The no-fail guarantee: a function will always succeed in what it tries to do (and thus never has a need to throw an exception, thus, no-fail is a slightly stronger form of no-throw). Examples of code that should be no-fail:

move constructors and move assignment (move semantics, covered in chapter M)
swap functions
clear/erase/reset functions on containers
operations on std::unique_ptr (also covered in chapter M)
functions that higher-level no-fail functions need to call
When to use noexcept

Just because your code doesn’t explicitly throw any exceptions doesn’t mean you should start sprinkling noexcept around your code. By default, most functions are potentially throwing, so if your function calls other functions, there is a good chance it calls a function that is potentially throwing, and thus is potentially throwing too.

The standard library’s policy is to use noexcept only on functions that must not throw or fail. Functions that are potentially throwing but do not actually throw exceptions (due to implementation) typically are not marked as noexcept.

Best practice

Use the noexcept specifier in specific cases where you want to express a no-fail or no-throw guarantee.

Best practice

If you are uncertain whether a function should have a no-fail/no-throw guarantee, error on the side of caution and do not mark it with noexcept. Reversing a decision to use noexcept violates an interface commitment to the user about the behavior of the function. Making guarantees stronger by retroactively adding noexcept is considered safe.

Why it’s useful to mark functions as non-throwing

There are a few good reasons to mark functions a non-throwing:

Non-throwing functions can be safely called from functions that are not exception-safe, such as destructors
Functions that are noexcept can enable the compiler to perform some optimizations that would not otherwise be available. Because a noexcept function cannot throw an exception outside the function, the compiler doesn’t have to worry about keeping the runtime stack in an unwindable state, which can allow it to produce faster code.
There are also a few cases where knowing a function is noexcept allows us to produce more efficient implementations in our own code: the standard library containers (such as std::vector) are noexcept aware and will use the noexcept operator to determine whether to use move semantics (faster) or copy semantics (slower) in some places (we cover move semantics in chapter M).
Dynamic exception specifications

Optional reading

Before C++11, and until C++17, dynamic exception specifications were used in place of noexcept. The dynamic exception specifications syntax uses the throw keyword to list which exception types a function might directly or indirectly throw:

int doSomething() throw(); // does not throw exceptions
int doSomething() throw(std::out_of_range, int*); // may throw either std::out_of_range or a pointer to an integer
int doSomething() throw(...); // may throw anything
Due to factors such as incomplete compiler implementations, some incompatibility with template functions, common misunderstandings about how they worked, and the fact that the standard library mostly didn’t use them, the dynamic exception specifications were deprecated in C++11 and removed from the language in C++17 and C++20. See this paper for more context.