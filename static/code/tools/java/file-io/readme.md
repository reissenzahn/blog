
The support for reading and writing is in two major parts: The InputStream/OutputStream/Reader/Writer classes, which are the traditional ways of reading/writing files. In modern Java, a new class, java.nio.file.Files, is provided.


10.1 About InputStreams/OutputStreams and Readers/Writers

Java provides two sets of classes for reading and writing. The InputStream/Output
Stream section of package java.io (see Figure 10-1) is for reading or writing bytes of
data. Older languages tended to assume that a byte (which is a machine-specific col‐
lection of bits, usually eight bits on modern computers) is exactly the same thing as a
character—a letter, digit, or other linguistic element. However, Java is designed to be
used internationally, and eight bits is simply not enough to handle the many different
character sets used around the world. Script-based languages, and pictographic lan‐
guages like Chinese and Japanese, each have many more than 256 characters, the
maximum that can be represented in an eight-bit byte. The unification of these many
character code sets is called, not surprisingly, Unicode. Both Java and XML use Uni‐
code as their character sets, allowing you to read and write text in any of these human
languages. But you should use Readers and Writers, not Streams, for textual data.
Unicode itself doesn’t solve the entire problem. Many of these human languages were
used on computers long before Unicode was invented, and they didn’t all pick the
same representation as Unicode. And they all have zillions of files encoded in a par‐
ticular representation that isn’t Unicode. So routines are needed when reading and
writing to convert between Unicode String objects used inside the Java machine and
the particular external representation in which a user’s files are written. These con‐
verters are packaged inside a powerful set of classes called Readers and Writers.
Readers and Writers should always be used instead of InputStreams and Output
Streams when you want to deal with characters instead of bytes. We’ll see more on
this conversion, and how to specify which conversion, a little later in this chapter.
10.1 About InputStreams/OutputStreams and Readers/Writers | 299
Figure 10-1. java.io classes
300 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
6 A poor choice of name: it was new in Java SE 1.4. But newer than InputStream/OutputStream (Java 1.0) and
Readers/Writers (1.1).
See Also
One topic not addressed in depth here is the reading/writing capabilities of Channels
classes in the Java “new I/O” package.6
 This part of NIO is more complex to use than
either Files or the input/output streams, and the benefits accrue primarily in largescale server-side processing. Recipe 4.5 provides one example of using NIO. The NIO
package is given full coverage in the book Java NIO by Ron Hitchens (O’Reilly).
Another topic not covered here is that of having the read or write occur concurrently
with other program activity. This requires the use of threads, or multiple flows of
control within a single program. Threaded I/O is a necessity in many programs: those
reading from slow devices such as tape drives, those reading from or writing to net‐
work connections, and those with a GUI. For this reason, the topic is given consider‐
able attention, in the context of multithreaded applications, in Chapter 16.
For traditional I/O topics, Elliotte Rusty Harold’s Java I/O, although somewhat dated,
should be considered the antepenultimate documentation. The penultimate reference
is the javadoc documentation, while the ultimate reference is, if you really need it, the
source code for the Java API. Due in part to the quality of the javadoc documentation,
I have not needed to refer to the source code in writing this chapter.
10.2 Reading a Text File
Problem
The Java documentation doesn’t have methods for opening files. How do I open and
read a text file and then either process it a line at a time, or get a collection of all the
lines?
Solution
Use the Files::lines() method, which returns a Stream of Strings. Or, use
Files.newBufferedReader(), Files.newBufferedWriter(), Files.newInput
Stream(), and Files.newOutputStream(). Or, construct a FileReader or a FileIn
putStream. Once you have that, construct a BufferedReader, and use the older $
$while ((line == readLine()) != null)$$ pattern.
10.2 Reading a Text File | 301
6 Not strictly true; there is, but only in the java.nio.FileChannel class, which we’re not covering.
Discussion
There is no explicit open operation,6
 perhaps as a kind of rhetorical flourish of the
Java API’s object-oriented design.
The quickest way to process a text file a line at a time is to use Files.lines(), which
takes a Path argument and returns a functional Stream<String> into which it feeds
the lines from the file:
Files.lines(Path.of("myFile.txt")).forEach(System.out::println);
The Files class has several other static methods which open a file and read some or
all of it:
List<String> Files.readAllLines(Path)
Reads the whole file into a List<String>.
byte[] Files.readAllBytes
Reads the whole file into an array of bytes.
There is a series of methods with names like newReader(), newBufferedWriter(),
etc., each of which takes a Path argument and return the appropriate Reader/Writer
or InputStream/OutputStream. A Path is a descriptor for an abstract path (filename)
that may or may not exist. The explicit constructors for a FileReader, FileWriter,
FileInputStream, or FileOutputStream take a filename or an instance of the older
File class containing the path. These operations correspond to the “open” operation
in most other languages’ I/O packages.
Historically, Java used to require use of the code pattern while ((line == read
Line()) != null to read lines from a BufferedReader. This still works, of course,
and will continue to work until the last JavaBean sets in the west, in the far future.
Example 10-1 shows the code for each of these ways of reading lines from a file.
Example 10-1. main/src/main/java/io/ReadLines.java (reading lines from a file)
 System.out.println("Using Path.lines()");
 Files.lines(Path.of(fileName)).forEach(System.out::println);
 System.out.println("Using Path.readAllLines()");
 List<String> lines = Files.readAllLines(Path.of(fileName));
 lines.forEach(System.out::println);
 System.out.println("Using BufferedReader.lines().forEach()");
 new BufferedReader(new FileReader(fileName)).lines().forEach(s -> {
302 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 System.out.println(s);
 });
 System.out.println("The old-fashioned way");
 BufferedReader is = new BufferedReader(new FileReader(fileName));
 String line;
 while ((line = is.readLine()) != null) {
 System.out.println(line);
 }
Most of these methods can throw the checked exception IOException, so you must
have a throws clause or a try/catch around these invocations.
If you create an InputStream, OutputStream, Reader, or Writer, you should close it
when finished. This avoids memory leaks and, in the case of writing, ensures that all
buffered data is actually written to disk. One way to ensure this is not forgotten is to
use the try-with-resources syntax. This puts the declaration and definition of a Close
able resource into the try statement:
 static void oldWayShorter() throws IOException {
 try (BufferedReader is =
 new BufferedReader(new FileReader(INPUT_FILE_NAME));
 BufferedOutputStream bytesOut = new BufferedOutputStream(
 new FileOutputStream(OUTPUT_FILE_NAME.replace("\\.", "-1.")));) {
 // Read from is, write to bytesOut
 String line;
 while ((line = is.readLine()) != null) {
 line = doSomeProcessingOn(line);
 bytesOut.write(line.getBytes("UTF-8"));
 bytesOut.write('\n');
 }
 }
 }
The lines() and read-related methods in Files obviate the need for closing the
resource, but not the need for handling IOException; the compiler or IDE will
remind you if you forget those.
There are options that can be passed to the Files methods that open a file; these are
discussed in the sidebar “Understanding I/O Options: StandardOpenOptions, FileAt‐
tribute, PosixFileAttribute, and More” on page 321.
To read the entire contents of a file into single string, in Java 8+, use Files.read
String():
String input = Files.readString(Path.of(INPUT_FILE_NAME)));
10.2 Reading a Text File | 303
In older Java versions, use my FileIO.readerToString() method. This will read the
entire named file into one long string, with embedded newline (\n) characters
between each line. To read a binary file, use Files.readAllBytes() instead.
See Also
There is formal documentation online for Files and Path.
10.3 Reading from the Standard Input or from the
Console/Controlling Terminal
Problem
You want to read from the program’s standard input or directly from the program’s
controlling terminal or console terminal.
Solution
For the standard input, read bytes by wrapping a BufferedInputStream() around
System.in. For reading text, use an InputStreamReader and a BufferedReader. For
the console or controlling terminal, use Java’s System.console() method to obtain a
Console object, and use its methods.
Discussion
Sometimes you really do need to read from the standard input, or console. One rea‐
son is that simple test programs are often console-driven. Another is that some pro‐
grams naturally require a lot of interaction with the user and you want something
faster than a GUI (consider an interactive mathematics or statistical exploration pro‐
gram). Yet another is piping the output of one program directly to the input of
another, a very common operation among Unix users and quite valuable on other
platforms, such as Windows, that support this operation.
Standard input
Most desktop platforms support the notion of standard input (a keyboard, a file, or
the output from another program) and standard output (a terminal window, a printer,
a file on disk, or the input to yet another program). Most such systems also support a
standard error output so that error messages can be seen by the user even if the stan‐
dard output is being redirected. When programs on these platforms start up, the
three streams are preassigned to particular platform-dependent handles, or file
descriptors. The net result is that ordinary programs on these operating systems can
304 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
read the standard input or write to the standard output or standard error stream
without having to open any files or make any other special arrangements.
Java continues this tradition and enshrines it in the System class. The static variables
System.in, System.out, and System.err are connected to the three operating system
streams before your program begins execution (an application is free to reassign
these; see Recipe 10.10). So, to read the standard input, you need only refer to the
variable System.in and call its methods. For example, to read one byte from the stan‐
dard input, you call the read method of System.in, which returns the byte in an int
variable:
int b = System.in.read( );
But is that enough? No, because the read() method can throw an IOException. So
you must either declare that your program throws an IOException:
public static void main(String args[]) throws IOException {
...
}
Or you can put a try/catch block around the read() method:
 int b = 0;
 try {
 b = System.in.read();
 System.out.println("Read this data: " + (char)b);
 } catch (Exception e) {
 System.out.println("Caught " + e);
 }
