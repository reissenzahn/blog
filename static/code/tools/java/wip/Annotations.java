Annotations in Java 8 are repeatable. Let’s dive directly into an example to figure that out.

First, we define a wrapper annotation which holds an array of the actual annotations:

@interface Hints {
    Hint[] value();
}

@Repeatable(Hints.class)
@interface Hint {
    String value();
}
Java 8 enables us to use multiple annotations of the same type by declaring the annotation @Repeatable.

Variant 1: Using the container annotation (old school)#
@Hints({@Hint("hint1"), @Hint("hint2")})
class Person {}
Variant 2: Using repeatable annotations (new school)#
@Hint("hint1")
@Hint("hint2")
class Person {}
Using variant 2 the java compiler implicitly sets up the @Hints annotation under the hood. That’s important for reading annotation informations via reflection.

Hint hint = Person.class.getAnnotation(Hint.class);
System.out.println(hint);                   // null

Hints hints1 = Person.class.getAnnotation(Hints.class);
System.out.println(hints1.value().length);  // 2

Hint[] hints2 = Person.class.getAnnotationsByType(Hint.class);
System.out.println(hints2.length);          // 2
Although we never declared the @Hints annotation on the Person class, it’s still readable via getAnnotation(Hints.class). However, the more convenient method is getAnnotationsByType which grants direct access to all annotated @Hint annotations.

Furthermore the usage of annotations in Java 8 is expanded to two new targets:

@Target({ElementType.TYPE_PARAMETER, ElementType.TYPE_USE})
@interface MyAnnotation {}