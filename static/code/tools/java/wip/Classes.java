
// a class is a collection of fields that hold values and methods that operate on those values; a class defines a new reference type

// an object is an instance of a class; objects are created by instantiating a class
Point p = new Point(1.0, 2.0);

// kinds of members: class fields, class methods, instance fields, instance methods

// a static member belongs to the class itself while a non-static member is associated with the instances of a class

// a class field is associated with the class in which it is defined rather than with an instance of the class

// the names of class fields and methods are qualified by the unique names of the classes that contain them; the class name does not need to be specified when a member is used inside the class in which it is defined

// class methods are also declared with the static modifier

// a class method can use any class fields and class methods of its own class though it cannot use any instance fields or methods because it is not associated with an instance of the class

// this is a reference to the current object

// class methods are not associated with a specific instance, so have no this reference, and no access to instance fields

// instance fields are associated with instances of the class

// an instance method operates on a specific instance of a class (an object)

// final specifies that the value of a field may never be changed after it has been initialized

// transient indicates that a field is not part of the persistent state of an object and that it need not be serialized along with the rest of the object

public class Circle {

  // class field
  public static final double PI = 3.14159;

  // instance field
  public double r;

  // class method
  public static double radiansToDegrees(double radians) {
    return radians * 180 / PI;
  }

  // instance methods
  public double area() {
    return PI * r * r;
  }

  public double circumference() {
    return 2 * PI * r;
  }
}

System.out.println(Circle.PI);
System.out.println(Circle.radiansToDegrees(2.0));

Circle c = new Circle(); // Create a Circle object; store a ref in c
c.r = 2.0; // Assign a value to its instance field r
Circle d = new Circle(); // Create a different Circle object
d.r = c.r * 2; // Make this one twice as big

Circle c = new Circle();
c.r = 2.0; // Set an instance field of the object
double a = c.area(); // Invoke an instance method of the object

// a class may extends another class in which case it is a subclass of that superclass; a subclass inherits the members of its superclass and may declare new members or override inherited methods with new implementations

// a member of a class can have an access modifier (public, protected, no modifier or private) that specifies its visibility and accessibility to clients and subclasses

// an interface is a reference type that defines method signatures (usually without implementations); a class can implement one or more interfaces in which case it must provide implementations for the methods defined by those interfaces

// interfaces can include a default implementation of a method which makes it optional for implementing classes to provide an implementation

// an abstract class is one whose implementation is incomplete and cannot be instantiated; any class with one or more abstract methods must be declared abstract

// the final modifier species that the class may not be extended


// THIS

// all instance methods are implemented by using an implicit this argument which holds a reference to the object through which the method is invoked

// whenever a method accesses the members in its class, it is implicit that it is accessing fields in the object referred to by the this parameter

// public double area() { return Circle.PI * this.r * this.r; }

// this is required when a method parameter or local variable in a method has the same name of one of the fields of the class

public void setRadius(double r) {
 this.r = r; // Assign the argument (r) to the field (this.r)
 // Note that we cannot just say r = r
}

public class Classes {

  public static void main(String[] args) {

  }
}


// a constructor is a class member which is responsible for initializing the fields of a class as new instances of the class are created

// if an explicit constructor is not defined then a default constructor is automatically provided that takes no arguments and performs no special initialization

public class Circle {
 public static final double PI = 3.14159; // A constant
 // An instance field that holds the radius of the circle
 protected double r;
 // The constructor: initialize the radius field
 public Circle(double r) { this.r = r; }
 // The instance methods: compute values based on the radius
 public double circumference() { return 2 * PI * r; }
 public double area() { return PI * r*r; }
 public double radius() { return r; }
}

Circle c = new Circle(0.25);

// we can define multiple constructors as long as each constructor has a different parameter list
public Circle() { r = 1.0; }
public Circle(double r) { this.r = r; }

// the this keyword can be used inside a constructor to invoke another constructor though this call can only appear as the first statement in the constructor
public Circle(double r) { this.r = r; }
public Circle() { this(1.0); }


// Field Defaults and Initializers

// we can provide default values for fields
public static final double PI = 3.14159;
public double r = 1.0;


