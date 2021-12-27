abstract class Beverage {
  protected String description;

  public Beverage() {
    description = "Unknown Beverage";
  }

  public String getDescription() {
    return description;
  }

  public abstract double cost();
}

class Espresso extends Beverage {

  public Espresso() {
    description = "Espresso";
  }

  public double cost() {
    return 1.99;
  }
}

class HouseBlend extends Beverage {

  public HouseBlend() {
    description = "House Blend Coffee";
  }

  public double cost() {
    return .89;
  }
}

class DarkRoast extends Beverage {

  public DarkRoast() {
    description = "Dark Roast";
  }

  public double cost() {
    return .99;
  }
}


// to make the decorators interchangeable with a Beverage by extending the Beverage class
abstract class CondimentDecorator extends Beverage {
  
  // we are using the Beverage supertype to refer to the Beverage so the Decorator can wrap any beverage
  protected Beverage beverage;

  public abstract String getDescription();
}

class SteamedMilk extends CondimentDecorator {

  // we instantiate the decorator with a reference to the object we are wrapping
  public SteamedMilk(Beverage beverage) {
    this.beverage = beverage;
  }

  public String getDescription() {
    return beverage.getDescription() + ", Steamed Milk";
  }

  // to compute the cost we first delegate the call to the wrapped object and then we add the cost of the condiment
  public double cost() {
    return beverage.cost() + .10;
  }
}

class Mocha extends CondimentDecorator {

  public Mocha(Beverage beverage) {
    this.beverage = beverage;
  }

  public String getDescription() {
    return beverage.getDescription() + ", Mocha";
  }

  public double cost() {
    return beverage.cost() + .20;
  }
}

class Whip extends CondimentDecorator {

  public Whip(Beverage beverage) {
    this.beverage = beverage;
  }

  public String getDescription() {
    return beverage.getDescription() + ", Whip";
  }

  public double cost() {
    return beverage.cost() + .15;
  }
}


public class Example {

  public static void main(String[] args) {
    Beverage beverage = new Espresso();

    System.out.println(beverage.getDescription() + " ($" + beverage.cost() + ")");
 
    beverage = new Mocha(beverage);
    beverage = new Mocha(beverage);
    beverage = new Whip(beverage);
    
    System.out.println(beverage.getDescription() + " ($" + beverage.cost() + ")");
  }
}