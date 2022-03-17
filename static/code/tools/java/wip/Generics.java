



List<CenteredCircle> shapes = new ArrayList<CenteredCircle>();

// Create some centered shapes, and store them in the list
shapes.add(new CenteredCircle(1.0, 1.0, 1.0));
// Next line will cause a compilation error
shapes.add(new CenteredSquare(2.5, 2, 3));
// List<CenteredCircle>::get() returns a CenteredCircle, no cast needed
CenteredCircle c = shapes.get(0);

// generics allow for the use of type parameters in types and methods

interface Box<T> {
 void box(T t);
 T unbox();
}

interface List<E> extends Collection<E> {
 boolean add(E e);
 E get(int index);
 // other methods omitted
}

Note how the type parameter E can be used as a parameter for both return types and
method arguments. We don’t assume that the payload type has any specific proper‐
ties, and only make the basic assumption of consistency—that the type we put in is
the same type that we will later get back out.
This enhancement has effectively introduced a new kind of type to Java’s type sys‐
tem—by combining the container type with the value of the type parameter we are
making new types.



Diamond Syntax
When we create an instance of a generic type, the righthand side of the assignment
statement repeats the value of the type parameter. This is usually unnecessary, as the
compiler can infer the values of the type parameters. In modern versions of Java, we
can leave out the repeated type values in what is called diamond syntax.
Let’s look at an example of how to use diamond syntax, by rewriting one of our ear‐
lier examples:
// Create a List-of-CenteredCircle using diamond syntax
List<CenteredCircle> shapes = new ArrayList<>();










Boxing and unboxing operations, used to convert
from the primitive type to the wrapper class, are automatically inserted.

// an interface or class may be declared to take one or more type parameters that should be supplied when you declare a variable belonging to the interface or class or when you create a new instance of a class

// generics are implemented by erasure because the types List<Integer>, List<String>, and List<List<String>> are all represented at run-time by the same type List

// the term erasure is a slight misnomer, since the process erases type parameters but adds casts

// generics implicitly perform the same cast that is explicitly performed without generics

// generics come with the following guarantee: the implicit casts added by the compilation of generics never fail (Cast-iron guarantee) provided no unchecked warnings have been issued by the compiler

// erasure allows the same library to be accessed in both non-generic and generic forms

// at the bytecode level, code that doesn’t use generics looks just like code that does

// executing new ArrayList<String>() allocates a list, but does not store in the list any indication of the type of its elements. In the jargon, we say that Java reifies array component types but does not reify list element types (or other generic types).


1.2 Boxing and Unboxing
Recall that every type in Java is either a reference type or a primitive type. A reference
type is any class, interface, or array type. All reference types are subtypes of class
Object, and any variable of reference type may be set to the value null. As shown in the
6 | Chapter 1: Introduction
following table, there are eight primitive types, and each of these has a corresponding
library class of reference type. The library classes are located in the package java.lang.
Primitive Reference
byte Byte
short Short
int Integer
long Long
float Float
double Double
boolean Boolean
char Character
Conversion of a primitive type to the corresponding reference type is called boxing and
conversion of the reference type to the corresponding primitive type is called unboxing.
Java with generics automatically inserts boxing and unboxing coercions where appropriate. If an expression e of type int appears where a value of type Integer is expected,
boxing converts it to new Integer(e) (however, it may cache frequently occurring values). If an expression e of type Integer appears where a value of type int is expected,
unboxing converts it to the expression e.intValue(). For example, the sequence:
List<Integer> ints = new ArrayList<Integer>();
ints.add(1);
int n = ints.get(0);
is equivalent to the sequence:
List<Integer> ints = new ArrayList<Integer>();
ints.add(Integer.valueOf(1));
int n = ints.get(0).intValue();
The call Integer.valueOf(1) is similar in effect to the expression new Integer(1), but
may cache some values for improved performance, as we explain shortly.
Here, again, is the code to find the sum of a list of integers, conveniently packaged as
a static method:
public static int sum (List<Integer> ints) {
 int s = 0;
 for (int n : ints) { s += n; }
 return s;
}
Why does the argument have type List<Integer> and not List<int>? Because type
parameters must always be bound to reference types, not primitive types. Why does
the result have type int and not Integer? Because result types may be either primitive
1.2 Boxing and Unboxing | 7
or reference types, and it is more efficient to use the former than the latter. Unboxing
occurs when each Integer in the list ints is bound to the variable n of type int.
We could rewrite the method, replacing each occurrence of int with Integer:
public static Integer sumInteger(List<Integer> ints) {
 Integer s = 0;
 for (Integer n : ints) { s += n; }
 return s;
}
This code compiles but performs a lot of needless work. Each iteration of the loop
unboxes the values in s and n, performs the addition, and boxes up the result again.
With Sun’s current compiler, measurements show that this version is about 60 percent
slower than the original.
Look Out for This! One subtlety of boxing and unboxing is that == is defined differently on primitive and on reference types. On type int, it is defined by equality of values,
and on type Integer, it is defined by object identity. So both of the following assertions
succeed using Sun’s JVM:
List<Integer> bigs = Arrays.asList(100,200,300);
assert sumInteger(bigs) == sum(bigs);
assert sumInteger(bigs) != sumInteger(bigs); // not recommended
In the first assertion, unboxing causes values to be compared, so the results are equal.
In the second assertion, there is no unboxing, and the two method calls return distinct
Integer objects, so the results are unequal even though both Integer objects represent
the same value, 600.We recommend that you never use == to compare values of type
Integer. Either unbox first, so == compares values of type int, or else use equals to
compare values of type Integer.
A further subtlety is that boxed values may be cached. Caching is required when boxing
an int or short value between–128 and 127, a char value between '\u0000' and
'\u007f', a byte, or a boolean; and caching is permitted when boxing other values.
Hence, in contrast to our earlier example, we have the following:
List<Integer> smalls = Arrays.asList(1,2,3);
assert sumInteger(smalls) == sum(smalls);
assert sumInteger(smalls) == sumInteger(smalls); // not recommended
This is because 6 is smaller than 128, so boxing the value 6 always returns exactly the
same object. In general, it is not specified whether boxing the same value twice should
return identical or distinct objects, so the inequality assertion shown earlier may either
fail or succeed depending on the implementation. Even for small values, for which ==
will compare values of type Integer correctly, we recommend against its use. It is clearer
and cleaner to use equals rather than == to compare values of reference type, such as
Integer or String.
8 | Chapter 1: Introduction
1.3 Foreach
Here, again, is our code that computes the sum of a list of integers.
List<Integer> ints = Arrays.asList(1,2,3);
int s = 0;
for (int n : ints) { s += n; }
assert s == 6;
The loop in the third line is called a foreach loop even though it is written with the
keyword for. It is equivalent to the following:
for (Iterator<Integer> it = ints. iterator(); it.hasNext(); ) {
 int n = it.next();
 s += n;
}
The emphasized code corresponds to what was written by the user, and the unemphasized code is added in a systematic way by the compiler. It introduces the variable it
of type Iterator<Integer> to iterate over the list ints of type List<Integer>. In general,
the compiler invents a new name that is guaranteed not to clash with any name already
in the code. Note that unboxing occurs when the expression it.next() of type Inte
ger is assigned to the variable n of type int.
The foreach loop can be applied to any object that implements the interface Itera
ble<E> (in package java.lang), which in turn refers to the interface Iterator<E> (in
package java.util). These define the methods iterator, hasNext, and next, which are
used by the translation of the foreach loop (iterators also have a method remove, which
is not used by the translation):
interface Iterable<E> {
 public Iterator<E> iterator();
}
interface Iterator<E> {
 public boolean hasNext();
 public E next();
 public void remove();
}
All collections, sets, and lists in the Collections Framework implement the Itera
ble<E> interface; and classes defined by other vendors or users may implement it as well.
The foreach loop may also be applied to an array:
public static int sumArray(int[] a) {
 int s = 0;
 for (int n : a) { s += n; }
 return s;
}
The foreach loop was deliberately kept simple and catches only the most common case.
You need to explicitly introduce an iterator if you wish to use the remove method or to
1.3 Foreach | 9
iterate over more than one list in parallel. Here is a method that removes negative
elements from a list of doubles:
public static void removeNegative(List<Double> v) {
 for (Iterator<Double> it = v.iterator(); it.hasNext();) {
 if (it.next() < 0) it.remove();
 }
}
Here is a method to compute the dot product of two vectors, represented as lists of
doubles, both of the same length. Given two vectors, u1
, … , un
 and v1
, … , vn
, it
computes u1
 * v1> + … + un
 * vn
:
public static double dot(List<Double> u, List<Double> v) {
 if (u.size() != v.size())
 throw new IllegalArgumentException("different sizes");
 double d = 0;
 Iterator<Double> uIt = u.iterator();
 Iterator<Double> vIt = v.iterator();
 while (uIt.hasNext()) {
 assert uIt.hasNext() && vIt.hasNext();
 d += uIt.next() * vIt.next();
 }
 assert !uIt.hasNext() && !vIt.hasNext();
 return d;
}
Two iterators, uIt and vIt, advance across the lists u and v in lock step. The loop
condition checks only the first iterator, but the assertions confirm that we could have
used the second iterator instead, since we previously tested both lists to confirm that
they have the same length.
1.4 Generic Methods and Varargs
Here is a method that accepts an array of any type and converts it to a list:
class Lists {
 public static <T> List<T> toList(T[] arr) {
 List<T> list = new ArrayList<T>();
 for (T elt : arr) list.add(elt);
 return list;
 }
}
The static method toList accepts an array of type T[] and returns a list of type
List<T>, and does so for any type T. This is indicated by writing <T> at the beginning
of the method signature, which declares T as a new type variable. A method which
declares a type variable in this way is called a generic method. The scope of the type
variable T is local to the method itself; it may appear in the method signature and the
method body, but not outside the method.
The method may be invoked as follows:
10 | Chapter 1: Introduction
List<Integer> ints = Lists.toList(new Integer[] { 1, 2, 3 });
List<String> words = Lists.toList(new String[] { "hello", "world" });
In the first line, boxing converts 1, 2, 3 from int to Integer.
Packing the arguments into an array is cumbersome. The vararg feature permits a special, more convenient syntax for the case in which the last argument of a method is an
array. To use this feature, we replace T[] with T… in the method declaration:
class Lists {
 public static <T> List<T> toList(T... arr) {
 List<T> list = new ArrayList<T>();
 for (T elt : arr) list.add(elt);
 return list;
 }
}
Now the method may be invoked as follows:
List<Integer> ints = Lists.toList(1, 2, 3);
List<String> words = Lists.toList("hello", "world");
This is just shorthand for what we wrote above. At run time, the arguments are packed
into an array which is passed to the method, just as previously.
Any number of arguments may precede a last vararg argument. Here is a method that
accepts a list and adds all the additional arguments to the end of the list:
public static <T> void addAll(List<T> list, T... arr) {
 for (T elt : arr) list.add(elt);
}
Whenever a vararg is declared, one may either pass a list of arguments to be implicitly
packed into an array, or explicitly pass the array directly. Thus, the preceding method
may be invoked as follows:
List<Integer> ints = new ArrayList<Integer>();
Lists.addAll(ints, 1, 2);
Lists.addAll(ints, new Integer[] { 3, 4 });
assert ints.toString().equals("[1, 2, 3, 4]");
We will see later that when we attempt to create an array containing a generic type, we
will always receive an unchecked warning. Since varargs always create an array, they
should be used only when the argument does not have a generic type (see Section 6.8).
In the preceding examples, the type parameter to the generic method is inferred, but it
may also be given explicitly, as in the following examples:
List<Integer> ints = Lists.<Integer>toList();
List<Object> objs = Lists.<Object>toList(1, "two");
Explicit parameters are usually not required, but they are helpful in the examples given
here. In the first example, without the type parameter there is too little information for
the type inference algorithm used by Sun's compiler to infer the correct type. It infers
that the argument to toList is an empty array of an arbitrary generic type rather than
1.4 Generic Methods and Varargs | 11
an empty array of integers, and this triggers the unchecked warning described earlier.
(The Eclipse compiler uses a different inference algorithm, and compiles the same line
correctly without the explicit parameter.) In the second example, without the type
parameter there is too much information for the type inference algorithm to infer the
correct type. You might think that Object is the only type that an integer and a string
have in common, but in fact they also both implement the interfaces Serializable and
Comparable. The type inference algorithm cannot choose which of these three is the
correct type.
In general, the following rule of thumb suffices: in a call to a generic method, if there
are one or more arguments that correspond to a type parameter and they all have the
same type then the type parameter may be inferred; if there are no arguments that
correspond to the type parameter or the arguments belong to different subtypes of the
intended type then the type parameter must be given explicitly.
When a type parameter is passed to a generic method invocation, it appears in angle
brackets to the left, just as in the method declaration. The Java grammar requires that
type parameters may appear only in method invocations that use a dotted form. Even
if the method toList is defined in the same class that invokes the code, we cannot
shorten it as follows:
List<Integer> ints = <Integer>toList(); // compile-time error
This is illegal because it will confuse the parser.
Methods Arrays.asList and Collections.addAll in the Collections Framework are
similar to toList and addAll shown earlier. (Both classes are in package java.util.) The
Collections Framework version of asList does not return an ArrayList, but instead
returns a specialized list class that is backed by a given array. Also, its version of
addAll acts on general collections, not just lists.
1.5 Assertions
We clarify our code by liberal use of the assert statement. Each occurrence of assert
is followed by a boolean expression that is expected to evaluate to true. If assertions
are enabled and the expression evaluates to false, an AssertionError is thrown, including an indication of where the error occurred. Assertions are enabled by invoking
the JVM with the -ea or -enableassertions flag.
We only write assertions that we expect to evaluate to true. Since assertions may not
be enabled, an assertion should never have side effects upon which any nonassertion
code depends. When checking for a condition that might not hold (such as confirming
that the arguments to a method call are valid), we use a conditional and throw an
exception explicitly.
12 | Chapter 1: Introduction
To sum up, we have seen how generics, boxing and unboxing, foreach loops, and
varargs work together to make Java code easier to write, having illustrated this through
the use of the Collections Framework.
1.5 Assertions | 13

