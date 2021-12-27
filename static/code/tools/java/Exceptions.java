
// an exception is a signal that indicates some sort of exceptional condition has occurred

// to throw an exception is to signal an exception condition; to catch an exception is to handle it

// an exception is represented by an object that is either an instance of Exception or an instance of a subclass of Exception

// an exception can either be checked or unchecked

if (j == 0) {
  throw new ArithmeticException("divide by zero!");
}


// a thrown exception propagates up the call stack until it is caught or causes the program to exit as a result of not being handled



Each catch block can handle more than one different exception—to
indicate that a catch block should handle multiple exceptions, we use the | symbol
to separate the different exceptions a catch block should handle.

try {

} catch (SomeException e) {

} catch (AnotherException | YetAnotherException e) {

} finally {
  // cleanup code that is alway run
}



// any type that a catch block wishes to handle must be some subclass of Throwable

looks for a catch clause with an argument that matches the same type as the exception object or a superclass of that
type. The interpreter invokes the first such catch clause it finds. 

the finally clause is guaranteed to be executed regardless of how the code in the try block completes



// try-with-resources provides a mechanism for automatically closing resources that require cleanup

// objects requiring cleanup are scope to the try block and are cleaned up automatically no matter how this block is exited

try (InputStream is = new FileInputStream("example.txt")) {
 // ... process the file
}


// Checked and Unchecked Exceptions

// there are two types of exceptions: checked and unchecked exceptions

// exceptions are Throwable objects and these fall into two main categories specified by the Error (unchecked) and Exception (checked) subclasses; there is also a subclass of Exception called RuntimeException and any subclass of RuntimeException is also unchecked

// checked exceptions arise in specific, well-defined circumstances, and very often are conditions from which the application may be able to partially or fully recover (e.g. FileNotFoundException)

// unchecked exceptions arise from failures that cannot easily be predicted or anticipated, due to such things as runtime conditions or abuse of library code (e.g. NullPointerException)

// practically any method can throw an unchecked exception at essentially any time and recovery from an unchecked exception is usually very difficult due to their sheer unpredictability


// if you write a method that throws a checked exception then you must use a throws clause to declare the exception in the method signature

// further, if a method calls a method that can throw a checked exception then you must either handle that exception or use throws to declare that the method can also throw that exception


public static estimateHomepageSize(String host) throws IOException {
 URL url = new URL("htp://"+ host +"/");
 try (InputStream in = url.openStream()) {
 return in.available();
 }
}
