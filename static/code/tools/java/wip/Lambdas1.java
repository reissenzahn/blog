

// lambda expressions allow small bits of code to be written inline as literals

// lambdas use type inference

(p, q) -> { /* method body */ }


Lambda expressions represent the creation of an object of a specific type. The type of the instance that is created is known as the target type of the lambda.

Only certain types are eligible to be the target of a lamba.

Target types are also called functional interfaces and they must:

• Be interfaces
• Have only one nondefault method (but may have other methods that are
default)

Runnable r = () -> System.out.println("Hello");

actually represents the creation of an object, which is assigned to a variable r, of
type Runnable.



When javac encounters a lambda expression, it interprets it as the body of a method with a specific signature—but which method?

To resolve this question, javac looks at the surrounding code. To be legal Java code, the lambda expression must satisfy the following properties:

• The lambda must appear where an instance of an interface type is expected.
• The expected interface type should have exactly one mandatory method.
• The expected interface method should have a signature that exactly matches
that of the lambda expression.

If this is the case, then an instance is created of a type that implements the expected
interface, and uses the lambda body as the implementation for the mandatory
method.
This slightly complex conversion approach comes from the desire to keep Java’s type
system as purely nominative (based on names). The lambda expression is said to be
converted to an instance of the correct interface type.
From this discussion, we can see that although Java 8 has added lambda expres‐
sions, they have been specifically designed to fit into Java’s existing type system—
which has a very strong emphasis on nominal types (rather than the other possible
sorts of types that exist in some other programming languages).
Let’s consider an example of lambda conversion—the list() method of the
java.io.File class. This method lists the files in a directory. Before it returns the
list, though, it passes the name of each file to a FilenameFilter object that the pro‐
grammer must supply. This FilenameFilter object accepts or rejects each file, and
is a SAM type defined in the java.io package:
@FunctionalInterface
public interface FilenameFilter {
 boolean accept(File dir, String name);
}
The type FilenameFilter carries the @FunctionalInterface to indicate that it is a
suitable type to be used as the target type for a lambda. However, this annotation is
not required and any type that meets the requirements (by being an interface and a
SAM type) can be used as a target type.
This is because the JDK and the existing corpus of Java code already had a huge
number of SAM types available before Java 8 was released. To require potential tar‐
get types to carry the annotation would have prevented lambdas from being retro‐
fitted to existing code for no real benefit.
In code that you write, you should always try to indicate when
your types are usable as target types, which you can do by
adding the @FunctionalInterface to them. This aids read‐
ability and can help some automated tools as well.
Type
System
Lambda Expressions | 171
Here’s how we can define a FilenameFilter class to list only those files whose
names end with .java, using a lambda:
File dir = new File("/src"); // The directory to list
String[] filelist = dir.list((d, fName) -> fName.endsWith(".java"));
For each file in the list, the block of code in the lambda expression is evaluated. If
the method returns true (which happens if the filename ends in .java), then the file
is included in the output—which ends up in the array filelist.
This pattern, where a block of code is used to test if an element of a container
matches a condition, and to only return the elements that pass the condition, is
called a filter idiom—and is one of the standard techniques of functional program‐
ming, which we will discuss in more depth presently.
Method References
Recall that we can think of lambda expressions as objects representing methods that
don’t have names. Now, consider this lambda expression:
// In real code this would probably be
// shorter because of type inference
(MyObject myObj) -> myObj.toString()
This will be autoconverted to an implementation of a @FunctionalInterface type
that has a single nondefault method that takes a single MyObject and returns a
String—specifically, the string obtained by calling toString() on the instance of
MyObject. However, this seems like excessive boilerplate, and so Java 8 provides a
syntax for making this easier to read and write:
MyObject::toString
This is a shorthand, known as a method reference, that uses an existing method as a
lambda expression. The method reference syntax is completely equivalent to the
previous form expressed as a lambda. It can be thought of as using an existing
method, but ignoring the name of the method, so it can be used as a lambda and
then autoconverted in the usual way. Java defines four types of method reference,
which are equivalent to four slightly different lambda expression forms (see
Table 4-1).
Table 4-1. Method references
Name Method reference Equivalent lambda
Unbound Trade::getPrice trade -> trade.getPrice()
Bound System.out::println s -> System.out.println(s)
Static System::getProperty key -> System.getProperty(key)
172 | Chapter 4: The Java Type System
Name Method reference Equivalent lambda
Constructor Trade::new price -> new Trade(price)
The form we originally introduced can be seen to be an unbound method reference.
When we use an unbound method reference, it is equivalent to a lambda that is
expecting an instance of the type that contains the method reference—in Table 4-1
that is a Trade object.
It is called an unbound method reference because the receiver object needs to be
supplied (as the first argument to the lambda) when the method reference is used.
That is, we are going to call getPrice() on some Trade object, but the supplier of
the method reference has not defined which one—that is left up to the user of the
reference.
By contrast, a bound method reference always includes the receiver as part of the
instantiation of the method reference. In Table 4-1, the receiver is System.out—so
when the reference is used, the println() method will always be called on Sys
tem.out, and all the parameters of the lambda will be used as method parameters to
println().
We will discuss use cases for method references versus lambda expressions in more
detail in the next chapter.
Functional Programming
Java is fundamentally an object-oriented lanaguage. However, with the arrival of
lambda expressions, it becomes much easier to write code that is closer to the func‐
tional approach.
There’s no single definition of exactly what constitutes a func‐
tional language—but there is at least a consensus that it should
at minimum contain the ability to represent a function as a
value that can be put into a variable.
Java has always (since version 1.1) been able to represent functions via inner classes,
but the syntax was complex and lacking in clarity. Lambda expressions greatly sim‐
plify that syntax, and so it is only natural that more developers will be seeking to use
aspects of functional programming in their Java code, now that it is considerably
easier to do so.
The first taste of functional programming that Java developers are likely to
encounter are three basic idioms that are remarkably useful:
map()
The map idiom is used with lists and list-like containers. The idea is that a
function is passed in that is applied to each element in the collection, and a new
Type
System
Lambda Expressions | 173
collection is created—consisting of the results of applying the function to each
element in turn. This means that a map idiom converts a collection of one type
to a collection of potentially a different type.
filter()
We have already met an example of the filter idiom, when we discussed how to
replace an anonymous implementation of FilenameFilter with a lambda. The
filter idiom is used for producing a new subset of a collection, based on some
selection criteria. Note that in functional programming, it is normal to produce
a new collection, rather than modifying an existing one in-place.
reduce()
The reduce idiom has several different guises. It is an aggregation operation,
which can be called fold, accumulate, or aggregate as well as reduce. The basic
idea is to take an initial value, and an aggregation (or reduction) function, and
apply the reduction function to each element in turn, building up a final result
for the whole collection by making a series of intermediate results—similar to a
“running total”—as the reduce operation traverses the collection.
Java has full support for these key functional idioms (and several others). The
implementation is explained in some depth in Chapter 8, where we discuss Java’s
data structures and collections, and in particular the stream abstraction, which
makes all of this possible.
Let’s conclude this introduction with some words of caution. It’s worth noting that
Java is best regarded as having support for “slightly functional programming.” It is
not an especially functional language, nor does it try to be. Some particular aspects
of Java that militate against any claims to being a functional language include the
following:
• Java has no structural types, which means no “true” function types. Every
lambda is automatically converted to the appropriate nominal target type.
• Type erasure causes problems for functional programming—type safety can be
lost for higher-order functions.
• Java is inherently mutable (as we’ll discuss in Chapter 6)—mutability is often
regarded as highly undesirable for functional languages.
• The Java collections are imperative, not functional. Collections must be con‐
verted to streams to use functional style.
Despite this, easy access to the basics of functional programing—and especially idi‐
oms such as map, filter, and reduce—is a huge step forward for the Java community.
These idioms are so useful that a large majority of Java developers will never need or
miss the more advanced capabilities provided by languages with a more thorough‐
bred functional pedigree.
In truth, many of these techniques were possible using nested types, via patterns like
callbacks and handlers, but the syntax was always quite cumbersome, especially
174 | Chapter 4: The Java Type System
given that you had to explicitly define a completely new type even when you only
needed to express a single line of code in the callback.
Lexical Scoping and Local Variables
A local variable is defined within a block of code that defines its scope, and outside
of that scope, a local variable cannot be accessed and ceases to exist. Only code
within the curly braces that define the boundaries of a block can use local variables
defined in that block. This type of scoping is known as lexical scoping, and just
defines a section of source code within which a variable can be used.
It is common for programmers to think of such a scope as temporal instead—that is,
to think of a local variable as existing from the time the JVM begins executing the
block until the time control exits the block. This is usually a reasonable way to think
about local variables and their scope. However, lambda expressions (and anony‐
mous and local classes, which we will meet later) have the ability to bend or break
this intuition somewhat.
This can cause effects that some developers initially find surprising. This is because
lambdas can use local variables, and so they can contain copies of values from lexi‐
cal scopes that no longer exist. This can been seen in the following code:
public interface IntHolder {
 public int getValue();
}
public class Weird {
 public static void main(String[] args) {
 IntHolder[] holders = new IntHolder[10];
 for (int i = 0; i < 10; i++) {
 final int fi = i;
 holders[i] = () -> {
 return fi;
 };
 }
 // The lambda is now out of scope, but we have 10 valid instances
 // of the class the lambda has been converted to in our array.
 // The local variable fi is not in our scope here, but is still
 // in scope for the getValue() method of each of those 10 objects.
 // So call getValue() for each object and print it out.
 // This prints the digits 0 to 9.
 for (int i = 0; i < 10; i++) {
 System.out.println(holders[i].getValue());
 }
 }
}
Each instance of a lambda has an automatically created private copy of each of the
final local variables it uses, so, in effect, it has its own private copy of the scope that
existed when it was created. This is sometimes referred to as a captured variable.



