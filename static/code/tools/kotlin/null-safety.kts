

// the type system distinguishes between references that can hold null (nullable references) and those that cannot (non-null references)

// a reference is non-null by default
var a: String = "abc"
a = null  // compile error

// notice that you can always call a method or access a property on a non-null reference and be guaranteed to cause a NullPointerException
val l = a.length


// to allow nulls you must declare a variable as a nullable type
var b: String? = "abc"
b = null
print(b)

// attempting to access a property on a nullable type will result in a compile error
var l = b.length  // compile error

// if the variable is immutable then we can explicitly check for null (immutable here means a local variable that is not modified between the check and its usage or it is a member val that has a backing field and is not overridable)
val l = if (b != null) b.length else -1

// alternatively we can use the safe call operator which returns null if the variable is null or the property if it is not null
println(b?.length)

// 

Safe calls are useful in chains. For example, Bob is an employee who may be assigned to a department (or not). That department may in turn have another employee as a department head. To obtain the name of Bob's department head (if there is one), you write the following:

bob?.department?.head?.name
Copied!
Such a chain returns null if any of the properties in it is null.

To perform a certain operation only for non-null values, you can use the safe call operator together with let:

val listWithNulls: List<String?> = listOf("Kotlin", null)
for (item in listWithNulls) {
    item?.let { println(it) } // prints Kotlin and ignores null
}
Open in Playground →
Target: JVM
Running on v.1.6.10
A safe call can also be placed on the left side of an assignment. Then, if one of the receivers in the safe calls chain is null, the assignment is skipped and the expression on the right is not evaluated at all:

// If either `person` or `person.department` is null, the function is not called:
person?.department?.head = managersPool.getManager()


Elvis operator
When you have a nullable reference, b, you can say "if b is not null, use it, otherwise use some non-null value":

val l: Int = if (b != null) b.length else -1
Copied!
Instead of writing the complete if expression, you can also express this with the Elvis operator ?::

val l = b?.length ?: -1
Copied!
If the expression to the left of ?: is not null, the Elvis operator returns it, otherwise it returns the expression to the right. Note that the expression on the right-hand side is evaluated only if the left-hand side is null.

Since throw and return are expressions in Kotlin, they can also be used on the right-hand side of the Elvis operator. This can be handy, for example, when checking function arguments:

fun foo(node: Node): String? {
    val parent = node.getParent() ?: return null
    val name = node.getName() ?: throw IllegalArgumentException("name expected")
    // ...
}
Copied!
The !! operator﻿
The third option is for NPE-lovers: the not-null assertion operator (!!) converts any value to a non-null type and throws an exception if the value is null. You can write b!!, and this will return a non-null value of b (for example, a String in our example) or throw an NPE if b is null:

val l = b!!.length
Copied!
Thus, if you want an NPE, you can have it, but you have to ask for it explicitly and it won’t appear out of the blue.

Safe casts﻿
Regular casts may result in a ClassCastException if the object is not of the target type. Another option is to use safe casts that return null if the attempt was not successful:

val aInt: Int? = a as? Int
Copied!
Collections of a nullable type﻿
If you have a collection of elements of a nullable type and want to filter non-null elements, you can do so by using filterNotNull:

val nullableList: List<Int?> = listOf(1, 2, null, 4)
val intList: List<Int> = nullableList.filterNotNull()