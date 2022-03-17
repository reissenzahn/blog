public abstract class Pet {
  
  // an abstract class can contain fields and non-abstract methods
  protected String name;
  protected int age;
  
  // an abstract class can contain constructors
  protected Pet(String name, int age) {
    this.name = name;
    this.age = age;
  }
 
  // an abstract class can contain abstract methods that must be implemented in non-abstract subclasses
  public abstract void say();
}

// abstract classes cannot be instantiated
Pet pet = new Pet("Unnamed", 5);  // error


// below are two concrete subclasses of Pet
class Cat extends Pet {

  public Cat(String name, int age) {
    super(name, age);
  }
 
  @Override
  public void say() {
    System.out.println("Meow!");
  }
}
 
class Dog extends Pet {
 
  public Dog(String name, int age) {
    super(name, age);
  }
 
  @Override
  public void say() {
    System.out.println("Woof!");
  }
}

Dog dog = new Dog("Boss", 5);
Cat cat = new Cat("Tiger", 2);
        
dog.say();  // Woof!
cat.say();  // Meow!

// an abstract class is a class that cannot be instantiated and may contain abstract methods
abstract class Shape {

  protected String color;

  public Shape(String color) {
    this.color = color;
  }

  public String getColor() {
    return color;
  }

  // an abstract method has no implementation
  public abstract double area();

  // static, private and final methods cannot be abstract as there would be no way to override them
  public abstract double circumference();
}

// a subclass of an abstract class can only be instantiated if it provides implementations for all inherited abstract methods; otherwise, it must also be declared as abstract
class Circle extends Shape {

  public static final double PI = 3.14159;

  protected double radius;
  
  public Circle(String color, double radius) {
    super(color);
    this.radius = radius;
  }
  
  public double getRadius() {
    return radius;
  }
  
  public double area() {
    return PI * radius * radius;
  }
  
  public double circumference() {
    return 2 * PI * radius;
  }
}

class Rectangle extends Shape {
  
  protected double width;
  protected double height;

  public Rectangle(String color, double width, double height) {
    super(color);
    this.width = width;
    this.height = height;
  }

  public double getWidth() {
    return width;
  }
  
  public double getHeight() {
    return height;
  }
  
  public double area() {
    return width * height;
  } 
  
  public double circumference() {
    return 2 * (width + height);
  }
}

public class AbstractClasses {

  // $ javac AbstractClasses.java && java AbstractClasses
  public static void main(String[] args) {
    Shape[] shapes = new Shape[3];

    shapes[0] = new Circle("red", 2.0);
    shapes[1] = new Rectangle("blue", 1.0, 3.0);
    shapes[2] = new Rectangle("green", 4.0, 2.0);

    double total = 0;

    for (int i = 0; i < shapes.length; i++) {
      total += shapes[i].area();
    }

    System.out.println(total);  // 
  }
}







