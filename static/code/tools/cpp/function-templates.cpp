
Let’s say you wanted to write a function to calculate the maximum of two numbers. You might do so like this:

int max(int x, int y)
{
    return (x > y) ? x : y;
}
While the caller can pass different values into the function, the type of the parameters is fixed, so the caller can only pass in int values. That means this function really only works well for integers (and types that can be promoted to int).

So what happens later when you want to find the max of two double values? Because C++ requires us to specify the type of all function parameters, the solution is to create a new overloaded version of max() with parameters of type double:

double max(double x, double y)
{
    return (x > y) ? x: y;
}
Note that the code for the implementation of the double version of max() is exactly the same as for the int version of max()! In fact, this implementation works for many different types: including int, double, long, long double, and even new types that you’ve created yourself (which we’ll cover how to do in future lessons).

Having to create overloaded functions with the same implementation for each set of parameter types we want to support is a maintenance headache, a recipe for errors, and a clear violation of the DRY (don’t repeat yourself) principle. There’s a less-obvious challenge here as well: a programmer who wishes to use the max() function may wish to call it with a parameter type that the author of the max() did not anticipate (and thus did not write an overloaded function for).

What we are really missing is some way to write a single version of max() that can work with arguments of any type (even types that may not have been anticipated when the code for max() was written). Normal functions are simply not up to the task here. Fortunately, C++ supports another feature that was designed specifically to solve this kind of problem.

Welcome to the world of C++ templates.

Introduction to C++ templates

In C++, the template system was designed to simplify the process of creating functions (or classes) that are able to work with different data types.

Instead of manually creating a bunch of mostly-identical functions or classes (one for each set of different types), we instead create a single template. Just like a normal definition, a template describes what a function or class looks like. Unlike a normal definition (where all types must be specified), in a template we can use one or more placeholder types. A placeholder type represents some type that is not known at the time the template is written, but that will be provided later.

Once a template is defined, the compiler can use the template to generate as many overloaded functions (or classes) as needed, each using different actual types!

The end result is the same -- we end up with a bunch of mostly-identical functions or classes (one for each set of different types). But we only have to create and maintain a single template, and the compiler does all the hard work for us.

Key insight

The compiler can use a single template to generate a family of related functions or classes, each using a different set of types.

As an aside…

Because the concept behind templates can be hard to describe in words, let’s try an analogy.

If you were to look up the word “template” in the dictionary, you’d find a definition that was similar to the following: “a template is a model that serves as a pattern for creating similar objects”. One type of template that is very easy to understand is that of a stencil. A stencil is a thin piece of material (such as a piece of cardboard or plastic) with a shape cut out of it (e.g. a happy face). By placing the stencil on top of another object, then spraying paint through the hole, you can very quickly replicate the cut-out shape. The stencil itself only needs to be created once, and then it can be reused as many times as desired, to create the cut out shape in as many different colors as you like. Even better, the color of a shape made with the stencil doesn’t have to be determined until the stencil is actually used.

A template is essentially a stencil for creating functions or classes. We create the template (our stencil) once, and then we can use it as many times as needed, to stencil out a function or class for a specific set of actual types. Those actual types don’t need to be determined until the template is actually used.

Because the actual types aren’t determined until the template is used in a program (not when the template is written), the author of the template doesn’t have to try to anticipate all of the actual types that might be used. This means template code can be used with types that didn’t even exist when the template was written! We’ll see how this comes in handy later, when we start exploring the C++ standard library, which is absolutely full of template code!

Key insight

Templates can work with types that didn’t even exist when the template was written. This helps make template code both flexible and future proof!

In the rest of this lesson, we’ll introduce and explore how to create templates for functions, and describe how they work in more detail. We’ll save discussion of class templates until after we’ve covered what classes are.

Function templates

A function template is a function-like definition that is used to generate one or more overloaded functions, each with a different set of actual types. This is what will allow us to create functions that can work with many different types.

When we create our function template, we use placeholder types (also called template types) for any parameter types, return types, or types used in the function body that we want to be specified later.

Function templates are something that is best taught by example, so let’s convert our normal max(int, int) function from the example above into a function template. It’s surprisingly easy, and we’ll explain what’s happening along the way.

Creating a templated max function

Here’s the int version of max() again:

int max(int x, int y)
{
    return (x > y) ? x : y;
}
Note that we use type int three times in this function: once for parameter x, once for parameter y, and once for the return type of the function.

