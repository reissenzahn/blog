
// classes can have type parameters
class Box<T>(t: T) {
    var value = t
}

// to create an instance of such a class we provide the type arguments
val box: Box<Int> = Box<Int>(1)

// the type arguments can be omitted if they can be inferred (e.g. from the constructor arguments)
val box = Box(1)


Declaration-site variance

Let's suppose that there is a generic interface Source<T> that does not have any methods that take T as a parameter, only methods that return T:

// Java
interface Source<T> {
    T nextT();
}
Copied!
Then, it would be perfectly safe to store a reference to an instance of Source<String> in a variable of type Source<Object> - there are no consumer-methods to call. But Java does not know this, and still prohibits it:

// Java
void demo(Source<String> strs) {
    Source<Object> objects = strs; // !!! Not allowed in Java
    // ...
}
Copied!
To fix this, you should declare objects of type Source<? extends Object>. Doing so is meaningless, because you can call all the same methods on such a variable as before, so there's no value added by the more complex type. But the compiler does not know that.

In Kotlin, there is a way to explain this sort of thing to the compiler. This is called declaration-site variance: you can annotate the type parameter T of Source to make sure that it is only returned (produced) from members of Source<T>, and never consumed. To do this, use the out modifier:

interface Source<out T> {
    fun nextT(): T
}

fun demo(strs: Source<String>) {
    val objects: Source<Any> = strs // This is OK, since T is an out-parameter
    // ...
}
Copied!
The general rule is this: when a type parameter T of a class C is declared out, it may occur only in the out-position in the members of C, but in return C<Base> can safely be a supertype of C<Derived>.

In other words, you can say that the class C is covariant in the parameter T, or that T is a covariant type parameter. You can think of C as being a producer of T's, and NOT a consumer of T's.

The out modifier is called a variance annotation, and since it is provided at the type parameter declaration site, it provides declaration-site variance. This is in contrast with Java's use-site variance where wildcards in the type usages make the types covariant.

In addition to out, Kotlin provides a complementary variance annotation: in. It makes a type parameter contravariant, meaning it can only be consumed and never produced. A good example of a contravariant type is Comparable:

interface Comparable<in T> {
    operator fun compareTo(other: T): Int
}

fun demo(x: Comparable<Number>) {
    x.compareTo(1.0) // 1.0 has type Double, which is a subtype of Number
    // Thus, you can assign x to a variable of type Comparable<Double>
    val y: Comparable<Double> = x // OK!
}
Copied!
The words in and out seem to be self-explanatory (as they’ve already been used successfully in C# for quite some time), and so the mnemonic mentioned above is not really needed. It can in fact be rephrased at a higher level of abstraction:

The Existential Transformation: Consumer in, Producer out!:-)

Type projections﻿
Use-site variance: type projections﻿
It is very easy to declare a type parameter T as out and avoid trouble with subtyping on the use site, but some classes can't actually be restricted to only return T's! A good example of this is Array:

class Array<T>(val size: Int) {
    operator fun get(index: Int): T { ... }
    operator fun set(index: Int, value: T) { ... }
}
Copied!
This class can be neither co- nor contravariant in T. And this imposes certain inflexibilities. Consider the following function:

fun copy(from: Array<Any>, to: Array<Any>) {
    assert(from.size == to.size)
    for (i in from.indices)
        to[i] = from[i]
}
Copied!
This function is supposed to copy items from one array to another. Let's try to apply it in practice:

val ints: Array<Int> = arrayOf(1, 2, 3)
val any = Array<Any>(3) { "" }
copy(ints, any)
//   ^ type is Array<Int> but Array<Any> was expected
Copied!
Here you run into the same familiar problem: Array<T> is invariant in T, and so neither Array<Int> nor Array<Any> is a subtype of the other. Why not? Again, this is because copy could have an unexpected behavior, for example, it may attempt to write a String to from, and if you actually pass an array of Int there, a ClassCastException will be thrown later.

To prohibit the copy function from writing to from, you can do the following:

fun copy(from: Array<out Any>, to: Array<Any>) { ... }
Copied!
This is type projection, which means that from is not a simple array, but is rather a restricted (projected) one. You can only call methods that return the type parameter T, which in this case means that you can only call get(). This is our approach to use-site variance, and it corresponds to Java's Array<? extends Object> while being slightly simpler.

You can project a type with in as well:

fun fill(dest: Array<in String>, value: String) { ... }
Copied!
Array<in String> corresponds to Java's Array<? super String>. This means that you can pass an array of CharSequence or an array of Object to the fill() function.

Star-projections﻿
Sometimes you want to say that you know nothing about the type argument, but you still want to use it in a safe way. The safe way here is to define such a projection of the generic type, that every concrete instantiation of that generic type will be a subtype of that projection.

Kotlin provides so-called star-projection syntax for this:

For Foo<out T : TUpper>, where T is a covariant type parameter with the upper bound TUpper, Foo<*> is equivalent to Foo<out TUpper>. This means that when the T is unknown you can safely read values of TUpper from Foo<*>.

For Foo<in T>, where T is a contravariant type parameter, Foo<*> is equivalent to Foo<in Nothing>. This means there is nothing you can write to Foo<*> in a safe way when T is unknown.

For Foo<T : TUpper>, where T is an invariant type parameter with the upper bound TUpper, Foo<*> is equivalent to Foo<out TUpper> for reading values and to Foo<in Nothing> for writing values.

If a generic type has several type parameters, each of them can be projected independently. For example, if the type is declared as interface Function<in T, out U> you could use the following star-projections:

Function<*, String> means Function<in Nothing, String>.

Function<Int, *> means Function<Int, out Any?>.

Function<*, *> means Function<in Nothing, out Any?>.

Star-projections are very much like Java's raw types, but safe.

Generic functions﻿
Classes aren’t the only declarations that can have type parameters. Functions can, too. Type parameters are placed before the name of the function:

fun <T> singletonList(item: T): List<T> {
    // ...
}

fun <T> T.basicToString(): String { // extension function
    // ...
}
Copied!
To call a generic function, specify the type arguments at the call site after the name of the function:

val l = singletonList<Int>(1)
Copied!
Type arguments can be omitted if they can be inferred from the context, so the following example works as well:

val l = singletonList(1)
Copied!
Generic constraints﻿
The set of all possible types that can be substituted for a given type parameter may be restricted by generic constraints.

Upper bounds﻿
The most common type of constraint is an upper bound, which corresponds to Java's extends keyword:

fun <T : Comparable<T>> sort(list: List<T>) {  ... }
Copied!
The type specified after a colon is the upper bound, indicating that only a subtype of Comparable<T> can be substituted for T. For example:

sort(listOf(1, 2, 3)) // OK. Int is a subtype of Comparable<Int>
sort(listOf(HashMap<Int, String>())) // Error: HashMap<Int, String> is not a subtype of Comparable<HashMap<Int, String>>
Copied!
The default upper bound (if there was none specified) is Any?. Only one upper bound can be specified inside the angle brackets. If the same type parameter needs more than one upper bound, you need a separate where-clause:

fun <T> copyWhenGreater(list: List<T>, threshold: T): List<String>
    where T : CharSequence,
          T : Comparable<T> {
    return list.filter { it > threshold }.map { it.toString() }
}
Copied!
The passed type must satisfy all conditions of the where clause simultaneously. In the above example, the T type must implement both CharSequence and Comparable.

Type erasure﻿
The type safety checks that Kotlin performs for generic declaration usages are done at compile time. At runtime, the instances of generic types do not hold any information about their actual type arguments. The type information is said to be erased. For example, the instances of Foo<Bar> and Foo<Baz?> are erased to just Foo<*>.

Therefore, there is no general way to check whether an instance of a generic type was created with certain type arguments at runtime, and the compiler prohibits such is-checks.

Type casts to generic types with concrete type arguments, for example, foo as List<String>, cannot be checked at runtime. These unchecked casts can be used when type safety is implied by high-level program logic but cannot be inferred directly by the compiler. The compiler issues a warning on unchecked casts, and at runtime, only the non-generic part is checked (equivalent to foo as List<*>).

The type arguments of generic function calls are also only checked at compile time. Inside the function bodies, the type parameters cannot be used for type checks, and type casts to type parameters (foo as T) are unchecked. However, reified type parameters of inline functions are substituted by the actual type arguments in the inlined function body at the call sites and so can be used for type checks and casts, with the same restrictions for instances of generic types as described above.


Generic functions

Functions can have generic parameters, which are specified using angle brackets before the function name:

fun <T> singletonList(item: T): List<T> { /*...*/ }

Copied!
For more information on generic functions, see Generics.

