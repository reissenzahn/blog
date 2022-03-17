

// all exception classes inherit from Throwable and every exception has a message, a stack trace and an optional cause

To throw an exception object, use the throw expression:

// an exception is thrown using a throw expression
throw Exception("Error!")

// an exception is caught using a try...catch expression consisting of zero or more catch blocks and an optional finally block
try {

} catch (e: Exception) {

} finally {

}


try is an expression, which means it can have a return value:

val a: Int? = try { input.toInt() } catch (e: NumberFormatException) { null }


Copied!
The returned value of a try expression is either the last expression in the try block or the last expression in the catch block (or blocks). The contents of the finally block don't affect the result of the expression.



Checked exceptions﻿

// 

If you want to alert callers about possible exceptions when calling Kotlin code from Java, Swift, or Objective-C, you can use the @Throws annotation. Read more about using this annotation for Java and for Swift and Objective-C.








