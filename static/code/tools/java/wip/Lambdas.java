


List<String> list = Arrays.asList("a", "b", "c", "d", "e");

// an anonymous class often feels verbose
Collections.sort(list, new Comparator<String>() {
    @Override
    public int compare(String a, String b) {
        return b.compareTo(a);
    }
});

// a lambda corresponds to a type specified by a functional interface and is matched to the abstract method declared in the interface

// a lambda provides a concise alternative to an anonymous class
Collections.sort(list, (String a, String b) -> {
  return b.compareTo(a);
});

// for one line method bodies you can skip the braces and return keyword
Collections.sort(list, (String a, String b) -> b.compareTo(a));

// type parameters can also be omitted as they can be inferred
Collections.sort(list, (a, b) -> b.compareTo(a));





Lambda Scopes#

// lambda expressions can read final local variables from the outer scope
final int i = 1;
Converter<Integer, String> converter = (from) -> String.valueOf(from + num);
converter.convert(2);     // 3


Accessing outer scope variables from lambda expressions is very similar to anonymous objects. You can access final variables from the local outer scope as well as instance fields and static variables.

But different to anonymous objects the variable num does not have to be declared final. This code is also valid:

int num = 1;
Converter<Integer, String> stringConverter =
        (from) -> String.valueOf(from + num);

stringConverter.convert(2);     // 3
However num must be implicitly final for the code to compile. The following code does not compile:

int num = 1;
Converter<Integer, String> stringConverter =
        (from) -> String.valueOf(from + num);
num = 3;
Writing to num from within the lambda expression is also prohibited.

Accessing fields and static variables#
In constrast to local variables we have both read and write access to instance fields and static variables from within lambda expressions. This behaviour is well known from anonymous objects.

class Lambda4 {
    static int outerStaticNum;
    int outerNum;

    void testScopes() {
        Converter<Integer, String> stringConverter1 = (from) -> {
            outerNum = 23;
            return String.valueOf(from);
        };

        Converter<Integer, String> stringConverter2 = (from) -> {
            outerStaticNum = 72;
            return String.valueOf(from);
        };
    }
}

Accessing Default Interface Methods#

Remember the formula example from the first section? Interface Formula defines a default method sqrt which can be accessed from each formula instance including anonymous objects. This does not work with lambda expressions.

Default methods cannot be accessed from within lambda expressions. The following code does not compile:

Formula formula = (a) -> sqrt( a * 100);