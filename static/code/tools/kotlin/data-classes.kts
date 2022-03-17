

// 

It is not unusual to create classes whose main purpose is to hold data

data class User(val name: String, val age: Int)
Copied!
The compiler automatically derives the following members from all properties declared in the primary constructor:

equals()/hashCode() pair

toString() of the form "User(name=John, age=42)"

componentN() functions corresponding to the properties in their order of declaration.

copy() function (see below).

To ensure consistency and meaningful behavior of the generated code, data classes have to fulfill the following requirements:

The primary constructor needs to have at least one parameter.

All primary constructor parameters need to be marked as val or var.

Data classes cannot be abstract, open, sealed, or inner.

Additionally, the generation of data class members follows these rules with regard to the members’ inheritance:

If there are explicit implementations of equals(), hashCode(), or toString() in the data class body or final implementations in a superclass, then these functions are not generated, and the existing implementations are used.

If a supertype has componentN() functions that are open and return compatible types, the corresponding functions are generated for the data class and override those of the supertype. If the functions of the supertype cannot be overridden due to incompatible signatures or due to their being final, an error is reported.

Providing explicit implementations for the componentN() and copy() functions is not allowed.

Data classes may extend other classes (see Sealed classes for examples).

On the JVM, if the generated class needs to have a parameterless constructor, default values for the properties have to be specified (see Constructors).

data class User(val name: String = "", val age: Int = 0)
Copied!
Properties declared in the class body﻿
The compiler only uses the properties defined inside the primary constructor for the automatically generated functions. To exclude a property from the generated implementations, declare it inside the class body:

data class Person(val name: String) {
    var age: Int = 0
}
Copied!
Only the property name will be used inside the toString(), equals(), hashCode(), and copy() implementations, and there will only be one component function component1(). While two Person objects can have different ages, they will be treated as equal.

val person1 = Person("John")
val person2 = Person("John")
person1.age = 10
person2.age = 20
Open in Playground →
Target: JVM
Running on v.1.6.10
Copying﻿
Use the copy() function to copy an object, allowing you to alter some of its properties while keeping the rest unchanged. The implementation of this function for the User class above would be as follows:

fun copy(name: String = this.name, age: Int = this.age) = User(name, age)
Copied!
You can then write the following:

val jack = User(name = "Jack", age = 1)
val olderJack = jack.copy(age = 2)
Copied!
Data classes and destructuring declarations﻿
Component functions generated for data classes make it possible to use them in destructuring declarations:

val jane = User("Jane", 35)
val (name, age) = jane
println("$name, $age years of age") // prints "Jane, 35 years of age"
Copied!
Standard data classes﻿
The standard library provides the Pair and Triple classes. In most cases, though, named data classes are a better design choice because they make the code more readable by providing meaningful names for the properties.

Data classes
Properties declared in the class body
Copying
Data classes and destructuring declarations
Standard data classes

