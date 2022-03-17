
// a functional interface must contain exactly one abstract method declaration (though it can also contain default methods)

// the @FunctionalInterface annotation causes the compiler to throw an error if a more than one abstract method is present
@FunctionalInterface
interface Converter<F, T> {
  T convert(F from);
}

// various built-in functional interfaces are available


public class FunctionalInterfaces {
  public static void main(String[] args) {
    Converter<String, Integer> converter = (from) -> Integer.valueOf(from);
    Integer i = converter.convert("123");
    System.out.println(i);  // 123


    // predicates are boolean-valued functions of one argument
    Predicate<String> predicate = (s) -> s.length() > 0;

    // various default methods are provided for composing predicates
    predicate.test("hello");
    predicate.negate().test("hello");
    predicate.test("hello")
  }
}


Predicate<Boolean> nonNull = Objects::nonNull;
Predicate<Boolean> isNull = Objects::isNull;

Predicate<String> isEmpty = String::isEmpty;
Predicate<String> isNotEmpty = isEmpty.negate();


// functions accept one argument and produce a result



// default methods are provided to chain multiple functions together
Function<String, Integer> toInteger = Integer::valueOf;
Function<String, String> backToString = toInteger.andThen(String::valueOf);

backToString.apply("123");     // "123"


// a Supplier produces a result of a given type
Supplier<Person> personSupplier = Person::new;
personSupplier.get();   // new Person


// a Consumer represents operations to be performed on a single input argument

Consumer<Person> greeter = (p) -> System.out.println("Hello, " + p.firstName);
greeter.accept(new Person("Luke", "Skywalker"));



// 

Comparator<Person> comparator = (p1, p2) -> p1.firstName.compareTo(p2.firstName);

Person p1 = new Person("John", "Doe");
Person p2 = new Person("Alice", "Wonderland");

comparator.compare(p1, p2);             // > 0
comparator.reversed().compare(p1, p2);  // < 0




