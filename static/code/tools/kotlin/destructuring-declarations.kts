

// a destructing declaration creates multiple variables at once 
val (name, age) = person

println(name)
println(age)



A destructuring declaration is compiled down to the following code:

val name = person.component1()
val age = person.component2()

Anything can be on the right-hand side of a destructuring declaration, as long as the required number of component functions can be called on it. And, of course, there can be component3() and component4() and so on.

The componentN() functions need to be marked with the operator keyword to allow using them in a destructuring declaration.

Destructuring declarations also work in for-loops:

for ((a, b) in collection) { ... }

Variables a and b get the values returned by component1() and component2() called on elements of the collection.


Example: returning two values from a function﻿
Assume that you need to return two things from a function - for example, a result object and a status of some sort. A compact way of doing this in Kotlin is to declare a data class and return its instance:

data class Result(val result: Int, val status: Status)
fun function(...): Result {
    // computations

    return Result(result, status)
}

// Now, to use this function:
val (result, status) = function(...)
Copied!
Since data classes automatically declare componentN() functions, destructuring declarations work here.

You could also use the standard class Pair and have function() return Pair<Int, Status>, but it's often better to have your data named properly.

Example: destructuring declarations and maps﻿
Probably the nicest way to traverse a map is this:

for ((key, value) in map) {
   // do something with the key and the value
}
Copied!
To make this work, you should

Present the map as a sequence of values by providing an iterator() function.

Present each of the elements as a pair by providing functions component1() and component2().

And indeed, the standard library provides such extensions:

operator fun <K, V> Map<K, V>.iterator(): Iterator<Map.Entry<K, V>> = entrySet().iterator()
operator fun <K, V> Map.Entry<K, V>.component1() = getKey()
operator fun <K, V> Map.Entry<K, V>.component2() = getValue()
Copied!
So you can freely use destructuring declarations in for-loops with maps (as well as collections of data class instances or similar).

Underscore for unused variables﻿
If you don't need a variable in the destructuring declaration, you can place an underscore instead of its name:

val (_, status) = getResult()
Copied!
The componentN() operator functions are not called for the components that are skipped in this way.

Destructuring in lambdas﻿
You can use the destructuring declarations syntax for lambda parameters. If a lambda has a parameter of the Pair type (or Map.Entry, or any other type that has the appropriate componentN functions), you can introduce several new parameters instead of one by putting them in parentheses:

map.mapValues { entry -> "${entry.value}!" }
map.mapValues { (key, value) -> "$value!" }
Copied!
Note the difference between declaring two parameters and declaring a destructuring pair instead of a parameter:

{ a -> ... } // one parameter
{ a, b -> ... } // two parameters
{ (a, b) -> ... } // a destructured pair
{ (a, b), c -> ... } // a destructured pair and another parameter
Copied!
If a component of the destructured parameter is unused, you can replace it with the underscore to avoid inventing its name:

map.mapValues { (_, value) -> "$value!" }
Copied!
You can specify the type for the whole destructured parameter or for a specific component separately:

map.mapValues { (_, value): Map.Entry<Int, String> -> "$value!" }

map.mapValues { (_, value: String) -> "$value!" }