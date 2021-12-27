

class ChocolateBoiler {

  private boolean empty;
  private boolean boiled;

  private static ChocolateBoiler instance;

  private ChocolateBoiler() {
    empty = true;
    boiled = false;
  }

  public static synchronized ChocolateBoiler getInstance() {
    if (instance == null) {
      instance = new ChocolateBoiler();
    }

    return instance;
  }

  public void fill() {
    if (isEmpty()) {
      // fill the boiler with mixture

      empty = false;
      boiled = false;
    }
  }

  public void boil() {
    if (!isEmpty() && !isBoiled()) {
      // bring the mixture to a boil

      boiled = true;
    }
  }

  public void drain() {
    if (!isEmpty() && isBoiled()) {
      // drain the boiled mixture

      empty = true;
    }
  }

  public boolean isEmpty() {
    return empty;
  }

  public boolean isBoiled() {
    return boiled;
  }
}


public class SingletonPattern {

  public static void main(String[] args) {

  }
}