To create a function template, we’re going to do two things. First, we’re going to replace our specific types with template types. In this case, because we have only one type that needs replacing (int), we only need one template type. It’s common convention to use single capital letters (starting with T) to represent template types.

Here’s our new function that uses a single template type:

T max(T x, T y) // won't compile because we haven't defined T
{
    return (x > y) ? x : y;
}
Best practice

Use a single capital letter (starting with T) to name your template types (e.g. T, U, V, etc…)

This is a good start -- however, it won’t compile because the compiler doesn’t know what T is! And this is still a normal function, not a function template.

Second, we’re going to tell the compiler that this is a function template, and that T is a template type. This is done using what is called a template parameter declaration:

template <typename T> // this is the template parameter declaration
T max(T x, T y) // this is the function template definition for max<T>
{
    return (x > y) ? x : y;
}
Let’s take a slightly closer look at the template parameter declaration. We start with the keyword template, which tells the compiler that we’re creating a template. Next, we specify all of the template types that our template will use inside angled brackets (<>). For each template type, we use the keyword typename or class, followed by the name of the template type (e.g. T).

Each template function (or template class) needs its own template parameter declaration.

As an aside…

There is no difference between the typename and class keywords in this context. You will often see people use the class keyword since it was introduced into the language earlier. However, we prefer the newer typename keyword, because it makes it clearer that the template type can be replaced by any type (such as a fundamental type), not just class types.

Because this function template has one template type named T, we’ll refer to it as max<T>.

Related content

We discuss how to create function templates with multiple template types in lesson 8.15 -- Function templates with multiple template types.

Believe it or not, we’re done!

In the next lesson, we’ll look at how we use our max<T> function template to generate max() functions with parameters of different types.


---


In the previous lesson (8.13 -- Function templates), we introduced function templates, and converted a normal max() function into a max<T> function template:

template <typename T>
T max(T x, T y)
{
    return (x > y) ? x : y;
}
In this lesson, we’ll focus on how function templates are used.

Using a function template

Function templates are not actually functions -- their code isn’t compiled or executed directly. Instead, function templates have one job: to generate functions (that are compiled and executed).

To use our max<T> function template, we can make a function call with the following syntax:

max<actual_type>(arg1, arg2); // actual_type is some actual type, like int or double
This looks a lot like a normal function call -- the primary difference is the addition of the type in angled brackets (called a template argument), which specifies the actual type that will be used in place of template type T.

Let’s take a look at this in a simple example:

#include <iostream>

template <typename T>
T max(T x, T y)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max<int>(1, 2) << '\n'; // instantiates and calls function max<int>(int, int)

    return 0;
}
When the compiler encounters the function call max<int>(1, 2), it will determine that a function definition for max<int>(int, int) does not already exist. Consequently, the compiler will use our max<T> function template to create one.

The process of creating functions (with specific types) from function templates (with template types) is called function template instantiation (or instantiation for short). When this process happens due to a function call, it’s called implicit instantiation. An instantiated function is often called a function instance (instance for short) or a template function. Function instances are normal functions in all regards.

The process for instantiating a function is simple: the compiler essentially clones the function template and replaces the template type (T) with the actual type we’ve specified (int).

So when we call max<int>(1, 2), the function that gets instantiated looks something like this:

template<> // ignore this for now
int max<int>(int x, int y) // the generated function max<int>(int, int)
{
    return (x > y) ? x : y;
}
Here’s the same example as above, showing what the compiler actually compiles after all instantiations are done:

#include <iostream>

// a declaration for our function template (we don't need the definition any more)
template <typename T>
T max(T x, T y);

template<>
int max<int>(int x, int y) // the generated function max<int>(int, int)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max<int>(1, 2) << '\n'; // instantiates and calls function max<int>(int, int)

    return 0;
}
You can compile this yourself and see that it works. An instantiated function is only instantiated the first time a function call is made. Further calls to the function are routed to the already instantiated function.

Let’s do another example:

#include <iostream>

template <typename T>
T max(T x, T y) // function template for max(T, T)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max<int>(1, 2) << '\n'; // instantiates and calls function max<int>(int, int)
    std::cout << max<int>(4, 3) << '\n'; // calls already instantiated function max<int>(int, int)
    std::cout << max<double>(1, 2) << '\n'; // instantiates and calls function max<double>(double, double)

    return 0;
}
This works similarly to the previous example, but our function template will be used to generate two functions this time: one time replacing T with int, and the other time replacing T with double. After all instantiations, the program will look something like this:

#include <iostream>

// a declaration for our function template (we don't need the definition any more)
template <typename T>
T max(T x, T y);

template<>
int max<int>(int x, int y) // the generated function max<int>(int, int)
{
    return (x > y) ? x : y;
}

template<>
double max<double>(double x, double y) // the generated function max<double>(double, double)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max<int>(1, 2) << '\n'; // instantiates and calls function max<int>(int, int)
    std::cout << max<int>(4, 3) << '\n'; // calls already instantiated function max<int>(int, int)
    std::cout << max<double>(1, 2) << '\n'; // instantiates and calls function max<double>(double, double)

    return 0;
}
One additional thing to note here: when we instantiate max<double>, the instantiated function has parameters of type double. Because we’ve provided int arguments, those arguments will be implicitly converted to double.

Template argument deduction

In most cases, the actual types we want to use for instantiation will match the type of our function parameters. For example:

std::cout << max<int>(1, 2) << '\n'; // specifying we want to call max<int>
In this function call, we’ve specified that we want to replace T with int, but we’re also calling the function with int arguments.

In cases where the type of the arguments match the actual type we want, we do not need to specify the actual type -- instead, we can use template argument deduction to have the compiler deduce the actual type that should be used from the argument types in the function call.

For example, instead of making a function call like this:

std::cout << max<int>(1, 2) << '\n'; // specifying we want to call max<int>
We can do one of these instead:

std::cout << max<>(1, 2) << '\n';
std::cout << max(1, 2) << '\n';
In either case, the compiler will see that we haven’t provided an actual type, so it will attempt to deduce an actual type from the function arguments that will allow it to generate a max() function where all template parameters match the type of the provided arguments. In this example, the compiler will deduce that using function template max<T> with actual type int allows it to instantiate function max<int>(int, int) where the type of both template parameters (int) matches the type of the provided arguments (int).

The difference between the two cases has to do with how the compiler resolves the function call from a set of overloaded functions. In the top case (with the empty angled brackets), the compiler will only consider max<int> template function overloads when determining which overloaded function to call. In the bottom case (with no angled brackets), the compiler will consider both max<int> template function overloads and max non-template function overloads.

For example:

#include <iostream>

template <typename T>
T max(T x, T y)
{
    return (x > y) ? x : y;
}

int max(int x, int y)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max<int>(1, 2) << '\n'; // selects max<int>
    std::cout << max<>(1, 2) << '\n'; // deduces max<int>(int, int) (non-template functions not considered)
    std::cout << max(1, 2) << '\n'; // calls function max(int, int)

    return 0;
}
Note how the syntax in the bottom case looks identical to a normal function call! This is usually the preferred syntax used when invoking function templates (and the one we’ll default to in future examples, unless required to do otherwise).

Best practice

Favor the normal function call syntax when using function templates.

Function templates with non-template parameters

It’s possible to create function templates that have both template types and non-template type parameters. The template parameters can be matched to any type, and the non-template parameters work like the parameters of normal functions.

For example:

template <typename T>
int someFcn (T x, double y)
{
    return 5;
}

int main()
{
    someFcn(1, 3.4); // matches someFcn(int, double)
    someFcn(1, 3.4f); // matches someFcn(int, double) -- the float is promoted to a double
    someFcn(1.2, 3.4); // matches someFcn(double, double)
    someFcn(1.2f, 3.4); // matches someFcn(float, double)
    someFcn(1.2f, 3.4f); // matches someFcn(float, double) -- the float is promoted to a double

    return 0;
}
This function template has a templated first parameter, but the second parameter is fixed with type double. Note that the return type can also be any type. In this case, our function will always return an int value.

Instantiated functions may not always compile

Consider the following program:

#include <iostream>

template <typename T>
T addOne(T x)
{
    return x + 1;
}

int main()
{
    std::cout << addOne(1) << '\n';
    std::cout << addOne(2.3) << '\n';

    return 0;
}
The compiler will effectively compile and execute this:

#include <iostream>

template <typename T>
T addOne(T x);

template<>
int addOne<int>(int x)
{
    return x + 1;
}

template<>
double addOne<double>(double x)
{
    return x + 1;
}

