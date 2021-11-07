

# Creational


## Singleton


```java
// eager initialization

```

- [Singleton Pattern ~ OODesign.com](https://www.oodesign.com/singleton-pattern.html)
- [Singleton Considered Stupid ~ Steve Yegge](https://sites.google.com/site/steveyegge2/singleton-considered-stupid)





## Factory Method

The Factory Method pattern is a creational pattern that provides an interface for creating objects in a superclass but allows subclasses to alter the type of objects that will be created.




# Behavioral

## Strategy

The strategy pattern is a behavioral design pattern that lets you define a set of algorithms in separate classes and make their objects interchangeable.

The strategy pattern allows you to take a class that does something specific in a lot of different ways and extract all of these algorithms into separate classes called strategies.

The original class, called the context, must have a field for storing a reference to one of the strategies. The context delegates the work to a linked strategy object instead of executing it on its own.

The Context maintains a reference to one of the concrete strategies and communicates with this object only via the strategy interface.

The Strategy interface is common to all concrete strategies. It declares a method the context uses to execute a strategy.

Concrete Strategies implement different variations of an algorithm the context uses.

The context calls the execution method on the linked strategy object each time it needs to run the algorithm. The context doesn’t know what type of strategy it works with or how the algorithm is executed.

The Client creates a specific strategy object and passes it to the context. The context exposes a setter which lets clients replace the strategy associated with the context at runtime.


```java
interface Strategy {

  void algorithm();
}

class SomeStrategy implements Strategy {

  public void algorithm() {
    System.out.println("some algorithm");
  }
}

class AnotherStrategy implements Strategy {

  public void algorithm() {
    System.out.println("another algorithm");
  }
}

class Context {

  private Strategy strategy;

  public Context() {
  }

  public void setStrategy(Strategy strategy) {
    this.strategy = strategy;
  }

  public void operation() {
    strategy.algorithm();
  }
}

public class Client {

  public static void main(String[] args) {

    Context context = new Context();

    context.setStrategy(new SomeStrategy());
    context.operation();

    context.setStrategy(new AnotherStrategy());
    context.operation();
  }
}
```





---
title: "Design Patterns"
date: 2020-09-29
draft: false
---

<ul>
  <li>
    <a href="#introduction">Introduction</a>
  </li>
  <li>
    <a href="#resources">Resources</a>
  </li>
</ul>


<!-- START: Introduction -->

<h3 id="introduction">Introduction</h3>

<p>

</p>

<!-- 
  In software engineering, a software design pattern is a general, reusable solution to a commonly occurring problem within a given context in software design. It is not a finished design that can be transformed directly into source or machine code. Rather, it is a description or template for how to solve a problem that can be used in many different situations. Design patterns are formalized best practices that the programmer can use to solve common problems when designing an application or system.
-->

<!-- 
  Open-closed principle: Classes should be open for extension, but closed for modification.
-->

<!-- END: Introduction -->



<h3 id="builder">Builder</h3>

<p>

</p>

<!-- 
  Builder is a creational design pattern that lets you construct complex objects step by step. The pattern allows you to produce different types and representations of an object using the same construction code.

  Imagine a complex object that requires laborious, step-by-step initialization of many fields and nested objects. Such initialization code is usually buried inside a monstrous constructor with lots of parameters. Or even worse: scattered all over the client code.

  The Builder pattern suggests that you extract the object construction code out of its own class and move it to separate objects called builders.

  The pattern organizes object construction into a set of steps. To create an object, you execute a series of these steps on a builder object. The important part is that you don’t need to call all of the steps. You can call only those steps that are necessary for producing a particular configuration of an object.

  In this case, you can create several different builder classes that implement the same set of building steps, but in a different manner. Then you can use these builders in the construction process (i.e., an ordered set of calls to the building steps) to produce different kinds of objects.

  You can go further and extract a series of calls to the builder steps you use to construct a product into a separate class called director. The director class defines the order in which to execute the building steps, while the builder provides the implementation for those steps.

  Having a director class in your program isn’t strictly necessary. You can always call the building steps in a specific order directly from the client code. However, the director class might be a good place to put various construction routines so you can reuse them across your program.

  In addition, the director class completely hides the details of product construction from the client code. The client only needs to associate a builder with a director, launch the construction with the director, and get the result from the builder.
-->




<!-- START: Singleton -->

<h3 id="singleton">Singleton</h3>

<p>
  The singleton pattern is a creational design pattern that ensures that a class has only one instance while providing a global access point to that instance. This can be achieved by making the default constructor of a class private and providing a static method that returns the single instance.
</p>

<figure>
  <img src="/img/design-patterns/singleton.svg" style="max-width: 206px; width: 100%; height: auto">
  <figcaption>
    <i>figure 1</i>
  </figcaption>
</figure>

<!--
@startuml name

skinparam classAttributeIconSize 0

hide empty members

class Singleton {
    {static} - instance: Singleton
    - Singleton()
    {static} + getInstance(): Singleton
}

Singleton <-- Singleton
@enduml
-->

<ul>
  <li>
    The constructor is declared private to prevent the class from being instantiated outside the class.
  </li>
  <li>
    A static <code>instance</code> variable is used to hold the single instance of the class.
  </li>
  <li>
    A static <code>getInstance</code> method is provided to return a reference to the singleton instance. 
  </li>
</ul>

<pre><code class="java">public final class Database {

  // the instance is stored as a private static variable
  private static final Database instance = new Database();

  // the constructor is declared private to prevent instantiation of the singleton from other classes
  private Database() {
  }

  // a static method is provided for returning a reference to the instance of the singleton
  public static Database getInstance() {
    return instance;
  }
}

public class Example {

  public static void main(String[] args) {
    Database database = Database.getInstance();
    // ...
  }
}</code></pre>

<!-- END: Singleton -->



<!-- START: Factory Method -->

<h3 id="factory-method">Factory Method</h3>

<p>
  The Factory Method is a creational design pattern that consists of defining an interface for creating an object in a superclass but allowing subclasses to decide which class to instantiate.
</p>

<figure>
  <img src="/img/design-patterns/factory-method.svg" style="max-width: 579px; width: 100%; height: auto">
  <figcaption>
    <i>figure 2</i>
  </figcaption>
</figure>

<!-- 
@startuml name

skinparam classAttributeIconSize 0

hide empty members

abstract class Creator {
    + operation()
    {abstract} + createProduct(): Product
}
Product <.. Creator

class SomeCreator {
    + createProduct(): Product
}
Creator <|-- SomeCreator

class AnotherCreator {
    + createProduct(): Product
}
Creator <|-- AnotherCreator

interface Product

class SomeProduct
Product <|.. SomeProduct

class AnotherProduct
Product <|.. AnotherProduct

@enduml
-->

<ul>
  <li>
    The <code>Product</code> interface is implemented by various concrete classes.
  </li>
  <li>
    The <code>Creator</code> class requires an object that implements the <code>Product</code> interface.
  </li>
  <li>
    The <code>Creator</code> class declares an abstract <code>createProduct()</code> method which it uses to create a product object.
  </li>
  <li>
    Concrete subclasses of <code>Creator</code> are responsible for overriding the <code>createProduct()</code> method in which they choose which class to instantiate.
  </li>
</ul>

<pre><code class="java">
</code></pre>

<!-- END: Factory Method -->





<!-- START: Proxy -->

<h3 id="proxy">Proxy</h3>

<p>
  The Proxy Pattern is a structural design pattern that provides a proxy object that functions as an interface for a certain object. The proxy controls access to the original object and may perform some operation before or after the request gets through to the original object.
</p>

<figure>
  <img src="/img/design-patterns/proxy.svg" style="max-width: 266px; width: 100%; height: auto">
  <figcaption>
    <i>figure 4</i>
  </figcaption>
</figure>

<ul>
  <li>
    The <code>Subject</code> interface declares the interface that the proxy must implement to disguise itself as a <code>ConcreteSubject</code> object.
  </li>
  <li>
    The <code>Proxy</code> class holds a reference to a <code>ConcreteSubject</code> which it passes requests to after it finishes its processing.
  </li>
</ul>

<!-- 
@startuml name

skinparam classAttributeIconSize 0

hide empty members

interface Subject {
    + operation()
}
Subject <|.. ConcreteSubject
Subject <|.. Proxy

class ConcreteSubject {
    + operation();
}

class Proxy {
    - subject: ConcreteSubject
    + Proxy(subject: ConcreteSubject)
    + operation()
}
Proxy o.. ConcreteSubject

@enduml
-->

<pre><code class="java"></code></pre>

<!-- END: Proxy -->


<!-- START: Strategy -->



<!-- END: Strategy -->



<!-- START: Visitor -->

<h3 id="visitor">Visitor</h3>

<p>
  The Visitor Pattern is a behavioral design pattern that separates algorithms from the objects on which they operate.
</p>

<!-- 
  The Visitor pattern suggests that you place the new behavior into a separate class called visitor, instead of trying to integrate it into existing classes.
  
  The original object that had to perform the behavior is now passed to one of the visitor’s methods as an argument, providing the method access to all necessary data contained within the object.

  The visitor class may define not one, but a set of methods, each of which could take arguments of different types.

  It uses a technique called Double Dispatch, which helps to execute the proper method on an object without cumbersome conditionals. Instead of letting the client select a proper version of the method to call, how about we delegate this choice to objects we’re passing to the visitor as an argument? Since the objects know their own classes, they’ll be able to pick a proper method on the visitor less awkwardly. They “accept” a visitor and tell it what visiting method should be executed.
-->

<figure>
  <img src="/img/design-patterns/visitor.svg" style="max-width: 544px; width: 100%; height: auto">
  <figcaption>
    <i>figure 4</i>
  </figcaption>
</figure>

<!-- 
@startuml name

skinparam classAttributeIconSize 0

hide empty members

interface Element {
    + accept(visitor: Visitor)
}
Element <|.. SomeElement
Element <|.. AnotherElement

class SomeElement {
    + someFeature()
    + accept(visitor: Visitor)
}
SomeElement <.. Visitor

class AnotherElement {
    + anotherFeature()
    + accept(visitor: Visitor)
}
AnotherElement <.. Visitor

interface Visitor {
    + visit(someElement: SomeElement)
    + visit(anotherElement: AnotherElement)
}
Visitor <|.. SomeVisitor
Visitor <|.. AnotherVisitor
Visitor <.. Element

class SomeVisitor {
    + visit(someElement: SomeElement)
    + visit(anotherElement: AnotherElement)
}

class AnotherVisitor {
    + visit(someElement: SomeElement)
    + visit(anotherElement: AnotherElement)
}

@enduml
-->

<pre><code class="java">interface CreditCard {
  
  String getName();
  void accept(OfferVisitor visitor);
}

class BronzeCreditCard implements CreditCard {

  public String getName() {
    return "bronze";
  }
  
  public void accept(OfferVisitor visitor) {
    visitor.visitBronzeCreditCard(this);
  }
}

class SilverCreditCard implements CreditCard {

  public String getName() {
    return "silver";
  }

  public void accept(OfferVisitor visitor) {
    visitor.visitSilverCreditCard(this);
  }
}

class GoldCreditCard implements CreditCard {

  public String getName() {
    return "gold";
  }

  public void accept(OfferVisitor visitor) {
    visitor.visitGoldCreditCard(this);
  }
}

interface OfferVisitor {

  void visitBronzeCreditCard(BronzeCreditCard bronzeCreditCard);
  void visitSilverCreditCard(SilverCreditCard silverCreditCard);
  void visitGoldCreditCard(GoldCreditCard goldCreditCard);
}

class GasOfferVisitor implements OfferVisitor {
  public void visitBronzeCreditCard(BronzeCreditCard bronzeCreditCard) {
    System.out.println("Computing gas offer for bronze credit card");
  }

  public void visitSilverCreditCard(SilverCreditCard silverCreditCard) {
    System.out.println("Computing gas offer for bronze credit card");
  }

  public void visitGoldCreditCard(GoldCreditCard goldCreditCard) {
    System.out.println("Computing gas offer for bronze credit card");
  }
}

class HotelOfferVisitor implements OfferVisitor {
  public void visitBronzeCreditCard(BronzeCreditCard bronzeCreditCard) {
    System.out.println("Computing hotel offer for bronze credit card");
  }

  public void visitSilverCreditCard(SilverCreditCard silverCreditCard) {
    System.out.println("Computing hotel offer for bronze credit card");
  }

  public void visitGoldCreditCard(GoldCreditCard goldCreditCard) {
    System.out.println("Computing hotel offer for bronze credit card");
  }
}

public class Example {

  public static void main(String[] args) {

    CreditCard creditCard = new GoldCreditCard();
    OfferVisitor offerVisitor = new GasOfferVisitor();

    creditCard.accept(offerVisitor);
  }
}</code></pre>

<!-- 
  https://www.youtube.com/watch?v=TeZqKnC2gvA
-->

<!-- END: Visitor -->


<!-- START: Observer -->

<h3 id="observer">Observer</h3>


<figure>
  <img src="/img/design-patterns/observer.svg" style="max-width: 544px; width: 100%; height: auto">
  <figcaption>
    <i>figure 4</i>
  </figcaption>
</figure>

<!-- 
  @startuml name

  skinparam classAttributeIconSize 0

  hide empty members

  class Subject {
      - observers: []Observer
      
      registerObserver()
      removeObserver()
      notifyObservers()
      + operation()
  }
  Subject o.. Observer

  interface Observer {
      update()
  }
  Observer <|.. ConcreteObserverA
  Observer <|.. ConcreteObserverB

  class ConcreteObserverA {
      update()
  }


  class ConcreteObserverB {
      update()
  }

  @enduml
-->

<ul>
  <li>

  </li>
</ul>

<pre><code class="java"></code></pre>

<!-- END: Observer -->




<!-- START: Resources -->

<h3 id="resources">Resources</h3>

<ul>
  <li>
    <a href=""></a>
  </li>
</ul>


<!-- END: Resources -->




<!-- fill-opacity="0" -->