CHAPTER 2
Subtyping and Wildcards
Now that we’ve covered the basics, we can start to cover more-advanced features of
generics, such as subtyping and wildcards. In this section, we’ll review how subtyping
works and we’ll see how wildcards let you use subtyping in connection with generics.
We’ll illustrate our points with examples from the Collections Framework.
2.1 Subtyping and the Substitution Principle
Subtyping is a key feature of object-oriented languages such as Java. In Java, one type
is a subtype of another if they are related by an extends or implements clause. Here are
some examples:
Integer is a subtype of Number
Double is a subtype of Number
ArrayList<E> is a subtype of List<E>
List<E> is a subtype of Collection<E>
Collection<E> is a subtype of Iterable<E>
Subtyping is transitive, meaning that if one type is a subtype of a second, and the second
is a subtype of a third, then the first is a subtype of the third. So, from the last two lines
in the preceding list, it follows that List<E> is a subtype of Iterable<E>. If one type is
a subtype of another, we also say that the second is a supertype of the first. Every
reference type is a subtype of Object, and Object is a supertype of every reference type.
We also say, trivially, that every type is a subtype of itself.
The Substitution Principle tells us that wherever a value of one type is expected, one
may provide a value of any subtype of that type:
Substitution Principle: a variable of a given type may be assigned a value of any subtype
of that type, and a method with a parameter of a given type may be invoked with an
argument of any subtype of that type.
15
Consider the interface Collection<E>. One of its methods is add, which takes a parameter of type E:
interface Collection<E> {
 public boolean add(E elt);
 ...
}
According to the Substitution Principle, if we have a collection of numbers, we may
add an integer or a double to it, because Integer and Double are subtypes of Number.
List<Number> nums = new ArrayList<Number>();
nums.add(2);
nums.add(3.14);
assert nums.toString().equals("[2, 3.14]");
Here, subtyping is used in two ways for each method call. The first call is permitted
because nums has type List<Number>, which is a subtype of Collection<Number>, and 2
has type Integer (thanks to boxing), which is a subtype of Number. The second call is
similarly permitted. In both calls, the E in List<E> is taken to be Number.
It may seem reasonable to expect that since Integer is a subtype of Number, it follows
that List<Integer> is a subtype of List<Number>. But this is not the case, because the
Substitution Principle would rapidly get us into trouble. It is not always safe to assign
a value of type List<Integer> to a variable of type List<Number>. Consider the following
code fragment:
List<Integer> ints = new ArrayList<Integer>();
ints.add(1);
ints.add(2);
List<Number> nums = ints; // compile-time error
nums.add(3.14);
assert ints.toString().equals("[1, 2, 3.14]"); // uh oh!
This code assigns variable ints to point at a list of integers, and then assigns nums to
point at the same list of integers; hence the call in the fifth line adds a double to this
list, as shown in the last line. This must not be allowed! The problem is prevented by
observing that here the Substitution Principle does not apply: the assignment on the
fourth line is not allowed because List<Integer> is not a subtype of List<Number>, and
the compiler reports that the fourth line is in error.
What about the reverse? Can we take List<Number> to be a subtype of List<Integer>?
No, that doesn’t work either, as shown by the following code:
List<Number> nums = new ArrayList<Number>();
nums.add(2.78);
nums.add(3.14);
List<Integer> ints = nums; // compile-time error
assert ints.toString().equals("[2.78, 3.14]"); // uh oh!
16 | Chapter 2: Subtyping and Wildcards
The problem is prevented by observing that here the Substitution Principle does not
apply: the assignment on the fourth line is not allowed because List<Number> is not a
subtype of List<Integer>, and the compiler reports that the fourth line is in error.
So List<Integer> is not a subtype of List<Number>, nor is List<Number> a subtype of
List<Integer>; all we have is the trivial case, where List<Integer> is a subtype of itself,
and we also have that List<Integer> is a subtype of Collection<Integer>.
Arrays behave quite differently; with them, Integer[] is a subtype of Number[]. We will
compare the treatment of lists and arrays later (see Section 2.5).
Sometimes we would like lists to behave more like arrays, in that we want to accept
not only a list with elements of a given type, but also a list with elements of any subtype
of a given type. For this purpose, we use wildcards.
2.2 Wildcards with extends
Another method in the Collection interface is addAll, which adds all of the members
of one collection to another collection:
interface Collection<E> {
 ...
 public boolean addAll(Collection<? extends E> c);
 ...
}
Clearly, given a collection of elements of type E, it is OK to add all members of another
collection with elements of type E. The quizzical phrase "? extends E" means that it is
also OK to add all members of a collection with elements of any type that is a subtype
of E. The question mark is called a wildcard, since it stands for some type that is a
subtype of E.
Here is an example. We create an empty list of numbers, and add to it first a list of
integers and then a list of doubles:
List<Number> nums = new ArrayList<Number>();
List<Integer> ints = Arrays.asList(1, 2);
List<Double> dbls = Arrays.asList(2.78, 3.14);
nums.addAll(ints);
nums.addAll(dbls);
assert nums.toString().equals("[1, 2, 2.78, 3.14]");
The first call is permitted because nums has type List<Number>, which is a subtype of
Collection<Number>, and ints has type List<Integer>, which is a subtype of Collec
tion<? extends Number>. The second call is similarly permitted. In both calls, E is taken
to be Number. If the method signature for addAll had been written without the wildcard,
then the calls to add lists of integers and doubles to a list of numbers would not have
been permitted; you would only have been able to add a list that was explicitly declared
to be a list of numbers.
2.2 Wildcards with extends | 17
We can also use wildcards when declaring variables. Here is a variant of the example
at the end of the preceding section, changed by adding a wildcard to the second line:
List<Integer> ints = new ArrayList<Integer>();
ints.add(1);
ints.add(2);
List<? extends Number> nums = ints;
nums.add(3.14); // compile-time error
assert ints.toString().equals("[1, 2, 3.14]"); // uh oh!
Before, the fourth line caused a compile-time error (because List<Integer> is not a
subtype of List<Number>), but the fifth line was fine (because a double is a number, so
you can add a double to a List<Number>). Now, the fourth line is fine (because
List<Integer> is a subtype of List<? extends Number>), but the fifth line causes a compile-time error (because you cannot add a double to a List<? extends Number>, since it
might be a list of some other subtype of number). As before, the last line shows why
one of the preceding lines is illegal!
In general, if a structure contains elements with a type of the form ? extends E, we can
get elements out of the structure, but we cannot put elements into the structure. To
put elements into the structure we need another kind of wildcard, as explained in the
next section.
2.3 Wildcards with super
Here is a method that copies into a destination list all of the elements from a source
list, from the convenience class Collections:
public static <T> void copy(List<? super T> dst, List<? extends T> src) {
 for (int i = 0; i < src.size(); i++) {
 dst.set(i, src.get(i));
 }
}
The quizzical phrase ? super T means that the destination list may have elements of
any type that is a supertype of T, just as the source list may have elements of any type
that is a subtype of T.
Here is a sample call.
List<Object> objs = Arrays.<Object>asList(2, 3.14, "four");
List<Integer> ints = Arrays.asList(5, 6);
Collections.copy(objs, ints);
assert objs.toString().equals("[5, 6, four]");
As with any generic method, the type parameter may be inferred or may be given explicitly. In this case, there are four possible choices, all of which type-check and all of
which have the same effect:
18 | Chapter 2: Subtyping and Wildcards
Collections.copy(objs, ints);
Collections.<Object>copy(objs, ints);
Collections.<Number>copy(objs, ints);
Collections.<Integer>copy(objs, ints);
The first call leaves the type parameter implicit; it is taken to be Integer, since that is
the most specific choice that works. In the third line, the type parameter T is taken to
be Number. The call is permitted because objs has type List<Object>, which is a subtype
of List<? super Number> (since Object is a supertype of Number, as required by the
wildcard) and ints has type List<Integer>, which is a subtype of List<? extends Num
ber> (since Integer is a subtype of Number, as required by the extends wildcard).
We could also declare the method with several possible signatures.
public static <T> void copy(List<T> dst, List<T> src)
public static <T> void copy(List<T> dst, List<? extends T> src)
public static <T> void copy(List<? super T> dst, List<T> src)
public static <T> void copy(List<? super T> dst, List<? extends T> src)
The first of these is too restrictive, as it only permits calls when the destination and
source have exactly the same type. The remaining three are equivalent for calls that use
implicit type parameters, but differ for explicit type parameters. For the example calls
above, the second signature works only when the type parameter is Object, the third
signature works only when the type parameter is Integer, and the last signature works
(as we have seen) for all three type parameters—i.e., Object, Number, and Integer. Always use wildcards where you can in a signature, since this permits the widest range
of calls.
2.4 The Get and Put Principle
It may be good practice to insert wildcards whenever possible, but how do you decide
which wildcard to use? Where should you use extends, where should you use super,
and where is it inappropriate to use a wildcard at all?
Fortunately, a simple principle determines which is appropriate.
The Get and Put Principle: use an extends wildcard when you only get values out of a
structure, use a super wildcard when you only put values into a structure, and don’t use
a wildcard when you both get and put.
We already saw this principle at work in the signature of the copy method:
public static <T> void copy(List<? super T> dest, List<? extends T> src)
The method gets values out of the source src, so it is declared with an extends wildcard,
and it puts values into the destination dst, so it is declared with a super wildcard.
Whenever you use an iterator, you get values out of a structure, so use an extends
wildcard. Here is a method that takes a collection of numbers, converts each to a double, and sums them up:
2.4 The Get and Put Principle | 19
public static double sum(Collection<? extends Number> nums) {
 double s = 0.0;
 for (Number num : nums) s += num.doubleValue();
 return s;
}
Since this uses extends, all of the following calls are legal:
List<Integer> ints = Arrays.asList(1,2,3);
assert sum(ints) == 6.0;
List<Double> doubles = Arrays.asList(2.78,3.14);
assert sum(doubles) == 5.92;
List<Number> nums = Arrays.<Number>asList(1,2,2.78,3.14);
assert sum(nums) == 8.92;
The first two calls would not be legal if extends was not used.
Whenever you use the add method, you put values into a structure, so use a super
wildcard. Here is a method that takes a collection of numbers and an integer n, and
puts the first n integers, starting from zero, into the collection:
public static void count(Collection<? super Integer> ints, int n) {
 for (int i = 0; i < n; i++) ints.add(i);
}
Since this uses super, all of the following calls are legal:
List<Integer> ints = new ArrayList<Integer>();
count(ints, 5);
assert ints.toString().equals("[0, 1, 2, 3, 4]");
List<Number> nums = new ArrayList<Number>();
count(nums, 5); nums.add(5.0);
assert nums.toString().equals("[0, 1, 2, 3, 4, 5.0]");
List<Object> objs = new ArrayList<Object>();
count(objs, 5); objs.add("five");
assert objs.toString().equals("[0, 1, 2, 3, 4, five]");
The last two calls would not be legal if super was not used.
Whenever you both put values into and get values out of the same structure, you should
not use a wildcard.
public static double sumCount(Collection<Number> nums, int n) {
 count(nums, n);
 return sum(nums);
}
The collection is passed to both sum and count, so its element type must both extend
Number (as sum requires) and be super to Integer (as count requires). The only two classes
that satisfy both of these constraints are Number and Integer, and we have picked the
first of these. Here is a sample call:
20 | Chapter 2: Subtyping and Wildcards
List<Number> nums = new ArrayList<Number>();
double sum = sumCount(nums,5);
assert sum == 10;
Since there is no wildcard, the argument must be a collection of Number.
If you don’t like having to choose between Number and Integer, it might occur to you
that if Java let you write a wildcard with both extends and super, you would not need
to choose. For instance, we could write the following:
double sumCount(Collection<? extends Number super Integer> coll, int n)
// not legal Java!
Then we could call sumCount on either a collection of numbers or a collection of integers.
But Java doesn’t permit this. The only reason for outlawing it is simplicity, and conceivably Java might support such notation in the future. But, for now, if you need to
both get and put then don’t use wildcards.
The Get and Put Principle also works the other way around. If an extends wildcard is
present, pretty much all you will be able to do is get but not put values of that type;
and if a super wildcard is present, pretty much all you will be able to do is put but not
get values of that type.
For example, consider the following code fragment, which uses a list declared with an
extends wildcard:
List<Integer> ints = new ArrayList<Integer>();
ints.add(1);
ints.add(2);
List<? extends Number> nums = ints;
double dbl = sum(nums); // ok
nums.add(3.14); // compile-time error
The call to sum is fine, because it gets values from the list, but the call to add is not,
because it puts a value into the list. This is just as well, since otherwise we could add
a double to a list of integers!
Conversely, consider the following code fragment, which uses a list declared with a
super wildcard:
List<Object> objs = new ArrayList<Object>();
objs.add(1);
objs.add("two");
List<? super Integer> ints = objs;
ints.add(3); // ok
double dbl = sum(ints); // compile-time error
Now the call to add is fine, because it puts a value into the list, but the call to sum is not,
because it gets a value from the list. This is just as well, because the sum of a list
containing a string makes no sense!
The exception proves the rule, and each of these rules has one exception. You cannot
put anything into a type declared with an extends wildcard—except for the value
null, which belongs to every reference type:
2.4 The Get and Put Principle | 21
List<Integer> ints = new ArrayList<Integer>();
ints.add(1);
ints.add(2);
List<? extends Number> nums = ints;
nums.add(null); // ok
assert nums.toString().equals("[1, 2, null]");
Similarly, you cannot get anything out from a type declared with a super wildcard—
except for a value of type Object, which is a supertype of every reference type:
List<Object> objs = Arrays.<Object>asList(1,"two");
List<? super Integer> ints = objs;
String str = "";
for (Object obj : ints) str += obj.toString();
assert str.equals("1two");
You may find it helpful to think of ? extends T as containing every type in an interval
bounded by the type of null below and by T above (where the type of null is a subtype
of every reference type). Similarly, you may think of ? super T as containing every type
in an interval bounded by T below and by Object above.
It is tempting to think that an extends wildcard ensures immutability, but it does not.
As we saw earlier, given a list of type List<? extends Number>, you may still add null
values to the list. You may also remove list elements (using remove, removeAll, or retai
nAll) or permute the list (using swap, sort, or shuffle in the convenience class Collec
tions; see Section 17.1.1). If you want to ensure that a list cannot be changed, use the
method unmodifiableList in the class Collections; similar methods exist for other collection classes (see Section 17.3.2). If you want to ensure that list elements cannot be
changed, consider following the rules for making a class immutable given by Joshua
Bloch in his book Effective Java (Addison-Wesley) in Chapter 4 (item “Minimize mutability”/“Favor immutability”); for example, in Part II, the classes CodingTask and
PhoneTask in Section 12.1 are immutable, as is the class PriorityTask in Section 13.2.
Because String is final and can have no subtypes, you might expect that
List<String> is the same type as List<? extends String>. But in fact the former is a
subtype of the latter, but not the same type, as can be seen by an application of our
principles. The Substitution Principle tells us it is a subtype, because it is fine to pass a
value of the former type where the latter is expected. The Get and Put Principle tells us
that it is not the same type, because we can add a string to a value of the former type
but not the latter.
2.5 Arrays
It is instructive to compare the treatment of lists and arrays in Java, keeping in mind
the Substitution Principle and the Get and Put Principle.
In Java, array subtyping is covariant, meaning that type S[] is considered to be a subtype
of T[] whenever S is a subtype of T. Consider the following code fragment, which al22 | Chapter 2: Subtyping and Wildcards
locates an array of integers, assigns it to an array of numbers, and then attempts to
assign a double into the array:
Integer[] ints = new Integer[] {1,2,3};
Number[] nums = ints;
nums[2] = 3.14; // array store exception
assert Arrays.toString(ints).equals("[1, 2, 3.14]"); // uh oh!
Something is wrong with this program, since it puts a double into an array of integers!
Where is the problem? Since Integer[] is considered a subtype of Number[], according
to the Substitution Principle the assignment on the second line must be legal. Instead,
the problem is caught on the third line, and it is caught at run time. When an array is
allocated (as on the first line), it is tagged with its reified type (a run-time representation
of its component type, in this case, Integer), and every time an array is assigned into
(as on the third line), an array store exception is raised if the reified type is not compatible with the assigned value (in this case, a double cannot be stored into an array of
Integer).
In contrast, the subtyping relation for generics is invariant, meaning that type List<S> is
not considered to be a subtype of List<T>, except in the trivial case where S and T are
identical. Here is a code fragment analogous to the preceding one, with lists replacing
arrays:
List<Integer> ints = Arrays.asList(1,2,3);
List<Number> nums = ints; // compile-time error
nums.set(2, 3.14);
assert ints.toString().equals("[1, 2, 3.14]"); // uh oh!
Since List<Integer> is not considered to be a subtype of List<Number>, the problem is
detected on the second line, not the third, and it is detected at compile time, not run
time.
Wildcards reintroduce covariant subtyping for generics, in that type List<S> is considered to be a subtype of List<? extends T> when S is a subtype of T. Here is a third
variant of the fragment:
List<Integer> ints = Arrays.asList(1,2,3);
List<? extends Number> nums = ints;
nums.set(2, 3.14); // compile-time error
assert ints.toString().equals("[1, 2, 3.14]"); // uh oh!
As with arrays, the third line is in error, but, in contrast to arrays, the problem is detected
at compile time, not run time. The assignment violates the Get and Put Principle, because you cannot put a value into a type declared with an extends wildcard.
Wildcards also introduce contravariant subtyping for generics, in that type List<S> is
considered to be a subtype of List<? super T> when S is a supertype of T (as opposed
to a subtype). Arrays do not support contravariant subtyping. For instance, recall that
the method count accepted a parameter of type Collection<? super Integer> and filled
it with integers. There is no equivalent way to do this with an array, since Java does
not permit you to write (? super Integer)[].
2.5 Arrays | 23
Detecting problems at compile time rather than at run time brings two advantages, one
minor and one major. The minor advantage is that it is more efficient. The system does
not need to carry around a description of the element type at run time, and the system
does not need to check against this description every time an assignment into an array
is performed. The major advantage is that a common family of errors is detected by the
compiler. This improves every aspect of the program’s life cycle: coding, debugging,
testing, and maintenance are all made easier, quicker, and less expensive.
Apart from the fact that errors are caught earlier, there are many other reasons to prefer
collection classes to arrays. Collections are far more flexible than arrays. The only operations supported on arrays are to get or set a component, and the representation is
fixed. Collections support many additional operations, including testing for containment, adding and removing elements, comparing or combining two collections, and
extracting a sublist of a list. Collections may be either lists (where order is significant
and elements may be repeated) or sets (where order is not significant and elements may
not be repeated), and a number of representations are available, including arrays, linked
lists, trees, and hash tables. Finally, a comparison of the convenience classes Collec
tions and Arrays shows that collections offer many operations not provided by arrays,
including operations to rotate or shuffle a list, to find the maximum of a collection, and
to make a collection unmodifiable or synchronized.
Nonetheless, there are a few cases where arrays are preferred over collections. Arrays
of primitive type are much more efficient since they don’t involve boxing; and assignments into such an array need not check for an array store exception, because arrays
of primitive type do not have subtypes. And despite the check for array store exceptions,
even arrays of reference type may be more efficient than collection classes with the
current generation of compilers, so you may want to use arrays in crucial inner loops.
As always, you should measure performance to justify such a design, especially since
future compilers may optimize collection classes specially. Finally, in some cases arrays
may be preferable for reasons of compatibility.
To summarize, it is better to detect errors at compile time rather than run time, but
Java arrays are forced to detect certain errors at run time by the decision to make array
subtyping covariant. Was this a good decision? Before the advent of generics, it was
absolutely necessary. For instance, look at the following methods, which are used to
sort any array or to fill an array with a given value:
public static void sort(Object[] a);
public static void fill(Object[] a, Object val);
Thanks to covariance, these methods can be used to sort or fill arrays of any reference
type. Without covariance and without generics, there would be no way to declare
methods that apply for all types. However, now that we have generics, covariant arrays
are no longer necessary. Now we can give the methods the following signatures, directly
stating that they work for all types:
public static <T> void sort(T[] a);
public static <T> void fill(T[] a, T val);
24 | Chapter 2: Subtyping and Wildcards
In some sense, covariant arrays are an artifact of the lack of generics in earlier versions
of Java. Once you have generics, covariant arrays are probably the wrong design choice,
and the only reason for retaining them is backward compatibility.
Sections Section 6.4–Section 6.8 discuss inconvenient interactions between generics
and arrays. For many purposes, it may be sensible to consider arrays a deprecated
type.We return to this point in Section 6.9.
2.6 Wildcards Versus Type Parameters
The contains method checks whether a collection contains a given object, and its generalization, containsAll, checks whether a collection contains every element of another
collection. This section presents two alternate approaches to giving generic signatures
for these methods. The first approach uses wildcards and is the one used in the Java
Collections Framework. The second approach uses type parameters and is often a more
appropriate alternative.
Wildcards Here are the types that the methods have in Java with generics:
interface Collection<E> {
 ...
 public boolean contains(Object o);
 public boolean containsAll(Collection<?> c);
 ...
}
The first method does not use generics at all! The second method is our first sight of
an important abbreviation. The type Collection<?> stands for:
Collection<? extends Object>
Extending Object is one of the most common uses of wildcards, so it makes sense to
provide a short form for writing it.
These methods let us test for membership and containment:
Object obj = "one";
List<Object> objs = Arrays.<Object>asList("one", 2, 3.14, 4);
List<Integer> ints = Arrays.asList(2, 4);
assert objs.contains(obj);
assert objs.containsAll(ints);
assert !ints.contains(obj);
assert !ints.containsAll(objs);
The given list of objects contains both the string "one" and the given list of integers,
but the given list of integers does not contain the string "one", nor does it contain the
given list of objects.
The tests ints.contains(obj) and ints.containsAll(objs) might seem silly. Of course,
a list of integers won’t contain an arbitrary object, such as the string "one". But it is
permitted because sometimes such tests might succeed:
2.6 Wildcards Versus Type Parameters | 25
Object obj = 1;
List<Object> objs = Arrays.<Object>asList(1, 3);
List<Integer> ints = Arrays.asList(1, 2, 3, 4);
assert ints.contains(obj);
assert ints.containsAll(objs);
In this case, the object may be contained in the list of integers because it happens to be
an integer, and the list of objects may be contained within the list of integers because
every object in the list happens to be an integer.
Type Parameters You might reasonably choose an alternative design for collections
—a design in which you can only test containment for subtypes of the element type:
interface MyCollection<E> { // alternative design
 ...
 public boolean contains(E o);
 public boolean containsAll(Collection<? extends E> c);
 ...
}
Say we have a class MyList that implements MyCollection. Now the tests are legal only
one way around:
Object obj = "one";
MyList<Object> objs = MyList.<Object>asList("one", 2, 3.14, 4);
MyList<Integer> ints = MyList.asList(2, 4);
assert objs.contains(obj);
assert objs.containsAll(ints)
assert !ints.contains(obj); // compile-time error
assert !ints.containsAll(objs); // compile-time error
The last two tests are illegal, because the type declarations require that we can only test
whether a list contains an element of a subtype of that list. So we can check whether a
list of objects contains a list of integers, but not the other way around.
Which of the two styles is better is a matter of taste. The first permits more tests, and
the second catches more errors at compile time (while also ruling out some sensible
tests). The designers of the Java libraries chose the first, more liberal, alternative, because someone using the Collections Framework before generics might well have written a test such as ints.containsAll(objs), and that person would want that test to
remain valid after generics were added to Java. However, when designing a new generic
library, such as MyCollection, when backward compatibility is less important, the design that catches more errors at compile time might make more sense.
Arguably, the library designers made the wrong choice. Only rarely will a test such as
ints.containsAll(objs) be required, and such a test can still be permitted by declaring
ints to have type List<Object> rather than type List<Integer>. It might have been better
to catch more errors in the common case rather than to permit more-precise typing in
an uncommon case.
The same design choice applies to other methods that contain Object or Collection<?
> in their signature, such as remove, removeAll, and retainAll.
26 | Chapter 2: Subtyping and Wildcards
2.7 Wildcard Capture
When a generic method is invoked, the type parameter may be chosen to match the
unknown type represented by a wildcard. This is called wildcard capture.
Consider the method reverse in the convenience class java.util.Collections, which
accepts a list of any type and reverses it. It can be given either of the following two
signatures, which are equivalent:
public static void reverse(List<?> list);
public static void <T> reverse(List<T> list);
The wildcard signature is slightly shorter and clearer, and is the one used in the library.
If you use the second signature, it is easy to implement the method:
public static void <T> reverse(List<T> list) {
 List<T> tmp = new ArrayList<T>(list);
 for (int i = 0; i < list.size(); i++) {
 list.set(i, tmp.get(list.size()-i-1));
 }
}
This copies the argument into a temporary list, and then writes from the copy back into
the original in reverse order.
If you try to use the first signature with a similar method body, it won’t work:
public static void reverse(List<?> list) {
 List<Object> tmp = new ArrayList<Object>(list);
 for (int i = 0; i < list.size(); i++) {
 list.set(i, tmp.get(list.size()-i-1)); // compile-time error
 }
}
Now it is not legal to write from the copy back into the original, because we are trying
to write from a list of objects into a list of unknown type. Replacing List<Object> with
List<?> won’t fix the problem, because now we have two lists with (possibly different)
unknown element types.
Instead, you can implement the method with the first signature by implementing a
private method with the second signature, and calling the second from the first:
public static void reverse(List<?> list) { rev(list); }
private static <T> void rev(List<T> list) {
 List<T> tmp = new ArrayList<T>(list);
 for (int i = 0; i < list.size(); i++) {
 list.set(i, tmp.get(list.size()-i-1));
 }
}
Here we say that the type variable T has captured the wildcard. This is a generally useful
technique when dealing with wildcards, and it is worth knowing.
2.7 Wildcard Capture | 27
Another reason to know about wildcard capture is that it can show up in error messages,
even if you don’t use the above technique. In general, each occurrence of a wildcard is
taken to stand for some unknown type. If the compiler prints an error message containing this type, it is referred to as capture of ?. For instance, with Sun’s current
compiler, the incorrect version of reverse generates the following error message:
Capture.java:6: set(int,capture of ?) in java.util.List<capture of ?>
cannot be applied to (int,java.lang.Object)
 list.set(i, tmp.get(list.size()-i-1));
 ^
