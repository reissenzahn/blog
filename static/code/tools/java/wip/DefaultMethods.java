
// an interface can have a non-abstract method implementations called default methods
interface Formula {
  double calculate(int a);

  default double sqrt(int a) {
    return Math.sqrt(a);
  }
}




public class DefaultInterfaceMethods {

  public static void main(String[] args) {
    
    // concrete classes do not have to provide implementations for default methods though they can be overridden
    Formula formula = new Formula() {
      @Override
      public double calculate(int a) {
        return sqrt(a * 100);
      }
    };

    formula.calculate(100);     // 100.0
    formula.sqrt(16);           // 4.0
  }
}


// default methods enable you to add new functionality to interfaces while ensuring compatibility with older implementations of those interfaces

// default methods enable you to add new functionality to existing interfaces and ensure binary compatibility with code written for older versions of those interfaces

