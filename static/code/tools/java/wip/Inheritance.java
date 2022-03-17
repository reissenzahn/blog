
// a subclass inherits the fields and methods of its superclass


class Shape


public class PlaneCircle extends Circle {

  private final double cx, cy;
  
  public PlaneCircle(double r, double x, double y) {
    super(r); // Invoke the constructor of the superclass, Circle()
    this.cx = x; // Initialize the instance field cx
    this.cy = y; // Initialize the instance field cy
  }
  
  public double getCentreX() {
    return cx;
  }
  
  public double getCentreY() {
    return cy;
  }

  public boolean isInside(double x, double y) {
    double dx = x - cx, dy = y - cy; // Distance from center
    double distance = Math.sqrt(dx*dx + dy*dy); // Pythagorean theorem
    return (distance < r); // Returns true or false
  }
}

double ratio = pc.circumference() / pc.area();


// Unit circle at the origin
PlaneCircle pc = new PlaneCircle(1.0, 0.0, 0.0);
Circle c = pc; // Assigned to a Circle variable without casting

This assignment of a PlaneCircle object to a Circle variable can be done without a
cast. As we discussed in Chapter 2, a conversion like this is always legal.

// Narrowing conversions require a cast (and a runtime check by the VM)
PlaneCircle pc2 = (PlaneCircle) c;
boolean origininside = ((PlaneCircle) c).isInside(0.0, 0.0);




// When a class is declared with the final modifier, it means that it cannot be subclassed

// a class may only have a single direct superclass:
public class PlaneCircle extends Circle { ... }

// if the superclass is not specified with an extends clause, then the superclass is taken to be the class java.lang.Objec

// the Object class  is the only class in Java that does not have a superclass and all other classes inherit (directly or indirectly) the methods of Object

// a subclass cannot extend more than one superclass


// the super keyword can be used in the constructor of a subclass to invoke a constructor in the superclass; the call to super() must appear as the first statement in the constructor

public PlaneCircle(double r, double x, double y) {
 super(r); // Invoke the constructor of the superclass, Circle()
 this.cx = x; // Initialize the instance field cx
 this.cy = y; // Initialize the instance field cy
}


// if the first statement in a constructor does not explicitly invoke another constructor with this() or super() then a call to super() is automatically inserted; if the superclass does not have a constructor that takes no arguments then this implicit invocation causes a compilation error

// this way a constructor can also count on the fields of its superclass to be initialized by the time it runs



// if a class is declared without a constructor, Java implicitly adds a constructor to the class. This default constructor does nothing but invoke the superclass constructor.

If a class does not declare any constructors, it is given a no-argument constructor by
default. Classes declared public are given public constructors. All other classes are
given a default constructor that is declared without any visibility modifier: such a
constructor has default visibility.


// classes that should never be instantiated  should define a private constructor; such a constructor can never be invoked from outside of the class



// HIDING FIELDS

// if a field in a subclass has the same name as a field class in the superclass then the field in the subclass hides the field in the superclass

// the super keyword can be used to refer to the hidden field
super.r

// alternatively we can cast this (or any instance of the class) to the appropriate superclass and then access the field; this is particularly useful when you need to refer to a hidden field defined in a class that is not the immediate superclass
((Superclass) this).r


Suppose classes A, B, and C all define a field named x and that C is a subclass of B, which is a subclass of A. Let c be an instance of C.

x // Field x in class C
this.x // Field x in class C
super.x // Field x in class B
((B)this).x // Field x in class B
((A)this).x // Field x in class A
super.super.x // Illegal; does not refer to x in class A

c.x // Field x of class C
((B)c).x // Field x of class B
((A)c).x // Field x of class A

// class fields can also be hidden and, while super can be used, you can always refer to a class field by prepending the name of the desired class



Overriding Superclass Methods

// when a class defines an instance method using the same name, return type, and parameters as a method in its superclass, that method overrides the method of the superclass. When the method is invoked for an object of the class, it is the new definition of the method that is called

// The return type of the overriding method may be a subclass of the return type of the original method (instead of being exactly the same type). This is known as a covariant return.

public class Car {
 public static final double LITRE_PER_100KM = 8.9;

 protected double topSpeed;
 protected double fuelTankCapacity;
 private int doors;
 
 public Car(double topSpeed, double fuelTankCapacity, int doors) {
this.topSpeed = topSpeed;
 this.fuelTankCapacity = fuelTankCapacity;
 this.doors = doors;
 }
 
 public double getTopSpeed() {
 return topSpeed;
 }
 
 public int getDoors() {
 return doors;
 }
 
 public double getFuelTankCapacity() {
 return fuelTankCapacity;
 }
 
 public double range() {
 return 100 * fuelTankCapacity / LITRE_PER_100KM;
 }
}

