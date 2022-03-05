
class CaffeineBeverage {

  final void prepareRecipe() {
    boilWater();
    brew();
    pourInCup();
    
    if (customerWantsCondiments()) {
      addCondiments();
    }
  }

  abstract void brew();

  abstract void addCondiments();

  void boilWater() {
    System.out.println("Boiling water");
  }

  void pourInCup() {
    System.out.println("Pouring into cup");
  }

  boolean customerWantsCondiments() {
    return true;
  }
}

class Coffee extends CaffeineBeverage {

  public void brew() {
    System.out.println("Dripping Coffee through filter");
  }

  public void addCondiments() {
    System.out.println("Adding Sugar and Milk");
  }
}

class Tea extends CaffeineBeverage {

  public void brew() {
    System.out.println("Steeping the tea");
  }
 
  public void addCondiments() {
    System.out.println("Adding Lemon");
  }
}

public class CoffeeWithHook extends CaffeineBeverageWithHook {
 public void brew() {
 System.out.println("Dripping Coffee through filter");
 }
 public void addCondiments() {
 System.out.println("Adding Sugar and Milk");
 }
 public boolean customerWantsCondiments() {
 String answer = getUserInput();
 if (answer.toLowerCase().startsWith("y")) {
 return true;
 } else {
 return false;
 }
 }
 private String getUserInput() {
 String answer = null;
 System.out.print("Would you like milk and sugar with your coffee (y/n)? ");
 BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
 try {
 answer = in.readLine();
 } catch (IOException ioe) {
 System.err.println("IO error trying to read your answer");
 }
 if (answer == null) {
 return "no";
 }
 return answer;
 }
}


public class TemplateMethodPattern {

  public static void main(String[] args) {
    Tea tea = new Tea();
    tea.prepareRecipe();

    Coffee coffee = new Coffee();
    coffee.prepareRecipe();
  }
}