In this case, it makes sense to print the results inside the try block because there’s no
point in trying to print the value you read, if the read() threw an IOException.
That code works and gives you the ability to read a byte at a time from the standard
input. But most applications are designed in terms of larger units, such as integers, or
a line of text. To read a value of a known type, such as int, from the standard input,
you can use the Scanner class (covered in more detail in Recipe 10.6):
Scanner sc = Scanner.create(System.in);
int i = sc.nextInt();
For reading characters of text with an input character converter so that your program
will work with multiple input encodings around the world, use a Reader class. The
particular subclass that allows you to read lines of characters is a BufferedReader.
But there’s a hitch. Remember I mentioned those two categories of input classes,
Streams and Readers? But I also said that System.in is a Stream, and you want a
Reader. How do you get from a Stream to a Reader? A crossover class called Input
StreamReader is tailor-made for this purpose. Just pass your Stream (like System.in)
to the InputStreamReader constructor and you get back a Reader, which you in turn
10.3 Reading from the Standard Input or from the Console/Controlling Terminal | 305
pass to the BufferedReader constructor. The usual idiom for writing this in Java is to
nest the constructor calls:
BufferedReader is = new BufferedReader(new InputStreamReader(System.in));
You can then read lines of text using the readLine() method. This method takes no
argument and returns a String that is made up for you by readLine() containing the
characters (converted to Unicode) from the next line of text in the file. When there
are no more lines of text, the literal value null is returned:
public class CatStdin {
 public static void main(String[] av) {
 try (BufferedReader is =
 new BufferedReader(new InputStreamReader(System.in))) {
 String inputLine;
 while ((inputLine = is.readLine()) != null) {
 System.out.println(inputLine);
 }
 } catch (IOException e) {
 System.out.println("IOException: " + e);
 }
 }
}
To read a single Integer from the standard input, read a line and parse it using Inte
ger.parseInt(). To read a series of integers, one per line, you could combine these
with a functional style, since the BufferedReader has a lines() method that pro‐
duces a Stream<String>:
public class ReadStdinIntsFunctional {
 private static Stream<Integer> parseIntSafe(String s) {
 try {
 return Stream.of(Integer.parseInt(s));
 } catch (NumberFormatException e) {
 return Stream.empty();
 }
 }
 public static void main(String[] args) throws IOException {
 try (BufferedReader is =
 new BufferedReader(new InputStreamReader(System.in));) {
 is.lines()
 .flatMap(ReadStdinIntsFunctional::parseIntSafe)
 .forEach(System.out::println);
 }
 }
}
306 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
The Console (Controlling Terminal)
The Console class is intended for reading directly from a program’s controlling termi‐
nal. When you run an application from a terminal window or command prompt win‐
dow on most systems, its console and its standard input are both connected to the
terminal, by default. However, the standard input can be changed by piping or redi‐
rection on most OSes. If you really want to read from wherever the user is sitting,
bypassing any indirections, then the Console class is usually your friend.
You cannot instantiate Console yourself; you must get an instance from the System
class’s console() method. You can then call methods such as readLine(), which
behaves largely like the method of the same name in the BufferedReader class used
in the previous recipe.
The following code shows an example of prompting for a name and reading it from
the console:
main/src/main/java/io/ConsoleRead.java
public class ConsoleRead {
 public static void main(String[] args) {
 String name = System.console().readLine("What is your name?");
 System.out.println("Hello, " + name.toUpperCase());
 }
}
One complication is that the System.console() method can return null if the con‐
sole isn’t connected. Annoyingly, some IDEs, including Eclipse, don’t manage to set
up a controlling terminal when you use the Run As→Java Application mechanism. So
production-quality code should always check for null before trying to use the Con
sole. If it fails, use a logger or just plain System.out.
One facility the Console class is quite useful for is reading a password without having
it echo. This has been a standard facility of command-line applications for decades, as
the most obvious way of preventing shoulder surfing—somebody looking over your
shoulder to see your password. Nonecho password reading is now supported in Java:
the Console class has a readPassword() method that takes a prompt argument,
intended to be used like: cons.readPassword("Password:"). This method returns an
array of bytes, which can be used directly in some encryption and security APIs, or
can easily be converted into a String. It is generally advised to overwrite the byte
array after use to prevent security leaks when other code can access the stack,
although the benefits of this are probably reduced when you’ve constructed a String.
There’s an example of this in the online code in io/ReadPassword.java.
10.3 Reading from the Standard Input or from the Console/Controlling Terminal | 307
6 The central character in Yann Martel’s novel Life of Pi would have been born in 1956, according to informa‐
tion in Wikipedia.
10.4 Printing with Formatter and printf
Problem
You want an easy way to use java.util.Formatter class’s capability for simple print‐
ing tasks.
Solution
Use Formatter for printing values with fine-grained control over the formatting. Use
String.format() or PrintWriter.printf() / PrintStream.printf().
Discussion
The Formatter class is patterned after C’s printf routines. In fact, PrintStream and
PrintWriter have convenience routines named printf() that simply delegate to the
stream or writer’s format() method, which uses a default Formatter instance. Unlike
in C, however, Java is a strongly typed language, so invalid arguments will throw an
exception rather than generating gibberish. There are also convenience routines
static String.format() and printf() in PrintWRiter/PrintStream for use when
you want to format a String without the bother of creating the Formatter explicitly.
The underlying Formatter class in java.util works on a String containing format
codes. For each item that you want to format, you put a format code. The format code
consists of a percent sign, optionally an argument number followed by a dollar sign,
optionally a field width or precision, and a format type (e.g., d for decimal integer,
that is, an integer with no decimal point, and f for floating point). A simple use might
look like the following:
System.out.printf("%1$04d - the year of %2$f%n", 1956, Math.PI);
System.out.printf("%04d - the year of %f%n", 1956, Math.PI);
As shown in Figure 10-2, the “%1$04d” controls formatting of the year, and the
“%2$f ” controls formatting of the value of PI.6
308 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
Figure 10-2. Format codes examined
Many format codes are available; Table 10-1 lists some of the more common ones.
For a complete description, refer to the javadoc for java.util.Formatter.
Table 10-1. Formatter format codes
Code Meaning
c Character (argument must be char or integral type containing valid character value).
d “decimal int”—integer to be printed as a decimal (radix 10) with no decimal point (argument must be integral type).
f Floating-point value with decimal fraction (must be numeric); field width may be followed by decimal point and
fractional digit field width; e.g., 7.2f.
e Floating-point value in scientific notation.
g Floating-point value, as per f or e, depending on magnitude.
s Generic format; if value is null, prints “null”; else if arg implements Formattable, format as per arg.for
matTo(); else format as per arg.toString().
t Date codes; follow with secondary code. Common date codes are shown in Table 10-2. Argument must be long,
Long, Calendar, or Date.
n Newline; insert the platform-dependent line ending character.
% Insert a literal % character.
Note also that you may, but are not required to, put a parameter order number
between the % and the format code. For example, in “%2$04d”, the “2$” means to for‐
mat the second parameter, regardless of the order of the parameters. This is primarily
useful with dates (see the following example, where you need to format several differ‐
ent portions of the same Date or Calendar, or any time you want to format the same
object more than once) and in internationalization, where different languages may
require words to be in a different order within a sentence.
Some examples of using a Formatter are shown in Example 10-2.
10.4 Printing with Formatter and printf | 309
Example 10-2. main/src/main/java/io/FormatterDemo.java
public class FormatterDemo {
 public static void main(String[] args) {
 // The arguments to all these format methods consist of
 // a format code String and 1 or more arguments.
 // Each format code consists of the following:
 // % - code lead-in
 // N$ - OPTIONAL parameter number (1-based) after the format code
 // N - field width
 // L - format letter (d: decimal(int); f: float; s: general; many more)
 // For the full(!) story, see javadoc for java.util.Formatter.
 // Most general (cumbersome) way of proceding.
 Formatter fmtr = new Formatter();
 Object result = fmtr.format("%1$04d - the year of %2$f", 1956, Math.PI);
 System.out.println(result);
 fmtr.close();
 // Shorter way using static String.format(), default parameter numbering.
 Object stringResult = String.format("%04d - the year of %f", 1956, Math.PI);
 System.out.println(stringResult);
 // A shorter way using PrintStream/PrintWriter.format, more in line with
 // other languages. But this way you should provide the newline delimiter
 // using %n (rather than \n as that is platform-dependent!).
 System.out.printf("%04d - the year of %f%n", 1956, Math.PI);
 // Format doubles with more control
 System.out.printf("PI is approximately %4.2f%n", Math.PI);
 }
}
Running FormatterDemo produces this:
C:> javac FormatterDates.java
C:> java io.FormatterDates
1956 - The year of 3.141593
1956 - The year of 3.141593
1956 - The year of 3.141593
PI is about 3.14
For formatting date and time objects, a large variety of format codes are available—
about 40 in all. Date and time objects are discussed in Chapter 6. Table 10-3 shows
the more common date/time format codes. Each must be preceded by a t, so to for‐
mat the first argument as a year, you would use %1$tY.
310 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
Table 10-2. Formatting codes for dates and times
Format code Meaning
Y Year (at least four digits)
m Month as two-digit (leading zeros) number
B Locale-specific month name (b for abbreviated)
d Day of month (two digits, leading zeros)
e Day of month (one or two digits)
A Locale-specific day of week (a for abbreviated)
H or I Hour in 24-hour (H) or 12-hour (I) format (two digits, leading zeros)
M Minute (two digits)
S Second (two digits)
P/p Locale-specific AM or PM in uppercase (if P) or lowercase (if p)
R or T 24-hour time combination: %tH:%tM (if R) or %tH:%tM:%tS (if T)
D Date formatted as %tm/%td/%ty
In my opinion, embedding these codes directly in applications that you distribute
or make available as web applications is often a bad idea, because any direct use of
them assumes that you know the correct order to print these fields in all locales
around the world. Trust me, you don’t. Instead of these, I recommend the use of Date
TimeFormatter, covered in Recipe 6.2, to control the order of arguments. However,
for quick-and-dirty work, as well as for writing log or data files that must be in a
given format because some other program reads them, these are OK.
Some date examples are shown in Example 10-3.
Example 10-3. main/src/main/java/io/FormatterDates.java
public class FormatterDates {
 public static void main(String[] args) {
 // Format number as dates e.g., 2020-06-28
 System.out.printf("%4d-%02d-%2d%n", 2020, 6, 28);
 // Format fields directly from a Date object: multiple fields from "1$"
 // (hard-coded formatting for Date not advisable; see I/O chapter)
 LocalDate today = LocalDate.now();
 // Print in a form like e.g., "July 4, 2020"
 System.out.printf("Today is %1$tB %1$td, %1$tY%n", today);
 }
}
Running this FormatterDates class produces the following output:
10.4 Printing with Formatter and printf | 311
C:> java io.FormatterDates
2020-06-28
Today is January 01, 2020
10.5 Scanning Input with StreamTokenizer
Problem
You need to scan a file with more fine-grained resolution than the readLine()
method of the BufferedReader class and its subclasses.
Solution
Use a StreamTokenizer, readLine() and a StringTokenizer, the Scanner class (see
Recipe 10.6), regular expressions (Chapter 4), or one of several third-party parser
generators.
Discussion
Though you could, in theory, read a file one character at a time and analyze each
character, that is a pretty low-level approach. The read() method in the Reader class
is defined to return int so that it can use the time-honored value -1 (defined as EOF
in Unix <stdio.h> for years) to indicate that you have read to the end of the file:
main/src/main/java/io/ReadCharsOneAtATime.java
public class ReadCharsOneAtATime {
 void doFile(Reader is) throws IOException {
 int c;
 while ((c=is.read( )) != -1) {
 System.out.print((char)c);
 }
 }
}
Notice the cast to char; the program compiles fine without it, but it does not print
correctly because c is declared as int. Variable c must be declared int to be able to
compare against the end-of-file value -1. For example, the integer value correspond‐
ing to capital A treated as an int prints as 65, whereas with (char) prints the
character A.
We discussed the StringTokenizer class extensively in Recipe 3.1. The combination
of readLine() and StringTokenizer provides a simple means of scanning a file. Sup‐
pose you need to read a file in which each line consists of a name like
user@host.domain, and you want to split the lines into users and host addresses. You
could use this:
312 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
public class ScanStringTok {
 public static void main(String[] av) throws IOException {
 if (av.length == 0)
 System.err.printf("Usage: %s filename [...]%n",
 ScanStringTok.class.getSimpleName());
 else
 for (int i=0; i<av.length; i++)
 process(av[i]);
 }
 static void process(String fileName) {
 String s = null;
 try (BufferedReader is =
 new BufferedReader(new FileReader(fileName));) {
 while ((s = is.readLine()) != null) {
 StringTokenizer st = new StringTokenizer(s, "@", true);
 String user = (String)st.nextElement();
 st.nextElement();
 String host = (String)st.nextElement();
 System.out.println("User name: " + user +
 "; host part: " + host);
 // Do something useful with the user and host parts...
 }
 } catch (NoSuchElementException ix) {
 System.err.println("Malformed input " + s);
 } catch (IOException e) {
 System.err.println(e);
 }
 }
}
The StreamTokenizer class in java.util provides slightly more capabilities for scan‐
ning a file. It reads characters and assembles them into words, or tokens. It returns
these tokens to you along with a type code describing the kind of token it found. This
type code is one of four predefined types (StringTokenizer.TT_WORD, TT_NUMBER,
TT_EOF, or TT_EOL for the end-of-line) or the char value of an ordinary character
(such as 32 for the space character). Methods such as ordinaryCharacter() allow
you to specify how to categorize characters, while others such as slashSlashCom
ment() allow you to enable or disable features.
Example 10-4 shows a StreamTokenizer used to implement a simple immediatemode stack-based calculator:
2 2 + =
4
22 7 / =
3.141592857
10.5 Scanning Input with StreamTokenizer | 313
I read tokens as they arrive from the StreamTokenizer. Numbers are put on the
stack. The four operators (+, -, \*, and /) are immediately performed on the two ele‐
ments at the top of the stack, and the result is put back on the top of the stack. The =
operator causes the top element to be printed, but is left on the stack so that you can
say this:
4 5 * = 2 / =
20.0
10.0
Example 10-4. main/src/main/java/io/SimpleCalcStreamTok.java (simple calculator
using StreamTokenizer)
public class SimpleCalcStreamTok {
 /** The StreamTokenizer input */
 protected StreamTokenizer tf;
 /** The output file */
 protected PrintWriter out = new PrintWriter(System.out, true);
 /** The variable name (not used in this version) */
 protected String variable;
 /** The operand stack */
 protected Stack<Double> s = new Stack<>();
 /* Driver - main program */
 public static void main(String[] av) throws IOException {
 if (av.length == 0)
 new SimpleCalcStreamTok(
 new InputStreamReader(System.in)).doCalc();
 else
 for (int i=0; i<av.length; i++)
 new SimpleCalcStreamTok(av[i]).doCalc();
 }
 /** Construct by filename */
 public SimpleCalcStreamTok(String fileName) throws IOException {
 this(new FileReader(fileName));
 }
 /** Construct from an existing Reader */
 public SimpleCalcStreamTok(Reader rdr) throws IOException {
 tf = new StreamTokenizer(rdr);
 // Control the input character set:
 tf.slashSlashComments(true); // treat "//" as comments
 tf.ordinaryChar('-'); // used for subtraction
 tf.ordinaryChar('/'); // used for division
 }
 /** Construct from a Reader and a PrintWriter
 */
 public SimpleCalcStreamTok(Reader in, PrintWriter out) throws IOException {
 this(in);
314 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 setOutput(out);
 }
 /**
 * Change the output destination.
 */
 public void setOutput(PrintWriter out) {
 this.out = out;
 }
 protected void doCalc() throws IOException {
 int iType;
 double tmp;
 while ((iType = tf.nextToken()) != StreamTokenizer.TT_EOF) {
 switch(iType) {
 case StreamTokenizer.TT_NUMBER: // Found a number, push value to stack
 push(tf.nval);
 break;
 case StreamTokenizer.TT_WORD:
 // Found a variable, save its name. Not used here.
 variable = tf.sval;
 break;
 case '+':
 // + operator is commutative.
 push(pop() + pop());
 break;
 case '-':
 // - operator: order matters.
 tmp = pop();
 push(pop() - tmp);
 break;
 case '*':
 // Multiply is commutative.
 push(pop() * pop());
 break;
 case '/':
 // Handle division carefully: order matters!
 tmp = pop();
 push(pop() / tmp);
 break;
 case '=':
 out.println(peek());
 break;
 default:
 out.println("What's this? iType = " + iType);
 }
 }
 }
 void push(double val) {
 s.push(Double.valueOf(val));
 }
10.5 Scanning Input with StreamTokenizer | 315
 double pop() {
 return ((Double)s.pop()).doubleValue();
 }
 double peek() {
 return ((Double)s.peek()).doubleValue();
 }
 void clearStack() {
 s.removeAllElements();
 }
}
10.6 Scanning Input with the Scanner Class
Problem
You want to scan a simple input file consisting of various numbers and strings in a
known format.
Solution
Read with Scanner’s next() methods.
Discussion
The Scanner class lets you read an input source by tokens, somewhat analogous to
the StreamTokenizer described in Recipe 10.5. The Scanner is more flexible in some
ways (it lets you break tokens based on spaces or regular expressions) but less in oth‐
ers (you need to know the kind of token you are reading). This class bears some
resemblance to the C-language scanf() function, but in the Scanner you specify the
input token types by calling methods like nextInt(), nextDouble(), and so on. Here
is a simple example of scanning:
 String sampleDate = "25 Dec 1988";
 try (Scanner sDate = new Scanner(sampleDate)) {
 int dayOfMonth = sDate.nextInt();
 String month = sDate.next();
 int year = sDate.nextInt();
 System.out.printf("%d-%s-%02d%n", year, month, dayOfMonth);
 }