// a class can have any number of static initializers which can be used to initialize static fields
public class TrigCircle {
 // Here are our static lookup tables and their own initializers
 private static final int NUMPTS = 500;
 private static double sines[] = new double[NUMPTS];
 private static double cosines[] = new double[NUMPTS];
 // Here's a static initializer that fills in the arrays
 static {
 double x = 0.0;
 double delta_x = (Circle.PI/2)/(NUMPTS-1);
 for(int i = 0, x = 0.0; i < NUMPTS; i++, x += delta_x) {
 sines[i] = Math.sin(x);
 cosines[i] = Math.cos(x);
 }
 }
 // The rest of the class is omitted...
}





// The null keyword is a special literal value that is a reference to nothing, or an
// absence of a reference. You can assign null to variables of any reference type. For example:
// String s = null;
// Point p = null;



// METHODS

Methods
A method is a named sequence of Java statements that can be invoked by other Java
code. When a method is invoked, it is passed zero or more values known as argu‐
ments. The method performs some computations and, optionally, returns a value.
As described earlier in “Expressions and Operators” on page 33, a method invoca‐
tion is an expression that is evaluated by the Java interpreter. Because method invo‐
cations can have side effects, however, they can also be used as expression
statements. This section does not discuss method invocation, but instead describes
how to define methods.
Defining Methods
You already know how to define the body of a method; it is simply an arbitrary
sequence of statements enclosed within curly braces. What is more interesting about
a method is its signature.
3
 The signature specifies the following:
• The name of the method
• The number, order, type, and name of the parameters used by the method
• The type of the value returned by the method
• The checked exceptions that the method can throw (the signature may also list
unchecked exceptions, but these are not required)
• Various method modifiers that provide additional information about the
method
Java Syntax
Methods | 71
A method signature defines everything you need to know about a method before
calling it. It is the method specification and defines the API for the method. In order
to use the Java platform’s online API reference, you need to know how to read a
method signature. And, in order to write Java programs, you need to know how to
define your own methods, each of which begins with a method signature.
A method signature looks like this:
modifiers type name (paramlist ) [ throws exceptions ]
The signature (the method specification) is followed by the method body (the
method implementation), which is simply a sequence of Java statements enclosed in
curly braces. If the method is abstract (see Chapter 3), the implementation is omit‐
ted, and the method body is replaced with a single semicolon.
The signature of a method may also include type variable declarations—such meth‐
ods are known as generic methods. Generic methods and type variables are dis‐
cussed in Chapter 4.
Here are some example method definitions, which begin with the signature and are
followed by the method body:
// This method is passed an array of strings and has no return value.
// All Java programs have an entry point with this name and signature.
public static void main(String[] args) {
 if (args.length > 0) System.out.println("Hello " + args[0]);
 else System.out.println("Hello world");
}
// This method is passed two double arguments and returns a double.
static double distanceFromOrigin(double x, double y) {
 return Math.sqrt(x*x + y*y);
}
// This method is abstract which means it has no body.
// Note that it may throw exceptions when invoked.
protected abstract String readText(File f, String encoding)
 throws FileNotFoundException, UnsupportedEncodingException;
