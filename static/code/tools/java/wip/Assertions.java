public class Assertions {

  // $ javac Assertions.java && java -ea Assertions
  public static void main(String[] args) {
    // to enable exceptions we must use the -ea option when invoking the java command

    // an assertion throws an AssertionError if an expression evaluates to false
    assert 2 + 2 == 4;

    // we can specify an error code or message to be passed to the AssertionError constructor
    assert 2 + 2 == 5 : "Something is very wrong!";
  }
}