The Scanner recognizes Java’s eight built-in types, in addition to BigInteger and Big
Decimal. It can also return input tokens as Strings or by matching regular expres‐
sions (see Chapter 4). Table 10-3 lists the “next” methods and corresponding “has”
methods; the “has” method returns true if the corresponding “next” method would
succeed. There is no nextString() method; just use next() to get the next token as a
String.
316 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
6 If this were code in a maintained project, I might factor out some of the common code among these two cal‐
culators, as well as the one in Recipe 5.12, and divide the code better using interfaces. However, this would
detract from the simplicity of self-contained examples.
Table 10-3. Scanner methods
Returned
type
“has” method “next” method Comment
String hasNext() next() The next complete token from this scanner
String hasNext(Pattern) next(Pattern) The next string that matches the given regular
expression (regex)
String hasNext(String) next(String) The next token that matches the regex pattern
constructed from the specified string
BigDeci
mal
hasNextBigDeci
mal()
nextBigDeci
mal()
The next token of the input as a BigDecimal
BigIn
teger
hasNextBigIn
teger()
nextBigIn
teger()
The next token of the input as a BigInteger
boolean hasNextBoolean() nextBoolean() The next token of the input as a boolean
byte hasNextByte() nextByte() The next token of the input as a byte
double hasNextDouble() nextDouble() The next token of the input as a double
float hasNextFloat() nextFloat() The next token of the input as a float
int hasNextInt() nextInt() The next token of the input as an int
String N/A nextLine() Reads up to the end-of-line, including the line ending
long hasNextLong() nextLong() The next token of the input as a long
short hasNextShort() nextShort() The next token of the input as a short
The Scanner class is constructed with an input source, which can be an InputStream,
a String, or Readable (Readable is an interface that Reader and all its subclasses
implement).
One way to use the Scanner class is based on the Iterator pattern, using while (scan
ner.hasNext()) to control the iteration. Example 10-5 shows the simple calculator
from Recipe 10.5 rewritten6
 to use the Scanner class.
Example 10-5. main/src/main/java/io/simpleCalcScanner.java (simple calculator using
java.util.Scanner)
public class SimpleCalcScanner {
 /** The Scanner */
 protected Scanner scan;
 /** The output */
 protected PrintWriter out = new PrintWriter(System.out, true);
10.6 Scanning Input with the Scanner Class | 317
 /** The variable name (not used in this version) */
 protected String variable;
 /** The operand stack; no operators are pushed,
 * so it can be a stack of Double
 */
 protected Stack<Double> s = new Stack<>();
 /* Driver - main program */
 public static void main(String[] args) throws IOException {
 if (args.length == 0)
 new SimpleCalcScanner(
 new InputStreamReader(System.in)).doCalc();
 else
 for (String arg : args) {
 new SimpleCalcScanner(arg).doCalc();
 }
 }
 /** Construct a SimpleCalcScanner by name */
 public SimpleCalcScanner(String fileName) throws IOException {
 this(new FileReader(fileName));
 }
 /** Construct a SimpleCalcScanner from an open Reader */
 public SimpleCalcScanner(Reader rdr) throws IOException {
 scan = new Scanner(rdr);
 }
 /** Construct a SimpleCalcScanner from a Reader and a PrintWriter */
 public SimpleCalcScanner(Reader rdr, PrintWriter pw) throws IOException {
 this(rdr);
 setWriter(pw);
 }
 /** Change the output to go to a new PrintWriter */
 public void setWriter(PrintWriter pw) {
 out = pw;
 }
 protected void doCalc() throws IOException {
 double tmp;
 while (scan.hasNext()) {
 if (scan.hasNextDouble()) {
 push(scan.nextDouble());
 } else {
 String token;
 switch(token = scan.next()) {
 case "+":
 // Found + operator, perform it immediately.
318 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 push(pop() + pop());
 break;
 case "-":
 // Found - operator, perform it (order matters).
 tmp = pop();
 push(pop() - tmp);
 break;
 case "*":
 // Multiply is commutative.
 push(pop() * pop());
 break;
 case "/":
 // Handle division carefully: order matters!
 tmp = pop();
 push(pop() / tmp);
 break;
 case "=":
 out.println(peek());
 break;
 default:
 out.println("What's this? " + token);
 break;
 }
 }
 }
 }
 void push(double val) {
 s.push(Double.valueOf(val));
 }
 double pop() {
 return ((Double)s.pop()).doubleValue();
 }
 double peek() {
 return ((Double)s.peek()).doubleValue();
 }
 void clearStack() {
 s.removeAllElements();
 }
}
10.7 Scanning Input with Grammatical Structure
Problem
You need to parse a file whose structure can be described as grammatical (in the sense
of computer languages, not natural languages).
10.7 Scanning Input with Grammatical Structure | 319
Solution
Use one of many parser generators.
Discussion
Although the StreamTokenizer class (see Recipe 10.5) and Scanner (see Recipe 10.6)
are useful, they know only a limited number of tokens and have no way of specifying
that the tokens must appear in a particular order. To do more advanced scanning, you
need some special-purpose scanning tools. Parser generators have a long history in
computer science. The best-known examples are the C-language yacc (Yet Another
Compiler Compiler) and lex, released with Seventh Edition Unix in the 1970s and
discussed in lex & yacc by Doug Brown et al. (O’Reilly), and their open source clones
bison and ex. These tools let you specify the lexical structure of your input using
some pattern language such as regular expressions (see Chapter 4). For example, you
might say that an email address consists of a series of alphanumerics, followed by an
at sign (@), followed by a series of alphanumerics with periods embedded, like this:
name: [A-Za-z0-9]+@[A-Za-z0-0.]
or
name: \w+#[\w.]
The tool then writes code that recognizes the characters you have described. These
tools also have a grammatical specification, which says, for example, that the keyword
EMAIL must appear, followed by a colon, followed by a name token, as previously
defined.
There are several good third-party parser generator tools for Java. They vary widely
based on complexity, power, and ease of use:
• One of the best known and most elaborate is ANTLR.
• JavaCC is an open source project at https://javacc.org.
• JParsec lets you write the parser in straight Java, so it’s all built at compile time
(most of the others require a separate parse generation step, with the build and
debugging issues that raises). JParsec is on GitHub. * JFlex and CUP work
together like the original yacc and lex, as grammar parser and lexical scanner,
respectively.
• Parboiled uses Parsing Expression Grammar (PEG) to also build the parser at
compile time. See GitHub for more information. * The Rats! parser generator is
part of the eXTensible Compiler Project at New York University.
• There are others; a more complete list is maintained at Java Source.
These parser generators can be used to write grammars for a wide variety of pro‐
grams, from simple calculators—such as the one in Recipe 10.6—through HTML and
320 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
CORBA/IDL, up to full Java and C/C++ parsers. Examples of these are included with
the downloads. Unfortunately, the learning curve for parsers in general precludes
providing a simple and comprehensive example here, let alone comparing them intel‐
ligently. Refer to the documentation and the numerous examples provided with each
distribution.
As an alternative to using one of these, you could simply roll your own recursive
descent parser; and once you learn how to do so, you may find it’s not really that diffi‐
cult, quite possibly even less hassle than dealing with the extra parser generator soft‐
ware (depending on the complexity of the grammar involved, obviously).
Java developers have a range of choices, including simple line-at-a-time scanners
using StringTokenizer, fancier token-based scanners using StreamTokenizer, a
Scanner class to scan simple tokens (see Recipe 10.6), regular expressions (see Chap‐
ter 4), and third-party solutions including grammar-based scanners based on the
parsing tools listed here.
Understanding I/O Options: StandardOpenOptions, FileAttribute,
PosixFileAttribute, and More
There are several sets of options that can be applied when creating or opening a file.
The main option sets that can be applied include those listed in Table 10-4:
Table 10-4. Sets of options
Name Examples Usage/notes
CopyOption StandardCopyOp
tion.REPLACE_EXISTING
Files methods that copy
LinkOption LinkOption.NOFOL
LOW_LINKS
Files methods that write data or read
attributes
FileAttribute Name-value pair, used in Files.cre
ate*() methods
OpenOption StandardOpenOp
tion.READ,APPEND
Files.new{In,Out}put
Stream()
PosixFilePermis
sion
OWNER_READ,OTHER_WRITE FileAttribute
PosixFilePermis
sions
Set<PosixFilePermission> Conversions to/from rwx strings
A list of the standard OpenOption values is in Table 10-5. These control how a file is to
be accessed.
10.7 Scanning Input with Grammatical Structure | 321
Table 10-5. OpenOption StandardOpenOption values
Name Meaning
APPEND Write at the end of an existing file instead of overwriting it.
CREATE Create the file if it does not exist.
CREATE_NEW Create the file only if it is new; fails with FileAlreadyExistsExcep
tion if file already exists.
DELETE_ON_CLOSE Delete the file when the stream is closed. Useful for temporary files.
DSYNC Write data synchronously, i.e., every write is to be synchronized to disk
immediately.
READ Open the file for reading.
SPARSE Create as a sparse file, e.g., for random-access writing.
SYNC Write data and metadata synchronously, i.e., every write or attribute
change is to be synchronized to disk immediately.
TRUNCATE_EXISTING If the file exists, open for writing at the beginning, removing all contents
at open time.
WRITE Open for write access.
POSIX is the IEEE’s Portable Operating System Specification for Unix-like operating
systems. Java’s PosixPermission and its wrapper PosixPermissions are used to con‐
trol who can do what to a file on disk. These are based on the Unix/POSIX permis‐
sions laid down in early Unix systems in the early 1970s. There are three actors:
owner (which Unix calls user), group, and other (everyone else). Groups is a Unix/
POSIX mechanism: a user can be in one or many groups and has permissions based
on all the groups they are in; this is an early form of privilege separation. There are
three permissions: read, write, and execute. The latter grants permission to execute a
file, but is also used to grant permission to search (list) a directory. For decades these
have been expressed as a nine-character permissions string. For example, rwxr—r--
means the user has read, write, and execute permissions on a given file; other mem‐
bers of the file owner’s group have read-only access, and everyone else also has readonly access. The PosixPermissions wrapper class has methods for converting
between these concise strings and a Set of individual PosixPermission enum con‐
stants. The enum constants are the nine combinations of OWNER, GROUP, and OTHERS
with READ, WRITE, and EXECUTE. Here is a JShell example showing these file permission
conversion routines:
jshell> Set<PosixFilePermission> perms =
 PosixFilePermissions.fromString("rwxr-xr--");
perms ==> [OWNER_READ, OWNER_WRITE, OWNER_EXECUTE, GROUP_READ,
 GROUP_EXECUTE, OTHERS_READ]
jshell> Set<PosixFilePermission> nPerms =
 Set.of(PosixFilePermission.OWNER_READ, PosixFilePermission.GROUP_READ);
nPerms ==> [GROUP_READ, OWNER_READ]
322 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
jshell> PosixFilePermissions.toString(nPerms)
$7 ==> "r--r-----"
You can further convert the Set<PosixFilePermission> into a FileAttribute to be
used with the Files class createFile() or createDirectory() operations:
jshell> PosixFilePermissions.asFileAttribute(nPerms)
$8 ==> java.nio.file.attribute.PosixFilePermissions$1@ed17bee
jshell> $8.name()
$9 ==> "posix:permissions"
jshell> $8.value()
$10 ==> [OWNER_READ, GROUP_READ]
jshell> Files.createFile(Path.of("/tmp/xx"), $8);
$41 ==> /tmp/xx
jshell> /exit
$ ls -l /tmp/xx
-r--r----- 1 ian wheel 0 Dec 23 11:14 /tmp/xx
$
We see the file was created with only owner-read and group-read permissions, as
requested. Note that on *nix systems there is a user setting umask that may remove or
mask out permissions, so what you ask for may not be exactly what you get.
You can examine the attributes of a file using the FileAttribute interface or its
filesystem-specific subtypes. Here we’ll use the PosixFileAttributeView to show the
owner and permissions of the file we created:
 PosixFileAttributes attrs =
 Files.getFileAttributeView(filePath,
 PosixFileAttributeView.class)
 .readAttributes();
 System.out.format("File %s Owned by %s has perms %s%n",
 filePath,
 attrs.owner().getName(),
 PosixFilePermissions.toString(attrs.permissions()));
