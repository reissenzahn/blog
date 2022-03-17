

// functions are first-class which means they can be stored in variables and passsed as arguments to and returned from other functions


Higher-order functions

A higher-order function is a function that takes functions as parameters, or returns a function.

fun <T, R> Collection<T>.fold(
    initial: R,
    combine: (acc: R, nextElement: T) -> R
): R {
    var accumulator: R = initial
    for (element: T in this) {
        accumulator = combine(accumulator, element)
    }
    return accumulator
}


val items = listOf(1, 2, 3, 4, 5)
​
items.fold(0, { 
    acc: Int, i: Int -> 
    print("acc = $acc, i = $i, ") 
    val result = acc + i
    println("result = $result")
    // The last expression in a lambda is considered the return value:
    result
})
​

// Parameter types in a lambda are optional if they can be inferred
val joinedToString = items.fold("Elements:", { acc, i -> acc + " " + i })

// Function references can also be used for higher-order function calls:
val product = items.fold(1, Int::times)



Function types﻿

// a function type consists of a parenthesized list of parameter types nd a return type




All function types have a parenthesized list of parameter types and a return type: (A, B) -> C denotes a type that represents functions that take two arguments of types A and B and return a value of type C. The list of parameter types may be empty, as in () -> A. The Unit return type cannot be omitted.


Function types can optionally have an additional receiver type, which is specified before the dot in the notation: the type A.(B) -> C represents functions that can be called on a receiver object A with a parameter B and return a value C. Function literals with receiver are often used along with these types.

Suspending functions belong to a special kind of function type that have a suspend modifier in their notation, such as suspend () -> Unit or suspend A.(B) -> C.

The function type notation can optionally include names for the function parameters: (x: Int, y: Int) -> Point. These names can be used for documenting the meaning of the parameters.

To specify that a function type is nullable, use parentheses as follows: ((Int, Int) -> Int)?.

Function types can also be combined using parentheses: (Int) -> ((Int) -> Unit).

The arrow notation is right-associative, (Int) -> (Int) -> Unit is equivalent to the previous example, but not to ((Int) -> (Int)) -> Unit.

You can also give a function type an alternative name by using a type alias:

typealias ClickHandler = (Button, ClickEvent) -> Unit
Copied!
Instantiating a function type﻿
There are several ways to obtain an instance of a function type:

Use a code block within a function literal, in one of the following forms:

a lambda expression: { a, b -> a + b },

an anonymous function: fun(s: String): Int { return s.toIntOrNull() ?: 0 }

Function literals with receiver can be used as values of function types with receiver.

Use a callable reference to an existing declaration:

a top-level, local, member, or extension function: ::isOdd, String::toInt,

a top-level, member, or extension property: List<Int>::size,

a constructor: ::Regex

These include bound callable references that point to a member of a particular instance: foo::toString.

Use instances of a custom class that implements a function type as an interface:

class IntTransformer: (Int) -> Int {
    override operator fun invoke(x: Int): Int = TODO()
}

val intFunction: (Int) -> Int = IntTransformer()
Copied!
The compiler can infer the function types for variables if there is enough information:

val a = { i: Int -> i + 1 } // The inferred type is (Int) -> Int
Copied!
Non-literal values of function types with and without a receiver are interchangeable, so the receiver can stand in for the first parameter, and vice versa. For instance, a value of type (A, B) -> C can be passed or assigned where a value of type A.(B) -> C is expected, and the other way around:

val repeatFun: String.(Int) -> String = { times -> this.repeat(times) }
val twoParameters: (String, Int) -> String = repeatFun // OK
​
fun runTransformation(f: (String, Int) -> String): String {
    return f("hello", 3)
}
val result = runTransformation(repeatFun) // OK
​
Open in Playground →
Target: JVM
Running on v.1.6.10
A function type with no receiver is inferred by default, even if a variable is initialized with a reference to an extension function. To alter that, specify the variable type explicitly.

Invoking a function type instance﻿
A value of a function type can be invoked by using its invoke(...) operator: f.invoke(x) or just f(x).

If the value has a receiver type, the receiver object should be passed as the first argument. Another way to invoke a value of a function type with receiver is to prepend it with the receiver object, as if the value were an extension function: 1.foo(2).

Example:

val stringPlus: (String, String) -> String = String::plus
val intPlus: Int.(Int) -> Int = Int::plus
​
println(stringPlus.invoke("<-", "->"))
println(stringPlus("Hello, ", "world!"))
​
println(intPlus.invoke(1, 1))
println(intPlus(1, 2))
println(2.intPlus(3)) // extension-like call
​
Open in Playground →
Target: JVM
Running on v.1.6.10
Inline functions﻿
Sometimes it is beneficial to use inline functions, which provide flexible control flow, for higher-order functions.

Lambda expressions and anonymous functions﻿
Lambda expressions and anonymous functions are function literals. Function literals are functions that are not declared but are passed immediately as an expression. Consider the following example:

max(strings, { a, b -> a.length < b.length })
Copied!
The function max is a higher-order function, as it takes a function value as its second argument. This second argument is an expression that is itself a function, called a function literal, which is equivalent to the following named function:

fun compare(a: String, b: String): Boolean = a.length < b.length
Copied!
Lambda expression syntax﻿
The full syntactic form of lambda expressions is as follows:

val sum: (Int, Int) -> Int = { x: Int, y: Int -> x + y }
Copied!
A lambda expression is always surrounded by curly braces.

Parameter declarations in the full syntactic form go inside curly braces and have optional type annotations.

The body goes after the ->.

If the inferred return type of the lambda is not Unit, the last (or possibly single) expression inside the lambda body is treated as the return value.

If you leave all the optional annotations out, what's left looks like this:

val sum = { x: Int, y: Int -> x + y }
Copied!
Passing trailing lambdas﻿
According to Kotlin convention, if the last parameter of a function is a function, then a lambda expression passed as the corresponding argument can be placed outside the parentheses:

val product = items.fold(1) { acc, e -> acc * e }
Copied!
Such syntax is also known as trailing lambda.

If the lambda is the only argument in that call, the parentheses can be omitted entirely:

run { println("...") }
Copied!
it: implicit name of a single parameter﻿
It's very common for a lambda expression to have only one parameter.

If the compiler can parse the signature without any parameters, the parameter does not need to be declared and -> can be omitted. The parameter will be implicitly declared under the name it:

ints.filter { it > 0 } // this literal is of type '(it: Int) -> Boolean'
Copied!
Returning a value from a lambda expression﻿
You can explicitly return a value from the lambda using the qualified return syntax. Otherwise, the value of the last expression is implicitly returned.

Therefore, the two following snippets are equivalent:

ints.filter {
    val shouldFilter = it > 0
    shouldFilter
}

ints.filter {
    val shouldFilter = it > 0
    return@filter shouldFilter
}
Copied!
This convention, along with passing a lambda expression outside of parentheses, allows for LINQ-style code:

strings.filter { it.length == 5 }.sortedBy { it }.map { it.uppercase() }
Copied!
Underscore for unused variables﻿
If the lambda parameter is unused, you can place an underscore instead of its name:

map.forEach { _, value -> println("$value!") }
Copied!
Destructuring in lambdas﻿
Destructuring in lambdas is described as a part of destructuring declarations.

Anonymous functions﻿
The lambda expression syntax above is missing one thing – the ability to specify the function’s return type. In most cases, this is unnecessary because the return type can be inferred automatically. However, if you do need to specify it explicitly, you can use an alternative syntax: an anonymous function.

fun(x: Int, y: Int): Int = x + y
Copied!
An anonymous function looks very much like a regular function declaration, except its name is omitted. Its body can be either an expression (as shown above) or a block:

fun(x: Int, y: Int): Int {
    return x + y
}
Copied!
The parameters and the return type are specified in the same way as for regular functions, except the parameter types can be omitted if they can be inferred from the context:

ints.filter(fun(item) = item > 0)
Copied!
The return type inference for anonymous functions works just like for normal functions: the return type is inferred automatically for anonymous functions with an expression body, but it has to be specified explicitly (or is assumed to be Unit) for anonymous functions with a block body.

When passing anonymous functions as parameters, place them inside the parentheses. The shorthand syntax that allows you to leave the function outside the parentheses works only for lambda expressions.

Another difference between lambda expressions and anonymous functions is the behavior of non-local returns. A return statement without a label always returns from the function declared with the fun keyword. This means that a return inside a lambda expression will return from the enclosing function, whereas a return inside an anonymous function will return from the anonymous function itself.

Closures﻿
A lambda expression or anonymous function (as well as a local function and an object expression) can access its closure, which includes the variables declared in the outer scope. The variables captured in the closure can be modified in the lambda:

var sum = 0
ints.filter { it > 0 }.forEach {
    sum += it
}
print(sum)
Copied!
Function literals with receiver﻿
Function types with receiver, such as A.(B) -> C, can be instantiated with a special form of function literals – function literals with receiver.

As mentioned above, Kotlin provides the ability to call an instance of a function type with receiver while providing the receiver object.

Inside the body of the function literal, the receiver object passed to a call becomes an implicit this, so that you can access the members of that receiver object without any additional qualifiers, or access the receiver object using a this expression.

This behavior is similar to that of extension functions, which also allow you to access the members of the receiver object inside the function body.

Here is an example of a function literal with receiver along with its type, where plus is called on the receiver object:

val sum: Int.(Int) -> Int = { other -> plus(other) }
Copied!
The anonymous function syntax allows you to specify the receiver type of a function literal directly. This can be useful if you need to declare a variable of a function type with receiver, and then to use it later.

val sum = fun Int.(other: Int): Int = this + other
Copied!
Lambda expressions can be used as function literals with receiver when the receiver type can be inferred from the context. One of the most important examples of their usage is type-safe builders:

class HTML {
    fun body() { ... }
}

fun html(init: HTML.() -> Unit): HTML {
    val html = HTML()  // create the receiver object
    html.init()        // pass the receiver object to the lambda
    return html
}

html {       // lambda with receiver begins here
    body()   // calling a method on the receiver object
}