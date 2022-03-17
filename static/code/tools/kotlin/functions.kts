Functions
A function with two Int parameters and Int return type.

fun sum(a: Int, b: Int): Int {
    return a + b
}

A function body can be an expression. Its return type is inferred.

fun sum(a: Int, b: Int) = a + b

A function that returns no meaningful value.

fun printSum(a: Int, b: Int): Unit {
    println("sum of $a and $b is ${a + b}")
}

Unit return type can be omitted.

fun printSum(a: Int, b: Int) {
println("sum of $a and $b is ${a + b}")
}


// functions are declared with the fun keyword
fun double(x: Int): Int {
  return 2 * x
}

val result = double(2)

Calling member functions uses dot notation:

Stream().read() // create instance of class Stream and call read()


Parameters﻿
fun powerOf(number: Int, exponent: Int): Int { /*...*/ }
Copied!

// you can use a trailing comma when you declare function parameters
fun powerOf(
    number: Int,
    exponent: Int, // trailing comma
) { /*...*/ }


// function parameters can have default values which are used when you skip the corresponding argument
fun read(
    b: ByteArray,
    off: Int = 0,
    len: Int = b.size,
) { /*...*/ }


Overriding methods always use the same default parameter values as the base method. When overriding a method that has default parameter values, the default parameter values must be omitted from the signature:

open class A {
    open fun foo(i: Int = 10) { /*...*/ }
}

class B : A() {
    override fun foo(i: Int) { /*...*/ }  // No default value is allowed.
}
Copied!
If a default parameter precedes a parameter with no default value, the default value can only be used by calling the function with named arguments:

fun foo(
    bar: Int = 0,
    baz: Int,
) { /*...*/ }

foo(baz = 1) // The default value bar = 0 is used
Copied!
If the last argument after default parameters is a lambda, you can pass it either as a named argument or outside the parentheses:

fun foo(
    bar: Int = 0,
    baz: Int = 1,
    qux: () -> Unit,
) { /*...*/ }

foo(1) { println("hello") }     // Uses the default value baz = 1
foo(qux = { println("hello") }) // Uses both default values bar = 0 and baz = 1
foo { println("hello") }        // Uses both default values bar = 0 and baz = 1
Copied!
Named arguments﻿
When calling a function, you can name one or more of its arguments. This can be helpful when a function has many arguments and it's difficult to associate a value with an argument, especially if it's a boolean or null value.

When you use named arguments in a function call, you can freely change the order they are listed in, and if you want to use their default values, you can just leave these arguments out altogether.

Consider the following function, reformat(), which has 4 arguments with default values.

fun reformat(
    str: String,
    normalizeCase: Boolean = true,
    upperCaseFirstLetter: Boolean = true,
    divideByCamelHumps: Boolean = false,
    wordSeparator: Char = ' ',
) { /*...*/ }
Copied!
When calling this function, you don’t have to name all its arguments:

reformat(
    "String!",
    false,
    upperCaseFirstLetter = false,
    divideByCamelHumps = true,
    '_'
)
Copied!
You can skip all the ones with default values:

reformat("This is a long String!")
Copied!
You are also able to skip specific arguments with default values, rather than omitting them all. However, after the first skipped argument, you must name all subsequent arguments:

reformat("This is a short String!", upperCaseFirstLetter = false, wordSeparator = '_')
Copied!
You can pass a variable number of arguments (vararg) with names using the spread operator:

fun foo(vararg strings: String) { /*...*/ }

foo(strings = *arrayOf("a", "b", "c"))
Copied!
On the JVM: You can't use the named argument syntax when calling Java functions because Java bytecode does not always preserve the names of function parameters.

Unit-returning functions﻿
If a function does not return a useful value, its return type is Unit. Unit is a type with only one value - Unit. This value does not have to be returned explicitly:

fun printHello(name: String?): Unit {
    if (name != null)
        println("Hello $name")
    else
        println("Hi there!")
    // `return Unit` or `return` is optional
}
Copied!
The Unit return type declaration is also optional. The above code is equivalent to:

fun printHello(name: String?) { ... }
Copied!
Single-expression functions﻿
When a function returns a single expression, the curly braces can be omitted and the body is specified after a = symbol:

fun double(x: Int): Int = x * 2
Copied!
Explicitly declaring the return type is optional when this can be inferred by the compiler:

fun double(x: Int) = x * 2
Copied!
Explicit return types﻿
Functions with block body must always specify return types explicitly, unless it's intended for them to return Unit, in which case specifying the return type is optional.

Kotlin does not infer return types for functions with block bodies because such functions may have complex control flow in the body, and the return type will be non-obvious to the reader (and sometimes even for the compiler).

