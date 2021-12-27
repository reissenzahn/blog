

// an interface is an abstract type that is used to specify behavior that classes must implement


	// an interface may not define any instance fields thought it can include constants that are declared both static and final





// interfaces cannot be instantiated but object reference may be specified to be of an interface type in which case they can be bound to an object that implements the interface




// interfaces can contain method signatures without method bodies
interface Animal {

	// all non-default methods of an interface are implicitly abstract and public so the keywords are usually omitted
	void move();

	// an interface can provide a method with a default implementation
	default void eat() {
		System.out.println("eating noisily...");
	}
} 

interface Flying {
	void fly();

	// an interface may contain static methods and private methods
	
}

interface Prey extends Animal {
	void runAway(Predator predator);
}

interface Predator extends Animal {	
	void chasePrey(Prey prey);
	void killPrey(Prey prey);
}

interface Venomous {
	void secreteVenom();
}

// an interface may inherit multiple interfaces
interface VenomousPredator extends Predator, Venomous {
	void killWithVenom(Prey prey);
}

// a class that implements an interface must implement all of the non-default methods described in the interface or be declared as abstract
class Lion implements Predator {

	public 
}

class Snake implements VenomousPredator {
	
	void 
}

// a class may implement multiple interfaces
class Eagle implements Predator, Flying {


}






// interfaces may extend other interfaces, and, like a class definition, an interface definition indicates this by including an extends clause

// when one interface extends another, it inherits all the methods and constants of its superinterface and can define new methods and constants

// the extends clause of an interface definition may include more than one superinterface

interface Positionable extends Centered {
 void setUpperRightCorner(double x, double y);
 double getUpperRightX();
 double getUpperRightY();
}

interface Transformable extends Scalable, Translatable, Rotatable {}
interface SuperShape extends Positionable, Transformable {}


// an interface that extends more than one interface inherits all the methods and constants from each of those interfaces and can define its own additional methods and constants; a class that implements such an interface must implement the abstract methods defined directly by the interface, as well as all the abstract methods inherited from all the superinterfaces



// a class uses implements to name one or more interfaces it implements

// if a class implements an interface but does not provide an implementation for every mandatory interface method, it inherits those unimplemented abstract methods from the interface and must be declared abstract

public class CenteredRectangle extends Rectangle implements Centered {
 // New instance fields
 private double cx, cy;
 // A constructor
 public CenteredRectangle(double cx, double cy, double w, double h) {
 super(w, h);
 this.cx = cx;
 this.cy = cy;
 }
 // We inherit all the methods of Rectangle but must
 // provide implementations of all the Centered methods.
 public void setCenter(double x, double y) { cx = x; cy = y; }
 public double getCenterX() { return cx; }
 public double getCenterY() { return cy; }
}

Shape[] shapes = new Shape[3]; // Create an array to hold shapes
// Create some centered shapes, and store them in the Shape[]
// No cast necessary: these are all compatible assignments
shapes[0] = new CenteredCircle(1.0, 1.0, 1.0);
shapes[1] = new CenteredSquare(2.5, 2, 3);
shapes[2] = new CenteredRectangle(2.3, 4.5, 3, 4);
// Compute average area of the shapes and
// average distance from the origin
double totalArea = 0;
double totalDistance = 0;

for(int i = 0; i < shapes.length; i++) {
 totalArea += shapes[i].area(); // Compute the area of the shapes
 // Be careful, in general, the use of instanceof to determine the
 // runtime type of an object is quite often an indication of a
 // problem with the design
 if (shapes[i] instanceof Centered) { // The shape is a Centered shape
 // Note the required cast from Shape to Centered (no cast would
 // be required to go from CenteredSquare to Centered, however).
 Centered c = (Centered) shapes[i];
 double cx = c.getCenterX(); // Get coordinates of the center
 double cy = c.getCenterY(); // Compute distance from origin
 totalDistance += Math.sqrt(cx*cx + cy*cy);
 }
}

System.out.println("Average area: " + totalArea/shapes.length);
System.out.println("Average distance: " + totalDistance/shapes.length);


// Interfaces are data types in Java, just like classes. When a class implements an interface, instances of that class can be assigned to variables of the interface type.


// DEFAULT METHODS

// a default method can be added to any interface and must include an implementation

// an implementing class may (but is not required to) implement the default method

// if an implementing class implements the default method, then the implementation in the class is used; otherwise, the default implementation is used

// one consequence of default methods is that when implementing multiple interfaces, it’s possible that two or more interfaces may contain a default method with a completely identical name and signature

interface Vocal {
 default void call() {
 System.out.println("Hello!");
 }
}

interface Caller {
 default void call() {
 Switchboard.placeCall(this);
 }
}

public class Person implements Vocal, Caller {
 // ... which default is used?
}

// If a class implements multiple interfaces in such a way as to cause a potential clash of default method implementations, the implementing class must override the clashing method and provide a definition of what is to be done.

// syntax is provided to allow the implementing class to simply call one of the interface default methods if that is what is required

public class Person implements Vocal, Caller {
 public void call() {
 // Can do our own thing
 // or delegate to either interface
 // e.g.,
 // Vocal.super.call();
 // or
 // Caller.super.call();
 }
}


// a default method can call another method on the interface, call any static method and use the this reference


// all interfaces are abstract and the modifier is optional in interface declarations

// a final class cannot be subclassed

// a final method cannot be overridden

// a final field cannot have its value changed; static final fields are compile-time constants

// a static inner class is a top-level class and is not associated with a member of the containing class






interface Flyable {
	void fly();
}

interface Eatable {
	void eat();
}

// Bird class will implement both interfaces
class Bird implements Flyable, Eatable {

	public void fly() {
		System.out.println("Bird flying");
	}

	public void eat() {
		System.out.println("Bird eats");
	}
	// It can have more own methods.
}
