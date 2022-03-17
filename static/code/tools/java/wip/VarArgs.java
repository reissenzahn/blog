// a vararg parameter must be the last parameter
class VarArgs {

  public static int max(int first, int... rest) {
    int max = first;
  
    for (int i : rest) {
      if (i > max) max = i;
    }
  
    return max;
  }

  public static void main(String[] args) {
    Example.max(0);
    Example.max(1, 2);
    Example.max(16, 8, 4, 2, 1);
  }
}



Variable-Length Argument Lists
Methods may be declared to accept, and may be invoked with, variable numbers of
arguments. Such methods are commonly known as varargs methods. The “print for‐
matted” method System.out.printf() as well as the related format() methods of
String use varargs, as do a number of important methods from the Reflection API
of java.lang.reflect.
To declare a variable-length argument list, follow the type of the last argument to
the method with an ellipsis (...), indicating that this last argument can be repeated
zero or more times. For example:
public static int max(int first, int... rest) {
 /* body omitted for now */
}
Varargs methods are handled purely by the compiler. They operate by converting
the variable number of arguments into an array. To the Java runtime, the max()
method is indistinguishable from this one:
public static int max(int first, int[] rest) {
 /* body omitted for now */
}
To convert a varargs signature to the “real” signature, simply replace ... with [ ].
Remember that only one ellipsis can appear in a parameter list, and it may only
appear on the last parameter in the list.
Let’s flesh out the max() example a little:
public static int max(int first, int... rest) {
 int max = first;
 for(int i : rest) { // legal because rest is actually an array
 if (i > max) max = i;
 }
 return max;
}
This max() method is declared with two arguments. The first is just a regular int
value. The second, however, may be repeated zero or more times. All of the follow‐
ing are legal invocations of max():
max(0)
max(1, 2)
max(16, 8, 4, 2, 1)
Java Syntax
Methods | 77
Because varargs methods are compiled into methods that expect an array of argu‐
ments, invocations of those methods are compiled to include code that creates and
initializes such an array. So the call max(1,2,3) is compiled to this:
max(1, new int[] { 2, 3 })
In fact, if you already have method arguments stored in an array, it is perfectly legal
for you to pass them to the method that way, instead of writing them out individu‐
ally. You can treat any ... argument as if it were declared as an array. The converse
is not true, however: you can only use varargs method invocation syntax when the
method is actually declared as a varargs method using an ellipsis.