There are other filesystem-specific views, such as DosFilewAttributeView for use on
FAT filesystems. FAT was copied from CPM-86 into the earliest releases of MS-DOS,
and expanded versions of it are still in use on USB memory cards and in consumer
devices.
10.7 Scanning Input with Grammatical Structure | 323
10.8 Copying a File
Problem
You need to copy a file in its entirety.
Solution
Use one of the Java 11 Files.copy() methods. If on an older release, use the explicit
read and write methods in the Readers/Writers or InputStream/OutputStreams.
Discussion
The Files class has several overloads of a copy method that makes quick work of this
requirement:
Path copy(Path, Path, CopyOption...) throws java.io.IOException;
long copy(InputStream, Path, CopyOption...) throws IOException;
long copy(Path, OutputStream) throws IOException;
For example:
Path p = Paths.get("my_new_file");
InputStream is = // open some file for reading
long newFileSize = Files.copy(is, p);
Long ago, Java’s I/O facilities did not package a lot of the common operations like
copying one file to another or reading a file into a String. So back then I wrote my
own package of helper methods. Users of older JDK versions may want to use FileIO
from my utilities package com.darwinsys.util. Here’s a simple demo program that
uses FileIO to copy a source file to a backup file:
main/src/demo/java/io/FileIoDemo.java
package com.darwinsys.io;
import java.io.IOException;
public class FileIoDemo {
 public static void main(String[] av) {
 try {
 FileIO.copyFile("FileIO.java", "FileIO.bak");
 FileIO.copyFile("FileIO.class", "FileIO-class.bak");
 } catch (IOException e) {
 System.err.println(e);
 }
 }
}
324 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
My copyFile method takes several forms, depending on whether you have two file‐
names, a filename and a PrintWriter, and so on. The code for FileIO itself is not
shown here but is online, in the darwinsys API download.
10.9 Reassigning the Standard Streams
Problem
You need to reassign one or more of the standard streams System.in, System.out, or
System.err.
Solution
Construct an InputStream or PrintStream as appropriate, and pass it to the appro‐
priate set method in the System class.
Discussion
The ability to reassign these streams corresponds to what Unix (or DOS command
line) users think of as redirection, or piping. This mechanism is commonly used to
make a program read from or write to a file without having to explicitly open it and
go through every line of code changing the read, write, print, etc. calls to refer to a
different stream object. The open operation is performed by the command-line inter‐
preter in Unix or DOS or by the calling class in Java.
Although you could just assign a new PrintStream to the variable System.out, best
practice is to use the defined method to replace it:
 String LOGFILENAME = "error.log";
 System.setErr(new PrintStream(new FileOutputStream(LOGFILENAME)));
 System.out.println("Please look for errors in " + LOGFILENAME);
 // Now assume this is somebody else's code; you'll see it
 // writing to stderr...
 int[] a = new int[5];
 a[10] = 0; // here comes an ArrayIndexOutOfBoundsException
The stream you use can be one that you’ve opened, as here, or one you inherited:
System.setErr(System.out); // merge stderr and stdout to same output file.
It could also be a stream connected to or from another Process you’ve started (see
Recipe 18.1), a network socket, or a URL. Anything that gives you a stream can be
used.
10.9 Reassigning the Standard Streams | 325
10.10 Duplicating a Stream as It Is Written; Reassigning
Standard Streams
Problem
You want anything written to a stream, such as the standard output System.out or
the standard error System.err, to appear there but also be logged in to a file.
Solution
Subclass PrintStream and have its write() methods write to two streams. Then use
system.setErr() or setOut() to replace the existing standard stream with a Print
Stream subclass.
Discussion
Some classes are meant to be subclassed. Here we’re just subclassing PrintStream and
adding a bit of functionality: a second PrintStream! I wrote a class called TeePrint
Stream, named after the ancient Unix command tee. That command allowed you to
duplicate, or tee off (from plumber’s pipe tee, not the game of golf or the local pest) a
copy of the data being written on a pipeline between two programs.
The original Unix tee command is used like this: the | character creates a pipeline in
which the standard output of one program becomes the standard input to the next.
This often-used example of pipes shows how many users are logged into a Unix
server:
who | wc -l
This runs the who program (which lists who is logged in to the system, one name per
line, along with the terminal port and login time) and sends its output, not to the ter‐
minal, but rather into the standard input of the word count (wc) program. Here, wc is
being asked to count lines, not words, hence the -l option. To tee a copy of the inter‐
mediate data into a file, you might say:
who | tee wholist | wc -l
which creates a file wholist containing the data. For the curious, the file wholist might
look something like this:
ian ttyC0 Mar 14 09:59
ben ttyC3 Mar 14 10:23
ian ttyp4 Mar 14 13:46 (laptop.darwinsys.com)
So both the previous command sequences would print 3 as their output.
326 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
TeePrintStream is an attempt to capture the spirit of the tee command. It can be used
like this:
System.setErr(new TeePrintStream(System.err, "err.log"));
// ...lots of code that occasionally writes to System.err... Or might.
System.setErr() is a means of specifying the destination of text printed to
System.err (there are also System.setOut() and System.setIn()). This code results
in any messages that printed to System.err to print to wherever System.err was pre‐
viously directed (normally the terminal, but possibly a text window in an IDE) and to
the file err.log.
This technique is not limited to the three standard streams. A TeePrintStream can be
passed to any method that wants a PrintStream. Or, for that matter, an Output
Stream. And you can adapt the technique for BufferedInputStreams, PrintWriters,
BufferedReaders, and so on.
Example 10-6 shows the source code for TeePrintStream.
Example 10-6. main/src/main/java/io/TeePrintStream.java
public class TeePrintStream extends PrintStream {
 /** The original/direct print stream */
 protected PrintStream parent;
 /** The filename we are tee-ing too, if known;
 * intended for use in future error reporting.
 */
 protected String fileName;
 /** The name for when the input filename is not known */
 private static final String UNKNOWN_NAME = "(opened Stream)";
 /** Construct a TeePrintStream given an existing PrintStream,
 * an opened OutputStream, and a boolean to control auto-flush.
 * This is the main constructor, to which others delegate via "this".
 */
 public TeePrintStream(PrintStream orig, OutputStream os, boolean flush)
 throws IOException {
 super(os, true);
 fileName = UNKNOWN_NAME;
 parent = orig;
 }
 /** Construct a TeePrintStream given an existing PrintStream and
 * an opened OutputStream.
 */
 public TeePrintStream(PrintStream orig, OutputStream os)
 throws IOException {
 this(orig, os, true);
10.10 Duplicating a Stream as It Is Written; Reassigning Standard Streams | 327
 }
 /* Construct a TeePrintStream given an existing Stream and a filename.
 */
 public TeePrintStream(PrintStream os, String fn) throws IOException {
 this(os, fn, true);
 }
 /* Construct a TeePrintStream given an existing Stream, a filename,
 * and a boolean to control the flush operation.
 */
 public TeePrintStream(PrintStream orig, String fn, boolean flush)
 throws IOException {
 this(orig, new FileOutputStream(fn), flush);
 fileName = fn;
 }
 /** Return true if either stream has an error. */
 public boolean checkError() {
 return parent.checkError() || super.checkError();
 }
 /** override write(). This is the actual "tee" operation. */
 public void write(int x) {
 parent.write(x); // "write once;
 super.write(x); // write somewhere else."
 }
 /** override write(). This is the actual "tee" operation. */
 public void write(byte[] x, int o, int l) {
 parent.write(x, o, l); // "write once;
 super.write(x, o, l); // write somewhere else."
 }
 /** Close both streams. */
 public void close() {
 parent.close();
 super.close();
 }
 /** Flush both streams. */
 public void flush() {
 parent.flush();
 super.flush();
 }
}
It’s worth mentioning that I do not need to override all the polymorphic forms of
print() and println(). Because these all ultimately use one of the forms of write(),
if you override the print and println methods to do the tee-ing as well, you can get
several additional copies of the data written out.
328 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
10.11 Reading/Writing a Different Character Set
Problem
You need to read or write a text file using a particular encoding.
Solution
Convert the text to or from internal Unicode by specifying a converter when you con‐
struct an InputStreamReader or PrintWriter.
Discussion
Classes InputStreamReader and OutputStreamWriter are the bridge from byteoriented Streams to character-based Readers. These classes read or write bytes and
translate them to or from characters according to a specified character encoding. The
UTF-16 character set used inside Java (char and String types) is a 16-bit character
set. But most character sets—such as ASCII, Swedish, Spanish, Greek, Turkish, and
many others—use only a small subset of that. In fact, many European language char‐
acter sets fit nicely into 8-bit characters. Even the larger character sets (script-based
and pictographic languages) don’t all use the same bit values for each particular char‐
acter. The encoding, then, is a mapping between Java characters and an external stor‐
age format for characters drawn from a particular national or linguistic character set.
To simplify matters, the InputStreamReader and OutputStreamWriter constructors
are the only places where you can specify the name of an encoding to be used in this
translation. If you do not specify an encoding, the platform’s (or user’s) default
encoding is used. PrintWriters, BufferedReaders, and the like all use whatever
encoding the InputStreamReader or OutputStreamWriter class uses. Because these
bridge classes only accept Stream arguments in their constructors, the implication is
that if you want to specify a nondefault converter to read or write a file on disk, you
must start by constructing not a FileReader or FileWriter, but a FileInputStream
or FileOutputStream!
// io/UseConverters.java
BufferedReader fromKanji = new BufferedReader(
 new InputStreamReader(new FileInputStream("kanji.txt"), "EUC_JP"));
PrintWriter toSwedish = new PrinterWriter(
 new OutputStreamWriter(new FileOutputStream("sverige.txt"), "Cp278"));
