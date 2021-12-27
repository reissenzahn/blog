Nested Types

// type definitions can be nested within other type definitions.

// these nested types are known as inner classes

// a nested type has access to the internals of the containing type in the same way that member variables and methods do

// types can be nested in four different ways:

// 1. static member types: a static member type is any type defined as a static member of another type; rested interfaces, enums, and annotations are always static (even if you don’t use the keyword).

// 2. 

Nonstatic member classes
A “nonstatic member type” is simply a member type that is not declared
static. Only classes can be nonstatic member types.


Local classes
A local class is a class that is defined and only visible within a block of Java
code. Interfaces, enums, and annotations may not be defined locally.
Anonymous classes
An anonymous class is a kind of local class that has no meaningful name in the
Java language. Interfaces, enums, and annotations cannot be defined
anonymously.
The term “nested types,” while a correct and precise usage, is not widely used by
developers. Instead, most Java programmers use the much vaguer term “inner class.”
Depending on the situation, this can refer to a nonstatic member class, local class,
or anonymous class, but not a static member type, with no real way to distinguish
between them.
Fortunately, although the terminology for describing nested types is not always
clear, the syntax for working with them is, and it is usually apparent from context
which kind of nested type is being discussed.
Until Java 11, nested types were implemented using a compiler
trick. Experienced Java programmers should note that this
detail has actually changed in Java 11, and it is no longer done
in quite the same way as it used to be.


