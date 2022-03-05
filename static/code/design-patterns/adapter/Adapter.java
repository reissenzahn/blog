interface Duck {

  public void quack();
  public void fly();
}

class MallardDuck implements Duck {
  
  public void quack() {
    System.out.println("Quack!");
  }
  
  public void fly() {
    System.out.println("*flies*");
  }
}


interface Turkey {
  
  public void gobble();
  public void fly();
}

class WildTurkey implements Turkey {
  public void gobble() {
    System.out.println("Gobble gobble!");
  }

  public void fly() {
    System.out.println("*flies short distance*");
  }
}


class TurkeyAdapter implements Duck {
  private Turkey turkey;

  public TurkeyAdapter(Turkey turkey) {
    this.turkey = turkey;
  }

  public void quack() {
    turkey.gobble();
  }

  public void fly() {
    for (int i = 0; i < 5; i++) {
      turkey.fly();
    }
  }
}


public class Adapter {

  // javac Adapter.java && java Adapter
  public static void main(String[] args) {
    Duck duck = new MallardDuck();
    duck.quack();
    duck.fly();

    Duck turkey = new TurkeyAdapter(new WildTurkey());
    turkey.quack();
    turkey.fly();
  }
}