Not that it would necessarily make sense to read a single file from Kanji and output it
in a Swedish encoding. For one thing, most fonts would not have all the characters of
both character sets; and, at any rate, the Swedish encoding certainly has far fewer
characters in it than the Kanji encoding. Besides, if that were all you wanted, you
could use a JDK tool with the ill-fitting name native2ascii (see its documentation for
10.11 Reading/Writing a Different Character Set | 329
details). A list of the supported encodings is also in the JDK documentation, in the
file docs/guide/internat/encoding.doc.html. A more detailed description is found in
Appendix B of Java I/O.
10.12 Those Pesky End-of-Line Characters
Problem
You really want to know about end-of-line characters.
Solution
Use \r and \n in whatever combination makes sense.
Discussion
If you are reading text (or bytes containing ASCII characters) in line mode using the
readLine() method, you’ll never see the end-of-line characters, and if you’re using a
PrintWriter with its println() method, the same applies. Thus you won’t be cursed
with having to figure out whether \n, \r, or \r\n appears at the end of each line.
If you want that level of detail, you have to read the characters or bytes one at a time,
using the read() methods. The only time I’ve found this necessary is in networking
code, where some of the line-mode protocols assume that the line ending is \r\n.
Even here, though, you can still work in line mode. When writing, pass \r\n into the
print() (not +deal with the characters:
outputSocket.print("HELO " + myName + "\r\n");
String response = inputSocket.readLine();
For the curious, the strange spelling of “hello” is used in SMTP, the mail sending pro‐
tocol, where commands are four letters.
10.13 Beware Platform-Dependent File Code
Problem
Chastened by the previous recipe, you now wish to write only platform-independent
code.
Solution
Use readLine() and println(). Avoid use of \n by itself; use File.separator if you
must.
330 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
Discussion
As mentioned in Recipe 10.12, if you just use readLine() and println(), you won’t
have to think about the line endings. But a particular problem, especially for former
programmers of C and related languages, is using the \n character in text strings to
mean a newline. What is particularly distressing about this code is that it works—
sometimes—usually on the developer’s own platform. But it will probably fail some‐
day, on some other system:
 String myName;
 public static void main(String[] argv) {
 BadNewline jack = new BadNewline("Jack Adolphus Schmidt, III");
 System.out.println(jack);
 }
 /**
 * DON'T DO THIS. THIS IS BAD CODE.
 */
 public String toString() {
 return "BadNewlineDemo@" + hashCode() + "\n" + myName;
 }
 // The obvious Constructor is not shown for brevity; it's in the code
The real problem is not that it fails on some platforms, though. What’s really wrong is
that it mixes formatting and I/O, or tries to. Don’t mix line-based display with
toString(); avoid multiline strings—output from toString() or any other stringreturning method. If you need to write multiple strings, then say what you mean:
 String myName;
 public static void main(String[] argv) {
 GoodNewline jack = new GoodNewline("Jack Adolphus Schmidt, III");
 jack.print(System.out);
 }
 protected void print(PrintStream out) {
 out.println(toString()); // classname and hashcode
 out.println(myName); // print name on next line
 }
Alternatively, if you need multiple lines, you could return an array or List of strings.
10.14 Reading/Writing Binary Data
Problem
You need to read or write binary data, as opposed to text.
10.14 Reading/Writing Binary Data | 331
Solution
Use a DataInputStream or DataOutputStream.
Discussion
The Stream classes have been in Java since the beginning of time and are optimal for
reading and writing bytes rather than characters. The data layer over them, compris‐
ing DataInputStream and DataOutputStream, is configured for reading and writing
binary values, including all of Java’s built-in types. Suppose that you want to write a
binary integer plus a binary floating-point value into a file and read it back later. This
code shows the writing part:
public class WriteBinary {
 public static void main(String[] argv) throws IOException {
 int i = 42;
 double d = Math.PI;
 String FILENAME = "binary.dat";
 DataOutputStream os = new DataOutputStream(
 new FileOutputStream(FILENAME));
 os.writeInt(i);
 os.writeDouble(d);
 os.close();
 System.out.println("Wrote " + i + ", " + d + " to file " + FILENAME);
 }
}
Should you need to write all the fields from an object, you should probably use one of
the methods described in Recipe 12.6.
10.15 Reading and Writing JAR or ZIP Archives
Problem
You need to create and/or extract from a JAR archive or a file in the well-known ZIP
archive format, as established by PkZip and used by Unix zip/unzip and WinZip.
Solution
You could use the jar program in the Java Development Kit because its file format is
identical to the ZIP format with the addition of the META-INF directory to contain
additional structural information. But because this is a book about programming,
you are probably more interested in the ZipFile and ZipEntry classes and the stream
classes to which they provide access.
332 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
6 There is no support for adding files to an existing archive, so make sure you put all the files in at once or be
prepared to re-create the archive from scratch.
Discussion
The class java.util.zip.ZipFile is not an I/O class per se, but a utility class that
allows you to read or write the contents of a JAR or ZIP-format file.6
 When construc‐
ted, it creates a series of ZipEntry objects, one to represent each entry in the archive.
In other words, the ZipFile represents the entire archive, and the ZipEntry repre‐
sents one entry, or one file that has been stored (and compressed) in the archive. The
ZipEntry has methods like getName(), which returns the name that the file had
before it was put into the archive, and getInputStream(), which gives you an Input
Stream that will transparently uncompress the archive entry by filtering it as you read
it. To create a ZipFile object, you need either the name of the archive file or a File
object representing it:
ZipFile zippy = new ZipFile(fileName);
To see whether a given file is present in the archive, you can call the getEntry()
method with a filename. More commonly, you’ll want to process all the entries; for
this, use the ZipFile object to get a list of the entries in the archive, in the form of an
Enumeration (see Recipe 7.6), as is done here:
Enumeration all = zippy.entries( );
while (all.hasMoreElements( )) {
 ZipEntry entry = (ZipEntry)all.nextElement( );
 ...
}
We can then process each entry as we wish. A simple listing program could be this:
if (entry.isDirectory( ))
 println("Directory: " + e.getName( ));
else
 println("File: " + e.getName( ));
A fancier version would extract the files. The program in Example 10-7 does both: it
lists by default, but with the -x (extract) switch, it actually extracts the files from the
archive.
Example 10-7. main/src/main/java/io/UnZip.java
public class UnZip {
 /** Constants for mode listing or mode extracting. */
 public static enum Mode {
 LIST,
 EXTRACT;
 }
10.15 Reading and Writing JAR or ZIP Archives | 333
 /** Whether we are extracting or just printing TOC */
 protected Mode mode = Mode.LIST;
 /** The ZipFile that is used to read an archive */
 protected ZipFile zippy;
 /** The buffer for reading/writing the ZipFile data */
 protected byte[] b = new byte[8092];
 /** Simple main program, construct an UnZipper, process each
 * .ZIP file from argv[] through that object.
 */
 public static void main(String[] argv) {
 UnZip u = new UnZip();
 for (int i=0; i<argv.length; i++) {
 if ("-x".equals(argv[i])) {
 u.setMode(Mode.EXTRACT);
 continue;
 }
 String candidate = argv[i];
 // System.err.println("Trying path " + candidate);
 if (candidate.endsWith(".zip") ||
 candidate.endsWith(".jar"))
 u.unZip(candidate);
 else System.err.println("Not a zip file? " + candidate);
 }
 System.err.println("All done!");
 }
 /** Set the Mode (list, extract). */
 protected void setMode(Mode m) {
 mode = m;
 }
 /** Cache of paths we've mkdir()ed. */
 protected SortedSet<String> dirsMade;
 /** For a given Zip file, process each entry. */
 public void unZip(String fileName) {
 dirsMade = new TreeSet<String>();
 try {
 zippy = new ZipFile(fileName);
 @SuppressWarnings("unchecked")
 Enumeration<ZipEntry> all = (Enumeration<ZipEntry>) zippy.entries();
 while (all.hasMoreElements()) {
 getFile((ZipEntry)all.nextElement());
 }
 } catch (IOException err) {
 System.err.println("IO Error: " + err);
 return;
 }
334 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 }
 protected boolean warnedMkDir = false;
 /** Process one file from the zip, given its name.
 * Either print the name, or create the file on disk.
 */
 protected void getFile(ZipEntry e) throws IOException {
 String zipName = e.getName();
 switch (mode) {
 case EXTRACT:
 if (zipName.startsWith("/")) {
 if (!warnedMkDir)
 System.out.println("Ignoring absolute paths");
 warnedMkDir = true;
 zipName = zipName.substring(1);
 }
 // if a directory, just return. We mkdir for every file,
 // since some widely used Zip creators don't put out
 // any directory entries, or put them in the wrong place.
 if (zipName.endsWith("/")) {
 return;
 }
 // Else must be a file; open the file for output
 // Get the directory part.
 int ix = zipName.lastIndexOf('/');
 if (ix > 0) {
 String dirName = zipName.substring(0, ix);
 if (!dirsMade.contains(dirName)) {
 File d = new File(dirName);
 // If it already exists as a dir, don't do anything
 if (!(d.exists() && d.isDirectory())) {
 // Try to create the directory, warn if it fails
 System.out.println("Creating Directory: " + dirName);
 if (!d.mkdirs()) {
 System.err.println(
 "Warning: unable to mkdir " + dirName);
 }
 dirsMade.add(dirName);
 }
 }
 }
 System.err.println("Creating " + zipName);
 FileOutputStream os = new FileOutputStream(zipName);
 InputStream is = zippy.getInputStream(e);
 int n = 0;
 while ((n = is.read(b)) >0)
 os.write(b, 0, n);
 is.close();
 os.close();
 break;
 case LIST:
10.15 Reading and Writing JAR or ZIP Archives | 335
 // Not extracting, just list
 if (e.isDirectory()) {
 System.out.println("Directory " + zipName);
 } else {
 System.out.println("File " + zipName);
 }
 break;
 default:
 throw new IllegalStateException("mode value (" + mode + ") bad");
 }
 }
}
See Also
People sometimes confuse the ZIP archive file format with the similarly named gzip
compression format. Gzip-compressed files can be read or written with the GZipIn
putStream and GZipOutputStream classes from java.io.
10.16 Finding Files in a Filesystem-Neutral Way with
getResource() and getResourceAsStream()
Problem
You want to load objects or files without referring to their absolute location in the
filesystem. You might want to do this for one of the following reasons:
• You are in a server (Java EE) environment.
• You want to be independent of file paths.
• You want to read a file in a unit test.
• You expect users to deploy the resource “somewhere” on the LASSPATH (possibly
even inside a JAR file).
Solution
Use getClass() or getClassLoader() and either getResource() or getResourceAs
Stream().
Discussion
There are three varieties of getResource() methods, some of which exist (with the
exact same signature) both in the Class class (see Chapter 17) and in the Class
Loader class (see Recipe 17.5). The methods in Class delegate to the ClassLoader, so
there is little difference between them. The methods are summarized in Table 10-6.
Table 10-6. The getResource* methods
Method signature In Class In ClassLoader
public InputStream getResourceAsStream(String); Y Y
public URL getResource(String); Y Y
public Enumeration<URL> getResources(String) throws IOException; N Y
The first method is designed to quickly and easily locate a resource, or file, on your
CLASSPATH. Using the Class version, or the other one with a standard ClassLoader
implementation, the resource can be a physical file or a file inside a JAR file. If you
define your own classloader, your imagination is the limit, as long as it can be repre‐
sented as an InputStream. This is commonly used as shown here:
InputStream is = getClass().getResourceAsStream("foo.properties");
// then do something with the InputStream...
The second form returns a URL, which can be interpreted in various ways (see the dis‐
cussion of reading from a URL in Recipe 12.1).
The third form, only usable with a ClassLoader instance, returns an Enumeration of
URL objects. This is intended to return all the resources that match a given string;
remember that a CLASSPATH can consist of pretty much any number of directories
and/or JAR files, so this will search all of them. This is useful for finding a series of
configuration files and merging them, perhaps. Or for finding out whether there is
more than one resource/file of a given name on your CLASSPATH.
Note that the resource name can be given as either a relative path or as an absolute
path. Assuming you are using Maven (see Recipe 1.7), then for the absolute path,
place the file relative to src/main/resources/ directory. For the relative path, place the
file in the same directory as your source code. The same rules apply in an IDE,
assuming you have made src/main/java and src/main/resources be treated as source
folders in your IDE configuration. The idea is that resource files get copied to your
CLASSPATH folder. For example, if you have two resource files, src/main/resources/
one.txt and src/main/java/MyPackage/two.txt, and your project is configured as
described, these two lines would work, if accessed from a program in MyPackage:
Class<?> c = getClass();
InputStream isOne = getResourceAsStream("/one.txt"); // note leading slash
InputStream isTwo = getResourceAsStream("two.txt"); // without leading slash
10.16 Finding Files in a Filesystem-Neutral Way with getResource() and getResourceAsStream() | 337
In either case, getResource() and getResourceAsStream() will
return null if they don’t find the resource; you should always check
for null to guard against faulty deployment. If it doesn’t find
anything matching, getResources() will return an empty
Enumeration.
If the file path has slashes between components (as in package/subpackage), the name
you path into any of the getResource methods should have a period in place of the
slash.
10.17 Getting File Information: Files and Path
Problem
You need to know all you can about a given file on disk.
Solution
Use java.nio.file.Files methods.
Discussion
The java.nio.file.Files class has a plural name both to differentiate it from the
legacy File class that it replaces and to remind us that it sometimes works on multi‐
ple files. There are two types of static methods in the Files class, information and
operational. The informational ones (see Table 10-7) simply give you information
about one file, such as boolean exists() or long size(). The operational ones (see
Table 10-8) either make changes to the filesystem or open a file for reading or writing.
Each of the operational ones can throw the checked exception IOException; only a
few of the informational ones can.
The vast majority of these methods have argument(s) of type java.nio.file.Path. A
Path represents a path into the filesystem, that is, a set of directories and possibly a
file, like “C:\Users\user\Downloads” or “/home/ian/Downloads”. The path may or
may not exist as a file on disk at the time you create a Path representing it. The Files
class can tell you whether the file represented by a given Path exists, can bring that
Path into being as a file or as a directory, and can either change the corresponding
file’s attributes or even destroy it if it does exist. Path objects are easily created with
Path.of(String name), which has several overloads.
Files in conjunction with Path offers pretty well everything you’d need to write a
full-blown file manager application, let alone the needs of a more typical application
338 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
needing file information and/or directory access. The Files class has a series of static
boolean methods that give basic information.
Table 10-7. Public static informational methods in java.nio.file.Files
Return type Method Notes
boolean exists(Path, LinkOption…);
Object getAttribute(Path, String, LinkOption…);
<V extends FileAttributeView> V getFileAttributeView(Path, Class<V>,
LinkOption…);
FileTime getLastModifiedTime(Path, LinkOption…);
UserPrincipal getOwner(Path, LinkOption…);
Set<PosixFilePermission> getPosixFilePermissions(Path, LinkOption…);
boolean isDirectory(Path, LinkOption…);
boolean isExecutable(Path); If Executable by current user
boolean isHidden(Path); If a “dot file” on Unix, or “hidden”
attribute set on some OSes
boolean isReadable(Path); If Readable by current user
boolean isRegularFile(Path, LinkOption…);
boolean isSameFile(Path, Path) throws IOException; Has to unwind filesys complexities like
“..”, symlinks, …
boolean isSymbolicLink(Path);
boolean isWritable(Path); If Writable by current user
long mismatch(Path, Path);
boolean notExists(Path, LinkOption…);
String probeContentType(Path) throws IOException; Tries to return MIME type of data
Path readSymbolicLink(Path) throws IOException;
long size(Path);
By “current user” we mean the account under which the current JVM instance is
being run.
Most of these methods are demonstrated in Example 10-8.
Example 10-8. main/src/main/java/io/FilesInfos.java
 println("exists", Files.exists(Path.of("/")));
 println("isDirectory", Files.isDirectory(Path.of("/")));
 println("isExecutable", Files.isExecutable(Path.of("/bin/cat")));
 println("isHidden", Files.isHidden(Path.of("~/.profile")));
 println("isReadable", Files.isReadable(Path.of("lines.txt")));
 println("isRegularFile", Files.isRegularFile(Path.of("lines.txt")));
 println("isSameFile", Files.isSameFile(Path.of("lines.txt"),
 Path.of("../main/lines.txt")));
10.17 Getting File Information: Files and Path | 339
 println("isSymbolicLink", Files.isSymbolicLink(Path.of("/var")));
 println("isWritable", Files.isWritable(Path.of("/tmp")));
 println("isDirectory", Files.isDirectory(Path.of("/")));
 println("notexists",
 Files.notExists(Path.of("no_such_file_as_skjfsjljwerjwj")));
 println("probeContentType", Files.probeContentType(Path.of("lines.txt")));
 println("readSymbolicLink", Files.readSymbolicLink(Path.of("/var")));
 println("size", Files.size(Path.of("lines.txt")));
Obviously the paths chosen are somewhat system-specific, but when run on my Unix
system, the boolean methods all returned true, and the last three returned this:
probeContentType returned text/plain
readSymbolicLink returned private/var
size returned 78
Table 10-8 shows the methods that make changes to filesystem entities.
Table 10-8. Public static operational methods in java.nio.file.Files
Return type Method
long copy(InputStream, Path, CopyOption…);
long copy(Path, OutputStream);
Path copy(Path, Path, CopyOption…);
Path createDirectories(Path, FileAttribute<?>…);
Path createDirectory(Path, FileAttribute<?>…);
Path createFile(Path, FileAttribute<?>…);
Path createLink(Path, Path);
Path createSymbolicLink(Path, Path, FileAttribute<?>…);
Path createTempDirectory(Path, String, FileAttribute<?>…);
Path createTempDirectory(String, FileAttribute<?>…);
Path createTempFile(Path, String, String, FileAttribute<?>…);
Path createTempFile(String, String, FileAttribute<?>…);
void delete(Path);
boolean deleteIfExists(Path);
Stream<Path> find(Path, int, BiPredicate<Path, BasicFileAttributes>, FileVisitOption…);
Stream<String> lines(Path);
Stream<String> lines(Path, Charset);
Stream<Path> list(Path);
Path move(Path, Path, CopyOption…);
BufferedReader newBufferedReader(Path);
BufferedReader newBufferedReader(Path, Charset);
BufferedWriter newBufferedWriter(Path, Charset, OpenOption…);
BufferedWriter newBufferedWriter(Path, OpenOption…);
340 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
Return type Method
SeekableByteChannel newByteChannel(Path, OpenOption…);
SeekableByteChannel newByteChannel(Path, Set<? extends OpenOption>, FileAttribute<?>…);
DirectoryStream<Path> newDirectoryStream(Path);
DirectoryStream<Path> newDirectoryStream(Path, String);
InputStream newInputStream(Path, OpenOption…);
OutputStream newOutputStream(Path, OpenOption…);
byte[] readAllBytes(Path);
List<String> readAllLines(Path);
List<String> readAllLines(Path, Charset);
<A extends BasicFileAttributes> A readAttributes(Path, Class<A>, LinkOption…);
Map<String, Object> readAttributes(Path, String, LinkOption…);
String readString(Path);
String readString(Path, Charset);
Path setAttribute(Path, String, Object, LinkOption…);
Path setLastModifiedTime(Path, FileTime);
Path setOwner(Path, UserPrincipal);
Path setPosixFilePermissions(Path, Set<PosixFilePermission>);
Path write(Path, Iterable<? extends CharSequence>, Charset, OpenOption…);
Path write(Path, Iterable<? extends CharSequence>, OpenOption…);
Path write(Path, byte[], OpenOption…);
Path writeString(Path, CharSequence, Charset, OpenOption…);
Path writeString(Path, CharSequence, OpenOption…);
Path is an interface whose implementation is provided by a provider class called File
system. Path has many methods, listed in Table 10-9.
Table 10-9. Public static operational methods in java.nio.file.Path
Access Return type Method
static Path of(String, String…);
static Path of(URI);
abstract FileSystem getFileSystem();
abstract boolean isAbsolute();
abstract Path getRoot();
abstract Path getFileName();
abstract Path getParent();
abstract int getNameCount();
abstract Path getName(int);
abstract Path subpath(int, int);
10.17 Getting File Information: Files and Path | 341
Access Return type Method
abstract boolean startsWith(Path);
default boolean startsWith(String);
abstract boolean endsWith(Path);
default boolean endsWith(String);
abstract Path normalize();
abstract Path resolve(Path);
default Path resolve(String);
default Path resolveSibling(Path);
default Path resolveSibling(String);
abstract Path relativize(Path);
abstract URI toUri();
abstract Path toAbsolutePath();
abstract Path toRealPath(LinkOption…) throws IOException;
default File toFile();
abstract WatchKey register(WatchService, WatchEvent$Kind<?>[], WatchEvent$Modifier…)
throws IOException;
default WatchKey register(WatchService, WatchEvent$Kind<?>…) throws IOException;
default Iterator<Path> iterator();
abstract int compareTo(Path);
abstract boolean equals(Object);
abstract int hashCode();
abstract String toString();
default int compareTo(Object);
To find the information about one file, you can use the informational methods in
Files and Path, as shown in Example 10-9.
Example 10-9. main/src/main/java/dir_file/FileStatus.java (getting file information)
public class FileStatus {
 public static void main(String[] argv) throws IOException {
 // Ensure that a filename (or something) was given in argv[0]
 if (argv.length == 0) {
 System.err.println("Usage: FileStatus filename");
 System.exit(1);
 }
 for (String a : argv) {
 status(a);
 }
 }
342 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 public static void status(String fileName) throws IOException {
 System.out.println("---" + fileName + "---");
 // Construct a Path object for the given file.
 Path p = Path.of(fileName);
 // See if it actually exists
 if (!Files.exists(p)) {
 System.out.println("file not found");
 System.out.println(); // Blank line
 return;
 }
 // Print full name
 System.out.println("Canonical name " + p.normalize());
 // Print parent directory if possible
 Path parent = p.getParent();
 if (parent != null) {
 System.out.println("Parent directory: " + parent);
 }
 // Check if the file is readable
 if (Files.isReadable(p)) {
 System.out.println(fileName + " is readable.");
 }
 // Check if the file is writable
 if (Files.isWritable(p)) {
 System.out.println(fileName + " is writable.");
 }
 // See if file, directory, or other. If file, print size.
 if (Files.isRegularFile(p)) {
 // Report on the file's size and possibly its type
 System.out.printf("File size is %d bytes, content type %s\n",
 Files.size(p),
 Files.probeContentType(p));
 } else if (Files.isDirectory(p)) {
 System.out.println("It's a directory");
 } else {
 System.out.println("I dunno! Neither a file nor a directory!");
 }
 // Report on the modification time.
 final FileTime d = Files.getLastModifiedTime(p);
 System.out.println("Last modified " + d);
 System.out.println(); // blank line between entries
 }
When run on MS Windows with the three arguments shown, it produces this output:
C:\javasrc\dir_file>java dir_file.FileStatus / /tmp/id /autoexec.bat
---/---
Canonical name C:\
File is readable.
10.17 Getting File Information: Files and Path | 343
File is writable.
Last modified 1970-01-01T00:00:00.00000Z
It's a directory
---/tmp/id---
file not found
---/autoexec.bat---
Canonical name C:\AUTOEXEC.BAT
Parent directory: \
File is readable.
File is writable.
Last modified 2019-10-13T12:43:05.123918Z
File size is 308 bytes.
As you can see, the so-called canonical name not only includes a leading directory
root of C:\, but also has had the name converted to uppercase. You can tell I ran that
on Windows. That version of Windows did not maintain timestamps on directories;
the value 0L gets interpreted as January 1, 1970 (not accidentally the same time base
as used on Unix since that time). On Unix, it behaves differently:
$ java dir_file.FileStatus / /tmp/id /autoexec.bat
---/---
Canonical name /
File is readable.
It's a directory
Last modified 2019-12-16T01:14:05.226108Z
---/tmp/id---
Canonical name /tmp/id
Parent directory: /tmp
File is readable.
File is writable.
File size is 36768 bytes, content type null
Last modified 2019-12-21T18:46:27.402108Z
---/autoexec.bat---
file not found
$
A typical Unix system has no autoexec.bat file. And Unix filenames (like those on a
Mac) can consist of upper- and lowercase characters: what you type is what you get.
Legacy compatibility
To use a Path with legacy code that needs the older java.io.File, simply use File
oldType = Path.toFile():
jshell> Path p = Path.of("/");
p ==> /
344 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
jshell> File f = p.toFile();
f ==> /
To go the other way, the File class has been retrofitted with a toPath() method:
jshell> File f = new File("/");
f ==> /
jshell> Path p = f.toPath();
p ==> /
10.18 Creating a New File or Directory
Problem
You need to create a new file on disk but not write any data into it; you need to create
a directory before you can create files in it.
Solution
For an empty file, use a java.nio.file.Files object’s createFile(Path) method.
Use the Files class’s createDirectory() or createDirectories() method to create
a directory.
Discussion
Files
You could easily create a new file by constructing a FileOutputStream or FileWriter
(see Recipe 12.6). But then you’d have to remember to close it as well. Sometimes you
want a file to exist, but you don’t want to bother putting anything into it. This might
be used, for example, as a simple form of interprogram communication: one program
could test for the presence of a file and interpret that to mean that the other program
has reached a certain state. Example 10-10 is code that simply creates an empty file
for each name you give.
Example 10-10. main/src/main/java/dir_file/Creat.java (creation of a file on disk)
/** Create file(s) by name. Final "e" omitted in homage to UNIX system call. */
public class Creat {
 public static void main(String[] argv) throws IOException {
 // Ensure that a filename (or something) was given in argv[0]
 if (argv.length == 0) {
 throw new IllegalArgumentException("Usage: Creat filename [...]");
 }
10.18 Creating a New File or Directory | 345
 for (String arg : argv) {
 // Constructing a Path object doesn't affect the disk, but
 // the Files.createFile() method does.
 final Path p = Path.of(arg);
 final Path created = Files.createFile(p);
 System.out.println(created);
 }
 }
}
java.nio.file.createFile() has an overload that takes a second argument of type
OpenOption. This is an empty interface that is implemented by the StandardOpenOp
tion enumeration. These options are listed in Table 10-5.
Directories
Of the two methods used for creating directories, createDirectory() creates just
one directory, whereas createDirectories() creates any intermediate directories
that are needed. For example, if /home/ian exists and is a directory, the call
shell> Files.createDirectory(Path.of("/Users/ian/abc"))
$11 ==> /Users/ian/abc
will succeed (unless the directory is already there), but the call
jshell> Files.createDirectory(Path.of("/Users/ian/once/twice/again"))
will fail with a java.nio.file.NoSuchFileException because the directory named
once does not exist. To create this path of directories, as you might expect by now, use
createDirectories() (plural):
jshell> Files.createDirectories(Path.of("/Users/ian/once/twice/again"))
$14 ==> /Users/ian/once/twice/again
Both variants return a Path object referring to the new directory if they succeed and
throw an exception if they fail. Notice that it is possible (but not likely) for createDir
ectories() to create some of the directories and then fail; in this case, the newly cre‐
ated directories are left in the filesystem.
10.19 Changing a File’s Name or Other Attributes
Problem
You need to change a file’s name on disk or some of its other attributes, such as set‐
ting the file to read-only or changing its modification time.
346 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
Solution
To change the name (or location), use a java.nio.file.Files static move() method.
For other attributes, use setLastModifiedTime() to change the timestamp, or one of
several other setters for mode or permission attributes.
Discussion
Similar to the Unix command line, there is no separate rename operation; the move
methods provide all functions for putting a file somewhere else, whether that is to the
same name in a different directory, a different name in the same directory, or a differ‐
ent name on a different disk or filesystem. Accordingly, the Files.move() method
requires two Path objects, one referring to the existing file and another referring to
the new name. Then call the Files.move() method, passing both path objects, first
the existing and then the desired name. This is easier to see than to explain, so here
goes:
public class Rename {
 public static void main(String[] argv) throws IOException {
 // Construct the Path object. Does NOT create a file on disk!
 final Path p = Path.of("MyCoolDocument"); // The file we will rename
 // Setup for the demo: create a new "old" file
 final Path oldName = Files.exists(p) ? p : Files.createFile(p);
 // Rename the backup file to "mydoc.bak"
 // Renaming requires a Path object for the target.
 final Path newName = Path.of("mydoc.bak");
 Files.deleteIfExists(newName); // In case previous run left it there
 Path p2 = Files.move(oldName, newName);
 System.out.println(p + " renamed to " + p2);
 }
}
For changing the attributes, there are several methods available, listed in Table 10-10.
Each of these has a return value of type boolean, with true meaning success.
Table 10-10. Files attribute setters
Method signature Description
setExecutable(boolean executable) Convenience method to set owner’s execute permission for
this file
setExecutable(boolean executable,
boolean ownerOnly)
Sets the owner’s or everybody’s execute permission for this
file
setLastModified(long time) Sets the last-modified time of the file or directory that this file
names
10.19 Changing a File’s Name or Other Attributes | 347
Method signature Description
setReadable(boolean readable) Convenience method to set owner’s read permission for this
file
setReadable(boolean readable, boolean
ownerOnly)
Sets the owner’s or everybody’s read permission for this file
setReadOnly() Convenience for setReadable(false)
setWritable(boolean writable) A convenience method to set the owner’s write permission for
this file
setWritable(boolean writable, boolean
ownerOnly)
Set owner’s or everybody’s write permission for this file
For the methods that take two arguments, the first enables or disables the feature on
the given file that matches the method name, and the second controls whether the
operation applies to the owner only or to everyone. The second argument is ignored
if the file lives on a filesystem that doesn’t support multiuser permissions or if the
operating system doesn’t support that. All the methods described in this recipe return
true if they succeed and false otherwise.
For example, boolean setReadable(boolean readable, boolean ownerOnly) lets
you specify who can read the given file. The readable argument is true or false
depending on whether you want it readable or not. The ownerOnly argument tries to
extend the readability choice to all users on a multiuser operating system, and is
ignored if not applicable.
setLastModified() allows you to play games with the modification time of a file.
This is normally not a good game to play, but it is useful in some types of backup/
restore programs. This method takes an argument that is the number of milliseconds
(not seconds) since the beginning of Unix time (January 1, 1970). You can get the
original value for the file by calling getLastModified() (see Recipe 10.17), or you
can get the value for a given date by calling the ZonedDateTime’s toInstant().getE
pochSecond() method (see Recipe 6.3) and multiplying by 1,000 to convert seconds
to milliseconds.
I encourage you to explore the operation of these methods using JShell (see Recipe
1.4). I’d suggest having a second window in which you can run ls -l or dir com‐
mands to see how the file is affected. Example 10-11 shows some of these methods
being explored in JShell.
Example 10-11. Exploring Files
jshell> var f = File.createTempFile("foo", "bar");
f ==> /tmp/foo9391300789087780984bar
jshell> f.createNewFile();
$4 ==> false
348 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
jshell> f.setReadOnly();
$5 ==> true
jshell> f.canRead();
$6 ==> true
jshell> f.canWrite();
$7 ==> false
jshell> f.setReadable(true);
$8 ==> true
jshell> f.canWrite();
$9 ==> false
jshell> f.setReadable(false, false);
$10 ==> true
jshell> f.canWrite();
$11 ==> false
10.20 Deleting a File
Problem
You need to delete one or more files from the disk.
Solution
Use java.nio.file.Files object’s delete(Path) or deleteIfExists(Path) method.
These delete the files referred to by the Path argument (subject of course to permis‐
sions) and directories (subject to permissions and to the directory being empty).
Discussion
This is not complicated. Simply construct a Path object for the file you wish to delete,
and call the static Files.delete() method:
public class Delete {
 public static void main(String[] argv) throws IOException {
 // Construct a File object for the backup created by editing
 // this source file. The file probably already exists.
 // Some text editors create backups by putting ~ at end of filename.
 File bkup = new File("Delete.java~");
 // Now, delete it:
 bkup.delete();

 }
}
Recall the caveat about permissions in the introduction to this chapter: if you don’t
have permission, you can get a return value of false or, possibly, a SecurityExcep
tion. Note also that there are some differences between platforms. Some versions of
Windows allow Java to remove a read-only file, but Unix does not allow you to
remove a file unless you have write permission on the directory it’s in. Nor does Unix
allow you to remove a directory that isn’t empty (there is even an exception, Director
yNotEmptyException, for the latter case). Here is a version of Delete with reporting
of success or failure:
public class Delete2 {
 static boolean hard = false; // True for delete, false for deleteIfExists
 public static void main(String[] argv) {
 for (String arg : argv) {
 if ("-h".equals(arg)) {
 hard = true;
 continue;
 }
 delete(arg);
 }
 }
 public static void delete(String fileName) {
 // Construct a File object for the file to be deleted.
 final Path target = Path.of(fileName);
 // Now, delete it:
 if (hard) {
 try {
 System.out.print("Using Files.delete(): ");
 Files.delete(target);
 System.err.println("** Deleted " + fileName + " **");
 } catch (IOException e) {
 System.out.println("Deleting " + fileName + " threw " + e);
 }
 } else {
 try {
 System.out.print("Using deleteIfExists(): ");
 if (Files.deleteIfExists(target)) {
 System.out.println("** Deleted " + fileName + " **");
 } else {
 System.out.println(
 "Deleting " + fileName + " returned false.");
 }
 } catch (IOException e) {
 System.out.println("Deleting " + fileName + " threw " + e);
 }
350 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 }
 }
}
The -h option allows this program to switch between delete() and deleteIfEx
ists(); you can see the difference by running it on things that exist, don’t exist, and
are not empty, using both methods. The output looks something like this on my Unix
box:
$ ls -ld ?
-rw-r--r-- 1 ian 512 0 Dec 21 16:35 a
drwxr-xr-x 2 ian 512 64 Dec 21 16:35 b
drwxr-xr-x 3 ian 512 96 Dec 21 16:22 c
$ java -cp target/classes dir_file.Delete2 a b c d
Using deleteIfExists(): ** Deleted a **
Using deleteIfExists(): ** Deleted b **
Using deleteIfExists(): Deleting c threw
 java.nio.file.DirectoryNotEmptyException: c
Using deleteIfExists(): Deleting d returned false.
# Here I put the files back the way they were, then run again with -h
$ java -cp target/classes dir_file.Delete2 -h a b c d
Using Files.delete(): ** Deleted a **
Using Files.delete(): ** Deleted b **
Using Files.delete(): Deleting c threw
 java.nio.file.DirectoryNotEmptyException: c
Using Files.delete(): Deleting d threw java.nio.file.NoSuchFileException: d
$ ls -l c
total 2
drwxr-xr-x 2 ian ian 512 Oct 8 16:50 d
$ java dir_file.Delete2 c/d c
Using deleteIfExists(): ** Deleted c/d **
Using deleteIfExists(): ** Deleted c **
$
10.21 Creating a Transient/Temporary File
Problem
You need to create a file with a unique temporary filename and/or or arrange for a file
to be deleted when your program is finished.
Solution
Use the java.nio.file.Files createTempFile() or createTempDirectory()
method. Use one of several methods to ensure your file is deleted on exit.
10.21 Creating a Transient/Temporary File | 351
Discussion
The Files class has static methods for creating temporary files and directories. Note
that a temporary file in this context is not deleted automatically; it is simply created in
a directory that is set aside for temporary files on that operating system (e.g., /tmp on
Unix). Here are the methods for creating tempory files and directories:
Path createTempFile(Path dir, String prefix, String sux, FileAttribute<?>… attrs)
Creates a new empty file in the specified directory, using the given prefix and suf‐
fix strings to generate its name
Path createTempFile(String prefix, String sux, FileAttribute<?>… attrs)
Creates an empty file in the default temporary-file directory, using the given pre‐
fix and suffix to generate its name
Path createTempDirectory(Path dir, String prefix, FileAttribute<?>… attrs)
Creates a new directory in the specified directory, using the given prefix to gener‐
ate its name
Path createTempDirectory(String prefix, FileAttribute<?>… attrs)
Creates a new directory in the default temporary-file directory, using the given
prefix to generate its name
The file attributes are discussed in the sidebar “Understanding I/O Options: Standar‐
dOpenOptions, FileAttribute, PosixFileAttribute, and More” on page 321.
There are various ways to arrange for a file to be deleted automatically. One is to use
the legacy java.io.File class, which has a explicit deleteOnExit() method. This
arranges for any file (no matter how it was created) to be deleted if it still exists when
the program exits. Here we arrange for a backup copy of a program to be deleted on
exit, and we also create a temporary file and arrange for it to be removed on exit.
Both files are gone after the program runs:
public class TempFiles {
 public static void main(String[] argv) throws IOException {
 // 1. Making an existing file temporary
 // Construct a File object for the backup created by editing
 // this source file. The file probably already exists.
 // My editor creates backups by putting ~ at the end of the name.
 File bkup = new File("Rename.java~");
 // Arrange to have it deleted when the program ends.
 bkup.deleteOnExit();
 // 2. Create a new temporary file.
 // Make a file object for foo.tmp, in the default temp directory
 Path tmp = Files.createTempFile("foo", "tmp");
 // Report on the filename that it made up for us.
352 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 System.out.println("Your temp file is " + tmp.normalize());
 // Arrange for it to be deleted at exit.
 tmp.toFile().deleteOnExit();
 // Now do something with the temporary file, without having to
 // worry about deleting it later.
 writeDataInTemp(tmp);
 }
 public static void writeDataInTemp(Path tempFile) throws IOException {
 // This version is dummy. Use your imagination.
 Files.writeString(tempFile, "This is a temp file");
 }
}
When run on a Unix system, this program looked like this, proving that the file was
created but removed when the JVM exited:
$ java TempFiles.java
Your temp file is /tmp/foo8423321910215054689tmp
$ ls -l /tmp/foo8423321910215054689tmp
ls: /tmp/foo8423321910215054689tmp: No such file or directory
$
The createTempFile() method is like createNewFile() (see Recipe 10.18) in that it
does create the file. Also be aware that, should the Java Virtual Machine terminate
abnormally, the deletion probably will not occur. There is no way to undo the setting
of deleteOnExit() short of renaming the file or something drastic like powering off
the computer before the program exits.
Another way to arrange for any file to be deleted when you are finished with it is to
create it with the DELETE_ON_CLOSE option (see Table 10-5) so it will be deleted when
you close the file.
A third, less likely method is to instead use a JVM shutdown hook. DELETE_ON_CLOSE
is probably the best option, particularly in a long-running application, like most
server-side apps. In these situations, the server could be running for weeks, months,
or even years. In the meantime all the temp files would accumulate and the JVM
would accumulate a large list of deferred work that it needs to perform upon shut‐
down. You’d probably run out of disk space or server memory or some other
resource. For most long-running apps of this kind, it’s better to use DELETE_ON_CLOSE
or even the explicit delete() operation. Another alternative is to use a scheduler ser‐
vice to periodically trigger removal of old temporary files.
10.22 Listing a Directory
Problem
You need to list the filesystem entries named in a directory.
10.22 Listing a Directory | 353
Solution
Use the java.nio.file.Files static method Stream<Path> list(Path dir), pass‐
ing the Path representing the directory.
Discussion
The java.nio.file.Files class contains several methods for working with directo‐
ries. If you just want to list the contents of a directory, use its list(Path) method.
For example, to list the filesystem entities named in the current directory, just write
the following:
Files.list(Path.of(".")).forEach(System.out::println);
This can become a complete program with as little as the following code. Note that on
many systems the Path objects are returned in the order they occur in the directory,
which isn’t sorted. In this simple example we use the Stream.sorted() method to
order the entries alphabetically:
public class Ls {
 public static void main(String args[]) throws IOException {
 Files.list(Path.of("."))
 .sorted()
 .forEach(dir -> {
 System.out.println(dir);
 });
 }
}
Of course, there’s lots of room for elaboration. You could print the names in multiple
columns across the page. Or even down the page because you know the number of
items in the list before you print. You could omit filenames with leading periods, as
does the Unix ls program. Or print the directory names first; I once used a directory
lister called lc that did this, and I found it quite useful.
If you want to process the directory recursively, you should not check each entry to
see if it’s a file or directory and recurse on directories. Instead, you should use one of
the walk() or walkFileTree() methods discussed in Recipe 10.26; these handle
recursion for you. There is also a set of Files.newDirectoryStream() methods, with
and without filter callbacks and other arguments, that return a Directory
Stream<Path>.
10.23 Getting the Directory Roots
Problem
You want to know about the top-level directories, such as C:\ and D:\ on Windows.
354 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
Solution
Use the static method FileSystems.getDefault().getRootDirectories(), which
returns an Iterable of Path objects, one for each root directory. You can print them
or do other operations on them.
Discussion
Operating systems differ in how they organize filesystems out of multiple disk drives
or partitions. Microsoft Windows has a low-level device-oriented approach in which
each disk drive has a root directory named A:\ for the first floppy (if you still have
one!), C:\ for the first hard drive, and other letters for CD-ROM and network drives.
This approach requires you to know the physical device that a file is on. Unix, Linux,
and macOS have a high-level approach with a single root directory /; and different
disks or partitions are mounted, or connected, into a single unified tree. This
approach sometimes requires you to figure out where a device file is mounted. Per‐
haps neither is easier, though the Unix approach is a bit more consistent. Either way,
Java makes it easy for you to get a list of the roots.
The static method FileSystems.getDefault().getRootDirectories() returns an
Iterable<Path> containing the available filesystem roots for whatever platform you
are running on. Here is a short program to list these:
FileSystems.getDefault().getRootDirectories().forEach(System.out::println);
C:> java dir_file.DirRoots
A:\
C:\
D:\
C:>
As you can see, the program listed my floppy drive (even though the floppy drive was
not only empty, but left at home while I wrote this recipe on my notebook computer
in my car in a parking lot), the hard disk drive, and the CD-ROM drive.
On Unix there is only one root directory:
$ java dir_file.DirRoots
/
$
One thing that is left out of the list of roots is the so-called UNC filename. UNC file‐
names are used on some Microsoft platforms to refer to a network-available resource
that hasn’t been mounted locally on a particular drive letter. If your system still uses
these, be aware they will not show up in the listDirectoryRoots() output.
10.23 Getting the Directory Roots | 355
10.24 Using the FileWatcher Service to Get Notified About
File Changes
Problem
You want to be notified when some other application updates one or more of the files
in which you are interested.
Solution
Use the java.nio.file.FileWatchService to get notified of changes to files auto‐
matically, instead of having to examine the files periodically.
Discussion
It is fairly common for a large application to want to be notified of changes to files,
without having to go and look at them periodically. For example, a Java Enterprise
web server wants to know when Servlets and other components get updated. An IDE
wants to know when files were modified by an external editor or a build script. Many
modern operating systems have had this capability for some time, and now it is avail‐
able in Java.
These are the basic steps to using the FileWatchService:
1. Create a Path object representing the directory you want to watch.
2. Get a WatchService by calling, for example, FileSystems.getDefault().new
WatchService().
3. Create an array of Kind enumerations for the things you want to watch (in our
example we watch for files being created or modified).
4. Register the WatchService and the Kind array onto the Path object.
5. From then on, you wait for the watcher to notify you. A typical implementation
is to enter a while (true) loop calling the WatchService’s take() method to get
an event and interpret the events to figure out what just happened.
Example 10-12 is a program that does just that. In addition, it starts another thread to
actually do some filesystem operations so that you can see the WatchService
operating.
Example 10-12. main/src/main/java/nio/FileWatchServiceDemo.java
public class FileWatchServiceDemo {
 final static String TEMP_DIR_PATH = "/tmp";
356 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 static final String FILE_SEMA_FOR = "MyFileSema.for";
 final static Path SEMAPHORE_PATH = Path.of(TEMP_DIR_PATH ,FILE_SEMA_FOR);
 static volatile boolean done = false;
 final static ExecutorService threadPool = Executors.newSingleThreadExecutor();
 public static void main(String[] args) throws Throwable {
 String tempDirPath = "/tmp";
 System.out.println("Starting watcher for " + tempDirPath);
 System.out.println("Semaphore file is " + SEMAPHORE_PATH);
 Path p = Paths.get(tempDirPath);
 WatchService watcher =
 FileSystems.getDefault().newWatchService();
 Kind<?>[] watchKinds = { ENTRY_CREATE, ENTRY_MODIFY };
 p.register(watcher, watchKinds);
 threadPool.submit(new DemoService());
 while (!done) {
 WatchKey key = watcher.take();
 for (WatchEvent<?> e : key.pollEvents()) {
 System.out.println(
 "Saw event " + e.kind() + " on " +
 e.context());
 if (e.context().toString().equals(FILE_SEMA_FOR)) {
 System.out.println("Semaphore found, shutting down watcher");
 done = true;
 }
 }
 if (!key.reset()) {
 System.err.println("WatchKey failed to reset!");
 }
 }
 }
 /**
 * Nested class whose only job is to wait a while, create a file in
 * the monitored directory, and then go away.
 */
 private final static class DemoService implements Runnable {
 public void run() {
 try {
 Thread.sleep(1000);
 System.out.println("DemoService: Creating file");
 Files.deleteIfExists(SEMAPHORE_PATH); // clean up from previous run
 Files.createFile(SEMAPHORE_PATH);
 Thread.sleep(1000);
 System.out.println("DemoService: Shutting down");
 } catch (Exception e) {
 System.out.println("Caught UNEXPECTED " + e);
 }
 }
 }
}
10.24 Using the FileWatcher Service to Get Notified About File Changes | 357
10.25 Program: Save User Data to Disk
Problem
You need to save user data to disk in a Java application. This may be in response to
File→Save in a GUI application, saving the file in a text editor, or saving configura‐
tion data in a non-GUI application. You have heard (correctly) that a well-behaved
application should never lose data.
Solution
Use this five-step plan, with appropriate variations:
1. Create a temporary file; arrange for it to be removed automatically with deleteO
nExit(true).
2. Write the user data to this file. Data format translation errors, if any, will be
thrown during this process, leaving the previous version of the user’s data file
intact.
3. Delete the backup file if it exists.
4. Rename the user’s previous file to *.bak.
5. Rename the temporary file to the saved file.
Discussion
As developers, we have to deal with the fact that saving a file to disk is full of risk.
There are many things that can go wrong in saving data, yet it is one of the most criti‐
cal parts of most applications. If you lose data that a person has spent hours input‐
ting, or even lose a setting that a user feels strongly about, she will despise your whole
application. The disk might fill up while we’re writing it, or it might be full before we
start. This is a user’s error, but we have to face it. So here’s a more detailed discussion
of the little five-step dance we should go through:
1. Create a temporary file that we will write to. Set this file to deleteOnExit(true)
so that if we fail in a later step we don’t clutter the disk. Because we are later going
to rename this file to become the user’s real file, and we don’t want to run out of
disk space during the rename, it is important that we create the file on the same
disk drive partition (drive letter or mount point) as the user’s real file; otherwise
the rename will silently morph into a copy-and-delete, which could fail due to
lack of disk space. See Recipe 10.21 for methods of deleting a file on exit.
2. Write the user data to this new temporary file. If we are transforming data—say,
getting it from a JDBC ResultSet or writing objects using a XML transformer—an
358 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
exception could be thrown. If we’re not careful, these exceptions can cause the
user’s data to be lost.
3. Delete the backup file if it exists. First time we do this it won’t exist; after that it
probably will. Be prepared either way.
4. Rename the user’s previous file to *.bak_.
5. Rename the temporary file to the save file.
This may seem like overkill, but it prevents career kill. I’ve done pretty much this in
numerous apps with various save file formats. This plan is the only really safe way
around all the problems that can occur. For example, the final step has to be a rename
not a copy, regardless of size considerations, to avoid the problem of the disk filling
up. So, to be correct, you have to ensure that the temp file gets created on the same
disk partition (drive letter or mount point) as the user’s file.
This is the basic plan to use the FileSaver:
• Instantiate it by calling the constructor.
• Call the getWriter() or getOutputStream() method.
• Use the output file to write the data.
• Call finish() on the FileSaver object.
main/src/main/java/com/darwinsys/io/FileSaver.java
// package com.darwinsys.io;
public class FileSaver {
 private enum State {
 /** The state before and after use */
 AVAILABLE,
 /** The state while in use */
 INUSE
 }
 private State state;
 private final Path inputFile;
 private final Path tmpFile;
 private final Path backupFile;
 private OutputStream mOutputStream;
 private Writer mWriter;
 public FileSaver(Path inputFile) throws IOException {
 // Step 1: Create temp file in right place; must be on same disk
 // as the original file, to avoid disk-full troubles later.
 this.inputFile = inputFile;
 tmpFile = Path.of(inputFile.normalize() + ".tmp");
10.25 Program: Save User Data to Disk | 359
 Files.createFile(tmpFile);
 tmpFile.toFile().deleteOnExit();
 backupFile = Path.of(inputFile.normalize() + ".bak");
 state = State.AVAILABLE;
 }
 /**
 * Return a reference to the contained File object, to
 * promote reuse (File objects are immutable so this
 * is at least moderately safe). Typical use would be:
 * <pre>
 * if (fileSaver == null ||
 * !(fileSaver.getFile().equals(file))) {
 * fileSaver = new FileSaver(file);
 * }
 * </pre>
 * @return the File object for the file to be saved
 */
 public Path getFile() {
 return inputFile;
 }
 /** Return an output file that the client should use to
 * write the client's data to.
 * @return An OutputStream, which should be wrapped in a
 * buffered OutputStream to ensure reasonable performance.
 * @throws IOException if the temporary file cannot be written
 */
 public OutputStream getOutputStream() throws IOException {
 if (state != State.AVAILABLE) {
 throw new IllegalStateException("FileSaver not opened");
 }
 mOutputStream = Files.newOutputStream(tmpFile);
 state = State.INUSE;
 return mOutputStream;
 }
 /** Return an output file that the client should use to
 * write the client's data to.
 * @return A BufferedWriter to write on the new file.
 * @throws IOException if the temporary file cannot be written
 */
 public Writer getWriter() throws IOException {
 if (state != State.AVAILABLE) {
 throw new IllegalStateException("FileSaver not opened");
 }
 mWriter = Files.newBufferedWriter(tmpFile);
 state = State.INUSE;
 return mWriter;
 }
360 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 /** Close the output file and rename the temp file to the original name.
 * @throws IOException If anything goes wrong
 */
 public void finish() throws IOException {
 if (state != State.INUSE) {
 throw new IllegalStateException("FileSaver not in use");
 }
 // Ensure both are closed before we try to rename.
 if (mOutputStream != null) {
 mOutputStream.close();
 }
 if (mWriter != null) {
 mWriter.close();
 }
 // Delete the previous backup file if it exists.
 Files.deleteIfExists(backupFile);
 // Rename the user's previous file to itsName.bak,
 // UNLESS this is a new file.
 if (Files.exists(inputFile) &&
 Files.move(inputFile, backupFile) == null) {
 throw new IOException(
 "Could not rename file to backup file " + backupFile);
 }
 // Rename the temporary file to the save file.
 if (Files.move(tmpFile, inputFile) == null) {
 throw new IOException("Could not rename temp file to save file");
 }
 state = State.AVAILABLE;
 }
}
Acknowledgments
The code in this program is my own, based on my experience in various applications.
I was prompted to package it up this way, and write it up, by a post by Brendon
McLean to the mailing list for the now-defunct Java Application Framework JSR-296.
10.26 Program: Find—Walking a File Tree
The program shown in Example 10-13 implements a subset of the Windows Find
Files dialog or the Unix find command. It has most of the structure needed to build a
more complete version of either of these. It accepts the following options from stan‐
dard Unix find (with limits):
10.26 Program: Find—Walking a File Tree | 361
-n name
Name to look for. Can include shell wildcards if quoted from the shell.
-s size
Size of file to look for. Can prefix with a plus sign to indicate greater than or a
minus sign to indicate less than.
-a, -o
And or or, but only one of these, between a -n and a -s.
The Files class has four methods for walking a file tree. Two return a lazily popula‐
ted Stream<Path>, and the other two invoke a callback FileVisitor for each file or
directory found. My find implementation uses the first one; the four are summarized
in Table 10-11.
Table 10-11. Files tree walk methods
Return Signature
Stream<Path> walk(Path start, FileVisitOption… options)
Stream<Path> walk(Path start, int maxDepth, FileVisitOption… options)
Path walkFileTree(Path start, FileVisitor<? super Path> visitor)
Path walkFileTree(Path start, Set<FileVisitOption> options, int maxDepth, FileVisitor<? super
Path> visitor)
Using the walk() methods is as simple as this:
Files.walk(startingPath).forEach(path -> {
// Do something with Path path; might be file, directory or other...
}
That code is near the start of the startWalkingAt() method in Example 10-13.
Example 10-13. main/src/main/java/dir_file/Find.java
/**
 * Find - find files by name, size, or other criteria. Non-GUI version.
 */
public class Find {
 public enum Conjunction { AND, OR };
 private static Logger logger = Logger.getLogger(Find.class.getSimpleName());
 static boolean started;
 /** Main program
 * @throws IOException If the Files.walkTree does so
 */
 public static void main(String[] args) throws IOException {
362 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 Find finder = new Find();
 if (args.length == 0) {
 finder.startWalkingAt(".");
 } else {
 for (int i = 0; i < args.length; i++) {
 if (args[i].charAt(0) == '-') {
 switch(args[i].substring(1)) {
 case "name":
 finder.filter.setNameFilter(args[++i]);
 continue;
 case "size":
 finder.filter.setSizeFilter(args[++i]);
 continue;
// Not implemented by back-end yet
// case "a":
// finder.filter.addConjunction(Conjunction.AND);
// continue;
// case "o":
// finder.filter.addConjunction(Conjunction.OR);
// continue;
 default: throw new IllegalArgumentException(
 "Unknown argument " + args[i]);
 }
 }
 finder.startWalkingAt(args[i]);
 }
 if (!started) {
 finder.startWalkingAt(".");
 }
 }
 }
 protected FindFilter filter = new FindFilter();
 public static void usage() {
 System.err.println(
 "Usage: Find [-n namefilter][-s sizefilter][dir...]");
 System.exit(1);
 }
 /** doName - handle one filesystem object by name */
 private void startWalkingAt(String s) throws IOException {
 logger.info("doName(" + s + ")");
 started = true;
 Path f = Path.of(s);
 if (!Files.exists(f)) {
 System.out.println(s + " does not exist");
 return;
 }
 Files.walk(f).forEach(fp -> {
 try {
10.26 Program: Find—Walking a File Tree | 363
 if (Files.isRegularFile(fp))
 doFile(fp);
 else if (Files.isDirectory(fp)) {
 doDir(fp);
 } else {
 System.err.println("Unknown type: " + s);
 }
 } catch (IOException e) {
 throw new RuntimeException("IO Exception: " + e);
 }
 });
 }
 /** doFile - process one regular file.
 * @throws IOException */
 private void doFile(Path f) throws IOException {
 if (filter.accept(f)) {
 System.out.println("f " + f);
 }
 }
 /** doDir - process a directory */
 private void doDir(Path d) {
 System.out.println("d " + d.normalize());
 }
}
Example 10-14 shows a class called FindFilter, the backend implementation of Find.
Example 10-14. main/src/main/java/dir_file/FindFilter.java
/** Class to encapsulate the filtration for Find.
 * For now just set*Filter() methods. Really needs to be a real
 * data structure (maybe LinkedList<FilterOp> or a Tree) for complex
 * requests like:
 * -n "*.html" -a \( -size < 0 -o mtime < 5 \).
 */
public class FindFilter {
 private enum SizeMode {GT, EQ, LT};
 SizeMode sizeMode;
 Find.Conjunction conj;
 long size;
 String name;
 Pattern nameRE;
 boolean debug = false;
 void setSizeFilter(String sizeFilter) {
 System.out.println("FindFilter.setSizeFilter()");
 sizeMode = SizeMode.EQ;
 char c = sizeFilter.charAt(0);
 if (c == '+') {
 sizeMode = SizeMode.GT;
364 | Chapter 10: Input and Output: Reading, Writing, and Directory Tricks
 sizeFilter = sizeFilter.substring(1);
 } else {
 if (c == '-') {
 sizeMode = SizeMode.LT;
 sizeFilter = sizeFilter.substring(1);
 }
 }
 size = Long.parseLong(sizeFilter);
 }
 /** Add a conjunction */
 public void addConjunction(Find.Conjunction conj) {
 System.out.println("FindFilter.addConjunction()");
 if (this.conj != null) {
 throw new IllegalArgumentException(
 "Only one conjucntion allowed in this version");
 }
 this.conj = conj;
 }
 /** Convert the given shell wildcard pattern into internal form (an RE) */
 void setNameFilter(String nameToFilter) {
 nameRE = makeNameFilter(nameToFilter);
 }
 Pattern makeNameFilter(String name) {
 StringBuilder sb = new StringBuilder('^');
 for (char c : name.toCharArray()) {
 switch(c) {
 case '.': sb.append("\\."); break;
 case '*': sb.append(".*"); break;
 case '?': sb.append('.'); break;
 // Some chars are special to RE and have to be escaped
 case '[': sb.append("\\["); break;
 case ']': sb.append("\\]"); break;
 case '(': sb.append("\\("); break;
 case ')': sb.append("\\)"); break;
 default: sb.append(c); break;
 }
 }
 sb.append('$');
 if (debug) {
 System.out.println("RE=\"" + sb + "\".");
 }
 // Should catch PatternException and rethrow for better diagnostics
 return Pattern.compile(sb.toString());
 }
 /** Do the filtering. For now, only filter on name, size or name+size */
 public boolean accept(Path p) throws IOException {
 if (debug) {
 System.out.println("FindFilter.accept(" + p + ")");
10.26 Program: Find—Walking a File Tree | 365
 }
 if (nameRE != null) {
 return nameRE.matcher(p.getFileName().toString()).matches();
 }
 // size handling
 if (sizeMode != null) {
 long sz = Files.size(p);
 switch (sizeMode) {
 case EQ:
 return (sz == size);
 case GT:
 return (sz > size);
 case LT:
 return (sz < size);
 }
 }
 // Catchall
 return false;
 }
 public String getName() {
 return name;
 }
}