Hence, if you see the quizzical phrase capture of ? in an error message, it will come
from a wildcard type. Even if there are two distinct wildcards, the compiler will print
the type associated with each as capture of ?. Bounded wildcards generate names that
are even more long-winded, such as capture of ? extends Number.
2.8 Restrictions on Wildcards
Wildcards may not appear at the top level in class instance creation expressions (new),
in explicit type parameters in generic method calls, or in supertypes (extends and
implements).
Instance Creation In a class instance creation expression, if the type is a parameterized
type, then none of the type parameters may be wildcards. For example, the following
are illegal:
List<?> list = new ArrayList<?>(); // compile-time error
Map<String, ? extends Number> map
 = new HashMap<String, ? extends Number>(); // compile-time error
This is usually not a hardship. The Get and Put Principle tells us that if a structure
contains a wildcard, we should only get values out of it (if it is an extends wildcard) or
only put values into it (if it is a super wildcard). For a structure to be useful, we must
do both. Therefore, we usually create a structure at a precise type, even if we use wildcard types to put values into or get values from the structure, as in the following example:
List<Number> nums = new ArrayList<Number>();
List<? super Number> sink = nums;
List<? extends Number> source = nums;
for (int i=0; i<10; i++) sink.add(i);
double sum=0; for (Number num : source) sum+=num.doubleValue();
Here wildcards appear in the second and third lines, but not in the first line that creates
the list.
Only top-level parameters in instance creation are prohibited from containing wildcards. Nested wildcards are permitted. Hence, the following is legal:
28 | Chapter 2: Subtyping and Wildcards
List<List<?>> lists = new ArrayList<List<?>>();
lists.add(Arrays.asList(1,2,3));
lists.add(Arrays.asList("four","five"));
assert lists.toString().equals("[[1, 2, 3], [four, five]]");
Even though the list of lists is created at a wildcard type, each individual list within it
has a specific type: the first is a list of integers and the second is a list of strings. The
wildcard type prohibits us from extracting elements from the inner lists as any type
other than Object, but since that is the type used by toString, this code is well typed.
One way to remember the restriction is that the relationship between wildcards and
ordinary types is similar to the relationship between interfaces and classes—wildcards
and interfaces are more general, ordinary types and classes are more specific, and instance creation requires the more specific information. Consider the following three
statements:
List<?> list = new ArrayList<Object>(); // ok
List<?> list = new List<Object>() // compile-time error
List<?> list = new ArrayList<?>() // compile-time error
The first is legal; the second is illegal because an instance creation expression requires
a class, not an interface; and the third is illegal because an instance creation expression
requires an ordinary type, not a wildcard.
You might wonder why this restriction is necessary. The Java designers had in mind
that every wildcard type is shorthand for some ordinary type, so they believed that
ultimately every object should be created with an ordinary type. It is not clear whether
this restriction is necessary, but it is unlikely to be a problem. (We tried hard to contrive
a situation in which it was a problem, and we failed!)
Generic Method Calls If a generic method call includes explicit type parameters, those
type parameters must not be wildcards. For example, say we have the following generic
method:
class Lists {
 public static <T> List<T> factory() { return new ArrayList<T>(); }
}
You may choose for the type parameters to be inferred, or you may pass an explicit
type parameter. Both of the following are legal:
List<?> list = Lists.factory();
List<?> list = Lists.<Object>factory();
If an explicit type parameter is passed, it must not be a wildcard:
List<?> list = Lists.<?>factory(); // compile-time error
As before, nested wildcards are permitted:
List<List<?>> = Lists.<List<?>>factory(); // ok
The motivation for this restriction is similar to the previous one. Again, it is not clear
whether it is necessary, but it is unlikely to be a problem.
2.8 Restrictions on Wildcards | 29
Supertypes When a class instance is created, it invokes the initializer for its supertype.
Hence, any restriction that applies to instance creation must also apply to supertypes.
In a class declaration, if the supertype or any superinterface has type parameters, these
types must not be wildcards.
For example, this declaration is illegal:
class AnyList extends ArrayList<?> {...} // compile-time error
And so is this:
class AnotherList implements List<?> {...} // compile-time error
But, as before, nested wildcards are permitted:
class NestedList extends ArrayList<List<?>> {...} // ok
The motivation for this restriction is similar to the previous two. As before, it is not
clear whether it is necessary, but it is unlikely to be a problem.



Comparison and Bounds
Now that we have the basics, let’s look at some more advanced uses of generics. This
chapter describes the interfaces Comparable<T> and Comparator<T>, which are used to
support comparison on elements. These interfaces are useful, for instance, if you want
to find the maximum element of a collection or sort a list. Along the way, we will
introduce bounds on type variables, an important feature of generics that is particularly
useful in combination with the Comparable<T> interface.
3.1 Comparable
The interface Comparable<T> contains a method that can be used to compare one object
to another:
interface Comparable<T> {
 public int compareTo(T o);
}
The compareTo method returns a value that is negative, zero, or positive depending upon
whether the argument is less than, equal to, or greater than the given object. When a
class implements Comparable, the ordering specified by this interface is called the natural
ordering for that class.
Typically, an object belonging to a class can only be compared with an object belonging
to the same class. For instance, Integer implements Comparable<Integer>:
Integer int0 = 0;
Integer int1 = 1;
assert int0.compareTo(int1) < 0;
The comparison returns a negative number, since 0 precedes 1 under numerical ordering. Similarly, String implements Comparable<String>:
String str0 = "zero";
String str1 = "one";
assert str0.compareTo(str1) > 0;
This comparison returns a positive number, since "zero" follows "one" under alphabetic
ordering.
31
The type parameter to the interface allows nonsensical comparisons to be caught at
compile time:
Integer i = 0;
String s = "one";
assert i.compareTo(s) < 0; // compile-time error
You can compare an integer with an integer or a string with a string, but attempting to
compare an integer with a string signals a compile-time error.
Comparison is not supported between arbitrary numerical types:
Number m = new Integer(2);
Number n = new Double(3.14);
assert m.compareTo(n) < 0; // compile-time error
Here the comparison is illegal, because the Number class does not implement the Compa
rable interface.
Consistent with Equals Usually, we require that two objects are equal if and only if
they compare as the same:
x.equals(y) if and only if x.compareTo(y) == 0
In this case, we say that the natural ordering is consistent with equals.
It is recommended that when designing a class you choose a natural ordering that is
consistent with equals. This is particularly important if you use the interfaces Sorted
Set or SortedMap in the Collections Framework, both of which compare items using
natural ordering. If two items that compare as the same are added to a sorted set, then
only one will be stored, even if the two items are not equal; the same is true for a sorted
map. (You may also specify a different ordering for use with a sorted set or sorted map,
using a comparator as described in Section 3.4; but in that case the specified ordering
should again be consistent with equals.)
Almost every class in the Java core libraries that has a natural ordering is consistent
with equals. An exception is java.math.BigDecimal, which compares as the same two
decimals that have the same value but different precisions, such as 4.0 and 4.00. Section 3.3 gives another example of a class with a natural ordering that is not consistent
with equals.
Comparison differs from equality in that is does not accept a null argument. If x is not
null, x. equals(null) must return false, while x.compareTo(null) must throw a Null
PointerException.
We use standard idioms for comparison, writing x.compareTo(y) < 0 instead of x <
y, and writing x.compareTo(y) <= 0 instead of x <= y. It is perfectly sensible to use the
last of these even on types such as java.math.BigDecimal, where the natural ordering
is not consistent with equals.
32 | Chapter 3: Comparison and Bounds
Contract for Comparable The contract for the Comparable<T> interface specifies three
properties. The properties are defined using the sign function, which is defined such
that sgn(x) returns -1, 0, or 1, depending on whether x is negative, zero, or positive.
First, comparison is anti-symmetric. Reversing the order of arguments reverses the result:
sgn(x.compareTo(y)) == -sgn(y.compareTo(x))
This generalizes the property for numbers:x < y if and only if y > x. It is also required
that x.compareTo(y) raises an exception if and only if y.compareTo(x) raises an exception.
Second, comparison is transitive. If one value is smaller than a second, and the second
is smaller than a third, then the first is smaller than the third:
if x.compareTo(y) < 0 and y.compareTo(z) < 0 then x.compareTo(z) < 0
This generalizes the property for numbers: if x < y and y < z then x < z.
Third, comparison is a congruence. If two values compare as the same then they compare the same way with any third value:
if x.compareTo(y) == 0 then sgn(x.compareTo(z)) == sgn(y.compareTo(z))
This generalizes the property for numbers: if x == y then x < z if and only if y < z.
Presumably, it is also required that if x.compareTo(y) == 0 then x.compareTo(z) raises
an exception if and only if y.compareTo(z) raises an exception, although this is not
explicitly stated.
It is strongly recommended that comparison be compatible with equality:
x.equals(y) if and only if x.compareTo(y) == 0
As we saw earlier, a few exceptional classes, such as java.math.BigDecimal, violate this
constraint.
However, it is always required that comparison be reflexive. Every value compares as
the same as itself:
x.compareTo(x) == 0
This follows from the first requirement, since taking x and y to be the same gives us
sgn(x.compareTo(x)) == -sgn(x.compareTo(x)).
Look Out for This! It’sworth pointing out a subtlety in the definition of comparison.
Here is the right way to compare two integers:
class Integer implements Comparable<Integer> {
 ...
 public int compareTo(Integer that) {
 return this.value < that.value ? -1 :
 this.value == that.value ? 0 : 1 ;
 }
 ...
}
3.1 Comparable | 33
The conditional expression returns – 1, 0, or 1 depending on whether the receiver is
less than, equal to, or greater than the argument. You might think the following code
would work just as well, since the method is permitted to return any negative integer
if the receiver is less than the argument:
class Integer implements Comparable<Integer> {
 ...
 public int compareTo(Integer that) {
 // bad implementation -- don't do it this way!
 return this.value - that.value;
 }
 ...
}
But this code may give the wrong answer when there is overflow. For instance, when
comparing a large negative value to a large positive value, the difference may be more
than the largest value that can be stored in an integer, Integer.MAX_VALUE.
3.2 Maximum of a Collection
In this section, we show how to use the Comparable<T> interface to find the maximum
element in a collection.We begin with a simplified version. The actual version found
in the Collections Framework has a type signature that is a bit more complicated, and
later we will see why.
Here is code to find the maximum element in a nonempty collection, from the class
Collections:
public static <T extends Comparable<T>> T max(Collection<T> coll) {
 T candidate = coll.iterator().next();
 for (T elt : coll) {
 if (candidate.compareTo(elt) < 0) candidate = elt;
 }
 return candidate;
}
We first saw generic methods that declare new type variables in the signature in Section 1.4. For instance, the method asList takes an array of type E[] and returns a result
of type List<E>, and does so for any type E. Here we have a generic method that declares
a bound on the type variable. The method max takes a collection of type Collec
tion<T> and returns a T, and it does this for any type T such that T is a subtype of
Comparable<T>.
The highlighted phrase in angle brackets at the beginning of the type signature declares
the type variable T, and we say that T is bounded by Comparable<T>. As with wildcards,
bounds for type variables are always indicated by the keyword extends, even when the
bound is an interface rather than a class, as is the case here. Unlike wildcards, type
variables must always be bounded using extends, never super.
34 | Chapter 3: Comparison and Bounds
In this case, the bound is recursive, in that the bound on T itself depends upon T. It is
even possible to have mutually recursive bounds, such as:
<T extends C<T,U>, U extends D<T,U>>
An example of mutually recursive bounds appears in Section 9.5.
The method body chooses the first element in the collection as a candidate for the
maximum, and then compares the candidate with each element in the collection, setting
the candidate to the element when the element is larger.We use iterator().next()
rather than get(0) to get the first element, because get is not defined on collections
other than lists. The method raises a NoSuchElement exception when the collection is
empty.
When calling the method, T may be chosen to be Integer (since lnteger implements
Comparable<Integer>) or String (since String implements Comparable<String>):
List<Integer> ints = Arrays.asList(0,1,2);
assert Collections.max(ints) == 2;
List<String> strs = Arrays.asList("zero","one","two");
assert Collections.max(strs).equals("zero");
But we may not choose T to be Number (since Number does not implement Comparable):
List<Number> nums = Arrays.asList(0,1,2,3.14);
assert Collections.max(nums) == 3.14; // compile-time error
As expected, here the call to max is illegal.
Here’s an efficiency tip. The preceding implementation used a foreach loop to increase
brevity and clarity. If efficiency is a pressing concern, you might want to rewrite the
method to use an explicit iterator, as follows:
public static <T extends Comparable<T>> T max(Collection<T> coll) {
 Iterator<T> it = coll.iterator();
 T candidate = it.next();
 while (it.hasNext()) {
 T elt = it.next();
 if (candidate.compareTo(elt) < 0) candidate = elt;
 }
 return candidate;
}
This allocates an iterator once instead of twice and performs one less comparison.
Signatures for methods should be as general as possible to maximize utility. If you can
replace a type parameter with a wildcard then you should do so. We can improve the
Signature of max by replacing:
<T extends Comparable<T>> T max(Collection<T> coll)
with:
<T extends Comparable<? super T>> T max(Collection<? extends T> coll)
3.2 Maximum of a Collection | 35
Following the Get and Put Principle, we use extends with Collection because we get
values of type T from the collection, and we use super with Comparable because we
put value of type T into the compareTo method. In the next section, we’ll see an example
that would not type-check if the super clause above was omitted.
If you look at the signature of this method in the Java library, you will see something
that looks even worse than the preceding code:
<T extends Object & Comparable<? super T>>
 T max(Collection<? extends T> coll)
