
// Iterator (Head First)

class MenuItem {

  private String name;
  private String description;
  private boolean vegetarian;
  private double price;

  public MenuItem(String name, boolean vegetarian, double price) {
    this.name = name;
    this.vegetarian = vegetarian;
    this.price = price;
  }

  public String getName() {
    return name;
  }

  public boolean isVegetarian() {
    return vegetarian;
  }

  public double getPrice() {
    return price;
  }
}

class DinnerMenu implements Iterable<MenuItem> {

  private ArrayList<MenuItem> menuItems;

  public DinnerMenu() {
    menuItems = new ArrayList<>();

    menuItems.add(new MenuItem("Prime Steak", 3.50, false));
    menuItems.add(new MenuItem("Beef Burger", 2.75, false));
    menuItems.add(new MenuItem("Pasta", 2.50, true));
  }

  
}


class DessertMenu implements Iterable<MenuItem> {

  private MenuItem[] menuItems;

  public DessertMenu() {
    menuItems = new MenuItem[3];

    menuItems[0] = new MenuItem("Ice Cream", 1.50, true);
    menuItems[1] = new MenuItem("Jelly and Custard", 1.00, false);
    menuItems[2] = new MenuItem("Chocolate Cake", 1.75, true);
  }
}










// Flyweight

// tree contains state unique for each tree
// class Tree {

//   private int x;
//   private int y;
//   private TreeType type;

//   public Tree(int x, int y, TreeType type) {
//     this.x = x;
//     this.y = y;
//     this.type = type;
//   }
// }


// Memento

// enum StarType {

//   SUN("sun"),
//   RED_GIANT("red giant"),
//   WHITE_DWARF("white dwarf"),
//   SUPERNOVA("supernova"),
//   DEAD("dead");

//   private final String title;

//   StarType(String title) {
//     this.title = title;
//   }

//   @Override
//   public String toString() {
//     return title;
//   }
// }

// class Star {

//   private StarType starType;
//   private int age;
//   private int mass;

//   public Star(StarType starType, int age, int mass) {
//     this.starType = starType;
//     this.age = age;
//     this.mass = mass;
//   }

//   public void advanceTime() {
//     age *= 2;
//     mass *= 8;

//     switch (type) {
//       case SUN:
//         type = StarType.RED_GIANT;
//         break;
//       case RED_GIANT:
//         type = StarType.WHITE_DWARF;
//         break;
//       case WHITE_DWARF:
//         type = StarType.SUPERNOVA;
//         break;
//       case SUPERNOVA:
//         type = StarType.DEAD;
//         break;
//       case DEAD:
//         age *= 2;
//         mass = 0;
//         break;
//       default:
//         break;
//     }
//   }
// }













// Factory Method

// abstract class Pizza {

//   private String name;
//   private String dough;
//   private String sauce;

//   void prepare();
//   void bake();
//   void cut();
//   void box();
// }

// class Margherita implements Pizza {

// }

// class Pepperoni implements Pizza {

// }

// class SimplePizzaFactory {

//   public Pizza createPizza(String type) {
//     Pizza pizza = null;

//     if (type.equals("cheese")) {
//       pizza = new CheesePizza();
//     } else if (type.equals("pepperoni")) {
//       pizza = new PepperoniPizza();
//     } else if (type.equals("clam")) {
//       pizza = new ClamPizza();
//     } else if (type.equals("veggie")) {
//       pizza = new VeggiePizza();
//     }

//     return Pizza;
//   }
// }

// abstract class Pizzeria {

//   public Pizza order(String type) {
//     Pizza pizza = createPizza(type);

//     pizza.prepare();
//     pizza.bake();
//     pizza.cut();
//     pizza.box();

//     return pizza;
//   }

//   abstract Pizza createPizza(String type);
// }



// public class Example {

//   public static void main(String[] args) {

//   }
// }