int main()
{
    std::cout << addOne(1) << '\n';   // calls addOne<int>(int)
    std::cout << addOne(2.3) << '\n'; // calls addOne<double>(double)

    return 0;
}
which will produce the result:

2
3.3
But what if we try something like this?

#include <iostream>
#include <string>

template <typename T>
T addOne(T x)
{
    return x + 1;
}

int main()
{
    std::string hello { "Hello, world!" };
    std::cout << addOne(hello) << '\n';

    return 0;
}
When the compiler tries to resolve addOne(hello) it won’t find a non-template function match for addOne(std::string), but it will find our function template for addOne(T), and determine that it can generate an addOne(std::string) function from it. Thus, the compiler will generate and compile this:

#include <iostream>
#include <string>

template <typename T>
T addOne(T x);

template<>
std::string addOne<std::string>(std::string x)
{
    return x + 1;
}

int main()
{
    std::string hello{ "Hello, world!" };
    std::cout << addOne(hello) << '\n';

    return 0;
}
However, this will generate a compile error, because x + 1 doesn’t make sense when x is a std::string. The obvious solution here is simply not to call addOne() with an argument of type std::string.

Generic programming

Because template types can be replaced with any actual type, template types are sometimes called generic types. And because templates can be written agnostically of specific types, programming with templates is sometimes called generic programming. Whereas C++ typically has a strong focus on types and type checking, in contrast, generic programming lets us focus on the logic of algorithms and design of data structures without having to worry so much about type information.

Conclusion

Once you get used to writing function templates, you’ll find they actually don’t take much longer to write than functions with actual types. Function templates can significantly reduce code maintenance and errors by minimizing the amount of code that needs to be written and maintained.

Function templates do have a few drawbacks, and we would be remiss not to mention them. First, the compiler will create (and compile) a function for each function call with a unique set of argument types. So while function templates are compact to write, they can expand into a crazy amount of code, which can lead to code bloat and slow compile times. The bigger downside of function templates is that they tend to produce crazy-looking, borderline unreadable error messages that are much harder to decipher than those of regular functions. These error messages can be quite intimidating, but once you understand what they are trying to tell you, the problems they are pinpointing are often quite straightforward to resolve.

These drawbacks are fairly minor compared with the power and safety that templates bring to your programming toolkit, so use templates liberally anywhere you need type flexibility! A good rule of thumb is to create normal functions at first, and then convert them into function templates if you find you need an overload for different parameter types.

Best practice

Use function templates to write generic code that can work with a wide variety of types whenever you have the need.


---



In lesson 8.13 -- Function templates, we wrote a function template to calculate the maximum of two values:

#include <iostream>

template <typename T>
T max(T x, T y)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max(1, 2) << '\n';   // will instantiate max(int, int)
    std::cout << max(1.5, 2.5) << '\n'; // will instantiate max(double, double)

    return 0;
}
Now consider the following similar program:

#include <iostream>

template <typename T>
T max(T x, T y)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max(2, 3.5) << '\n';  // compile error

    return 0;
}
You may be surprised to find that this program won’t compile. Instead, the compiler will issue a bunch of (probably crazy looking) error messages. On Visual Studio, the author got the following:

Project3.cpp(11,18): error C2672: 'max': no matching overloaded function found
Project3.cpp(11,28): error C2782: 'T max(T,T)': template parameter 'T' is ambiguous
Project3.cpp(4): message : see declaration of 'max'
Project3.cpp(11,28): message : could be 'double'
Project3.cpp(11,28): message : or       'int'
Project3.cpp(11,28): error C2784: 'T max(T,T)': could not deduce template argument for 'T' from 'double'
Project3.cpp(4): message : see declaration of 'max'
In our function call max(2, 3.5), we’re passing arguments of two different types: one int and one double. Because we’re making a function call without using angled brackets to specify an actual type, the compiler will first look to see if there is a non-template match for max(int, double). It won’t find one.

Next, the compiler will see if it can find a function template match (using template argument deduction, which we covered in lesson 8.14 -- Function template instantiation). However, this will also fail, for a simple reason: T can only represent a single type. There is no type for T that would allow the compiler to instantiate function template max<T>(T, T) into a function with two different parameter types. Put another way, because both parameters in the function template are of type T, they must resolve to the same actual type.

Since no non-template match was found, and no template match was found, the function call fails to resolve, and we get a compile error.