This is there for backward compatibility, as we will explain at the end of Section 3.6.
3.3 A Fruity Example
The Comparable<T> interface gives fine control over what can and cannot be compared.
Say that we have a Fruit class with subclasses Apple and Orange. Depending on how
we set things up, we may prohibit comparison of apples with oranges or we may permit such comparison.
Example 3-2 prohibits comparison of apples with oranges. Here are the three classes it
declares:
class Fruit {...}
class Apple extends Fruit implements Comparable<Apple> {...}
class Orange extends Fruit implements Comparable<Orange> {...}
Each fruit has a name and a size, and two fruits are equal if they have the same name
and the same size. Following good practice, we have also defined a hashCode method,
to ensure that equal objects have the same hash code. Apples are compared by comparing their sizes, and so are oranges. Since Apple implements Comparable<Apple>, it is
clear that you can compare apples with apples, but not with oranges. The test code
builds three lists, one of apples, one of oranges, and one containing mixed fruits. We
may find the maximum of the first two lists, but attempting to find the maximum of
the mixed list signals an error at compile time.
Example 3-1 permits comparison of apples with oranges. Compare these three class
declarations with those given previously (all differences between Examples Example 3-2 and Example 3-1 are highlighted):
class Fruit implements Comparable<Fruit> {...}
class Apple extends Fruit {...}
class Orange extends Fruit {...}
As before, each fruit has a name and a size, and two fruits are equal if they have the
same name and the same size. Now any two fruits are compared by ignoring their names
and comparing their sizes. Since Fruit implements Comparable<Fruit>, any two fruits
may be compared. Now the test code can find the maximum of all three lists, including
the one that mixes apples with oranges.
36 | Chapter 3: Comparison and Bounds
Recall that at the end of the previous section we extended the type signature of
compareTo to use super:
<T extends Comparable<? super T>> T max(Collection<? extends T> coll)
The second example shows why this wildcard is needed. If we want to compare two
oranges, we take T in the preceding code to be Orange:
Orange extends Comparable<? super Orange>
And this is true because both of the following hold:
Orange extends Comparable<Fruit> and Fruit super Orange
Without the super wildcard, finding the maximum of a List<Orange> would be illegal,
even though finding the maximum of a List<Fruit> is permitted.
Also note that the natural ordering used here is not consistent with equals (see Section 3.1). Two fruits with different names but the same size compare as the same, but
they are not equal.
3.4 Comparator
Sometimes we want to compare objects that do not implement the Comparable interface,
or to compare objects using a different ordering from the one specified by that interface.
The ordering provided by the Comparable interface is called the natural ordering, so the
Comparator interface provides, so to speak, an unnatural ordering.
We specify additional orderings using the Comparator interface, which contains two
methods:
interface Comparator<T> {
 public int compare(T o1, T o2);
 public boolean equals(Object obj);
}
The compare method returns a value that is negative, zero, or positive depending upon
whether the first object is less than, equal to, or greater than the second object—just
as with compareTo. (The equals method is the one familiar from class Object; it is included
in the interface to remind implementors that equal comparators must have compare
methods that impose the same ordering.)
Example 3-1. Permitting comparison of apples with oranges
abstract class Fruit implements Comparable<Fruit> {
 protected String name;
 protected int size;
 protected Fruit(String name, int size) {
 this.name = name; this.size = size;
 }
 public boolean equals(Object o) {
 if (o instanceof Fruit) {
 Fruit that = (Fruit)o;
3.4 Comparator | 37
 return this.name.equals(that.name) && this.size == that.size;
 } else return false;
 }
 public int hashCode() {
 return name.hashCode()*29 + size;
 }
 public int compareTo(Fruit that) {
 return this.size < that.size ? - 1 :
 this.size == that.size ? 0 : 1 ;
 }
}
class Apple extends Fruit {
 public Apple(int size) { super("Apple", size); }
}
class Orange extends Fruit {
 public Orange(int size) { super("Orange", size); }
}
class Test {
 public static void main(String[] args) {
 Apple a1 = new Apple(1); Apple a2 = new Apple(2);
 Orange o3 = new Orange(3); Orange o4 = new Orange(4);
 List<Apple> apples = Arrays.asList(a1,a2);
 assert Collections.max(apples).equals(a2);
 List<Orange> oranges = Arrays.asList(o3,o4);
 assert Collections.max(oranges).equals(o4);
 List<Fruit> mixed = Arrays.<Fruit>asList(a1,o3);
 assert Collections.max(mixed).equals(o3); // ok
 }
}
Example 3-2. Prohibiting comparison of apples with oranges
abstract class Fruit {
 protected String name;
 protected int size;
 protected Fruit(String name, int size) {
 this.name = name; this.size = size;
 }
 public boolean equals(Object o) {
 if (o instanceof Fruit) {
 Fruit that = (Fruit)o;
 return this.name.equals(that.name) && this.size == that.size;
 } else return false;
 }
 public int hashCode() {
 return name.hashCode()*29 + size;
 }
 protected int compareTo(Fruit that) {
 return this.size < that.size ? -1 :
 this.size == that.size ? 0 : 1 ;
 }
}
38 | Chapter 3: Comparison and Bounds
class Apple extends Fruit implements Comparable<Apple> {
 public Apple(int size) { super("Apple", size); }
 public int compareTo(Apple a) { return super.compareTo(a); }
}
class Orange extends Fruit implements Comparable<Orange> {
 public Orange(int size) { super("Orange", size); }
 public int compareTo(Orange o) { return super.compareTo(o); }
}
class Test {
 public static void main(String[] args) {
 Apple a1 = new Apple(1); Apple a2 = new Apple(2);
 Orange o3 = new Orange(3); Orange o4 = new Orange(4);

 List<Apple> apples = Arrays.asList(a1,a2);
 assert Collections.max(apples).equals(a2);
 List<Orange> oranges = Arrays.asList(o3,o4);
 assert Collections.max(oranges).equals(o4);
 List<Fruit> mixed = Arrays.<Fruit>asList(a1,o3);
 assert Collections.max(mixed).equals(o3); // compile-time error
 }
}
Here is a comparator that considers the shorter of two strings to be smaller. Only if two
strings have the same length are they compared using the natural (alphabetic) ordering.
Comparator<String> sizeOrder =
 new Comparator<String>() {
 public int compare(String s1, String s2) {
 return
 s1.length() < s2.length() ? -1 :
 s1.length() > s2.length() ? 1 :
 s1.compareTo(s2) ;
 }
};
Here is an example:
assert "two".compareTo("three") > 0;
assert sizeOrder.compare("two","three") < 0;
In the natural alphabetic ordering, "two" is greater than "three", whereas in the size
ordering it is smaller.
The Java libraries always provide a choice between Comparable and Comparator. For
every generic method with a type variable bounded by Comparable, there is another
generic method with an additional argument of type Comparator. For instance, corresponding to:
public static <T extends Comparable<? super T>>
 T max(Collection<? extends T> coll)
we also have:
3.4 Comparator | 39
public static <T>
 T max(Collection<? extends T> coll, Comparator<? super T> cmp)
There are similar methods to find the minimum. For example, here is how to find the
maximum and minimum of a list using the natural ordering and using the size ordering:
Collection<String> strings = Arrays.asList("from","aaa","to","zzz");
assert max(strings).equals("zzz");
assert min(strings).equals("aaa");
assert max(strings,sizeOrder).equals("from");
assert min(strings,sizeOrder).equals("to");
The string "from" is the maximum using the size ordering because it is longest, and
"to" is minimum because it is shortest.
Here is the code for a version of max using comparators:
public static <T>
 T max(Collection<? extends T> coll, Comparator<? super T> cmp)
{
 T candidate = coll.iterator().next();
 for (T elt : coll) {
 if (cmp.compare(candidate, elt) < 0) { candidate = elt; }
 }
 return candidate;
}
Compared to the previous version, the only change is that where before we wrote
candidate.compareTo(elt), now we write cmp.compare(candidate,elt). (For easy reference, this code and what follows is summarized in Example 3-3.)
It is easy to define a comparator that provides the natural ordering:
public static <T extends Comparable<? super T>>
 Comparator<T> naturalOrder()
{
 return new Comparator<T> {
 public int compare(T o1, T o2) { return o1.compareTo(o2); }
 }
}
Using this, it is easy to define the version of max that uses the natural ordering in terms
of the version that uses a given comparator:
public static <T extends Comparable<? super T>>
 T max(Collection<? extends T> coll)
{
 return max(coll, Comparators.<T>naturalOrder());
}
A type parameter must be explicitly supplied for the invocation of the generic method
naturalOrder, since the algorithm that infers types would fail to work out the correct
type otherwise.
It is also easy to define a method that takes a comparator and returns a new comparator
with the reverse of the given ordering:
40 | Chapter 3: Comparison and Bounds
public static <T> Comparator<T>
 reverseOrder(final Comparator<T> cmp)
{
 return new Comparator<T>() {
 public int compare(T o1, T o2) { return cmp.compare(o2,o1); }
 };
}
This simply reverses the order of arguments to the comparator. (By the contract for
comparators, it would be equivalent to leave the arguments in the original order but
negate the result.) And here is a method that returns the reverse of the natural ordering:
public static <T extends Comparable<? super T>>
 Comparator<T> reverseOrder()
{
 return new Comparator<T>() {
 public int compare(T o1, T o2) { return o2.compareTo(o1); }
 };
}
Similar methods are provided in java.util.Collections, see Section 17.4.
Finally, we can define the two versions of min in terms of the two versions of max by
using the two versions of reverseOrder:
public static <T>
 T min(Collection<? extends T> coll, Comparator<? super T> cmp)
{
 return max(coll, reverseOrder(cmp));
}
public static <T extends Comparable<? super T>>
 T min(Collection<? extends T> coll)
{
 return max(coll, Comparators.<T>reverseOrder());
}
(This ends the code summarized in Example 3-3.)
The Collections Framework does provide two versions each of min and max with the
signatures given here, see Section 17.1. However, if you examine the source code of the
library, you will see that none of the four is defined in terms of any of the others; instead,
each is defined directly. The more direct version is longer and harder to maintain, but
faster. With Sun’s current JVM, measurements show a speedup of around 30 percent.
Whether such a speedup is worth the code duplication depends on the situation in
which the code is used. Since the Java utilities might well be used in a critical inner
loop, the designers of the library were right to prefer speed of execution over economy
of expression. But this is not always the case. An improvement of 30 percent may sound
impressive, but it’s insignificant unless the total time of the program is large and the
routine appears in a heavily-used inner loop. Don’t make your own code needlessly
prolix just to eke out a small improvement.
As a final example of comparators, here is a method that takes a comparator on elements
and returns a comparator on lists of elements:
3.4 Comparator | 41
public static <E>
 Comparator<List<E>> listComparator(final Comparator<? super E> comp) {
 return new Comparator<List<E>>() {
 public int compare(List<E> list1, List<E> list2) {
 int n1 = list1.size();
 int n2 = list2.size();
 for (int i = 0; i < Math.min(n1,n2); i++) {
 int k = comp.compare(list1.get(i), list2.get(i));
 if (k != 0) return k;
 }
 return (n1 < n2) ? -1 : (n1 == n2) ? 0 : 1;
 }
 };
}
The loop compares corresponding elements of the two lists, and terminates when corresponding elements are found that are not equal (in which case, the list with the smaller
element is considered smaller) or when the end of either list is reached (in which case,
the shorter list is considered smaller). This is the usual ordering for lists; if we convert
a string to a list of characters, it gives the usual ordering on strings.
3.5 Enumerated Types
Java 5 includes support for enumerated types. Here is a simple example:
enum Season { WINTER, SPRING, SUMMER, FALL }
Each enumerated type declaration can be expanded into a corresponding class in a
stylized way. The corresponding class is designed so that it has exactly one instance for
each of the enumerated constants, bound to a suitable static final variable. For example,
the enum declaration above expands into a class called Season. Exactly four instances of
this class exist, bound to four static final variables with the names WINTER, SPRING,
SUMMER, and FALL.
Each class that corresponds to an enumerated type is a subclass of java.lang.Enum. Its
definition in the Java documentation begins like this:
class Enum<E extends Enum<E>>
You may find this frightening at first sight—both of us certainly did! But don’t panic.Actually, we’ve already seen something similar. The worrying phrase E extends
Enum<E> is a lot like the phrase T extends Comparable<T> that we encountered in the
definition of max (see Section 3.2), and we’ll see that they appear for related reasons.
To understand what’s going on, we need to take a look at the code. Example 3-4 shows
the base class Enum and Example 3-5 shows the class Season that corresponds to the
enumerated type declaration above. (The code for Enum follows the source in the Java
library, but we have simplified a few points.)
Here is the first line of the declaration for the Enum class:
public abstract class Enum<E extends Enum<E>> implements Comparable<E>
42 | Chapter 3: Comparison and Bounds
And here is the first line of the declaration for the Season class:
class Season extends Enum<Season>
Example 3-3. Comparators
class Comparators {
 public static <T>
 T max(Collection<? extends T> coll, Comparator<? super T> cmp)
 {
 T candidate = coll.iterator().next();
 for (T elt : coll) {
 if (cmp.compare(candidate, elt) < 0) { candidate = elt; }
 }
 return candidate;
 }
 public static <T extends Comparable<? super T>>
 T max(Collection<? extends T> coll)
 {
 return max(coll, Comparators.<T>naturalOrder());
 }
 public static <T>
 T min(Collection<? extends T> coll, Comparator<? super T> cmp)
 {
 return max(coll, reverseOrder(cmp));
 }
 public static <T extends Comparable<? super T>>
 T min(Collection<? extends T> coll)
 {
 return max(coll, Comparators.<T>reverseOrder());
 }
 public static <T extends Comparable<? super T>>
 Comparator<T> naturalOrder()
 {
 return new Comparator<T>() {
 public int compare(T o1, T o2) { return o1.compareTo(o2); }
 };
 }
 public static <T> Comparator<T>
 reverseOrder(final Comparator<T> cmp)
 {
 return new Comparator<T>() {
 public int compare(T o1, T o2) { return cmp.compare(o2,o1); }
 };
 }
 public static <T extends Comparable<? super T>>
 Comparator<T> reverseOrder()
 {
 return new Comparator<T>() {
 public int compare(T o1, T o2) { return o2.compareTo(o1); }
 };
 }
}
3.5 Enumerated Types | 43
Example 3-4. Base class for enumerated types
public abstract class Enum<E extends Enum<E>> implements Comparable<E> {
 private final String name;
 private final int ordinal;
 protected Enum(String name, int ordinal) {
 this.name = name; this.ordinal = ordinal;
 }
 public final String name() { return name; }
 public final int ordinal() { return ordinal; }
 public String toString() { return name; }
 public final int compareTo(E o) {
 return ordinal - o.ordinal;
 }
}
Example 3-5. Class corresponding to an enumerated type
// corresponds to
// enum Season { WINTER, SPRING, SUMMER, FALL }
final class Season extends Enum<Season> {
 private Season(String name, int ordinal) { super(name,ordinal); }
 public static final Season WINTER = new Season("WINTER",0);
 public static final Season SPRING = new Season("SPRING",1);
 public static final Season SUMMER = new Season("SUMMER",2);
 public static final Season FALL = new Season("FALL",3);
 private static final Season[] VALUES = { WINTER, SPRING, SUMMER, FALL };
 public static Season[] values() { return VALUES.clone(); }
 public static Season valueOf(String name) {
 for (Season e : VALUES) if (e.name().equals(name)) return e;
 throw new IllegalArgumentException();
 }
}
Matching things up, we can begin to see how this works. The type variable E stands for
the subclass of Enum that implements a particular enumerated type, such as Season.
Every E must satisfy:
E extends Enum<E>
So we can take E to be Season, since:
Season extends Enum<Season>
Furthermore, the declaration of Enum tells us that:
Enum<E> implements Comparable<E>
So it follows that:
Enum<Season> implements Comparable<Season>
Hence, we are allowed to compare two values of type Season with each other, but we
cannot compare a value of type Season with a value of any other type.
Without the type variable, the declaration of the Enum class would begin like this:
class Enum implements Comparable<Enum>
44 | Chapter 3: Comparison and Bounds
And the declaration for the Season class would begin like this:
class Season extends Enum
This is simpler, but it is too simple. With this definition, Season would implement
Comparable<Enum> rather than Comparable<Season>, which would mean that we could
compare a value of type Season with a value of any enumerated type, which is certainly
not what we want!
In general, patterns like T extends Comparable<T> and E extends Enum<E> often arise
when you want to pin down types precisely. We’ll see further examples of this when
we look at the Strategy and Subject-Observer design patterns, in Sections Section 9.4
and Section 9.5.
The rest of the definition is a straightforward application of the typesafe enum pattern
described by Joshua Bloch in Effective Java (Addison-Wesley), which in turn is an instance of the singleton pattern described by Gamma, Helm, Johnson, and Vlissides in
Design Patterns (Addison-Wesley).
The base class Enum defines two fields, a string name and an integer ordinal, that are
possessed by every instance of an enumerated type; the fields are final because once
they are initialized, their value never changes. The constructor for the class is protected,
to ensure that it is used only within subclasses of this class. Each enumeration class
makes the constructor private, to ensure that it is used only to create the enumerated
constants. For instance, the Season class has a private constructor that is invoked exactly
four times in order to initialize the final variables WINTER, SPRING, SUMMER, and FALL.
The base class defines accessor methods for the name and ordinal fields. The toString
method returns the name, and the compareTo method just returns the difference of the
ordinals for the two enumerated values. (Unlike the definition of Integer in Section 3.1, this is safe because there is no possibility of overflow.) Hence, constants have
the same ordering as their ordinals—for example, WINTER precedes SUMMER.
Lastly, there are two static methods in every class that corresponds to an enumerated
type. The values method returns an array of all the constants of the type. It returns a
(shallow) clone of the internal array. Cloning is vital to ensure that the client cannot
alter the internal array. Note that you don’t need a cast when calling the clone
method, because cloning for arrays now takes advantage of covariant return types (see
Section 3.8). The valueOf method takes a string and returns the corresponding constant,
found by searching the internal array. It returns an IllegalArgumentException if the
string does not name a value of the enumeration.
3.6 Multiple Bounds
We have seen many examples where a type variable or wildcard is bounded by a single
class or interface. In rare situations, it may be desirable to have multiple bounds, and
we show how to do so here.


