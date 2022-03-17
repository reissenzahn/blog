
// By default, top-level classes are accessible within the package in which they are defined. However, if a top-level class is declared public, it is accessible everywhere.

// access control modifiers specify the access rules for the field or method

// nested classes are members of a class and so obey the member access-control rules

// the members of a class are always accessible within the body of the class.

// all the fields and methods of a class can always be used within the body of the class itself

// the default,  members are accessible throughout the package in which the class is defined (package access)

// public means that the member is accessible anywhere the containing class is accessible

// private means the member is never accessible except within the class itself

// protected means the member is accessible to all classes within the package (the same as the default package accessibility) and also accessible within the body of any subclass of the class

// notice that protected means an instance of the subclass can access the inherited member and not that the instance can access protected fields of arbitrary instances of the superclass

// default access is more restrictive than protected as it does not allow access by subclasses outside the package



public class A {
 protected final String name;
 public A(String named) {
 name = named;
 }
 public String getName() {
 return name;
 }
}

public class B extends A {
 public B(String named) {
 super(named);
 }
 @Override
 public String getName() {
 return "B: " + name;
 }
}

Java packages do not “nest,” so javanut7.ch03.different is just a different package than javanut7.ch03; it is not contained inside it or related to it in any way.

public String examine(A a) {
 return "B sees: " + a.name;
 }

If we change the method to this:
 public String examine(B b) {
 return "B sees another B: " + b.name;
 }

then the compiler is happy, because instances of the same exact type can always see
each other’s protected fields. Of course, if B was in the same package as A, then any
instance of B could read any protected field of any instance of A because protected
fields are visible to every class in the same package.
Access control and inheritance


The Java specification states that:

• A subclass inherits all the instance fields and instance methods of its superclass accessible to it.
• If the subclass is defined in the same package as the superclass, it inherits all
non-private instance fields and methods.
• If the subclass is defined in a different package, it inherits all protected and
public instance fields and methods.
• private fields and methods are never inherited; neither are class fields or class
methods.
• Constructors are not inherited


However, some programmers are confused by the statement that a subclass does not
inherit the inaccessible fields and methods of its superclass. It could be taken to
imply that when you create an instance of a subclass, no memory is allocated for any
private fields defined by the superclass. This is not the intent of the statement,
however.

Every instance of a subclass does, in fact, include a complete
instance of the superclass within it, including all inaccessible
fields and methods.
This existence of potentially inaccessible members seems to be in conflict with the
statement that the members of a class are always accessible within the body of the
class. To clear up this confusion, we define “inherited members” to mean those
superclass members that are accessible.
Then the correct statement about member accessibility is: “All inherited members
and all members defined in this class are accessible.” An alternative way of saying
this is:
• A class inherits all instance fields and instance methods (but not constructors)
of its superclass.
• The body of a class can always access all the fields and methods it declares
itself. It can also access the accessible fields and members it inherits from its
superclass.
Member access summary
We summarize the member access rules in Table 3-1.
Table 3-1. Class member accessibility
 Member visibility
Accessible to Public Protected Default Private
Defining class Yes Yes Yes Yes
Class in same package Yes Yes Yes No
Subclass in different package Yes Yes No No
Nonsubclass different package Yes No No No
There are a few generally observed rules about what parts of a Java program should
use each visibility modifier. It is important that even beginning Java programmers
follow these rules:
• Use public only for methods and constants that form part of the public API of
the class. The only acceptable usage of public fields is for constants or immut‐
able objects, and they must be also declared final.
• Use protected for fields and methods that aren’t required by most program‐
mers using the class but that may be of interest to anyone creating a subclass as
part of a different package.
protected members are technically part of the exported API
of a class. They must be documented and cannot be changed
without potentially breaking code that relies on them.
• Use the default package visibility for fields and methods that are internal imple‐
mentation details but are used by cooperating classes in the same package.
• Use private for fields and methods that are used only inside the class and
should be hidden everywhere else.
If you are not sure whether to use protected, package, or private accessibility, start
with private. If this is overly restrictive, you can always relax the access restrictions
slightly (or provide accessor methods, in the case of fields).
This is especially important for designing APIs because increasing access restric‐
tions is not a backward-compatible change and can break code that relies on access
to those members.



Data Accessor Methods

// getter and setter methods allow for reading and writing of variables while also being able to perform error checking 
public class Circle { // The class is still public
 // This is a generally useful constant, so we keep it public
 public static final double PI = 3.14159;
 protected double r; // Radius is hidden but visible to subclasses
 // A method to enforce the restriction on the radius
 // This is an implementation detail that may be of interest to subclasses
 protected void checkRadius(double radius) {
 if (radius < 0.0)
 throw new IllegalArgumentException("radius may not be negative.");
 }
 // The non-default constructor
 public Circle(double r) {
 checkRadius(r);
 this.r = r;
 }
 // Public data accessor methods
 public double getRadius() { return r; }
 public void setRadius(double r) {
 checkRadius(r);
 this.r = r;
 }
 // Methods to operate on the instance field
 public double area() { return PI * r * r; }
 public double circumference() { return 2 * PI * r; }
}


<None> (package) Class A non-public class is accessible only in its package.
Interface A non-public interface is accessible only in its package.
Member A member that is not private, protected, or public has
package visibility and is accessible only within its package.
private Member The member is accessible only within the class that defines it.
protected Member The member is accessible only within the package in which it is defined and within subclasses.
public Class The class is accessible anywhere its package is.
Interface The interface is accessible anywhere its package is.
Member The member is accessible anywhere its class is.