Lambdas that capture variables like this are referred to as closures, and the variables
are said to have been closed over.

In practice, the preceding closure example is more verbose than it needs to be in
two separate ways:
• The lambda has an explicit scope {} and return statement.
• The variable fi is explicitly declared final.
The compiler javac helps with both of these.
Lambdas that only return the value of a single expression need not include a scope
or return; instead, the body of the lambda is just the expression without the need
for curly braces. In our example we have explicitly included the braces and return
statement to spell out that the lambda is defining its own scope.
In early versions of Java there were two hard requirements when closing over a
variable:
• The captures must not be modified after they have been captured (e.g., after the
lambda)
• The captured variables must be declared final
However, in recent Java versions, javac can analyze the code and detect whether the
programmer attempts to modify the captured variable after the scope of the lambda.
If not, then the final qualifier on the captured variable can be omitted (such a vari‐
able is said to be effectively final). If the final qualifier is omitted, then it is a
compile-time error to attempt to modify a captured variable after the lambda’s
scope.
The reason for this is that Java implements closures by copying the bit pattern of the
contents of the variable into the scope created by the closure. Further changes to the
contents of the closed-over variable would not be reflected in the copy contained in
closure scope, so the design decision was made to make such changes illegal, and a
compile-time error.
These assists from javac mean that we can rewrite the inner loop of the preceding
example to the very compact form:
for (int i = 0; i < 10; i++) {
 int fi = i;
176 | Chapter 4: The Java Type System
 holders[i] = () -> fi;
}