To demonstrate, we use three interfaces from the Java library. The Readable interface
has a read method to read into a buffer from a source, the Appendable interface has an
append method to copy from a buffer into a target, and the Closeable interface has a
close method to close a source or target. Possible sources and targets include files,
buffers, streams, and so on.
For maximum flexibility, we might want to write a copy method that takes any source
that implements both Readable and Closeable and any target that implements both
Appendable and Closeable:
public static <S extends Readable & Closeable,
 T extends Appendable & Closeable>
 void copy(S src, T trg, int size)
 throws IOException
{
 try {
 CharBuffer buf = CharBuffer.allocate(size);
 int i = src.read(buf);
 while (i >= 0) {
 buf.flip(); // prepare buffer for writing
 trg.append(buf);
 buf.clear(); // prepare buffer for reading
 i = src.read(buf);
 }
 } finally {
 src.close();
 trg.close();
 }
 }
This method repeatedly reads from the source into a buffer and appends from the buffer
into a target. When the source is empty, it closes both the source and the target. (This
example deviates from best practice in that the files are closed in a different block than
the one where they are opened.) The first line specifies that S ranges over any type that
implements both Readable and Closeable, and that T ranges over any type that implements Appendable and Closeable. When multiple bounds on a type variable appear,
they are separated by ampersands. You cannot use a comma, since that is already used
to separate declarations of type variables.
For example, this method may be called with two files as source and target, or with the
same two files wrapped in buffers as source and target:
int size = 32;
FileReader r = new FileReader("file.in");
FileWriter w = new FileWriter("file.out");
copy(r,w,size);
BufferedReader br = new BufferedReader(new FileReader("file.in"));
BufferedWriter bw = new BufferedWriter(new FileWriter("file.out"));
copy(br,bw,size);
46 | Chapter 3: Comparison and Bounds
Other possible sources include FilterReader, PipedReader, and StringReader, and other
possible targets include FilterWriter, PipedWriter, and PrintStream. But you could not
use StringBuffer as a target, since it implements Appendable but not Closeable.
If you are picky, you may have spotted that all classes that implement both Readable
and Closeable are subclasses of Reader, and almost all classes that implement Appenda
ble and Closeable are subclasses of Writer. So you might wonder why we don’t simplify
the method signature like this:
public static void copy(Reader src, Writer trg, int size)
This will indeed admit most of the same classes, but not all of them. For instance,
PrintStream implements Appendable and Closeable but is not a subclass of Writer. Furthermore, you can’t rule out the possibility that some programmer using your code
might have his or her own custom class that, say, implements Readable and Closea
ble but is not a subclass of Reader.
When multiple bounds appear, the first bound is used for erasure.We sawa use of this
earlier in Section 3.2:
public static <T extends Object & Comparable<? super T>>
 T max(Collection<? extends T> coll)
Without the highlighted text, the erased type signature for max would have Compara
ble as the return type, whereas in legacy libraries the return type is Object. Maintaining
compatibility with legacy libraries is further discussed in Chapter 5 and Section 8.4.
3.7 Bridges
As we mentioned earlier, generics are implemented by erasure: when you write code
with generics, it compiles in almost exactly the same way as the code you would have
written without generics. In the case of a parameterized interface such as Compara
ble<T>, this may cause additional methods to be inserted by the compiler; these additional methods are called bridges.
Example 3-6 shows the Comparable interface and a simplified version of the Integer
class in Java before generics. In the nongeneric interface, the compareTo method takes
an argument of type Object. In the nongeneric class, there are two compareTo methods.
The first is the naïve method you might expect, to compare an integer with another
integer. The second compares an integer with an arbitrary object: it casts the object to
an integer and calls the first method. The second method is necessary in order to override the compareTo method in the Comparable interface, because overriding occurs only
when the method signatures are identical. This second method is called a bridge.
Example 3-7 shows what happens when the Comparable interface and the Integer class
are generified. In the generic interface, the compareTo method takes an argument of type
T. In the generic class, a single compareTo method takes an argument of type Integer.
3.7 Bridges | 47
The bridge method is generated automatically by the compiler. Indeed, the compiled
version of the code for both examples is essentially identical.
Example 3-6. Legacy code for comparable integers
interface Comparable {
 public int compareTo(Object o);
}
class Integer implements Comparable {
 private final int value;
 public Integer(int value) { this.value = value; }
 public int compareTo(Integer i) {
 return (value < i.value) ? -1 : (value == i.value) ? 0 : 1;
 }
 public int compareTo(Object o) {
 return compareTo((Integer)o);
 }
}
Example 3-7. Generic code for comparable integers
interface Comparable<T> {
 public int compareTo(T o);
}
class Integer implements Comparable<Integer> {
 private final int value;
 public Integer(int value) { this.value = value; }
 public int compareTo(Integer i) {
 return (value < i.value) ? -1 : (value == i.value) ? 0 : 1;
 }
}
You can see the bridge if you apply reflection. Here is code that finds all methods with
the name compareTo in the class Integer, using toGenericString to print the generic
signature of a method (see Section 7.5).
for (Method m : Integer.class.getMethods())
 if (m.getName().equals("compareTo"))
 System.out.println(m.toGenericString());
Running this code on the generic version of the Integer class produces the following
output:
public int Integer.compareTo(Integer)
public bridge int Integer.compareTo(java.lang.Object)
This indeed contains two methods, both the declared method that takes an argument
of type Integer and the bridge method that takes an argument of type Object. (As of
this writing, the Sun JVM prints volatile instead of bridge, because the bit used in Java
bytecode to indicate bridge methods is also used to indicate volatile fields; this bug is
expected to be fixed in a future release.)
Bridges can play an important role when converting legacy code to use generics; see
Section 8.4.
48 | Chapter 3: Comparison and Bounds
3.8 Covariant Overriding
Java 5 supports covariant method overriding. This feature is not directly related to
generics, but we mention it here because it is worth knowing, and because it is implemented using a bridging technique like that described in the previous section.
In Java 1.4 and earlier, one method can override another only if the argument and return
types match exactly. In Java 5, a method can override another if the argument types
match exactly and the return type of the overriding method is a subtype of the return
type of the other method.
The clone method of class Object illustrates the advantages of covariant overriding:
class Object {
 ...
 public Object clone() { ... }
}
In Java 1.4, any class that overrides clone must give it exactly the same return type,
namely Object:
class Point {
 public int x;
 public int y;
 public Point(int x, int y) { this.x=x; this.y=y; }
 public Object clone() { return new Point(x,y); }
}
Here, even though clone always returns a Point, the rules require it to have the return
type Object. This is annoying, since every invocation of clone must cast its result.
Point p = new Point(1,2);
Point q = (Point)p.clone();
In Java 5, it is possible to give the clone method a return type that is more to the point:
class Point {
 public int x;
 public int y;
 public Point(int x, int y) { this.x=x; this.y=y; }
 public Point clone() { return new Point(x,y); }
}
Now we may clone without a cast:
Point p = new Point(1,2);
Point q = p.clone();
Covariant overriding is implemented using the bridging technique described in the
previous section. As before, you can see the bridge if you apply reflection. Here is code
that finds all methods with the name clone in the class Point:
for (Method m : Point.class.getMethods())
 if (m.getName().equals("clone"))
 System.out.println(m.toGenericString());
3.8 Covariant Overriding | 49
Running this code on the covariant version of the Point class produces the following
output:
public Point Point.clone()
public bridge java.lang.Object Point.clone()
Here the bridging technique exploits the fact that in a class file two methods of the
same class may have the same argument signature, even though this is not permitted
in Java source. The bridge method simply calls the first method. (Again, as of this
writing, the Sun JVM prints volatile instead of bridge.)
50 | Chapter 3: Comparison and Bounds




CHAPTER 4: Declarations

class Pair<T, U> {
 private final T first;
 private final U second;
 public Pair(T first, U second) { this.first=first; this.second=second; }
 public T getFirst() { return first; }
 public U getSecond() { return second; }
}
The type parameters T and U are declared at the beginning of the class, not in the constructor. However, actual type parameters are passed to the constructor whenever it is
invoked:
Pair<String, Integer> pair = new Pair<String, Integer>("one",2);
assert pair.getFirst().equals("one") && pair.getSecond() == 2;
Look Out for This! A common mistake is to forget the type parameters when invoking
the constructor:
Pair<String, Integer> pair = new Pair("one",2);
This mistake produces a warning, but not an error. It is taken to be legal, because
Pair is treated as a raw type, but conversion from a raw type to the corresponding
parameterized type generates an unchecked warning; see Section 5.3, which explains
how the -Xlint:unchecked flag can help you spot errors of this kind.

