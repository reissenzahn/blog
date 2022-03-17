

// the var keyword can be used to declare a local variable while allowing the compiler to infer the correct type from the assignment of the variable

public class TypeInference {

  public static void main(String[] args) {

  }
}


String text = "Hello Java 9";

Now you can replace String with var. The compiler infers the correct type from the assignment of the variable. In this case text is of type String:

var text = "Hello Java 10";
Variables declared with var are still statically typed. You cannot reassign incompatible types to such variables. This code snippet does not compile:

var text = "Hello Java 11";
text = 23;  // Incompatible types
You can also use final in conjunction with var to forbid reassigning the variable with another value:

final var text = "Banana";
text = "Joe";   // Cannot assign a value to final variable 'text'
Also var is not allowed when the compiler is incapable of infering the correct type of the variable. All of the following code samples result in compiler errors:

// Cannot infer type:
var a;
var nothing = null;
var lambda = () -> System.out.println("Pity!");
var method = this::someMethod;
Local variable type inference really shines with generics involved. In the next example current has a rather verbose type of Map<String, List<Integer>> which can be reduced to a single var keyword, saving you from typing a lot of boilerplate:

var myList = new ArrayList<Map<String, List<Integer>>>();

for (var current : myList) {
    // current is infered to type: Map<String, List<Integer>>
    System.out.println(current);
}
As of Java 11 the var keyword is also allowed for lambda parameters which enables you to add annotations to those parameters:

Predicate<String> predicate = (@Nullable var a) -> true;
Tip: In Intellij IDEA you can hover on a variable while holding CMD/CTRL to reveal the infered type of the variable (for keyboard junkies press CTRL + J).




Collections#
Collections such as List, Set and Map have been extended with new methods. List.of created a new immutable list from the given arguments. List.copyOf creates an immutable copy of the list.

var list = List.of("A", "B", "C");
var copy = List.copyOf(list);
System.out.println(list == copy);   // true
Because list is already immutable there’s no practical need to actually create a copy of the list-instance, therefore list and copy are the same instance. However if you copy a mutable list, copy is indeed a new instance so it’s garanteed there’s no side-effects when mutating the original list:

var list = new ArrayList<String>();
var copy = List.copyOf(list);
System.out.println(list == copy);   // false
When creating immutable maps you don’t have to create map entries yourself but instead pass keys and values as alternating arguments:

var map = Map.of("A", 1, "B", 2);
System.out.println(map);    // {B=2, A=1}
Immutable collections in Java 11 still use the same interfaces from the old Collection API. However if you try to modify an immutable collection by adding or removing elements, a java.lang.UnsupportedOperationException is thrown. Luckily Intellij IDEA warns via an inspection if you try to mutate immutable collections.

Streams#
Streams were introduced in Java 8 and now receive three new methods. Stream.ofNullable constructs a stream from a single element:

Stream.ofNullable(null)
    .count()   // 0
The methods dropWhile and takeWhile both accept a predicate to determine which elements to abandon from the stream:

Stream.of(1, 2, 3, 2, 1)
    .dropWhile(n -> n < 3)
    .collect(Collectors.toList());  // [3, 2, 1]

Stream.of(1, 2, 3, 2, 1)
    .takeWhile(n -> n < 3)
    .collect(Collectors.toList());  // [1, 2]
If you’re not yet familiar with Streams you should read my Java 8 Streams Tutorial.


InputStreams#
Last but not least InputStream finally gets a super useful method to transfer data to an OutputStream, a usecase that’s very common when working with streams of raw data.

var classLoader = ClassLoader.getSystemClassLoader();
var inputStream = classLoader.getResourceAsStream("myFile.txt");
var tempFile = File.createTempFile("myFileCopy", "txt");
try (var outputStream = new FileOutputStream(tempFile)) {
    inputStream.transferTo(outputStream);
}