
// an optional is a container for a value which may be null or non-null that provides methods for accessing the value while preventing a NullPointerException


Optional<String> something = Optional.of("hello");
Optional<String> nothing = Optional.of(null);

optional.isPresent();           // true
optional.get();                 // "bam"
optional.orElse("fallback");    // "bam"

optional.ifPresent((s) -> System.out.println(s.charAt(0)));     // "b"




Optionals also receive a few quite handy new methods, e.g. you can now simply turn optionals into streams or provide another optional as fallback for an empty optional:

Optional.of("foo").orElseThrow();     // foo
Optional.of("foo").stream().count();  // 1
Optional.ofNullable(null)
    .or(() -> Optional.of("fallback"))
    .get();                           // fallback
Strings#
One of the most basic classes String gets a few helper methods for trimming or checking whitespace and for streaming the lines of a string:

" ".isBlank();                // true
" Foo Bar ".strip();          // "Foo Bar"
" Foo Bar ".stripTrailing();  // " Foo Bar"
" Foo Bar ".stripLeading();   // "Foo Bar "
"Java".repeat(3);             // "JavaJavaJava"
"A\nB\nC".lines().count();    // 3