4.2 Static Members
Because generics are compiled by erasure, at run time the classes List<Integer>,
List<String>, and List<List<String>> are all implemented by a single class, namely
List. You can see this using reflection:
List<Integer> ints = Arrays.asList(1,2,3);
List<String> strings = Arrays.asList("one","two");
assert ints.getClass() == strings.getClass();
Here the class associated with a list of integers at run time is the same as the class
associated with a list of strings.
One consequence is that static members of a generic class are shared across all instantiations of that class, including instantiations at different types. Static members of a
class cannot refer to the type parameter of a generic class, and when accessing a static
member the class name should not be parameterized.
For example, here is a class, Cell<T>, in which each cell has an integer identifier and a
value of type T:
class Cell<T> {
 private final int id;
 private final T value;
 private static int count = 0;
 private static synchronized int nextId() { return count++; }
 public Cell(T value) { this.value=value; id=nextId(); }
 public T getValue() { return value; }
 public int getId() { return id; }
 public static synchronized int getCount() { return count; }
}
A static field, count, is used to allocate a distinct identifier to each cell. The static
nextId method is synchronized to ensure that unique identifiers are generated even in
the presence of multiple threads. The static getCount method returns the current count.
Here is code that allocates a cell containing a string and a cell containing an integer,
which are allocated the identifiers 0 and 1, respectively:
Cell<String> a = new Cell<String>("one");
Cell<Integer> b = new Cell<Integer>(2);
assert a.getId() == 0 && b.getId() == 1 && Cell.getCount() == 2;
Static members are shared across all instantiations of a class, so the same count is
incremented when allocating either a string or an integer cell.
Because static members are independent of any type parameters, we are not permitted
to follow the class name with type parameters when accessing a static member:
Cell.getCount(); // ok
Cell<Integer>.getCount(); // compile-time error
Cell<?>.getCount(); // compile-time error
The count is static, so it is a property of the class as a whole, not any particular instance.
52 | Chapter 4: Declarations
For the same reason, you may not refer to a type parameter anywhere within a static
member. Here is a second version of Cell, which attempts to use a static variable to
keep a list of all values stored in any cell:
class Cell2<T> {
 private final T value;
 private static List<T> values = new ArrayList<T>(); // illegal
 public Cell(T value) { this.value=value; values.add(value); }
 public T getValue() { return value; }
 public static List<T> getValues() { return values; } // illegal
}
Since the class may be used with different type parameters at different places, it makes
no sense to refer to T in the declaration of the static field values or the static method
getValues(), and these lines are reported as errors at compile time. If we want a list of
all values kept in cells, then we need to use a list of objects, as in the following variant:
class Cell2<T> {
 private final T value;
 private static List<Object> values = new ArrayList<Object>(); // ok
 public Cell(T value) { this.value=value; values.add(value); }
 public T getValue() { return value; }
 public static List<Object> getValues() { return values; } // ok
}
This code compiles and runs with no difficulty:
Cell2<String> a = new Cell2<String>("one");
Cell2<Integer> b = new Cell2<Integer>(2);
assert Cell2.getValues().toString().equals("[one, 2]");
4.3 Nested Classes
Java permits nesting one class inside another. If the outer class has type parameters and
the inner class is not static, then type parameters of the outer class are visible within
the inner class.
Example 4-1 shows a class implementing collections as a singly-linked list. The class
extends java.util.AbstractCollection, so it only needs to define the methods size,
add, and iterator. The class contains an inner class, Node, for the list nodes, and an
anonymous inner class implementing Iterator<E>. The type parameter E is in scope
within both of these classes.
Example 4-1. Type parameters are in scope for nested, nonstatic classes
public class LinkedCollection<E> extends AbstractCollection<E> {
 private class Node {
 private E element;
 private Node next = null;
 private Node(E elt) { element = elt; }
 }
 private Node first = new Node(null);
 private Node last = first;
4.3 Nested Classes | 53
 private int size = 0;
 public LinkedCollection() {}
 public LinkedCollection(Collection<? extends E> c) { addAll(c); }
 public int size() { return size; }
 public boolean add(E elt) {
 last.next = new Node(elt); last = last.next; size++;
 return true;
 }
 public Iterator<E> iterator() {
 return new Iterator<E>() {
 private Node current = first;
 public boolean hasNext() {
 return current.next != null;
 }
 public E next() {
 if (current.next != null) {
 current = current.next;
 return current.element;
 } else throw new NoSuchElementException();
 }
 public void remove() {
 throw new UnsupportedOperationException();
 }
 };
 }
}
For contrast, Example 4-2 shows a similar implementation, but this time the nested
Node class is static, and so the type parameter E is not in scope for this class. Instead,
the nested class is declared with its own type parameter, T. Where the previous version
referred to Node, the new version refers to Node<E>. The anonymous iterator class in the
preceding example has also been replaced by a nested static class, again with its own
type parameter.
If the node classes had been made public rather than private, you would refer to the
node class in the first example as LinkedCollection<E>.Node, whereas you would refer
to the node class in the second example as LinkedCollection.Node<E>.
Example 4-2. Type parameters are not in scope for nested, static classes
class LinkedCollection<E> extends AbstractCollection<E> {
 private static class Node<T> {
 private T element;
 private Node<T> next = null;
 private Node(T elt) { element = elt; }
 }
 private Node<E> first = new Node<E>(null);
 private Node<E> last = first;
 private int size = 0;
 public LinkedCollection() {}
 public LinkedCollection(Collection<? extends E> c) { addAll(c); }
 public int size() { return size; }
 public boolean add(E elt) {
 last.next = new Node<E>(elt); last = last.next; size++;
54 | Chapter 4: Declarations
 return true;
 }
 private static class LinkedIterator<T> implements Iterator<T> {
 private Node<T> current;
 public LinkedIterator(Node<T> first) { current = first; }
 public boolean hasNext() {
 return current.next != null;
 }
 public T next() {
 if (current.next != null) {
 current = current.next;
 return current.element;
 } else throw new NoSuchElementException();
 }
 public void remove() {
 throw new UnsupportedOperationException();
 }
 }
 public Iterator<E> iterator() {
 return new LinkedIterator<E>(first);
 }
}
Of the two alternatives described here, the second is preferable. Nested classes that are
not static are implemented by including a reference to the enclosing instance, since they
may, in general, access components of that instance. Static nested classes are usually
both simpler and more efficient.
4.4 How Erasure Works
The erasure of a type is defined as follows: drop all type parameters from parameterized
types, and replace any type variable with the erasure of its bound, or with Object if it
has no bound, or with the erasure of the leftmost bound if it has multiple bounds. Here
are some examples:
• The erasure of List<Integer>, List<String>, and List<List<String>> is List.
• The erasure of List<Integer>[] is List[].
• The erasure of List is itself, similarly for any raw type (see Section 5.3 for an explanation of raw types).
• The erasure of int is itself, similarly for any primitive type.
• The erasure of Integer is itself, similarly for any type without type parameters.
• The erasure of T in the definition of asList (see Section 1.4) is Object, because T
has no bound.
• The erasure of T in the definition of max (see Section 3.2) is Comparable, because T
has bound Comparable<? super T>.
• The erasure of T in the final definition of max (see Section 3.6) is Object, because
T has bound Object & Comparable<T> and we take the erasure of the leftmost bound.
4.4 How Erasure Works | 55
• The erasures of S and T in the definition of copy (see Section 3.6) are Readable and
Appendable, because S has bound Readable & Closeable and T has bound Appendable & Closeable.
• The erasure of LinkedCollection<E>.Node or LinkedCollection.Node<E> (see Section 4.3) is LinkedCollection.Node.
In Java, two distinct methods cannot have the same signature. Since generics are implemented by erasure, it also follows that two distinct methods cannot have signatures
with the same erasure. A class cannot overload two methods whose signatures have the
same erasure, and a class cannot implement two interfaces that have the same erasure.
For example, here is a class with two convenience methods. One adds together every
integer in a list of integers, and the other concatenates together every string in a list of
strings:
class Overloaded {
 public static int sum(List<Integer> ints) {
 int sum = 0;
 for (int i : ints) sum += i;
 return sum;
 }
 public static String sum(List<String> strings) {
 StringBuffer sum = new StringBuffer();
 for (String s : strings) sum.append(s);
 return sum.toString();
 }
}
This works as intended:
assert sum(Arrays.asList(1,2,3)) == 6;
assert sum(Arrays.asList("a","b")).equals("ab");
Here are the erasures of the signatures of the two methods:
int sum(List)
String sum(List)
The two methods have different return types, which is sufficient for Java to distinguish
them.
However, say we change the methods so that each appends its result to the end of the
argument list rather than returning a value:
class Overloaded2 {
 // compile-time error, cannot overload two methods with same erasure
 public static boolean allZero(List<Integer> ints) {
 for (int i : ints) if (i != 0) return false;
 return true;
 }
 public static boolean allZero(List<String> strings) {
 for (String s : strings) if (s.length() != 0) return false;
 return true;
56 | Chapter 4: Declarations
 }
}
We intend this code to work as follows:
assert allZero(Arrays.asList(0,0,0));
assert allZero(Arrays.asList("","",""));
However, in this case the erasures of the signatures of both methods are identical:
boolean allZero(List)
Therefore, a name clash is reported at compile time. It is not possible to give both
methods the same name and try to distinguish between them by overloading, because
after erasure it is impossible to distinguish one method call from the other.
For another example, here is a bad version of the integer class, that tries to make it
possible to compare an integer with either an integer or a long:
class Integer implements Comparable<Integer>, Comparable<Long> {
 // compile-time error, cannot implement two interfaces with same erasure
 private final int value;
 public Integer(int value) { this.value = value; }
 public int compareTo(Integer i) {
 return (value < i.value) ? -1 : (value == i.value) ? 0 : 1;
 }
 public int compareTo(Long l) {
 return (value < l.value) ? -1 : (value == l.value) ? 0 : 1;
 }
}
If this were supported, it would, in general, require a complex and confusing definition
of bridge methods (see Section 3.7). By far, the simplest and most understandable option is to ban this case.
4.4 How Erasure Works | 57





CHAPTER 6
Reification
The Oxford English Dictionary defines reify thus: “To convert mentally into a thing;
to materialize.” A plainer word with the same meaning is thingify. In computing, reification has come to mean an explicit representation of a type—that is, run-time type
information. In Java, arrays reify information about their component types, while generic types do not reify information about their type parameters.
The previous chapter was, in a sense, about the advantages of not reifying parameter
types. Legacy code makes no distinction between List<Integer> and List<String> and
List<List<String>>, so not reifying parameter types is essential to easing evolution and
promoting compatibility between legacy code and new code.
But now the time has come to pay the piper. Reification plays a critical role in certain
aspects of Java, and the absence of reification that is beneficial for evolution also necessarily leads to some rough edges. This chapter warns you of the limitations and describes some workarounds. The chapter deals almost entirely with things you might
wish you didn’t need to know—and, indeed, if you never use generic types in casts,
instance tests, exceptions, or arrays then you are unlikely to need the material covered
here.
We begin with a precise definition of what it means for a type in Java to be reifiable.We then consider corner cases related to reification, including instance tests and
casts, exceptions, and arrays. The fit between arrays and generics is the worst rough
corner in the language, and we encapsulate how to avoid the worst pitfalls with the
Principle of Truth in Advertising and the Principle of Indecent Exposure.
6.1 Reifiable Types
In Java, the type of an array is reified with its component type, while the type of a
parameterized type is reified without its type parameters. For instance, an array of
numbers will carry the reified type Number[], while a list of numbers will carry the reified
type ArrayList, not ArrayList<Number>; the raw type, not the parameterized type, is
reified. Of course, each element of the list will have a reified type attached to it—say
73
Integer or Double—but this is not the same as reifying the parameter type. If every
element in the list was an integer, we would not be able to tell whether we had an
ArrayList<Integer>, ArrayList<Number>, or ArrayList<Object>; if the list was empty,
we would not be able to tell what kind of empty list it was.
In Java, we say that a type is reifiable if the type is completely represented at run time
— that is, if erasure does not remove any useful information. To be precise, a type is
reifiable if it is one of the following:
• A primitive type
(such as int)
• A nonparameterized class or interface type
(such as Number, String, or Runnable)
• A parameterized type in which all type arguments are unbounded wildcards
(such as List<?>, ArrayList<?>, or Map<?, ?>)
• A raw type
(such as List, ArrayList, or Map)
• An array whose component type is reifiable
(such as int[], Number[], List<?>[], List[], or int[][])
A type is not reifiable if it is one of the following:
• A type variable
(such as T)
• A parameterized type with actual parameters
(such as List<Number>, ArrayList<String>, or Map<String, Integer>)
• A parameterized type with a bound
(such as List<? extends Number> or Comparable<? super String>)
So the type List<? extends Object> is not reifiable, even though it is equivalent to
List<?>. Defining reifiable types in this way makes them easy to identify syntactically.
6.2 Instance Tests and Casts
Instance tests and casts depend on examining types at run time, and hence depend on
reification. For this reason, an instance test against a type that is not reifiable reports
an error, and a cast to a type that is not reifiable usually issues a warning.
As an example, consider the use of instance tests and casts in writing equality. Here is
a fragment of the definition of the class Integer in java.lang (slightly simplified from
the actual source):
public class Integer extends Number {
 private final int value;
74 | Chapter 6: Reification
 public Integer(int value) { this.value=value; }
 public int intValue() { return value; }
 public boolean equals(Object o) {
 if (o instanceof Integer) {
 return value == ((Integer)o).intValue();
 } else return false;
 }
 ...
}
The equality method takes an argument of type Object, checks whether the object is
an instance of class Integer, and, if so, casts it to Integer and compares the values of
the two integers. This code works because Integer is a reifiable type: all of the information needed to check whether an object is an instance of Integer is available at run
time.
Now consider how one might define equality on lists, as in the class AbstractList in
java.util. A natural—but incorrect—way to define this is as follows:
import java.util.*;
public abstract class AbstractList<E>
 extends AbstractCollection<E> implements List<E>
{
 public boolean equals(Object o) {
 if (o instanceof List<E>) { // compile-time error
 Iterator<E> it1 = iterator();
 Iterator<E> it2 = ((List<E>)o).iterator(); // unchecked cast
 while (it1.hasNext() && it2.hasNext()) {
 E e1 = it1.next();
 E e2 = it2.next();
 if (!(e1 == null ? e2 == null : e1.equals(e2)))
 return false;
 }
 return !it1.hasNext() && !it2.hasNext();
 } else return false;
 }
 ...
}
Again, the equality method takes an argument of type Object, checks whether the object
is an instance of type List<E>, and, if so, casts it to List<E> and compares corresponding
elements of the two lists. This code does not work because List<E> is not a reifiable
type: some of the information needed to check whether an object is an instance of
List<E> is not available at run time. You can test whether an object implements the
interface List, but not whether its type parameter is E. Indeed, information on E is
missing doubly, as it is not available for either the receiver or the argument of the
equals method.
(Even if this code worked, there is a further problem. The contract for equality on lists
doesn’t mention types. A List<Integer> may be equal to a List<Object> if they contain
the same values in the same order. For instance, [1,2,3] should be equal to itself,
regardless of whether it is regarded as a list of integers or a list of objects.)
6.2 Instance Tests and Casts | 75
Compiling the preceding code reports two problems, an error for the instance test and
an unchecked warning for the cast:
% javac -Xlint:unchecked AbstractList.java
AbstractList.java:6: illegal generic type for instanceof
 if (!(o instanceof List<E>)) return false; // compile-time error
 ^
AbstractList.java:8: warning: [unchecked] unchecked cast
found : java.lang.Object
required: List<E>
 Iterator<E> it2 = ((List<E>)o).iterator(); // unchecked cast
 ^
1 error
1 warning
The instance check reports an error because there is no possible way to test whether
the given object belongs to the type List<E>. The cast reports an unchecked warning;
it will perform the cast, but it cannot check that the list elements are, in fact, of type E.
To fix the problem, we replace the nonreifiable type List<E> with the reifiable type
List<?>. Here is a corrected definition (again, slightly simplified from the actual source):
import java.util.*;
public abstract class AbstractList<E>
extends AbstractCollection<E> implements List<E> {
 public boolean equals(Object o) {
 if (o instanceof List<?>) {
 Iterator<E> it1 = iterator();
 Iterator<?> it2 = ((List<?>)o).iterator();
 while (it1.hasNext() && it2.hasNext()) {
 E e1 = it1.next();
 Object e2 = it2.next();
 if (!(e1 == null ? e2 == null : e1.equals(e2)))
 return false;
 }
 return !it1.hasNext() && !it2.hasNext();
 } else return false;
 }
 ...
}
In addition to changing the type of the instance test and the cast, the type of the second
iterator is changed from Iterator<E> to Iterator<?>, and the type of the second element
is changed from E to Object. The code type-checks, because even though the element
type of the second iterator is unknown, it is guaranteed that it must be a subtype of
Object, and the nested call to equals requires only that its second argument be an object.
(This code properly satisfies the contract for equality on lists. Now a List<Integer> will
be equal to a List<Object> if they contain the same values in the same order.)
Alternative fixes are possible. Instead of the wildcard types List<?> and Iterator<?>,
you could use the raw types List and Iterator, which are also reifiable. We recommend
using unbounded wildcard types in preference to raw types because they provide stronger static typing guarantees; many mistakes that are caught as an error when you use
76 | Chapter 6: Reification
unbounded wildcards will only be flagged as a warning if you use raw types. Also, you
could change the declaration of the first iterator to Iterator<?> and of the first element
to Object, so that they match the second iterator, and the code will still type-check. We
recommend always using type declarations that are as specific as possible; this helps
the compiler to catch more errors and to compile more-efficient code.
Nonreifiable Casts An instance test against a type that is not reifiable is always an
error. However, in some circumstances a cast to a type that is not reifiable is permitted.
For example, the following method converts a collection to a list:
public static <T> List<T> asList(Collection<T> c)
 throws InvalidArgumentException
{
 if (c instanceof List<?>) {
 return (List<T>)c;
 } else throw new InvalidArgumentException("Argument not a list");
}
Compiling this code succeeds with no errors or warnings. The instance test is not in
error because List<?> is a reifiable type. The cast does not report a warning because
the source of the cast has type Collection<T>, and any object with this type that implements the interface List must, in fact, have type List<T>.
Unchecked casts Only rarely will the compiler be able to determine that if a cast to a
nonreifiable type succeeds then it must yield a value of that type. In the remaining cases,
a cast to a type that is not reifiable is flagged with an unchecked warning, whereas an
instance test against a type that is not reifiable is always caught as an error. This is
because there is never any point to an instance test that cannot be performed, but there
may be a point to a cast that cannot be checked.
Type systems deduce facts about programs—for instance, that a certain variable always
contains a list of strings. But no type system is perfect; there will always be some facts
that a programmer can deduce but that the type system does not. To permit the programmer a workaround in such circumstances, the compiler issues warnings rather
than errors when performing some casts.
For example, here is code that promotes a list of objects into a list of strings, if the list
of objects contains only strings, and throws a class cast exception otherwise:
class Promote {
 public static List<String> promote(List<Object> objs) {
 for (Object o : objs)
 if (!(o instanceof String))
 throw new ClassCastException();
 return (List<String>)(List<?>)objs; // unchecked cast
 }
 public static void main(String[] args) {
 List<Object> objs1 = Arrays.<Object>asList("one","two");
 List<Object> objs2 = Arrays.<Object>asList(1,"two");
 List<String> strs1 = promote(objs1);
 assert (List<?>)strs1 == (List<?>)objs1;
6.2 Instance Tests and Casts | 77
 boolean caught = false;
 try {
 List<String> strs2 = promote(objs2);
 } catch (ClassCastException e) { caught = true; }
 assert caught;
 }
}
The method promote loops over the list of objects and throws a class cast exception if
any object is not a string. Hence, when the last line of the method is reached, it is safe
to cast the list of objects to a list of strings.
But the compiler cannot deduce this, so the programmer must use an unchecked cast.
It is illegal to cast a list of objects to a list of strings, so the cast must take place in two
steps. First, cast the list of objects into a list of wildcard type; this cast is safe. Second,
cast the list of wildcard type into a list of strings; this cast is permitted but generates an
unchecked warning:
% javac -Xlint:unchecked Promote.java
Promote.java:7: warning: [unchecked] unchecked cast
found : java.util.List
required: java.util.List<java.lang.String>
 return (List<String>)(List<?>)objs; // unchecked cast
 ^
