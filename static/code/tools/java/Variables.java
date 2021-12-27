
// a variable is a symbolic name for a location to store a value of a particular type

public class Variables {

  public static void main(String[] args) {
    // separate declaration and initialization
    int i;
    i = 1;
    System.out.println(i1);  // 1

    // declaration with initialization
    int j = 2;

    // multiple declaration
    float x, y, z;

    // a variable cannot be used until it has been initialized
    System.out.println(x);  // compile error

    // multiple declaration with initialization
    float u = 5, v = 6;

    // constant declaration
    final double PI;
    PI = 3.1415;

    // constant declaration with initialization
    final double E = 2.7182;

    // if a variable has an initialize then the var keyword can be use to infer the type
    var s = "hello";
  }
}
