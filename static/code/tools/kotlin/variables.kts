

// read-only local variables are defined using the val keyword and can be assigned a value only once

val a: Int = 1  // immediate assignment
val b = 2   // `Int` type is inferred
val c: Int  // Type required when no initializer is provided
c = 3       // deferred assignment

Variables that can be reassigned use the var keyword.

var x = 5 // `Int` type is inferred
x += 1


You can declare variables at the top level.

val PI = 3.14
var x = 0

fun incrementX() { 
    x += 1 
}