Static Member Types
A static member type is much like a regular top-level type. For convenience, how‐
ever, it is nested within the namespace of another type. Static member types have
the following basic properties:
• A static member type is like the other static members of a class: static fields and
static methods.
• A static member type is not associated with any instance of the containing class
(i.e., there is no this object).
• A static member type can access (only) the static members of the class that
contains it.
• A static member type has access to all the static members (including any
other static member types) of its containing type.
• Nested interfaces, enums, and annotations are implicitly static, whether or not
the static keyword appears.
• Any type nested within an interface or annotation is also implicitly static.
178 | Chapter 4: The Java Type System
• Static member types may be defined within top-level types or nested to any
depth within other static member types.
• A static member type may not be defined within any other kind of nested type.
Let’s look at a quick example of the syntax for static member types. Example 4-1
shows a helper interface defined as a static member of a containing class.
Example 4-1. Defining and using a static member interface
// A class that implements a stack as a linked list
public class LinkedStack {
 // This static member interface defines how objects are linked
 // The static keyword is optional: all nested interfaces are static
 static interface Linkable {
 public Linkable getNext();
 public void setNext(Linkable node);
 }
 // The head of the list is a Linkable object
 Linkable head;
 // Method bodies omitted
 public void push(Linkable node) { ... }
 public Object pop() { ... }
}
// This class implements the static member interface
class LinkableInteger implements LinkedStack.Linkable {
 // Here's the node's data and constructor
 int i;
 public LinkableInteger(int i) { this.i = i; }
 // Here are the data and methods required to implement the interface
 LinkedStack.Linkable next;
 public LinkedStack.Linkable getNext() { return next; }
 public void setNext(LinkedStack.Linkable node) { next = node; }
}
The example also shows how this interface is used both within the class that con‐
tains it and by external classes. Note the use of its hierarchical name in the external
class.
Features of static member types
A static member type has access to all static members of its containing type, includ‐
ing private members. The reverse is true as well: the methods of the containing
Type
System
Nested Types | 179
type have access to all members of a static member type, including the private
members. A static member type even has access to all the members of any other
static member types, including the private members of those types. A static mem‐
ber type can use any other static member without qualifying its name with the name
of the containing type.
Top-level types can be declared as either public or package-private (if they’re
declared without the public keyword). But declaring top-level types as private and
protected wouldn’t make a great deal of sense—protected would just mean the
same as package-private and a private top-level class would be unable to be
accessed by any other type.
Static member types, on the other hand, are members and so can use any access
control modifiers that other members of the containing type can. These modifiers
have the same meanings for static member types as they do for other members of a
type.
For example, in Example 4-1, the Linkable interface is declared public, so it can be
implemented by any class that is interested in being stored on a LinkedStack.
In code outside the containing class, a static member type is named by combining
the name of the outer type with that of the inner (e.g., LinkedStack.Linkable).
Under most circumstances, this syntax provides a helpful reminder that the inner
class is interconnected with its containing type. However, the Java language does
permit you to use the import directive to directly import a static member type:
import pkg.LinkedStack.Linkable; // Import a specific nested type
// Import all nested types of LinkedStack
import pkg.LinkedStack.*;
You can then reference the nested type without including the name of its enclosing
type (e.g., just as Linkable).
You can also use the import static directive to import a
static member type. See “Packages and the Java Namespace”
on page 94 in Chapter 2 for details on import and import
static.
However, importing a nested type obscures the fact that that type is closely associ‐
ated with its containing type—which is usually important information—and as a
result it is not commonly done.
Nonstatic Member Classes
A nonstatic member class is a class that is declared as a member of a containing class
or enumerated type without the static keyword:
180 | Chapter 4: The Java Type System
• If a static member type is analogous to a class field or class method, a nonstatic
member class is analogous to an instance field or instance method.
• Only classes can be nonstatic member types.
• An instance of a nonstatic member class is always associated with an instance
of the enclosing type.
• The code of a nonstatic member class has access to all the fields and methods
(both static and non-static) of its enclosing type.
• Several features of Java syntax exist specifically to work with the enclosing
instance of a nonstatic member class.
Example 4-2 shows how a member class can be defined and used. This example
extends the previous LinkedStack example to allow enumeration of the elements on
the stack by defining an iterator() method that returns an implementation of the
java.util.Iterator interface. The implementation of this interface is defined as a
member class.
Example 4-2. An iterator implemented as a member class
import java.util.Iterator;
public class LinkedStack {
 // Our static member interface
 public interface Linkable {
 public Linkable getNext();
 public void setNext(Linkable node);
 }
 // The head of the list
 private Linkable head;
 // Method bodies omitted here
 public void push(Linkable node) { ... }
 public Linkable pop() { ... }
 // This method returns an Iterator object for this LinkedStack
 public Iterator<Linkable> iterator() { return new LinkedIterator(); }
 // Here is the implementation of the Iterator interface,
 // defined as a nonstatic member class.
 protected class LinkedIterator implements Iterator<Linkable> {
 Linkable current;
 // The constructor uses a private field of the containing class
 public LinkedIterator() { current = head; }
 // The following three methods are defined
Type
System
Nested Types | 181
 // by the Iterator interface
 public boolean hasNext() { return current != null; }
 public Linkable next() {
 if (current == null)
 throw new java.util.NoSuchElementException();
 Linkable value = current;
 current = current.getNext();
 return value;
 }
 public void remove() { throw new UnsupportedOperationException(); }
 }
}
Notice how the LinkedIterator class is nested within the LinkedStack class.
Because LinkedIterator is a helper class used only within LinkedStack, having it
defined so close to where it is used by the containing class makes for a clean design,
just as we discussed when we introduced nested types.
Features of member classes
Like instance fields and instance methods, every instance of a nonstatic member
class is associated with an instance of the class in which it is defined. This means
that the code of a member class has access to all the instance fields and instance
methods (as well as the static members) of the containing instance, including any
that are declared private.
This crucial feature was already illustrated in Example 4-2. Here is the Linked
Stack.LinkedIterator() constructor again:
public LinkedIterator() { current = head; }
This single line of code sets the current field of the inner class to the value of the
head field of the containing class. The code works as shown, even though head is
declared as a private field in the containing class.
A nonstatic member class, like any member of a class, can be assigned one of the
standard access control modifiers. In Example 4-2, the LinkedIterator class is
declared protected, so it is inaccessible to code (in a different package) that uses
the LinkedStack class but is accessible to any class that subclasses LinkedStack.
Member classes have two important restrictions:
• A nonstatic member class cannot have the same name as any containing class
or package. This is an important rule, one that is not shared by fields and
methods.
• Nonstatic member classes cannot contain any static fields, methods, or types,
except for constant fields declared both static and final.
182 | Chapter 4: The Java Type System
Syntax for member classes
The most important feature of a member class is that it can access the instance fields
and methods in its containing object.
If we want to use explicit references, and make use of this, then we have to use a
special syntax for explicitly referring to the containing instance of the this object.
For example, if we want to be explicit in our constructor, we can use the following
syntax:
public LinkedIterator() { this.current = LinkedStack.this.head; }
The general syntax is classname.this, where classname is the name of a contain‐
ing class. Note that member classes can themselves contain member classes, nested
to any depth. However, because no member class can have the same name as any
containing class, the use of the enclosing class name prepended to this is a perfectly
general way to refer to any containing instance.
Local Classes
A local class is declared locally within a block of Java code rather than as a member
of a class. Only classes may be defined locally: interfaces, enumerated types, and
annotation types must be top-level or static member types. Typically, a local class is
defined within a method, but it can also be defined within a static initializer or
instance initializer of a class.
Just as all blocks of Java code appear within class definitions, all local classes are nes‐
ted within containing blocks. For this reason, local classes share many of the fea‐
tures of member classes. It is usually more appropriate to think of them as an
entirely separate kind of nested type.
See Chapter 5 for details as to when it’s appropriate to choose
a local class versus a lambda expression.
The defining characteristic of a local class is that it is local to a block of code. Like a
local variable, a local class is valid only within the scope defined by its enclosing
block. Example 4-3 illustrates how we can modify the iterator() method of the
LinkedStack class so it defines LinkedIterator as a local class instead of a member
class.
By doing this, we move the definition of the class even closer to where it is used and
hopefully improve the clarity of the code even further. For brevity, Example 4-3
shows only the iterator() method, not the entire LinkedStack class that contains
it.
Type
System
Nested Types | 183
Example 4-3. Defining and using a local class
// This method returns an Iterator object for this LinkedStack
public Iterator<Linkable> iterator() {
 // Here's the definition of LinkedIterator as a local class
 class LinkedIterator implements Iterator<Linkable> {
 Linkable current;
 // The constructor uses a private field of the containing class
 public LinkedIterator() { current = head; }
 // The following three methods are defined
 // by the Iterator interface
 public boolean hasNext() { return current != null; }
 public Linkable next() {
 if (current == null)
 throw new java.util.NoSuchElementException();
 Linkable value = current;
 current = current.getNext();
 return value;
 }
 public void remove() { throw new UnsupportedOperationException(); }
 }
 // Create and return an instance of the class we just defined
 return new LinkedIterator();
}
Features of local classes
Local classes have the following interesting features:
• Like member classes, local classes are associated with a containing instance and
can access any members, including private members, of the containing class.
• In addition to accessing fields defined by the containing class, local classes can
access any local variables, method parameters, or exception parameters that are
in the scope of the local method definition and are declared final.
Local classes are subject to the following restrictions:
• The name of a local class is defined only within the block that defines it; it can
never be used outside that block. (Note, however, that instances of a local class
created within the scope of the class can continue to exist outside of that scope.
This situation is described in more detail later in this section.)
• Local classes cannot be declared public, protected, private, or static.
184 | Chapter 4: The Java Type System
• Like member classes, and for the same reasons, local classes cannot contain
static fields, methods, or classes. The only exception is for constants that are
declared both static and final.
• Interfaces, enumerated types, and annotation types cannot be defined locally.
• A local class, like a member class, cannot have the same name as any of its
enclosing classes.
• As noted earlier, a local class can close over the local variables, method parame‐
ters, and even exception parameters that are in its scope but only if those vari‐
ables or parameters are effectively final.
Scope of a local class
In discussing nonstatic member classes, we saw that a member class can access any
members inherited from superclasses and any members defined by its containing
classes.
The same is true for local classes, but local classes can also behave like lambdas and
access effectively final local variables and parameters. Example 4-4 illustrates the
different kinds of fields and variables that may be accessible to a local class (or a
lambda, for that matter):
Example 4-4. Fields and variables available to a local class
class A { protected char a = 'a'; }
class B { protected char b = 'b'; }
public class C extends A {
 private char c = 'c'; // Private fields visible to local class
 public static char d = 'd';
 public void createLocalObject(final char e)
 {
 final char f = 'f';
 int i = 0; // i not final; not usable by local class
 class Local extends B
 {
 char g = 'g';
 public void printVars()
 {
 // All of these fields and variables are accessible to this class
 System.out.println(g); // (this.g) g is a field of this class
 System.out.println(f); // f is a final local variable
 System.out.println(e); // e is a final local parameter
 System.out.println(d); // (C.this.d) d field of containing class
 System.out.println(c); // (C.this.c) c field of containing class
 System.out.println(b); // b is inherited by this class
 System.out.println(a); // a is inherited by the containing class
 }
Type
System
Nested Types | 185
 }
 Local l = new Local(); // Create an instance of the local class
 l.printVars(); // and call its printVars() method.
 }
}
Local classes have quite a complex scoping structure, therefore. To see why, notice
that instances of a local class can have a lifetime that extends past the time that the
JVM exits the block where the local class is defined.
In other words, if you create an instance of a local class, that
instance does not automatically go away when the JVM fin‐
ishes executing the block that defines the class. So, even
though the definition of the class was local, instances of that
class can escape out of the place they were defined.
Local classe, therefore, behave like lambdas in many regards, although the use case
of local classes is more general than that of lambdas. However, in practice the extra
generality is rarely required, and lambdas are preferred wherever possible.
Anonymous Classes
An anonymous class is a local class without a name. It is defined and instantiated in
a single expression using the new operator. While a local class definition is a state‐
ment in a block of Java code, an anonymous class definition is an expression, which
means that it can be included as part of a larger expression, such as a method call.
For the sake of completeness, we cover anonymous classes
here, but for most use cases, lambda expressions (see “Lambda
Expressions” on page 169) have replaced anonymous classes.
Consider Example 4-5, which shows the LinkedIterator class implemented as an
anonymous class within the iterator() method of the LinkedStack class. Compare
it with Example 4-4, which shows the same class implemented as a local class.
Example 4-5. An enumeration implemented with an anonymous class
public Iterator<Linkable> iterator() {
 // The anonymous class is defined as part of the return statement
 return new Iterator<Linkable>() {
 Linkable current;
 // Replace constructor with an instance initializer
 { current = head; }
 // The following three methods are defined
 // by the Iterator interface
186 | Chapter 4: The Java Type System
 public boolean hasNext() { return current != null; }
 public Linkable next() {
 if (current == null)
 throw new java.util.NoSuchElementException();
 Linkable value = current;
 current = current.getNext();
 return value;
 }
 public void remove() { throw new UnsupportedOperationException(); }
 }; // Note the required semicolon. It terminates the return statement
}
As you can see, the syntax for defining an anonymous class and creating an instance
of that class uses the new keyword, followed by the name of a type and a class body
definition in curly braces. If the name following the new keyword is the name of a
class, the anonymous class is a subclass of the named class. If the name following
new specifies an interface, as in the two previous examples, the anonymous class
implements that interface and extends Object.
The syntax for anonymous classes does not include any way to
specify an extends clause, an implements clause, or a name
for the class.
Because an anonymous class has no name, it is not possible to define a constructor
for it within the class body. This is one of the basic restrictions on anonymous
classes. Any arguments you specify between the parentheses following the super‐
class name in an anonymous class definition are implicitly passed to the superclass
constructor. Anonymous classes are commonly used to subclass simple classes that
do not take any constructor arguments, so the parentheses in the anonymous class
definition syntax are often empty.
Because an anonymous class is just a type of local class, anonymous classes and local
classes share the same restrictions. An anonymous class cannot define any static
fields, methods, or classes, except for static final constants. Interfaces, enumer‐
ated types, and annotation types cannot be defined anonymously. Also, like local
classes, anonymous classes cannot be public, private, protected, or static.
The syntax for defining an anonymous class combines definition with instantiation,
similar to a lambda expression. Using an anonymous class instead of a local class is
not appropriate if you need to create more than a single instance of the class each
time the containing block is executed.
Because an anonymous class has no name, it is not possible to define a constructor
for an anonymous class. If your class requires a constructor, you must use a local
class instead.



// interfaces may contain nested types. Any such types are implicitly public and static