Variable number of arguments (varargs)﻿
You can mark a parameter of a function (usually the last one) with the vararg modifier:

fun <T> asList(vararg ts: T): List<T> {
    val result = ArrayList<T>()
    for (t in ts) // ts is an Array
        result.add(t)
    return result
}
Copied!
In this case, you can pass a variable number of arguments to the function:

val list = asList(1, 2, 3)
Copied!
Inside a function, a vararg-parameter of type T is visible as an array of T, as in the example above, where the ts variable has type Array<out T>.

Only one parameter can be marked as vararg. If a vararg parameter is not the last one in the list, values for the subsequent parameters can be passed using named argument syntax, or, if the parameter has a function type, by passing a lambda outside the parentheses.

When you call a vararg-function, you can pass arguments individually, for example asList(1, 2, 3). If you already have an array and want to pass its contents to the function, use the spread operator (prefix the array with *):

val a = arrayOf(1, 2, 3)
val list = asList(-1, 0, *a, 4)
Copied!
If you want to pass a primitive type array into vararg, you need to convert it to a regular (typed) array using the toTypedArray() function:

val a = intArrayOf(1, 2, 3) // IntArray is a primitive type array
val list = asList(-1, 0, *a.toTypedArray(), 4)
Copied!
Infix notation﻿
Functions marked with the infix keyword can also be called using the infix notation (omitting the dot and the parentheses for the call). Infix functions must meet the following requirements:

They must be member functions or extension functions.

They must have a single parameter.

The parameter must not accept variable number of arguments and must have no default value.

infix fun Int.shl(x: Int): Int { ... }

// calling the function using the infix notation
1 shl 2

// is the same as
1.shl(2)
Copied!
Infix function calls have lower precedence than arithmetic operators, type casts, and the rangeTo operator. The following expressions are equivalent:

1 shl 2 + 3 is equivalent to 1 shl (2 + 3)

0 until n * 2 is equivalent to 0 until (n * 2)

xs union ys as Set<*> is equivalent to xs union (ys as Set<*>)

On the other hand, an infix function call's precedence is higher than that of the boolean operators && and ||, is- and in-checks, and some other operators. These expressions are equivalent as well:

a && b xor c is equivalent to a && (b xor c)

a xor b in c is equivalent to (a xor b) in c

Note that infix functions always require both the receiver and the parameter to be specified. When you're calling a method on the current receiver using the infix notation, use this explicitly. This is required to ensure unambiguous parsing.

class MyStringCollection {
    infix fun add(s: String) { /*...*/ }

    fun build() {
        this add "abc"   // Correct
        add("abc")       // Correct
        //add "abc"        // Incorrect: the receiver must be specified
    }
}
Copied!
Function scope﻿
Kotlin functions can be declared at the top level in a file, meaning you do not need to create a class to hold a function, which you are required to do in languages such as Java, C#, and Scala. In addition to top level functions, Kotlin functions can also be declared locally as member functions and extension functions.

Local functions
Kotlin supports local functions, which are functions inside other functions:

fun dfs(graph: Graph) {
    fun dfs(current: Vertex, visited: MutableSet<Vertex>) {
        if (!visited.add(current)) return
        for (v in current.neighbors)
            dfs(v, visited)
    }

    dfs(graph.vertices[0], HashSet())
}
Copied!
A local function can access local variables of outer functions (the closure). In the case above, visited can be a local variable:

fun dfs(graph: Graph) {
    val visited = HashSet<Vertex>()
    fun dfs(current: Vertex) {
        if (!visited.add(current)) return
        for (v in current.neighbors)
            dfs(v)
    }

    dfs(graph.vertices[0])
}
Copied!
Member functions﻿
A member function is a function that is defined inside a class or object:

class Sample {
    fun foo() { print("Foo") }
}
Copied!
Member functions are called with dot notation:

Sample().foo() // creates instance of class Sample and calls foo
Copied!
For more information on classes and overriding members see Classes and Inheritance.


Tail recursive functions﻿


// marking a function with tailrec and 

When a function is marked with the tailrec modifier and meets the required formal conditions, the compiler optimizes out the recursion, leaving behind a fast and efficient loop based version instead:

val eps = 1E-10 // "good enough", could be 10^-15

tailrec fun findFixPoint(x: Double = 1.0): Double =
    if (Math.abs(x - Math.cos(x)) < eps) x else findFixPoint(Math.cos(x))

This code calculates the fixpoint of cosine, which is a mathematical constant. It simply calls Math.cos repeatedly starting at 1.0 until the result no longer changes, yielding a result of 0.7390851332151611 for the specified eps precision.


To be eligible for the tailrec modifier, a function must call itself as the last operation it performs. You cannot use tail recursion when there is more code after the recursive call, within try/catch/finally blocks, or on open functions.



