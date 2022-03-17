public class PrimitiveTypes {

  // $ javac PrimitiveTypes.java && java PrimitiveTypes
  public static void main(String[] args) {
    
    // if, else and else if
    int i = 2;

    if (i == 0) {
      System.out.println("zero");
    } else if (i % 2 == 0) {
      System.out.println("even");
    } else {
      System.out.println("odd");
    }

    // switch on byte, short, int, char, String or enum
    char c = 'y';

    switch (c) {
      case 'y':  // fallthrough
      case 'Y':
        System.out.println("yes!");
        break;
      case 'n':
      case 'N':
        System.out.println("no!");
        break;
      default:
        System.out.println("huh?");
        break;
    }


  }
}