public class SportsCar extends Car {
 private double efficiency;
 public SportsCar(double topSpeed) {
 super(topSpeed, 50.0, 2);
 if (topSpeed > 200.0) {
 efficiency = 200.0 / topSpeed;
 } else {
 efficiency = 1.0;
 }
 }
 public double getEfficiency() {
 return efficiency;
 }
 @Override
 public double range() {
 return 100 * fuelTankCapacity * efficiency / LITRE_PER_100KM;
 }
}

Just like fields, class methods can be hidden by a subclass but not overridden.





Method overriding is not like field hiding at all. You can refer to hidden fields simply by casting an object to an instance of the appropriate superclass, but you cannot invoke overridden instance methods with this technique.

While this difference between method overriding and field hiding may seem sur‐
prising at first, a little thought makes the purpose clear.
Suppose we are manipulating a bunch of Car and SportsCar objects, and store them
in an array of type Car[]. We can do this because SportsCar is a subclass of Car, so
all SportsCar objects are legal Car objects.
When we loop through the elements of this array, we don’t have to know or care
whether the element is actually a Car or an SportsCar. What we do care about very
much, however, is that the correct value is computed when we invoke the range()
method of any element of the array. In other words, we don’t want to use the for‐
mula for the range of a car when the object is actually a sports car!
All we really want is for the objects we’re computing the ranges of to “do the right
thing”—the Car objects to use their definition of how to compute their own range,
and the SportsCar objects to use the definition that is correct for them.





Virtual method lookup
If we have a Car[] array that holds Car and SportsCar objects, how does javac
know whether to call the range() method of the Car class or the SportsCar class for
any given item in the array? In fact, the source code compiler cannot know this at
compilation time.
Instead, javac creates bytecode that uses virtual method lookup at runtime. When
the interpreter runs the code, it looks up the appropriate range() method to call for
each of the objects in the array. That is, when the interpreter interprets the expres‐
sion o.range(), it checks the actual runtime type of the object referred to by the
variable o and then finds the range() method that is appropriate for that type.
Some other languages (such as C# or C++) do not do virtual
lookup by default and instead have a virtual keyword that
programmers must explicitly use if they want to allow sub‐
classes to be able to override a method.
The JVM does not simply use the range() method that is associated with the static
type of the variable o, as that would not allow method overriding to work in the way
detailed earlier. Virtual method lookup is the default for Java instance methods. See
Chapter 4 for more details about compile-time and runtime type and how this
affects virtual method lookup.
Invoking an overridden method
We’ve seen the important differences between method overriding and field hiding.
Nevertheless, the Java syntax for invoking an overridden method is quite similar to
the syntax for accessing a hidden field: both use the super keyword. The following
code illustrates:
class A {
 int i = 1; // An instance field hidden by subclass B
 int f() { return i; } // An instance method overridden by subclass B
}
class B extends A {
 int i; // This field hides i in A
 int f() { // This method overrides f() in A
 i = super.i + 1; // It can retrieve A.i like this
 return super.f() + i; // It can invoke A.f() like this
 }
}
Recall that when you use super to refer to a hidden field, it is the same as casting
this to the superclass type and accessing the field through that. Using super to
invoke an overridden method, however, is not the same as casting the this refer‐
ence. In other words, in the previous code, the expression super.f() is not the
same as ((A)this).f().
When the interpreter invokes an instance method with the super syntax, a modified
form of virtual method lookup is performed. The first step, as in regular virtual
method lookup, is to determine the actual class of the object through which the
method is invoked. Normally, the runtime search for an appropriate method defini‐
tion would begin with this class. When a method is invoked with the super syntax,
however, the search begins at the superclass of the class. If the superclass imple‐
ments the method directly, that version of the method is invoked. If the superclass
inherits the method, the inherited version of the method is invoked.
Note that the super keyword invokes the most immediately overridden version of a
method. Suppose class A has a subclass B that has a subclass C and that all three
classes define the same method f(). The method C.f() can invoke the method
B.f(), which it overrides directly, with super.f(). But there is no way for C.f() to
invoke A.f() directly: super.super.f() is not legal Java syntax. Of course, if C.f()
invokes B.f(), it is reasonable to suppose that B.f() might also invoke A.f().
This kind of chaining is relatively common with overridden methods: it is a way of
augmenting the behavior of a method without replacing the method entirely.
Don’t confuse the use of super to invoke an overridden
method with the super() method call used in a constructor to
invoke a superclass constructor. Although they both use the
same keyword, these are two entirely different syntaxes. In
particular, you can use super to invoke an overridden method
anywhere in the overriding class, while you can use super()
only to invoke a superclass constructor as the very first state‐
ment of a constructor.

It is also important to remember that super can be used only to invoke an overrid‐
den method from within the class that overrides it. Given a reference to a Sports
Car object e, there is no way for a program that uses e to invoke the range()
method defined by the Car class on e.





Class methods behave quite differently, and they cannot be overridden.