modifiers is zero or more special modifier keywords, separated from each other by
spaces. A method might be declared with the public and static modifiers, for
example. The allowed modifiers and their meanings are described in the next
section.
The type in a method signature specifies the return type of the method. If the
method does not return a value, type must be void. If a method is declared with a
non-void return type, it must include a return statement that returns a value of (or
is convertible to) the declared type.
A constructor is a block of code, similar to a method, that is used to initialize newly
created objects. As we’ll see in Chapter 3, constructors are defined in a very similar
way to methods, except that their signatures do not include this type specification.
72 | Chapter 2: Java Syntax from the Ground Up
The name of a method follows the specification of its modifiers and type. Method
names, like variable names, are Java identifiers and, like all Java identifiers, may
contain letters in any language represented by the Unicode character set. It is legal,
and often quite useful, to define more than one method with the same name, as
long as each version of the method has a different parameter list. Defining multiple
methods with the same name is called method overloading.
Unlike some other languages, Java does not have anonymous
methods. Instead, Java 8 introduces lambda expressions,
which are similar to anonymous methods, but which the Java
runtime automatically converts to a suitable named method—
see “Lambda Expressions” on page 81 for more details.
For example, the System.out.println() method we’ve seen already is an overloa‐
ded method. One method by this name prints a string and other methods by the
same name print the values of the various primitive types. The Java compiler
decides which method to call based on the type of the argument passed to the
method.
When you are defining a method, the name of the method is always followed by the
method’s parameter list, which must be enclosed in parentheses. The parameter list
defines zero or more arguments that are passed to the method. The parameter spec‐
ifications, if there are any, each consist of a type and a name and are separated from
each other by commas (if there are multiple parameters). When a method is
invoked, the argument values it is passed must match the number, type, and order
of the parameters specified in this method signature line. The values passed need
not have exactly the same type as specified in the signature, but they must be con‐
vertible to those types without casting.
When a Java method expects no arguments, its parameter list
is simply (), not (void). Java does not regard void as a type—
C and C++ programmers in particular should pay heed.
Java allows the programmer to define and invoke methods that accept a variable
number of arguments, using a syntax known colloquially as varargs. Varargs are
covered in detail later in this chapter.
The final part of a method signature is the throws clause, which is used to list the
checked exceptions that a method can throw. Checked exceptions are a category of
exception classes that must be listed in the throws clauses of methods that can
throw them.
If a method uses the throw statement to throw a checked exception, or if it calls
some other method that throws a checked exception, and does not catch or handle
that exception, the method must declare that it can throw that exception.
Java Syntax
Methods | 73
If a method can throw one or more checked exceptions, it specifies this by placing
the throws keyword after the argument list and following it by the name of the
exception class or classes it can throw. If a method does not throw any exceptions, it
does not use the throws keyword. If a method throws more than one type of excep‐
tion, separate the names of the exception classes from each other with commas.
More on this in a bit.
Method Modifiers
The modifiers of a method consist of zero or more modifier keywords such as pub
lic, static, or abstract. Here is a list of allowed modifiers and their meanings:
abstract
An abstract method is a specification without an implementation. The curly
braces and Java statements that would normally comprise the body of the
method are replaced with a single semicolon. A class that includes an abstract
method must itself be declared abstract. Such a class is incomplete and cannot
be instantiated (see Chapter 3).
final
A final method may not be overridden or hidden by a subclass, which makes
it amenable to compiler optimizations that are not possible for regular meth‐
ods. All private methods are implicitly final, as are all methods of any class
that is declared final.
native
The native modifier specifies that the method implementation is written in
some “native” language such as C and is provided externally to the Java pro‐
gram. Like abstract methods, native methods have no body: the curly braces
are replaced with a semicolon.
Implementing native Methods
When Java was first released, native methods were sometimes used for efficiency
reasons. That is almost never necessary today. Instead, native methods are used to
interface Java code to existing libraries written in C or C++. native methods are
implicitly platform-dependent, and the procedure for linking the implementation
with the Java class that declares the method is dependent on the implementation of
the Java virtual machine. native methods are not covered in this book.
public, protected, private
These access modifiers specify whether and where a method can be used out‐
side of the class that defines it. These very important modifiers are explained in
Chapter 3.
74 | Chapter 2: Java Syntax from the Ground Up
static
A method declared static is a class method associated with the class itself
rather than with an instance of the class (we cover this in more detail in Chap‐
ter 3).



Method overloading refers to the practice of defining multiple methods (in the same class) that have the same name but different parameter lists.


// public class Circle {

//   // class field (compile-time constant)
//   public static final double PI = 3.14159;

//   // class field
//   public static Date lastCreated;

//   // instance field
//   public double radius;

//   // a no-argument constructor if provided only if no constructors are defined
//   public Circle() {
//     this(0.0);
//   }

//   // constructor
//   public Circle(double radius) {
//     this.radius = radius;
//     lastCreated = new Date();
//   }

//   // instance method
//   public double area() {
//     return PI * r * r;
//   }

//   // class method
//   public static double radiansToDegrees(double radians) {
//     return radians * 180 / PI;
//   }
// }

// System.out.println(Circle.PI);
// System.out.println(Circle.radiansToDegrees(2.0));

// Circle c = new Circle(1.0);
// c.radius = 2.0;
// System.out.println(c.radius);
// System.out.println(c.area());

// // class fields are shared among all instances of the class
// new Circle()
// System.out.println(Circle.lastCreated);