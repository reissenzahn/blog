class GumballMachine {
  
  private State noQuarterState;
  private State hasQuarterState;
  private State soldState;
  private State soldOutState;
  private State winnerState;

  private State state;  // current state
  private int count;

  public GumballMachine(int count) {
    this.count = count;

    noQuarterState = new NoQuarterState(this);
    hasQuarterState = new HasQuarterState(this);
    soldState = new SoldState(this);
    soldOutState = new SoldOutState(this);

    if (count > 0) {
      state = noQuarterState;
    } else {
      state = soldOutState;
    }
  }

  // provide a method to allow state objects to transition the machine to a different state
  public void setState(State state) {
    this.state = state;
  }

  // actions delegate to the current state
  public void insertQuarter() {
    state.insertQuarter();
  }

  public void ejectQuarter() {
    state.ejectQuarter();
  }

  // notice that dispense is an internal action
  public void turnCrank() {
    state.turnCrank();
    state.dispense();
  }

  public void releaseBall() {
    System.out.println("A gumball comes rolling out the slot.");
    if (count != 0) {
      count--;
    }
  }

  public int getCount() {
    return count;
  }

  public State getSoldOutState() {
    return soldOutState;
  }

  public State getNoQuarterState() {
    return noQuarterState;
  }

  public State getHasQuarterState() {
    return hasQuarterState;
  }

  public State getSoldState() {
    return soldState;
  }

  public State getWinnerState() {
    return winnerState;
  }
}


// there is an interface for all states
interface State {

  void insertQuarter();
  void ejectQuarter();
  void turnCrank();
  void dispense();
}

class NoQuarterState implements State {

  private GumballMachine gumballMachine;

  public NoQuarterState(GumballMachine gumballMachine) {
    this.gumballMachine = gumballMachine;
  }

  public void insertQuarter() {
    System.out.println("You inserted a quarter.");
    gumballMachine.setState(gumballMachine.getHasQuarterState());
  }

  public void ejectQuarter() {
    System.out.println("You haven't inserted a quarter.");
  }

  public void turnCrank() {
    System.out.println("You turned, but there's no quarter.");
  }

  public void dispense() {
    System.out.println("You need to pay first.");
  }
}

class HasQuarterState implements State {

  private GumballMachine gumballMachine;
  private Random randomWinner = new Random(System.currentTimeMillis());

  public HasQuarterState(GumballMachine gumballMachine) {
    this.gumballMachine = gumballMachine;
  }

  public void insertQuarter() {
    System.out.println("You can't insert another quarter.");
  }

  public void ejectQuarter() {
    System.out.println("Quarter returned.");
    gumballMachine.setState(gumballMachine.getNoQuarterState());
  }

  public void turnCrank() {
    System.out.println("You turned the crank...");

    int winner = randomWinner.nextInt(10);

    if ((winner == 0) && (gumballMachine.getCount() > 1)) {
      gumballMachine.setState(gumballMachine.getWinnerState());
    } else {
      gumballMachine.setState(gumballMachine.getSoldState());
    }
  }

  public void dispense() {
    System.out.println("No gumball dispensed.");
  }
}

class SoldState implements State {

  private GumballMachine gumballMachine;

  public SoldState(GumballMachine gumballMachine) {
    this.gumballMachine = gumballMachine;
  }

  public void insertQuarter() {
    System.out.println("Please wait, we're already giving you a gumball.");
  }

  public void ejectQuarter() {
    System.out.println("Sorry, you already turned the crank.");
  }

  public void turnCrank() {
    System.out.println("Turning twice doesn't get you another gumball.");
  }

  public void dispense() {
    gumballMachine.releaseBall();
    
    if (gumballMachine.getCount() > 0) {
      gumballMachine.setState(gumballMachine.getNoQuarterState());
    } else {
      System.out.println("Out of gumballs!");
      gumballMachine.setState(gumballMachine.getSoldOutState());
    }
  }
}

class SoldOutState implements State {

  private GumballMachine gumballMachine;

  public SoldOutState(GumballMachine gumballMachine) {
    this.gumballMachine = gumballMachine;
  }

  public void insertQuarter() {
    System.out.println("You can't insert a quarter, the machine is sold out.");
  }
  
  public void ejectQuarter() {
  System.out.println("You can't eject, you haven't inserted a quarter yet.");
  }

  public void turnCrank() {
    System.out.println("You turned, but there are no gumballs.");
  }

  public void dispense() {
    System.out.println("No gumball dispensed.");
  }
}

class WinnerState implements State {

  private GumballMachine gumballMachine;

  public WinnerState(GumballMachine gumballMachine) {
    this.gumballMachine = gumballMachine;
  }

  public void insertQuarter() {
    System.out.println("Please wait, we're already giving you a gumball.");
  }

  public void ejectQuarter() {
    System.out.println("Sorry, you already turned the crank.");
  }

  public void turnCrank() {
    System.out.println("Turning twice doesn't get you another gumball.");
  }

  public void dispense() {
    gumballMachine.releaseBall();

    if (gumballMachine.getCount() == 0) {
      gumballMachine.setState(gumballMachine.getSoldOutState());
    } else {
      gumballMachine.releaseBall();
      System.out.println("YOU'RE A WINNER! You got two gumballs for your quarter");
      if (gumballMachine.getCount() > 0) {
        gumballMachine.setState(gumballMachine.getNoQuarterState());
      } else {
        System.out.println("Oops, out of gumballs!");
        gumballMachine.setState(gumballMachine.getSoldOutState());
      }
    }
  }
}


public class State {

  public static void main(String[] args) {
    GumballMachine gumballMachine = new GumballMachine(5);

    gumballMachine.insertQuarter();
    gumballMachine.ejectQuarter();
    gumballMachine.insertQuarter();
    gumballMachine.turnCrank();
  }
}