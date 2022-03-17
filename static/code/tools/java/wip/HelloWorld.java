public class HelloWorld {

  // $ javac HelloWorld.java && java HelloWorld arg1 arg2 arg3
  public static void main(String[] args) {
    System.out.println("Hello, Java!");

    for (String arg : args) {
      System.out.println(arg);
    }
  }
}