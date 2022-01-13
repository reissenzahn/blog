
// a namespace provides a scope region (called namespace scope) to the names declared inside of it

// any name that is not defined inside a class, function, or a namespace is considered to be part of an implicitly defined namespace called the global namespace

// all of the functionality in the standard library is found in the std namespace


Similarly, when accessing an identifier that is defined in a namespace (e.g. std::cout) , you need to tell the compiler that we’re looking for an identifier defined inside the namespace (std).


// an explicit namespace qualifier 

Explicit namespace qualifier std::

The most straightforward way to tell the compiler that we want to use cout from the std namespace is by explicitly using the std:: prefix. For example:

#include <iostream>

int main()
{
    std::cout << "Hello world!"; // when we say cout, we mean the cout defined in the std namespace
    return 0;
}
The :: symbol is an operator called the scope resolution operator. The identifier to the left of the :: symbol identifies the namespace that the name to the right of the :: symbol is contained within. If no identifier to the left of the :: symbol is provided, the global namespace is assumed.

So when we say std::cout, we’re saying “the cout that lives in namespace std“.

This is the safest way to use cout, because there’s no ambiguity about which cout we’re referencing (the one in the std namespace).



Using namespace std (and why to avoid it)

Another way to access identifiers inside a namespace is to use a using directive statement. Here’s our original “Hello world” program with a using directive:

#include <iostream>

using namespace std; // this is a using directive telling the compiler to check the std namespace when resolving identifiers with no prefix

int main()
{
    cout << "Hello world!"; // cout has no prefix, so the compiler will check to see if cout is defined locally or in namespace std
    return 0;
}
A using directive tells the compiler to check a specified namespace when trying to resolve an identifier that has no namespace prefix. So in the above example, when the compiler goes to determine what identifier cout is, it will check both locally (where it is undefined) and in the std namespace (where it will match to std::cout).

Many texts, tutorials, and even some compilers recommend or use a using directive at the top of the program. However, used in this way, this is a bad practice, and highly discouraged.

Consider the following program:

#include <iostream> // imports the declaration of std::cout

using namespace std; // makes std::cout accessible as "cout"

int cout() // declares our own "cout" function in the global namespace
{
    return 5;
}

int main()
{
    cout << "Hello, world!"; // Compile error!  Which cout do we want here?  The one in the std namespace or the one we defined above?

    return 0;
}
The above program doesn’t compile, because the compiler now can’t tell whether we want the cout function that we defined, or the cout that is defined inside the std namespace.

When using a using directive in this manner, any identifier we define may conflict with any identically named identifier in the std namespace. Even worse, while an identifier name may not conflict today, it may conflict with new identifiers added to the std namespace in future language revisions. This was the whole point of moving all of the identifiers in the standard library into the std namespace in the first place!



---


In lesson 2.8 -- Naming collisions and an introduction to namespaces, we introduced the concept of naming collisions and namespaces. As a reminder, a naming collision occurs when two identical identifiers are introduced into the same scope, and the compiler can’t disambiguate which one to use. When this happens, compiler or linker will produce an error because they do not have enough information to resolve the ambiguity. As programs become larger, the number of identifiers increases linearly, which in turn causes the probability of a naming collision occurring to increase exponentially.

Let’s revisit an example of a naming collision, and then show how we can resolve it using namespaces. In the following example, foo.cpp and goo.cpp are the source files that contain functions that do different things but have the same name and parameters.

foo.cpp:

// This doSomething() adds the value of its parameters
int doSomething(int x, int y)
{
    return x + y;
}
goo.cpp:

// This doSomething() subtracts the value of its parameters
int doSomething(int x, int y)
{
    return x - y;
}
main.cpp:

#include <iostream>

int doSomething(int x, int y); // forward declaration for doSomething

int main()
{
    std::cout << doSomething(4, 3) << '\n'; // which doSomething will we get?
    return 0;
}
If this project contains only foo.cpp or goo.cpp (but not both), it will compile and run without incident. However, by compiling both into the same program, we have now introduced two different functions with the same name and parameters into the same scope (the global scope), which causes a naming collision. As a result, the linker will issue an error:

goo.cpp:3: multiple definition of `doSomething(int, int)'; foo.cpp:3: first defined here
Note that this error happens at the point of redefinition, so it doesn’t matter whether function doSomething is ever called.

One way to resolve this would be to rename one of the functions, so the names no longer collide. But this would also require changing the names of all the function calls, which can be a pain, and is subject to error. A better way to avoid collisions is to put your functions into your own namespaces. For this reason the standard library was moved into the std namespace.

Defining your own namespaces

C++ allows us to define our own namespaces via the namespace keyword. Namespaces that you create for your own declarations are called user-defined namespaces. Namespaces provided by C++ (such as the global namespace) or by libraries (such as namespace std) are not considered user-defined namespaces.

Namespace identifiers are typically non-capitalized.

Here is an example of the files in the prior example rewritten using namespaces:

foo.cpp:

namespace foo // define a namespace named foo
{
    // This doSomething() belongs to namespace foo
    int doSomething(int x, int y)
    {
        return x + y;
    }
}
goo.cpp:

namespace goo // define a namespace named goo
{
    // This doSomething() belongs to namespace goo
    int doSomething(int x, int y)
    {
        return x - y;
    }
}
Now doSomething() inside of foo.cpp is inside the foo namespace, and the doSomething() inside of goo.cpp is inside the goo namespace. Let’s see what happens when we recompile our program.

main.cpp:

int doSomething(int x, int y); // forward declaration for doSomething

int main()
{
    std::cout << doSomething(4, 3) << '\n'; // which doSomething will we get?
    return 0;
}
The answer is that we now get another error!

ConsoleApplication1.obj : error LNK2019: unresolved external symbol "int __cdecl doSomething(int,int)" (?doSomething@@YAHHH@Z) referenced in function _main
In this case, the compiler was satisfied (by our forward declaration), but the linker could not find a definition for doSomething in the global namespace. This is because both of our versions of doSomething are no longer in the global namespace!

There are two different ways to tell the compiler which version of doSomething() to use, via the scope resolution operator, or via using statements (which we’ll discuss in a later lesson in this chapter).

For the subsequent examples, we’ll collapse our examples down to a one-file solution for ease of reading.

Accessing a namespace with the scope resolution operator (::)

The best way to tell the compiler to look in a particular namespace for an identifier is to use the scope resolution operator (::). The scope resolution operator tells the compiler that the identifier specified by the right-hand operand should be looked for in the scope of the left-hand operand.

Here is an example of using the scope resolution operator to tell the compiler that we explicitly want to use the version of doSomething() that lives in the foo namespace:

#include <iostream>

namespace foo // define a namespace named foo
{
    // This doSomething() belongs to namespace foo
    int doSomething(int x, int y)
    {
        return x + y;
    }
}

namespace goo // define a namespace named goo
{
    // This doSomething() belongs to namespace goo
    int doSomething(int x, int y)
    {
        return x - y;
    }
}

int main()
{
    std::cout << foo::doSomething(4, 3) << '\n'; // use the doSomething() that exists in namespace foo
    return 0;
}
This produces the expected result:

7
If we wanted to use the version of doSomething() that lives in goo instead:

#include <iostream>

namespace foo // define a namespace named foo
{
    // This doSomething() belongs to namespace foo
    int doSomething(int x, int y)
    {
        return x + y;
    }
}

namespace goo // define a namespace named goo
{
    // This doSomething() belongs to namespace goo
    int doSomething(int x, int y)
    {
        return x - y;
    }
}

int main()
{
    std::cout << goo::doSomething(4, 3) << '\n'; // use the doSomething() that exists in namespace goo
    return 0;
}
This produces the result:

1
The scope resolution operator is great because it allows us to explicitly pick which namespace we want to look in, so there’s no potential ambiguity. We can even do the following:

#include <iostream>

namespace foo // define a namespace named foo
{
    // This doSomething() belongs to namespace foo
    int doSomething(int x, int y)
    {
        return x + y;
    }
}

namespace goo // define a namespace named goo
{
    // This doSomething() belongs to namespace goo
    int doSomething(int x, int y)
    {
        return x - y;
    }
}

int main()
{
    std::cout << foo::doSomething(4, 3) << '\n'; // use the doSomething() that exists in namespace foo
    std::cout << goo::doSomething(4, 3) << '\n'; // use the doSomething() that exists in namespace goo
    return 0;
}
This produces the result:

7
1
Using the scope resolution operator with no name prefix

The scope resolution operator can also be used in front of an identifier without providing a namespace name (e.g. ::doSomething). In such a case, the identifier (e.g. doSomething) is looked for in the global namespace.

#include <iostream>

void print() // this print lives in the global namespace
{
	std::cout << " there\n";
}

namespace foo
{
	void print() // this print lives in the foo namespace
	{
		std::cout << "Hello";
	}
}

int main()
{
	foo::print(); // call print() in foo namespace
	::print(); // call print() in global namespace (same as just calling print() in this case)

	return 0;
}
In the above example, the ::print() performs the same as if we’d called print() with no scope resolution, so use of the scope resolution operator is superfluous in this case. But the next example will show a case where the scope resolution operator with no namespace can be useful.

Identifier resolution from within a namespace

If an identifier inside a namespace is used and no scope resolution is provided, the compiler will first try to find a matching declaration in that same namespace. If no matching identifier is found, the compiler will then check each containing namespace in sequence to see if a match is found, with the global namespace being checked last.

#include <iostream>

void print() // this print lives in the global namespace
{
	std::cout << " there\n";
}

namespace foo
{
	void print() // this print lives in the foo namespace
	{
		std::cout << "Hello";
	}

	void printHelloThere()
	{
		print(); // calls print() in foo namespace
		::print(); // calls print() in global namespace
	}
}

int main()
{
	foo::printHelloThere();

	return 0;
}
This prints:

Hello there
In the above example, print() is called with no scope resolution provided. Because this use of print() is inside the foo namespace, the compiler will first see if a declaration for foo::print() can be found. Since one exists, foo::print() is called.

If foo::print() had not been found, the compiler would have checked the containing namespace (in this case, the global namespace) to see if it could match a print() there.

Note that we also make use of the scope resolution operator with no namespace (::print()) to explicitly call the global version of print().

Multiple namespace blocks are allowed

It’s legal to declare namespace blocks in multiple locations (either across multiple files, or multiple places within the same file). All declarations within the namespace are considered part of the namespace.

circle.h:

#ifndef CIRCLE_H
#define CIRCLE_H

namespace basicMath
{
    constexpr double pi{ 3.14 };
}

#endif
growth.h:

#ifndef GROWTH_H
#define GROWTH_H

namespace basicMath
{
    // the constant e is also part of namespace basicMath
    constexpr double e{ 2.7 };
}

#endif
main.cpp:

#include "circle.h" // for basicMath::pi
#include "growth.h" // for basicMath::e

#include <iostream>

int main()
{
    std::cout << basicMath::pi << '\n';
    std::cout << basicMath::e << '\n';

    return 0;
}
This works exactly as you would expect:

3.14
2.7
The standard library makes extensive use of this feature, as each standard library header file contains its declarations inside a namespace std block contained within that header file. Otherwise the entire standard library would have to be defined in a single header file!

Note that this capability also means you could add your own functionality to the std namespace. Doing so causes undefined behavior most of the time, because the std namespace has a special rule, prohibiting extension from user code.

Warning

Do not add custom functionality to the std namespace.

When you separate your code into multiple files, you’ll have to use a namespace in the header and source file.

add.h

#ifndef ADD_H
#define ADD_H

namespace basicMath
{
    // function add() is part of namespace basicMath
    int add(int x, int y);
}

#endif
add.cpp

#include "add.h"

namespace basicMath
{
    // define the function add()
    int add(int x, int y)
    {
        return x + y;
    }
}
main.cpp

#include "add.h" // for basicMath::add()

#include <iostream>

int main()
{
    std::cout << basicMath::add(4, 3) << '\n';

    return 0;
}
If the namespace is omitted in the source file, the linker won’t find a definition of basicMath::add, because the source file only defines add (global namespace). If the namespace is omitted in the header file, “main.cpp” won’t be able to use basicMath::add, because it only sees a declaration for add (global namespace).

Nested namespaces

Namespaces can be nested inside other namespaces. For example:

#include <iostream>

namespace foo
{
    namespace goo // goo is a namespace inside the foo namespace
    {
        int add(int x, int y)
        {
            return x + y;
        }
    }
}

int main()
{
    std::cout << foo::goo::add(1, 2) << '\n';
    return 0;
}
Note that because namespace goo is inside of namespace foo, we access add as foo::goo::add.

Since C++17, nested namespaces can also be declared this way:

#include <iostream>

namespace foo::goo // goo is a namespace inside the foo namespace (C++17 style)
{
  int add(int x, int y)
  {
    return x + y;
  }
}

int main()
{
    std::cout << foo::goo::add(1, 2) << '\n';
    return 0;
}
Namespace aliases

Because typing the fully qualified name of a variable or function inside a nested namespace can be painful, C++ allows you to create namespace aliases, which allow us to temporarily shorten a long sequence of namespaces into something shorter:

#include <iostream>

namespace foo::goo
{
    int add(int x, int y)
    {
        return x + y;
    }
}

int main()
{
    namespace active = foo::goo; // active now refers to foo::goo

    std::cout << active::add(1, 2) << '\n'; // This is really foo::goo::add()

    return 0;
} // The active alias ends here
One nice advantage of namespace aliases: If you ever want to move the functionality within foo::goo to a different place, you can just update the active alias to reflect the new destination, rather than having to find/replace every instance of foo::goo.

#include <iostream>

namespace foo::goo
{
}

namespace v2
{
    int add(int x, int y)
    {
        return x + y;
    }
}

int main()
{
    namespace active = v2; // active now refers to v2

    std::cout << active::add(1, 2) << '\n'; // We don't have to change this

    return 0;
}
It’s worth noting that namespaces in C++ were not originally designed as a way to implement an information hierarchy -- they were designed primarily as a mechanism for preventing naming collisions. As evidence of this, note that the entirety of the standard library lives under the singular namespace std:: (with some nested namespaces used for newer library features). Some newer languages (such as C#) differ from C++ in this regard.

In general, you should avoid deeply nested namespaces.

When you should use namespaces

In applications, namespaces can be used to separate application-specific code from code that might be reusable later (e.g. math functions). For example, physical and math functions could go into one namespace (e.g. math::). Language and localization functions in another (e.g. lang::).

When you write a library or code that you want to distribute to others, always place your code inside a namespace. The code your library is used in may not follow best practices -- in such a case, if your library’s declarations aren’t in a namespace, there’s an elevated chance for naming conflicts to occur. As an additional advantage, placing library code inside a namespace also allows the user to see the contents of your library by using their editor’s auto-complete and suggestion feature.


---


C++ supports two variants of namespaces that are worth at least knowing about. We won’t build on these, so consider this lesson optional for now.

Unnamed (anonymous) namespaces

An unnamed namespace (also called an anonymous namespace) is a namespace that is defined without a name, like so:

#include <iostream>

namespace // unnamed namespace
{
    void doSomething() // can only be accessed in this file
    {
        std::cout << "v1\n";
    }
}

int main()
{
    doSomething(); // we can call doSomething() without a namespace prefix

    return 0;
}
This prints:

v1
All content declared in an unnamed namespace is treated as if it is part of the parent namespace. So even though function doSomething is defined in the unnamed namespace, the function itself is accessible from the parent namespace (which in this case is the global namespace), which is why we can call doSomething from main without any qualifiers.

This might make unnamed namespaces seem useless. But the other effect of unnamed namespaces is that all identifiers inside an unnamed namespace are treated as if they had internal linkage, which means that the content of an unnamed namespace can’t be seen outside of the file in which the unnamed namespace is defined.

For functions, this is effectively the same as defining all functions in the unnamed namespace as static functions. The following program is effectively identical to the one above:

#include <iostream>

static void doSomething() // can only be accessed in this file
{
    std::cout << "v1\n";
}

int main()
{
    doSomething(); // we can call doSomething() without a namespace prefix

    return 0;
}
Unnamed namespaces are typically used when you have a lot of content that you want to ensure stays local to a given file, as it’s easier to cluster such content in an unnamed namespace than individually mark all declarations as static. Unnamed namespaces will also keep user-defined types (something we’ll discuss in a later lesson) local to the file, something for which there is no alternative equivalent mechanism to do.

Inline namespaces

Now consider the following program:

#include <iostream>

void doSomething()
{
    std::cout << "v1\n";
}

int main()
{
    doSomething();

    return 0;
}
This prints:

v1
Pretty straightforward, right?

But let’s say you’re not happy with doSomething, and you want to improve it in some way that changes how it behaves. But if you do this, you risk breaking existing programs using the older version. How do you handle this?

One way would be to create a new version of the function with a different name. But over the course of many changes, you could end up with a whole set of almost-identically named functions (doSomething, doSomething_v2, doSomething_v3, etc…).

An alternative is to use an inline namespace. An inline namespace is a namespace that is typically used to version content. Much like an unnamed namespace, anything declared inside an inline namespace is considered part of the parent namespace. However, inline namespaces don’t give everything internal linkage.

To define an inline namespace, we use the inline keyword:

#include <iostream>

inline namespace v1 // declare an inline namespace named v1
{
    void doSomething()
    {
        std::cout << "v1\n";
    }
}

namespace v2 // declare a normal namespace named v2
{
    void doSomething()
    {
        std::cout << "v2\n";
    }
}

int main()
{
    v1::doSomething(); // calls the v1 version of doSomething()
    v2::doSomething(); // calls the v2 version of doSomething()

    doSomething(); // calls the inline version of doSomething() (which is v1)

    return 0;
}
This prints:

v1
v2
v1
In the above example, callers to doSomething will get the v1 (the inline version) of doSomething. Callers who want to use the newer version can explicitly call v2::dosomething(). This preserves the function of existing programs while allowing newer programs to take advantage of newer/better variations.

Alternatively, if you want to push the newer version:

#include <iostream>

namespace v1 // declare a normal namespace named v1
{
    void doSomething()
    {
        std::cout << "v1\n";
    }
}

inline namespace v2 // declare an inline namespace named v2
{
    void doSomething()
    {
        std::cout << "v2\n";
    }
}

int main()
{
    v1::doSomething(); // calls the v1 version of doSomething()
    v2::doSomething(); // calls the v2 version of doSomething()

    doSomething(); // calls the inline version of doSomething() (which is v2)

    return 0;
}
This prints:

v1
v2
v2
In this example, all callers to doSomething will get the v2 version by default (the newer and better version). Users who still want the older version of doSomething can explicitly call v1::doSomething() to access the old behavior. This means existing programs who want the v1 version will need to globally replace doSomething with v1::doSomething, but this typically won’t be problematic if the functions are well named.
