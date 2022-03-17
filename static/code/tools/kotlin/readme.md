
Returns and jumps

Functional (SAM) interfaces
Extensions
Generics: in, out, where
Nested and inner classes
Inline classes
Object expressions and declarations
Delegation
Delegated properties
Type aliases

Inline functions
Operator overloading
Type-safe builders
Using builders with builder type inference
This expressions
Asynchronous programming techniques
Coroutines
Annotations








Creating classes and instances﻿
To define a class, use the class keyword.

class Shape
Copied!
Properties of a class can be listed in its declaration or body.

class Rectangle(var height: Double, var length: Double) {
    var perimeter = (height + length) * 2
}
Copied!
The default constructor with parameters listed in the class declaration is available automatically.

val rectangle = Rectangle(5.0, 2.0)
println("The perimeter is ${rectangle.perimeter}")
Open in Playground →
Target: JVM
Running on v.1.6.10
Inheritance between classes is declared by a colon (:). Classes are final by default; to make a class inheritable, mark it as open.

open class Shape

class Rectangle(var height: Double, var length: Double): Shape() {
    var perimeter = (height + length) * 2
}
Copied!
See classes and objects and instances.

Comments﻿
Just like most modern languages, Kotlin supports single-line (or end-of-line) and multi-line (block) comments.

// This is an end-of-line comment

/* This is a block comment
   on multiple lines. */
Copied!
Block comments in Kotlin can be nested.

/* The comment starts here
/* contains a nested comment *⁠/
and ends here. */
Copied!
See Documenting Kotlin Code for information on the documentation comment syntax.

String templates﻿
var a = 1
// simple name in template:
val s1 = "a is $a" 
​
a = 2
// arbitrary expression in template:
val s2 = "${s1.replace("is", "was")}, but now is $a"
Open in Playground →
Target: JVM
Running on v.1.6.10
See String templates for details.

Conditional expressions﻿
fun maxOf(a: Int, b: Int): Int {
    if (a > b) {
        return a
    } else {
        return b
    }
}
Open in Playground →
Target: JVM
Running on v.1.6.10
In Kotlin, if can also be used as an expression.

fun maxOf(a: Int, b: Int) = if (a > b) a else b
Open in Playground →
Target: JVM
Running on v.1.6.10
See if-expressions.

for loop﻿
val items = listOf("apple", "banana", "kiwifruit")
for (item in items) {
    println(item)
}
Open in Playground →
Target: JVM
Running on v.1.6.10
or

val items = listOf("apple", "banana", "kiwifruit")
for (index in items.indices) {
    println("item at $index is ${items[index]}")
}
Open in Playground →
Target: JVM
Running on v.1.6.10
See for loop.

while loop﻿
val items = listOf("apple", "banana", "kiwifruit")
var index = 0
while (index < items.size) {
    println("item at $index is ${items[index]}")
    index++
}
Open in Playground →
Target: JVM
Running on v.1.6.10
See while loop.

when expression﻿
fun describe(obj: Any): String =
    when (obj) {
        1          -> "One"
        "Hello"    -> "Greeting"
        is Long    -> "Long"
        !is String -> "Not a string"
        else       -> "Unknown"
    }
Open in Playground →
Target: JVM
Running on v.1.6.10
See when expression.

Ranges﻿
Check if a number is within a range using in operator.

val x = 10
val y = 9
if (x in 1..y+1) {
    println("fits in range")
}
Open in Playground →
Target: JVM
Running on v.1.6.10
Check if a number is out of range.

val list = listOf("a", "b", "c")
​
if (-1 !in 0..list.lastIndex) {
    println("-1 is out of range")
}
if (list.size !in list.indices) {
    println("list size is out of valid list indices range, too")
}
Open in Playground →
Target: JVM
Running on v.1.6.10
Iterate over a range.

for (x in 1..5) {
    print(x)
}
Open in Playground →
Target: JVM
Running on v.1.6.10
Or over a progression.

for (x in 1..10 step 2) {
    print(x)
}
println()
for (x in 9 downTo 0 step 3) {
    print(x)
}
Open in Playground →
Target: JVM
Running on v.1.6.10
See Ranges and progressions.

Collections﻿
Iterate over a collection.

for (item in items) {
    println(item)
}
Open in Playground →
Target: JVM
Running on v.1.6.10
Check if a collection contains an object using in operator.

when {
    "orange" in items -> println("juicy")
    "apple" in items -> println("apple is fine too")
}
Open in Playground →
Target: JVM
Running on v.1.6.10
Using lambda expressions to filter and map collections:

val fruits = listOf("banana", "avocado", "apple", "kiwifruit")
fruits
    .filter { it.startsWith("a") }
    .sortedBy { it }
    .map { it.uppercase() }
    .forEach { println(it) }
Open in Playground →
Target: JVM
Running on v.1.6.10
See Collections overview.

Nullable values and null checks﻿
A reference must be explicitly marked as nullable when null value is possible. Nullable type names have ? at the end.

Return null if str does not hold an integer:

fun parseInt(str: String): Int? {
    // ...
}
Copied!
Use a function returning nullable value:

fun printProduct(arg1: String, arg2: String) {
    val x = parseInt(arg1)
    val y = parseInt(arg2)
​
    // Using `x * y` yields error because they may hold nulls.
    if (x != null && y != null) {
        // x and y are automatically cast to non-nullable after null check
        println(x * y)
    }
    else {
        println("'$arg1' or '$arg2' is not a number")
    }    
}
Open in Playground →
Target: JVM
Running on v.1.6.10
or

// ...
if (x == null) {
    println("Wrong number format in arg1: '$arg1'")
    return
}
if (y == null) {
    println("Wrong number format in arg2: '$arg2'")
    return
}
​
// x and y are automatically cast to non-nullable after null check
println(x * y)
Open in Playground →
Target: JVM
Running on v.1.6.10
See Null-safety.

Type checks and automatic casts﻿
The is operator checks if an expression is an instance of a type. If an immutable local variable or property is checked for a specific type, there's no need to cast it explicitly:

fun getStringLength(obj: Any): Int? {
    if (obj is String) {
        // `obj` is automatically cast to `String` in this branch
        return obj.length
    }
​
    // `obj` is still of type `Any` outside of the type-checked branch
    return null
}
Open in Playground →
Target: JVM
Running on v.1.6.10
or

fun getStringLength(obj: Any): Int? {
    if (obj !is String) return null
​
    // `obj` is automatically cast to `String` in this branch
    return obj.length
}
Open in Playground →
Target: JVM
Running on v.1.6.10
or even

fun getStringLength(obj: Any): Int? {
    // `obj` is automatically cast to `String` on the right-hand side of `&&`
    if (obj is String && obj.length > 0) {
        return obj.length
    }

    return null
}
fun getStringLength(obj: Any): Int? {
    // `obj` is automatically cast to `String` on the right-hand side of `&&`
    if (obj is String && obj.length > 0) {
        return obj.length
    }
​
    return null
}
Open in Playground →
Target: JVM
Running on v.1.6.10
See Classes and Type casts.