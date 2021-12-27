class MenuItem {

  String name;
  String description;
  boolean vegetarian;
  double price;

  public MenuItem(String name, String description, boolean vegetarian, double price) {
    this.name = name;
    this.description = description;
    this.vegetarian = vegetarian;
    this.price = price;
  }

   public String getName() {
    return name;
  }

  public String getDescription() {
    return description;
  }

  public double getPrice() {
    return price;
  }

  public boolean isVegetarian() {
    return vegetarian;
  }
}

class PancakeHouseMenu {
  
  List<MenuItem> menuItems;

  public PancakeHouseMenu() {
    menuItems = new ArrayList<MenuItem>();

    addItem("K&B's Pancake Breakfast", "Pancakes with scrambled eggs and toast", true, 2.99);
    
    addItem("Regular Pancake Breakfast", "Pancakes with fried eggs, sausage", false, 2.99);
    
    addItem("Blueberry Pancakes", "Pancakes made with fresh blueberries", true, 3.49);
    
    addItem("Waffles", "Waffles with your choice of blueberries or strawberries", true, 3.59);
  }

  public void addItem(String name, String description, boolean vegetarian, double price) {
    MenuItem menuItem = new MenuItem(name, description, vegetarian, price);
    menuItems.add(menuItem);
  }

  // public ArrayList<MenuItem> getMenuItems() {
  //   return menuItems;
  // }

  public Iterator createIterator() {
    return new PancakeHouseMenuIterator(menuItems);
  }

  // other menu methods here
}

class DinerMenu {

  static final int MAX_ITEMS = 6;
  int numberOfItems = 0;
  MenuItem[] menuItems;

  public DinerMenu() {
    menuItems = new MenuItem[MAX_ITEMS];

    addItem("Vegetarian BLT", "(Fakin') Bacon with lettuce & tomato on whole wheat", true, 2.99);
    
    addItem("BLT", "Bacon with lettuce & tomato on whole wheat", false, 2.99);
    
    addItem("Soup of the day", "Soup of the day, with a side of potato salad", false, 3.29);
    
    addItem("Hotdog", "A hot dog, with sauerkraut, relish, onions, topped with cheese", false, 3.05);
    
    // a couple of other Diner Menu items added here
  }

  public void addItem(String name, String description, boolean vegetarian, double price) {
    if (numberOfItems >= MAX_ITEMS) {
      System.err.println("Sorry, menu is full! Can't add item to menu");
    } else {
      menuItems[numberOfItems] = new MenuItem(name, description, vegetarian, price);
      numberOfItems++;
    }
  }

  // public MenuItem[] getMenuItems() {
  //   return menuItems;
  // }
 
  public Iterator createIterator() {
    return new DinerMenuIterator(menuItems);
  }

 // other menu methods here
}


interface Iterator {
  
  boolean hasNext();
  MenuItem next();
}

class DinerMenuIterator implements Iterator {
  MenuItem[] items;
  int position = 0;  // current position in array

  public DinerMenuIterator(MenuItem[] items) {
    this.items = items;
  }

  public MenuItem next() {
    MenuItem menuItem = items[position];
    position++;
    return menuItem;
  }

  public boolean hasNext() {
    if (position < items.length && items[position] != null) {
      return true;
    }

    return false;
  }
}

class PancakeHouseMenuIterator implements Iterator {
  ArrayList<MenuItem> items;
  int position = 0;

  public PancakeHouseMenuIterator(ArrayList<MenuItem> items) {
    this.items = items;
  }

    public MenuItem next() {
    MenuItem menuItem = items.get(position);
    position++;
    return menuItem;
  }

  public boolean hasNext() {
    if (position < items.size()) {
      return true;
    }

    return false;
  }
}


class Waitress {
  PancakeHouseMenu pancakeHouseMenu;
  DinerMenu dinerMenu;

  public Waitress(PancakeHouseMenu pancakeHouseMenu, DinerMenu dinerMenu) {
    this.pancakeHouseMenu = pancakeHouseMenu;
    this.dinerMenu = dinerMenu;
  }

  public void printMenu() {
    Iterator pancakeIterator = pancakeHouseMenu.createIterator();
    Iterator dinerIterator = dinerMenu.createIterator();
    
    System.out.println("MENU\n----\nBREAKFAST");
    printMenu(pancakeIterator);
    
    System.out.println("\nLUNCH");
    printMenu(dinerIterator);
 }

  private void printMenu(Iterator iterator) {
    while (iterator.hasNext()) {
      MenuItem menuItem = iterator.next();
      System.out.print(menuItem.getName() + ", ");
      System.out.print(menuItem.getPrice() + " -- ");
      System.out.println(menuItem.getDescription());
    }
  }

  // other methods here
}

public class IteratorPattern {

  public static void main(String[] args) {
    PancakeHouseMenu pancakeHouseMenu = new PancakeHouseMenu();
    DinerMenu dinerMenu = new DinerMenu();
    
    Waitress waitress = new Waitress(pancakeHouseMenu, dinerMenu);
    waitress.printMenu();
  }
}