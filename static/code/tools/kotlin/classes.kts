

// class declarations

// a class declaration consists of the class name, class header and class body
class Person {
}

// both the header and the body are optional; if the class has no body, the curly braces can be omitted
class Empty



// constructors

// a class can have a primary constructor and one or more secondary constructors

// the primary constructor is a part of the class header
class Person constructor(firstName: String) {
}

// if the primary constructor does not have any annotations or visibility modifiers then the constructor keyword can be omitted
class Person(firstName: String) {
}

// the primary constructor cannot contain any code though initialization code can be placed in initializer blocks

// the initializer blocks are executed during the initialization of an instance in the same order as they appear in the class body interleaved with the property initializers

class InitOrderDemo(name: String) {
  val firstProperty = "First property: $name".also(::println)
    
  init {
    println("First initializer block that prints $name")
  }
    
  val secondProperty = "Second property: ${name.length}".also(::println)
    
  init {
    println("Second initializer block that prints ${name.length}")
  }
}

// primary constructor parameters can be used in the initializer blocks and in property initializers declared in the class body
class Customer(name: String) {
  val customerKey = name.uppercase()
}

// a concise syntax can be used for declaring properties and initializing them from the primary constructor
class Person(val firstName: String, val lastName: String, var age: Int)

// such declarations can also include default values of the class properties
class Person(val firstName: String, val lastName: String, var isEmployed: Boolean = true)

//you can use a trailing comma when you declare class properties
class Person(
    val firstName: String,
    val lastName: String,
    var age: Int, // trailing comma
) {
}

// much like regular properties, properties declared in the primary constructor can be mutable (var) or read-only (val)

// if the constructor has annotations or visibility modifiers, the constructor keyword is required and the modifiers go before it

class Customer public @Inject constructor(name: String) { /*...*/ }




// a class can also declare secondary constructors, which are prefixed with constructor
class Person(val pets: MutableList<Pet> = mutableListOf())

class Pet {
  constructor(owner: Person) {
    owner.pets.add(this)
  }
}

// if the class has a primary constructor then each secondary constructor needs to delegate to the primary constructor (either directly or indirectly through another secondary constructor) using the this keyword
class Person(val name: String) {
  val children: MutableList<Person> = mutableListOf()
  
  constructor(name: String, parent: Person) : this(name) {
    parent.children.add(this)
  }
}

// Code in initializer blocks effectively becomes part of the primary constructor. Delegation to the primary constructor happens as the first statement of a secondary constructor, so the code in all initializer blocks and property initializers is executed before the body of the secondary constructor.

// Even if the class has no primary constructor, the delegation still happens implicitly, and the initializer blocks are still executed:

class Constructors {
    init {
        println("Init block")
    }
​
    constructor(i: Int) {
        println("Constructor $i")
    }
}

If a non-abstract class does not declare any constructors (primary or secondary), it will have a generated primary constructor with no arguments. The visibility of the constructor will be public.

If you don't want your class to have a public constructor, declare an empty primary constructor with non-default visibility:

class DontCreateMe private constructor () { /*...*/ }
Copied!
On the JVM, if all of the primary constructor parameters have default values, the compiler will generate an additional parameterless constructor which will use the default values. This makes it easier to use Kotlin with libraries such as Jackson or JPA that create class instances through parameterless constructors.

class Customer(val customerName: String = "")
Copied!
Creating instances of classes﻿
To create an instance of a class, call the constructor as if it were a regular function:

val invoice = Invoice()

val customer = Customer("Joe Smith")
Copied!
Kotlin does not have a new keyword.

The process of creating instances of nested, inner, and anonymous inner classes is described in Nested classes.

Class members﻿
Classes can contain:

Constructors and initializer blocks

Functions

Properties

Nested and inner classes

Object declarations



