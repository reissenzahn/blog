abstract class Duck {

  // each concrete duck object will assign a specific behavior to these variables at runtime
  private FlyBehavior flyBehavior;
  private QuackBehavior quackBehavior;

  public Duck() {}

  // a Duck subclass delegates its flying and quacking behaviors rather than using inherited or overridden behaviors
  public void performFly() {
    flyBehavior.fly();
  }

  public void performQuack() {
    quackBehavior.quack();
  }

  // all ducks swim in the same way
  public void swim() {
    System.out.println("*splash* *splash* *splash*");
  }
  
  // all duck look completely different
  public abstract void display();

  public void setFlyBehavior(FlyBehavior flyBehavior) {
    this.flyBehavior = flyBehavior;
  }

  public void setQuackBehavior(QuackBehavior quackBehavior) {
    this.quackBehavior = quackBehavior;
  }
}


// subclasses that inherit common fields and methods from Duck
class MallardDuck extends Duck {
  
  public MallardDuck() {
    setQuackBehavior(new Quack());
    setFlyBehavior(new FlyWithWings());
  }

  public void display() {
    System.out.println("Looks like a Mallard duck.");
  }
}

class RubberDuck extends Duck {
  
  public RubberDuck() {
    setQuackBehavior(new Squeak());
    setFlyBehavior(new FlyNope());
  }

  public void display() {
    System.out.println("Looks like a rubber duck.");
  }
}


// interface that defines the quacking behavior and its various implementations
interface QuackBehavior {
  public void quack();
}

// other types of objects can reuse our fly and quack behaviors because these behaviors are no longer hidden away in the Duck subclasses
class Quack implements QuackBehavior {

  public void quack() {
    System.out.println("Quack!");
  }
}

class Squeak implements QuackBehavior {

  public void quack() {
    System.out.println("Squeak!");
  }
}

class MuteQuack implements QuackBehavior  {

  public void quack() {
    System.out.println("...");
  }
}


// interface that defines the flying behavior and its various implementations
interface FlyBehavior {

  public void fly();
}

class FlyWithWings implements FlyBehavior {

  public void fly() {
    System.out.println("*flap wings*");
  }
}

class FlyNope implements FlyBehavior {

  public void fly() {
    System.out.println("*stays put*");
  }
}

// we can add new behaviors without modifying any of our existing behavior classes or touching any of the Duck subclasses that use flying behaviors
class FlyWithRocket implements FlyBehavior {

  public void fly() {
    System.out.println("*woosh!*");
  }
}

public class Example {
  public static void main(String[] args) {

    Duck mallardDuck = new MallardDuck();

    mallardDuck.performQuack();
    mallardDuck.performFly();


    Duck rubberDuck = new RubberDuck();

    rubberDuck.performFly();
    rubberDuck.setFlyBehavior(new FlyWithRocket());
    rubberDuck.performFly();
  }
}