You might wonder why the compiler didn’t generate function max<double>(double, double) and then use numeric conversion to type convert the int argument to a double. The answer is simple: type conversion is done only when resolving function overloads, not when performing template argument deduction.

This lack of type conversion is intentional for at least two reasons. First, it helps keep things simple: we either find an exact match between the function call arguments and template type parameters, or we don’t. Second, it allows us to create function templates for cases where we want to ensure that two or more parameters have the same type (as in the example above).

We’ll have to find another solution. Fortunately, we can solve this problem in (at least) three ways.

Use static_cast to convert the arguments to matching types

The first solution is to put the burden on the caller to convert the arguments into matching types. For example:

#include <iostream>

template <typename T>
T max(T x, T y)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max(static_cast<double>(2), 3.5) << '\n'; // convert our int to a double so we can call max(double, double)

    return 0;
}
Now that both arguments are of type double, the compiler will be able to instantiate max(double, double) that will satisfy this function call.

However, this solution is awkward and hard to read.

Provide an actual type

If we had written a non-template max(double, double) function, then we would be able to call max(int, double) and let the implicit type conversion rules convert our int argument into a double so the function call could be resolved:

#include <iostream>

double max(double x, double y)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max(2, 3.5) << '\n'; // the int argument will be converted to a double

    return 0;
}
However, when the compiler is doing template argument deduction, it won’t do any type conversions. Fortunately, we don’t have to use template argument deduction if we specify an actual type to be used instead:

#include <iostream>

template <typename T>
T max(T x, T y)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max<double>(2, 3.5) << '\n'; // we've provided actual type double, so the compiler won't use template argument deduction

    return 0;
}
In the above example, we call max<double>(2, 3.5). Because we’ve explicitly specified that T should be replaced with double, the compiler won’t use template argument deduction. Instead, it will just instantiate the function max<double>(double, double), and then type convert any mismatched arguments. Our int parameter will be implicitly converted to a double.

While this is more readable than using static_cast, it would be even nicer if we didn’t even have to think about the types when making a function call to max at all.

Functions templates with multiple template type parameters

The root of our problem is that we’ve only defined the single template type (T) for our function template, and then specified that both parameters must be of this same type.

The best way to solve this problem is to rewrite our function template in such a way that our parameters can resolve to different types. Rather than using one template type parameter T, we’ll now use two (T and U):

#include <iostream>

template <typename T, typename U> // We're using two template type parameters named T and U
T max(T x, U y) // x can resolve to type T, and y can resolve to type U
{
    return (x > y) ? x : y; // uh oh, we have a narrowing conversion problem here
}

int main()
{
    std::cout << max(2, 3.5) << '\n';

    return 0;
}
Because we’ve defined x with template type T, and y with template type U, x and y can now resolve their types independently. When we call max(2, 3.5), T can be an int and U can be a double. The compiler will happily instantiate max<int, double>(int, double) for us.

However, the above code still has a problem: using the usual arithmetic rules (8.4 -- Arithmetic conversions), double takes precedence over int, so our conditional operator will return a double. But our function is defined as returning a T -- in cases where T resolves to an int, our double return value will undergo a narrowing conversion to an int, which will produce a warning (and possible loss of data).

Making the return type a U instead doesn’t solve the problem, as we can always flip the order of the operands in the function call to flip the types of T and U.

How do we solve this? This is a good use for an auto return type -- we’ll let the compiler deduce what the return type should be from the return statement:

#include <iostream>

template <typename T, typename U>
auto max(T x, U y)
{
    return (x > y) ? x : y;
}

int main()
{
    std::cout << max(2, 3.5) << '\n';

    return 0;
}
This version of max now works fine with operands of different types.

Abbreviated function templates C++20

C++20 introduces a new use of the auto keyword: When the auto keyword is used as a parameter type in a normal function, the compiler will automatically convert the function into a function template with each auto parameter becoming an independent template type parameter. This method for creating a function template is called an abbreviated function template.

For example:

auto max(auto x, auto y)
{
    return (x > y) ? x : y;
}
is shorthand in C++20 for the following:

template <typename T, typename U>
auto max(T x, U y)
{
    return (x > y) ? x : y;
}
which is the same as the max function template we wrote above.

In cases where you want each template type parameter to be an independent type, this form is preferred as the removal of the template parameter declaration line makes your code more concise and readable.

Best practice

Feel free to use abbreviated function templates if each auto parameter should be an independent template type (and your language standard is set to C++20 or newer).