1 warning
The test code applies the method to two lists, one containing only strings (so it succeeds) and one containing an integer (so it raises an exception). In the first assertion,
to compare the object list and the string list, we must first cast both to the type List<?
> (this cast is safe), because attempting to compare a list of objects with a list of strings
raises a type error.
Exactly the same technique can be used to promote a raw list to a list of strings if the
raw list contains only strings. This technique is important for fitting together legacy
and generic code, and is one of the chief reasons for using erasure to implement generics.
A related technique is discussed in Section 8.1.
Another example of the use of unchecked casts to interface legacy and generic code
occurred in Section 5.4.1, where we needed an unchecked cast to the element type
(E) to make the type of the value returned by the legacy add method match its generic
signature.
You should minimize the number of unchecked casts in your code, but sometimes, as
in the case above, they cannot be avoided. In this book, we follow the convention that
we always place the comment unchecked cast on the line containing the cast, to document that this is an intentional workaround rather than an unintended slip; and we
recommend you do the same. It is important to put the comment on the same line as
the cast, so that when scanning the warnings issued by the compiler it is easy to confirm
that each line contains the comment. If it does not, then you should regard the warning
as equivalent to an error!
78 | Chapter 6: Reification
If a method deliberately contains unchecked casts, you may wish to precede it with the
annotation @SuppressWarnings("unchecked") in order to avoid spurious warnings. We
saw an application of this technique in Section 5.4.1.
As another example of the use of unchecked casts, in Section 6.5 we will see code that
uses an unchecked cast from type Object[] to type T[]. Because of the way the object
array is created, it is, in fact, guaranteed that the array will always have the correct type.
Unchecked casts in C (and in its descendant C++) are much more dangerous than
unchecked casts in Java. Unlike C, the Java runtime guarantees important security
properties even in the presence of unchecked casts; for instance, it is never permitted
to access an array with an index outside of the array bounds. Nonetheless, unchecked
casts in Java are a workaround that should be used with caution.
6.3 Exception Handling
In a try statement, each catch clause checks whether the thrown exception matches a
given type. This is the same as the check performed by an instance test, so the same
restriction applies: the type must be reifiable. Further, the type in a catch clause is
required to be a subclass of Throwable. Since there is little point in creating a subclass
of Throwable that cannot appear in a catch clause, the Java compiler complains if you
attempt to create a parameterized subclass of Throwable.
For example, here is a permissible definition of a new exception, which contains an
integer value:
class IntegerException extends Exception {
 private final int value;
 public IntegerException(int value) { this.value = value; }
 public int getValue() { return value; }
}
And here is a simple example of how to use the exception:
class IntegerExceptionTest {
 public static void main(String[] args) {
 try {
 throw new IntegerException(42);
 } catch (IntegerException e) {
 assert e.getValue() == 42;
 }
 }
}
The body of the try statement throws the exception with a given value, which is caught
by the catch clause.
In contrast, the following definition of a new exception is prohibited, because it creates
a parameterized type:
class ParametricException<T> extends Exception { // compile-time error
 private final T value;
6.3 Exception Handling | 79
 public ParametricException(T value) { this.value = value; }
 public T getValue() { return value; }
}
An attempt to compile the above reports an error:
% javac ParametricException.java
ParametricException.java:1: a generic class may not extend
java.lang.Throwable
class ParametricException<T> extends Exception { // compile-time error
 ^
1 error
This restriction is sensible because almost any attempt to catch such an exception must
fail, because the type is not reifiable. One might expect a typical use of the exception
to be something like the following:
class ParametricExceptionTest {
 public static void main(String[] args) {
 try {
 throw new ParametricException<Integer>(42);
 } catch (ParametricException<Integer> e) { // compile-time error
 assert e.getValue()==42;
 }
 }
}
This is not permitted, because the type in the catch clause is not reifiable. At the time
of this writing, the Sun compiler reports a cascade of syntax errors in such a case:
% javac ParametricExceptionTest.java
ParametricExceptionTest.java:5: <identifier> expected
 } catch (ParametricException<Integer> e) {
 ^
ParametricExceptionTest.java:8: ')' expected
 }
 ^
ParametricExceptionTest.java:9: '}' expected
}
 ^
3 errors
Because exceptions cannot be parametric, the syntax is restricted so that the type must
be written as an identifier, with no following parameter.
Type Variable in a Throws Clause Although subclasses of Throwable cannot be parametric, it is possible to use a type variable in the throws clause of a method declaration.
This technique is illustrated in Section 9.3.
6.4 Array Creation
Arrays reify their component types, meaning that they carry run-time information about
the type of their components. This reified type information is used in instance tests and
80 | Chapter 6: Reification
casts, and also used to check whether assignments into array components are permitted. Recall this example from Section 2.5.
Integer[] ints = new Integer[] {1,2,3};
Number[] nums = ints;
nums[2] = 3.14; // array store exception
The first line allocates a new array, with reified type information indicating that it is an
array of integers. The second line assigns this array to a variable containing an array of
numbers; this is permitted because arrays, unlike generic types, are covariant. The assignment on the third line raises an array store exception at run time because the assigned value is of type double, and this is not compatible with the reified type attached
to the array.
Because arrays must reify their component types, it is an error to create a new array
unless its component type is reifiable. The two main problems you are likely to encounter are when the type of the array is a type variable, and when the type of the array
is a parameterized type.
Consider the following (incorrect) code to convert a collection to an array:
import java.util.*;
class Annoying {
 public static <T> T[] toArray(Collection<T> c) {
 T[] a = new T[c.size()]; // compile-time error
 int i=0; for (T x : c) a[i++] = x;
 return a;
 }
}
This is an error, because a type variable is not a reifiable type. An attempt to compile
this code reports a generic array creation error:
% javac Annoying.java
Annoying.java:4: generic array creation
 T[] a = new T[c.size()]; // compile-time error
 ^
1 error
We discuss workarounds for this problem shortly.
As a second example, consider the following (incorrect) code that returns an array
containing two lists:
import java.util.*;
class AlsoAnnoying {
 public static List<Integer>[] twoLists() {
 List<Integer> a = Arrays.asList(1,2,3);
 List<Integer> b = Arrays.asList(4,5,6);
 return new List<Integer>[] {a, b}; // compile-time error
 }
}
This is an error, because a parameterized type is not a reifiable type. An attempt to
compile this code also reports a generic array creation error:
6.4 Array Creation | 81
% javac AlsoAnnoying.java
AlsoAnnoying.java:6: generic array creation
 return new List<Integer>[] {a, b}; // compile-time error
 ^
1 error
We also discuss workarounds for this problem shortly.
Inability to create generic arrays is one of the most serious restrictions in Java. Because
it is so annoying, it is worth reiterating the reason it occurs: generic arrays are problematic because generics are implemented via erasure, but erasure is beneficial because
it eases evolution.
The best workaround is to use ArrayList or some other class from the Collections
Framework in preference to an array. We discussed the tradeoffs between collection
classes and arrays in Section 2.5, and we noted that in many cases collections are preferable to arrays: because they catch more errors at compile time, because they provide
more operations, and because they offer more flexibility in representation. By far, the
best solution to the problems offered by arrays is to “just say no”: use collections in
preference to arrays.
Sometimes this won’t work, because you need an array for reasons of compatibility or
efficiency. Examples of this occur in the Collections Framework: for compatibility, the
method toArray converts a collection to an array; and, for efficiency, the class Array
List is implemented by storing the list elements in an array. We discuss both of these
cases in detail in the following sections, together with associated pitfalls and principles
that help you avoid them: the Principle of Truth in Advertising and the Principle of
Indecent Exposure. We also consider problems that arise with varargs and generic array
creation.
6.5 The Principle of Truth in Advertising
We saw in the previous section that a naïve method to convert a collection to an array
will not work. The first fix we might try is to add an unchecked cast, but we will see
shortly that this leads to even more perplexing problems. The correct fix will require
us to resort to reflection. Since the same issues arise when converting any generic structure to an array, it is worth understanding the problems and their solution. We will
study variations of the static toArray method from the previous section; the same ideas
apply to the toArray method in the Collection interface of the Collections Framework.
Here is a second attempt to convert a collection to an array, this time using an unchecked cast, and with test code added:
import java.util.*;
class Wrong {
 public static <T> T[] toArray(Collection<T> c) {
 T[] a = (T[])new Object[c.size()]; // unchecked cast
 int i=0; for (T x : c) a[i++] = x;
 return a;
82 | Chapter 6: Reification
 }
 public static void main(String[] args) {
 List<String> strings = Arrays.asList("one","two");
 String[] a = toArray(strings); // class cast error
 }
}
The code in the previous section used the phrase new T[c.size()] to create the array,
causing the compiler to report a generic array creation error. The new code instead
allocates an array of objects and casts it to type T[], which causes the compiler to issue
an unchecked cast warning:
% javac -Xlint Wrong.java
Wrong.java:4: warning: [unchecked] unchecked cast
found : java.lang.Object[]
required: T[]
 T[] a = (T[])new Object[c.size()]; // unchecked cast
 ^
