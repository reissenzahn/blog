import java.util.*;

abstract class Pizzeria {

  protected Pizza orderPizza(String type) {

    // orderPizza() is decoupled from various pizza subclasses
    Pizza pizza = createPizza(type);

    // this is what we want to keep consistent across all franchises and it will most likely stay the same
    pizza.prepare();
    pizza.bake();
    pizza.cut();
    pizza.box();

    return pizza;
  }

  // this will differ between franchises and vary as the pizza selections change
  abstract Pizza createPizza(String type);
}

class NYPizzeria extends Pizzeria {
  protected Pizza createPizza(String item) {
  
    if (item.equals("cheese")) {
      return new NYStyleCheesePizza();
    }
    
    // more logic for various pizza types
    // else if (item.equals("veggie")) {
    //   return new NYStyleVeggiePizza();
    // } else if (item.equals("clam")) {
    //   return new NYStyleClamPizza();
    // } else if (item.equals("pepperoni")) {
    //   return new NYStylePepperoniPizza();
    // }

    return null;
  }
}

class ChicagoPizzeria extends Pizzeria {

  protected Pizza createPizza(String item) {
    if (item.equals("cheese")) {
      return new ChicagoStyleCheesePizza();
    }

    // else if (item.equals("veggie")) {
    //   return new ChicagoStyleVeggiePizza();
    // } else if (item.equals("clam")) {
    //   return new ChicagoStyleClamPizza();
    // } else if (item.equals("pepperoni")) {
    //   return new ChicagoStylePepperoniPizza();
    // }

    return null;
  }
}


abstract class Pizza {

  protected String name;
  protected String dough;
  protected String sauce;
  List<String> toppings = new ArrayList<String>();

  protected void prepare() {
    System.out.println("Tossing dough, applying sauce and adding toppings.");
  }

  protected void bake() {
    System.out.println("Baking for 25 minutes at 350C.");
  }

  protected void cut() {
    System.out.println("Cutting the pizza into diagonal slices.");
  }

  protected void box() {
    System.out.println("Placing pizza in official box.");
  }

  public String getName() {
    return name;
  }
}

class NYStyleCheesePizza extends Pizza {

  public NYStyleCheesePizza() {
    name = "NY Style Sauce and Cheese Pizza";
    dough = "Thin Crust Dough";
    sauce = "Marinara Sauce";

    toppings.add("Grated Reggiano Cheese");
  }
}

class ChicagoStyleCheesePizza extends Pizza {

  public ChicagoStyleCheesePizza() {
    name = "Chicago Style Deep Dish Cheese Pizza";
    dough = "Extra Thick Crust Dough";
    sauce = "Plum Tomato Sauce";

    toppings.add("Shredded Mozzarella Cheese");
  }

  protected void cut() {
    System.out.println("Cutting the pizza into square slices.");
  }
}


public class FactoryMethod {

  public static void main(String[] args) {
    Pizzeria nyStore = new NYPizzeria();
    Pizza pizza = nyStore.orderPizza("cheese");
    System.out.println(pizza.getName());

    Pizzeria chicagoStore = new ChicagoPizzeria();
    pizza = chicagoStore.orderPizza("cheese");
    System.out.println(pizza.getName());
  }
}