1 warning
As you might guess from the name chosen for this program, this warning should not
be ignored. Indeed, running this program gives the following result:
% java Wrong
Exception in thread "main" java.lang.ClassCastException: [Ljava.lang.Object;
 at Wrong.main(Wrong.java:11)
The obscure phrase [Ljava.lang.Object is the reified type of the array, where [L indicates that it is an array of reference type, and java.lang.Object is the component type
of the array. The class cast error message refers to the line containing the call to toAr
ray. This error message may be confusing, since that line does not appear to contain a
cast!
In order to see what went wrong with this program, let’s look at how the program is
translated using erasure. Erasure drops type parameters on Collection and List, replaces occurrences of the type variable T with Object, and inserts an appropriate cast
on the call to toArray, yielding the following equivalent code:
import java.util.*;
class Wrong {
 public static Object[] toArray(Collection c) {
 Object[] a = (Object[])new Object[c.size()]; // unchecked cast
 int i=0; for (Object x : c) a[i++] = x;
 return a;
 }
 public static void main(String[] args) {
 List strings = Arrays.asList(args);
 String[] a = (String[])toArray(strings); // class cast error
 }
}
Erasure converts the unchecked cast to T[] into a cast to Object[], and inserts a cast to
String[] on the call to toArray. When run, the first of these casts succeeds. But even
6.5 The Principle of Truth in Advertising | 83
though the array contains only strings, its reified type indicates that it is an array of
Object, so the second cast fails.
In order to avoid this problem, you must stick to the following principle:
The Principle of Truth in Advertising: the reified type of an array must be a subtype
of the erasure of its static type.
The principle is obeyed within the body of toArray itself, where the erasure of T is
Object, but not within the main method, where T has been bound to String but the
reified type of the array is still Object.
Before we see how to create arrays in accordance with this principle, there is one more
point worth stressing. Recall that generics for Java are accompanied by a cast-iron
guarantee: no cast inserted by erasure will fail, so long as there are no unchecked warnings. The preceding principle illustrates the converse: if there are unchecked warnings,
then casts inserted by erasure may fail. Further, the cast that fails may be in a different
part of the source code than was responsible for the unchecked warning! This is why code
that generates unchecked warnings must be written with extreme care.
Array Begets Array “Tis money that begets money,” said Thomas Fuller in 1732,
observing that one way to get money is to already have money. Similarly, one way to
get a new array of a generic type is to already have an array of that type. Then the reified
type information for the new array can be copied from the old.
We therefore alter the previous method to take two arguments, a collection and an
array. If the array is big enough to hold the collection, then the collection is copied into
the array. Otherwise, reflection is used to allocate a new array with the same reified
type as the old, and then the collection is copied into the new array.
Here is code to implement the alternative:
import java.util.*;
class Right {
 public static <T> T[] Array(toCollection<T> c, T[] a) {
 if (a.length < c.size())
 a = (T[])java.lang.reflect.Array. // unchecked cast
 newInstance(a.get Class().getComponentType(), c.size());
 int i=0; for (T x : c) a[i++] = x;
 if (i < a.length) a[i] = null;
 return a;
 }
 public static void main(String[] args) {
 List<String> strings = Arrays.asList("one", "two");
 String[] a = toArray(strings, new String[0]);
 assert Arrays.toString(a).equals("[one, two]");
 String[] b = new String[] { "x","x","x","x" };
 toArray(strings, b);
 assert Arrays.toString(b).equals("[one, two, null, x]");
 }
}
84 | Chapter 6: Reification
This uses three methods from the reflection library to allocate a new array with the
same component type as the old array: the method getClass (in java.lang.Object)
returns a Class object representing the array type, T[]; the method getComponentType
(from java.lang.Class) returns a second Class object representing the array’s component type, T; and the method newInstance (in java.lang.reflect.Array) allocates a new
array with the given component type and size, again of type T[]. The result type of the
call to newInstance is Object, so an unchecked cast is required to cast the result to the
correct type T[].
In Java 5, the class Class has been updated to a generic class Class<T>; more on this
shortly.
(A subtle point: in the call to newInstance, why is the result type Object rather than
Object[]? Because, in general, newInstance may return an array of a primitive type such
as int[], which is a subtype of Object but not of Object[]. However, that won’t happen
here because the type variable T must stand for a reference type.)
The size of the new array is taken to be the size of the given collection. If the old array
is big enough to hold the collection and there is room left over, a null is written just
after the collection to mark its end.
The test code creates a list of strings of length two and then performs two demonstration
calls. Neither encounters the problem described previously, because the returned array
has the reified type String[], in accordance with the Principle of Truth in Advertising.
The first call is passed an array of length zero, so the list is copied into a freshly allocated
array of length two. The second call is passed an array of length four, so the list is copied
into the existing array, and a null is written past the end; the original array content after
the null is not affected. The utility method toString (in java.util.Arrays) is used to
convert the array to a string in the assertions.
The Collections Framework contains two methods for converting collections to arrays,
similar to the one we just discussed:
interface Collection<E> {
 ...
 public Object[] toArray();
 public <T> T[] toArray(T[] a)
}
The first method returns an array with the reified component type Object, while the
second copies the reified component type from the argument array, just as in the static
method above. Like that method, it copies the collection into the array if there is room
(and writes a null past the end of the collection if there is room for that), and allocates
a fresh array otherwise. A call to the first method, c.toArray(), returns the same result
as a call to the second method with an empty array of objects, c.toArray(new
Object[0]). These methods are discussed further at the beginning of Chapter 12.
Often on encountering this design, programmers presume that the array argument exists mainly for reasons of efficiency, in order to minimize allocations by reusing the
6.5 The Principle of Truth in Advertising | 85
array. This is indeed a benefit of the design, but its main purpose is to get the reified
types correct! Most calls to toArray will be with an argument array of length zero.
A Classy Alternative Some days it may seem that the only way to get money is to have
money. Not quite the same is true for arrays. An alternative to using an array to create
an array is to use an instance of class Class.
Instances of the class Class represent information about a class at run time; there are
also instances of this class that represent primitive types and arrays. In this text, we will
refer to instances of the Class class as class tokens.
In Java 5, the class Class has been made generic, and now has the form Class<T>. What
does the T stand for? An instance of type Class<T> represents the type T. For example,
String.class has type Class<String>.
We can define a variant of our previous method that accepts a class token of type
Class<T> rather than an array of type T[]. Applying newInstance to a class token of type
Class<T> returns a new array of type T[], with the component type specified by the class
token. The newInstance method still has a return type of Object (because of the same
problem with primitive arrays), so an unchecked cast is still required.
import java.util.*;
class RightWithClass {
 public static <T> T[] toArray(Collection<T> c, Class<T> k) {
 T[] a = (T[])java.lang.reflect.Array. // unchecked cast
 newInstance(k, c.size());
 int i=0; for (T x : c) a[i++] = x;
 return a;
 }
 public static void main(String[] args) {
 List<String> strings = Arrays.asList("one", "two");
 String[] a = toArray(strings, String.class);
 assert Arrays.toString(a).equals("[one, two]");
 }
}
The conversion method is now passed the class token String.class rather than an array
of strings.
The type Class<T> represents an interesting use of generics, quite different from collections or comparators. If you still find this use of generics confusing, don’t worry—
we’ll cover this subject in greater detail in Chapter 7.
6.6 The Principle of Indecent Exposure
Although it is an error to create an array with a component type that is not reifiable, it
is possible to declare an array with such a type and to perform an unchecked cast to
such a type. These features must be used with extreme caution, and it is worthwhile to
understand what can go wrong if they are not used properly. In particular, a library
should never publicly expose an array with a nonreifiable type.
86 | Chapter 6: Reification
Recall that Section 2.5 presents an example of why reification is necessary:
Integer[] ints = new Integer[] {1};
Number[] nums = ints;
nums[0] = 1.01; // array store exception
int n = ints[0];
This assigns an array of integers to an array of numbers, and then attempts to store a
double into the array of numbers. The attempt raises an array store exception because
of the check against the reified type. This is just as well, since otherwise the last line
would attempt to store a double into an integer variable.
Here is a similar example, where arrays of numbers are replaced by arrays of lists of
numbers:
List<Integer>[] intLists
 = (List<Integer>[])new List[] {Arrays.asList(1)}; // unchecked cast
List<? extends Number>[] numLists = intLists;
numLists[0] = Arrays.asList(1.01);
int n = intLists[0].get(0); // class cast exception!
This assigns an array of lists of integers to an array of lists of numbers, and then attempts
to store a list of doubles into the array of lists of numbers. This time the attempted store
does not fail, even though it should, because the check against the reified type is inadequate: the reified information contains only the erasure of the type, indicating that it
is an array of List, not an array of List<Integer>. Hence the store succeeds, and the
program fails unexpectedly elsewhere.
Example 6-1. Avoid arrays of nonreifiable type
DeceptiveLibrary.java:
import java.util.*;
public class DeceptiveLibrary {
 public static List<Integer>[] intLists(int size) {
 List<Integer>[] intLists =
 (List<Integer>[]) new List[size]; // unchecked cast
 for (int i = 0; i < size; i++)
 intLists[i] = Arrays.asList(i+1);
 return ints;
 }
}
InnocentClient.java:
import java.util.*;
public class InnocentClient {
 public static void main(String[] args) {
 List<Integer>[] intLists = DeceptiveLibrary.intLists(1);
 List<? extends Number>[] numLists = intLists;
 numLists[0] = Arrays.asList(1.01);
 int i = intLists[0].get(0); // class cast error!
 }
}
6.6 The Principle of Indecent Exposure | 87
Example 6-1 presents a similar example, divided into two classes in order to demonstrate how a poorly designed library can create problems for an innocent client. The
first class, called DeceptiveLibrary, defines a static method that returns an array of lists
of integers of a given size. Since generic array creation is not permitted, the array is
created with components of the raw type List, and a cast is used to give the components
the parameterized type List<Integer>. The cast generates an unchecked warning:
%javac -Xlint:unchecked DeceptiveLibrary.java
DeceptiveLibrary.java:5: warning: [unchecked] unchecked cast
found : java.util.List[]
required: java.util.List<java.lang.Integer>[]
 (List<Integer>[]) new List[size]; // unchecked cast
 ^
1 warning
Since the array really is an array of lists of integers, the cast appears reasonable, and
you might think that this warning could be safely ignored. As we shall see, you ignore
this warning at your peril!
The second class, called InnocentClient, has a main method similar to the previous
example. Because the unchecked cast appears in the library, no unchecked warning is
issued when compiling this code. However, running the code overwrites a list of integers with a list of doubles. Attempting to extract an integer from the array of lists of
integers causes the cast implicitly inserted by erasure to fail:
%java InnocentClient
Exception in thread "main" java.lang.ClassCastException: java.lang.Double
 at InnocentClient.main(InnocentClient.java:7)
As in the previous section, this error message may be confusing, since that line does
not appear to contain a cast!
In order to avoid this problem, you must stick to the following principle:
Principle of Indecent Exposure: never publicly expose an array where the components do not have a reifiable type.
Again, this is a case where an unchecked cast in one part of the program may lead to a
class cast error in a completely different part, where the cast does not appear in the
source code but is instead introduced by erasure. Since such errors can be extremely
confusing, unchecked casts must be used with extreme caution.
The Principle of Truth in Advertising and the Principle of Indecent Exposure are closely
linked. The first requires that the run-time type of an array is properly reified, and the
second requires that the compile-time type of an array must be reifiable.
It has taken some time for the importance of the Principle of Indecent Exposure to be
understood, even among the designers of generics for Java. For example, the following
two methods in the reflection library violate the principle:
TypeVariable<Class<T>>[] java.lang.Class.getTypeParameters()
TypeVariable<Method>[] java.lang.Reflect.Method.getTypeParameters()
88 | Chapter 6: Reification
Following the preceding model, it is not hard to create your own version of Innocent
Client that throws a class cast error at a point where there is no cast, where in this case
the role of DeceptiveLibrary is played by the official Java library! (At the time of going
to press, remedies for this bug are under consideration. Possible fixes are to delete the
type parameter from TypeVariable so that the methods return an array of reified type,
or to replace the arrays with lists.)
Don’t get caught out in the same way—be sure to follow the Principle of Indecent
Exposure rigorously in your own code!
6.7 How to Define ArrayList
We have argued elsewhere that it is usually preferable to use a list than to use an array.
There are a few places where this is not appropriate. In rare circumstances, you will
need to use an array for reasons of efficiency or compatibility. Also, of course, you need
to use arrays to implement ArrayList itself. Here we present the implementation of
ArrayList as a model of what to do in the rare circumstance where you do need to use
an array. Such implementations need to be written with care, as they necessarily involve
use of unchecked casts. We will see how the Principles of Indecent Exposure and of
Truth in Advertising figure in the implementation.
Example 6-2 shows the implementation. We have derived ArrayList by subclassing
from AbstractList. Classes derived from this class need to define only four methods,
namely get, set, add, and remove; the other methods are defined in terms of these.We
also indicate that the class implements RandomAccess, indicating that clients of the class
will have more efficient access using get than using the list iterator.
The class represents a list with elements of type E by two private fields: size of type
int containing the length of the list, and arr of type E[] containing the elements of
the list. The array must have a length at least equal to size, but it may have additional
unused elements at the end.
There are two places where new instances of the array are allocated, one in the initializer
for the class and one in the method that increases the array capacity (which in turn is
called from the add method). In both places, the array is allocated as an Object[] and
an unchecked cast is made to type E[].
It is essential that the field containing the array is private; otherwise, it would violate
both the Principle of Truth in Advertising and the Principle of Indecent Exposure. It
would violate the Principle of Truth in Advertising because E might be bound to a type
(such as String) other than Object. It would violate the Principle of Indecent Exposure
because E might be bound to a type (such as List<Integer>) that is not a reifiable type.
However, neither of these principles is violated because the array is not public: it is
stored in a private field, and no pointer to the array escapes from the class.We might
call this the Principle of Anything Goes Behind Closed Doors.
6.7 How to Define ArrayList | 89
The way we’ve defined ArrayList here is close to the actual definition in the source
released by Sun. Recently, Neal Gafter, the coauthor of that library, has argued that he
used bad style—that it would have been better to declare the private array to have type
Object[] and use casts to type (E) when retrieving elements from the array. There is
something to be said for this point, although there is also something to be said for the
style we have used here, which minimizes the need for unchecked casts.
The method toArray does return an array in public, but it uses the techniques described
in Section 6.5 in accordance with the Principle of Truth in Advertising. As before, there
is an argument array, and if it is not big enough to hold the collection, then reflection
is used to allocate a new array with the same reified type. The implementation is similar
to the one we saw earlier, except that the more efficient arraycopy routine can be used
to copy the private array into the public array to be returned.
6.8 Array Creation and Varargs
The convenient vararg notation allows methods to accept a variable number of arguments and packs them into an array, as discussed in Section 1.4. This notation is not
as convenient as you might like, because the arrays it creates suffer from the same issues
involving reification as other arrays.
Example 6-2. How to define ArrayList
import java.util.*;
class ArrayList<E> extends AbstractList<E> implements RandomAccess {
 private E[] arr;
 private int size = 0;
 public ArrayList(int cap) {
 if (cap < 0)
 throw new IllegalArgumentException("Illegal Capacity: "+cap);
 arr = (E[])new Object[cap]; // unchecked cast
 }
 public ArrayList() { this(10); }
 public ArrayList(Collection<? extends E> c) { this(c.size()); addAll(c); }
 public void ensureCapacity(int mincap) {
 int oldcap = arr.length;
 if (mincap > oldcap) {
 int newcap = Math.max(mincap, (oldcap*3)/2+1);
 E[] oldarr = arr;
 arr = (E[])new Object[newcap]; // unchecked cast
 System.arraycopy(oldarr,0,arr,0,size);
 }
 }
 public int size() { return size; }
 private void checkBounds(int i, int size) {
 if (i < 0 || i >= size)
 throw new IndexOutOfBoundsException("Index: "+i+", Size: "+size);
 }
 public E get(int i) { checkBounds(i,size); return arr[i]; }
 public E set(int i, E elt) {
90 | Chapter 6: Reification
 checkBounds(i,size); E old = arr[i]; arr[i] = elt; return old;
 }
 public void add(int i, E elt) {
 checkBounds(i,size+1); ensureCapacity(size+1);
 System.arraycopy(arr,i,arr,i+1,size-i); arr[i] = elt; size++;
 }
 public E remove(int i) {
 checkBounds(i,size); E old = arr[i]; arr[i] = null; size--;
 System.arraycopy(arr,i+1,arr,i,size-i); return old;
 }
 public <T> T[] toArray(T[] a) {
 if (a.length < size)
 a = (T[])java.lang.reflect.Array. // unchecked cast
 newInstance(a.getClass().getComponentType(), size);
 System.arraycopy(arr,0,a,0,size);
 if (size < a.length) a[size] = null;
 return a;
 }
 public Object[] toArray() { return toArray(new Object[0]); }
}
In Section 1.4 we discussed the method java.util.Arrays.asList, which is declared as
follows:
public static <E> List<E> asList(E... arr)
For instance, here are three calls to this method:
List<Integer> a = Arrays.asList(1, 2, 3);
List<Integer> b = Arrays.asList(4, 5, 6);
List<List<Integer>> x = Arrays.asList(a, b); // generic array creation
Recall that an argument list of variable length is implemented by packing the arguments
into an array and passing that. Hence these three calls are equivalent to the following:
List<Integer> a = Arrays.asList(new Integer[] { 1, 2, 3 });
List<Integer> b = Arrays.asList(new Integer[] { 4, 5, 6 });
List<List<Integer>> x
 = Arrays.asList(new List<Integer>[] { a, b }); // generic array creation
The first two calls are fine, but since List<Integer> is not a reifiable type, the third
warns of an unchecked generic array creation at compile time.
VarargError.java:6: warning: [unchecked] unchecked generic array creation
of type java.util.List<java.lang.Integer>[] for varargs parameter
 List<List<Integer>> x = Arrays.asList(a, b);
This warning can be confusing, particularly since that line of source code does not
contain an explicit instance of array creation!
A similar problem occurs if one attempts to create a list of a generic type. Here is a
method that uses Arrays.asList to create a list of length one containing a given element:
public static List<E> singleton(E elt) {
 return Arrays.asList(elt); // generic array creation
}
6.8 Array Creation and Varargs | 91
This also generates a warning, which can be confusing for the same reasons.
Normally, generic array creation reports an error. As a workaround, one can create the
array at a reifiable type and perform an unchecked cast. That workaround is not available for the array creation that is implicit in the use of varargs, so in this case generic
array creation issues a warning rather than an error. A generic array creation warning
is just like an unchecked warning, in that it invalidates the cast-iron guarantee that
accompanies generics. It is not too difficult to take each of the previous examples where
a mishap occurs as the result of an unchecked warning, and create a similar example
using varargs where a generic array creation warning is issued instead.
In our opinion, the convenience offered by varargs is outweighed by the danger inherent
in unchecked warnings, and we recommend that you never use varargs when the argument is of a nonreifiable type. For instance, in both of the preceding examples, instead of using Arrays.asList, we would have created a new ArrayList and used the
add method, even though this is less convenient and less efficient.
The need for generic array creation warnings and the associated workarounds would
not have arisen if the vararg notation had been defined to pack arguments into a list
rather than an array, taking T… to be equivalent to List<T> rather than T[]. Unfortunately, the vararg notation was designed before this problem was fully understood.
6.9 Arrays as a Deprecated Type?
We have seen that collections are superior to arrays in a number of ways:
• Collections provide more precise typing than arrays. With lists, one can write
List<T>, List<? extends T>, or List<? super T>; whereas with arrays, one can only
write T[], which corresponds to the second of the three options for lists. Moreprecise typing enables more errors to be detected at compile time rather than run
time. This makes coding, debugging, testing, and maintenance easier, and also
improves efficiency. (See Section 2.5.)
• Collections are more flexible than arrays. Collections offer a variety of representations, including arrays, linked lists, trees, and hash tables, whereas arrays have a
fixed representation, and the libraries offer a larger variety of methods and convenience algorithms on collections than on arrays. (See Section 2.5.)
• Collections may have elements of any type, whereas arrays should only have components of reifiable type. When creating an array, one must adhere to the Principle
of Truth in Advertising—the reified type must conform to the static type—and the
Principle of Indecent Exposure—never publicly expose an array where the components do not have reifiable type. (See Sections Section 6.5 and Section 6.6).
In retrospect, there are several places in Java 5 where avoiding the use of arrays might
have improved the design:
92 | Chapter 6: Reification
• Variable-length arguments (varargs) are represented by an array, and so are subject
to the same restrictions. If a vararg is bound to actual arguments of nonreifiable
type then a generic array creation warning is issued (which raises the same concerns
as an unchecked warning). For instance, the function Arrays.asList takes a vararg. There is no difficulty in applying this function to return a result of type
List<Integer>, but it is problematic to create a result of type List<List<Inte
ger>> or of type List<E>. This problem would not arise if lists had been used in
preference to arrays. (See Section 6.8.)
• Some methods in the Java library have signatures that violate the Principle of Indecent Exposure:
TypeVariable<Class<T>>[] java.lang.Class.getTypeParameters()
TypeVariable<Method>[] java.lang.Reflect.Method.getTypeParameters()
It is possible for code that invokes these methods to violate the cast-iron guarantee
that accompanies generics: it may raise a class cast exception where there is no
explicit cast in the code, even though no unchecked warning is issued by the compiler. (A warning was issued—and wrongly ignored—when the library was compiled.) Again, this problem would not arise if lists had been used in preference to
arrays. (See Section 6.6.)
One reason for some of the complexities in the Java 5 design was to provide good
support for the use of arrays. In retrospect, it might have been better to choose a design
that was simpler, but made arrays not quite as convenient to use:
• Arrays must be created with components of reifiable type, so some attempt was
made to make the notion of reifiable type as general as possible in order to minimize
this restriction. If the designers had been willing to restrict the notion of reified
type, they could have simplified it by including raw types (such as List), but excluding types with unbounded wildcards (such as List<?>). Had they done so,
reifiable types would have become synonymous with unparameterized types (that
is, primitive types, raw types, and types declared without a type parameter).
This changewould simplify the types permitted in an instance test. Consider the
following three tests:
obj instanceof List
obj instanceof List<?>
obj instanceof List<? extends Object>
Currently, the first two are permitted, but the third is not. With the proposed
restriction, only the first would be permitted. Arguably, this might be easier to
understand. It would also be consistent with the treatment of class tokens, since
currently List.class is permitted but List<?>.class is illegal.
• Currently, array creation is restricted to arrays of reifiable type. But it is permitted
to declare an array of nonreifiable type or to cast to an array type that is not reifiable,
at the cost of an unchecked warning somewhere in the code. As we have seen, such
6.9 Arrays as a Deprecated Type? | 93
warnings violate the cast-iron guarantee that accompanies generics, and may lead
to class cast errors even when the source code contains no casts.
A simpler and safer design would be to outlaw any reference to an array of nonreifiable type (using the simpler form of reifiable type just described). This design
would mean that one could never declare an array of type E[], where E is a type
variable.
This change would make it more complicated to implement ArrayList<E> (or similar classes). The type of the private variable must change from E[] to Object[],
and you must add an unchecked cast (E) to the result of get and similar methods.
But the complication is small, and arises only for the implementor of ArrayList (or
similar classes), not for the clients.
This change would also mean that you could not assign a generic type to the
toArray method for collections (or similar methods). Instead of:
public <T> T[] toArray(T[] arr)
we would have:
public Object[] toArray(Object[] arr)
and many uses of this method would require an explicit cast of the result. This does
make life more awkward for users, but arguably the improvement to simplicity and
safety would be worth it.
• The preceding changes would mean that often one would use lists in preference to
arrays. Use of lists could be made easier by permitting Java programmers to write
l[i] as an abbreviation for l.get(i), and l[i] = v as an abbreviation for
l.put(i,v). (Some people like this sort of “syntactic sugar,” while others think of
it as “syntactic rat poison.”)
Some of these changes can still be adapted in a backward compatible manner.We
mentioned in Section 6.8 that it may be desirable to add a second form of vararg based
on lists rather than arrays. Permitting abbreviations to make list indexing look like array
indexing could easily be incorporated in a future version of Java.
But the window for some of these changes has closed. Too many users have written
code with generic toArrays to permit reverting to the nongeneric version. Nonetheless,
it seems worthwhile to record this alternate design. Perhaps understanding how the
current design might have been simpler can lead to better insight and a better future
design.
Just as the Java 5 design might have been improved if it had put less emphasis on arrays,
your own code designs may be improved if you use collections and lists in preference
to arrays. Perhaps the time has come to regard arrays as a deprecated type?
94 | Chapter 6: Reification
6.10 Summing Up
We conclude by giving a checklist of places where reifiable types are required or recommended.
• An instance test must be against a reifiable type.
• A cast should usually be to a reifiable type. (A cast to a nonreifiable type usually
issues an unchecked warning.)
• A class that extends Throwable must not be parameterized.
• An array instance creation must be at a reifiable type.
• The reified type of an array must be a subtype of the erasure of its static type (see
the Principle of Truth in Advertising), and a publicly exposed array should be of a
reifiable type (see the Principle of Indecent Exposure).
• Varargs should be of a reifiable type. (A vararg of a nonreifiable type will issue an
unchecked warning.)
These restrictions arise from the fact that generics are implemented via erasure, and
they should be regarded as the price one pays for the ease of evolution that we explored
in the previous chapter.
For completeness, we also list restrictions connected with reflection:
• Class tokens correspond to reifiable types, and the type parameter in Class<T>
should be a reifiable type. (See Section 7.2.)
These are discussed in the next chapter.

