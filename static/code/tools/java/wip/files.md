
File I/O
6
7
14
15
19


# 19. Character Sets and Unicode

Java chars and strings are Unicode
(more specifically, the UTF-16 encoding of the Unicode character set).

19.1. The Unicode Character Set
The Unicode character set maps characters to integer code points. For instance, the
Latin letter A is assigned the code point 65. The Greek letter S is assigned the code
point 931. The musical symbol is assigned the code point 119,074. Unicode
has room for over one million characters, which is enough to hold every character
from all the world's scripts. The current version of Unicode (4.1) defines 97,655
different characters from many languages, including English, Russian, Arabic,
Hebrew, Greek, Korean, Chinese, Japanese, and Sanskrit.
The first 128 Unicode characters (characters 0 through 127) are identical to the
ASCII character set. The ASCII space is 32; therefore, 32 is the Unicode space. The
ASCII exclamation point is 33, so 33 is the Unicode exclamation point, and so on.
Table A-1 in Appendix A shows this character set. The next 128 Unicode characters
(characters 128 through 255) have the same values as the equivalent characters in
the Latin-1 character set defined by ISO standard 8859-1. Latin-1, a slight variation
of which is used by Windows, adds the various accented characters, umlauts,
cedillas, upside-down question marks, and other characters needed to write text in
most Western European languages. Table A-2 shows these characters. The first 128
characters in Latin-1 are identical to the ASCII character set.
Unicode is divided into blocks. For example, characters 0 through 127 are the Basic
Latin block and contain ASCII. Characters 128 through 255 are the Latin Extended-A
block and contain the upper 128 characters of the Latin-1 character set. Characters
9984 through 10,175 are the Dingbats block and contain the characters in the
popular Zapf Dingbats font. Characters 19,968 through 40,959 are the unified
Chinese-Japanese-Korean ideograph block.
For complete lists of all the Unicode characters and associated
glyphs, the canonical reference is The Unicode Standard, Version
4.0 by the Unicode Consortium (ISBN 0-321-18578-1). Online
versions of the character tables can be found at
http://unicode.org/charts/.
Although internally Java can handle full Unicode data (code points are just numbers,
after all), not all Java environments can display all Unicode characters. The biggest
problem is the lack of fonts. Few computers have fonts for all the scripts Java
supports. Even computers that possess the necessary fonts can't install a lot of them
because of their size. A normal, 8-bit outline font ranges from about 3060K. A
Unicode font that omits the Han ideographs will be about 10 times that size. A
Unicode font that includes the full range of Han ideographs will occupy between 5
and 7 MB. Furthermore, text display algorithms based on English often break down
when faced with right-to-left languages like Hebrew and Arabic, vertical languages
like the traditional Chinese used in Taiwan, or context-sensitive languages like
Arabic.
19.2. UTF-16
The integers to which Unicode maps characters can be encoded in a variety of ways.
The simplest approach is to write each integer as a normal big-endian 4-byte int.
This encoding scheme is called UCS-4. However, it's rather inefficient because the
vast majority of characters seen in practice have code points less than 65,535, and
in English text most are less than 127.
In practice, most Unicode text is encoded in either UTF-16 or UTF-8. UTF-16 uses two
bytes for characters with code points less than or equal to 65,535 and four bytes for
characters with code points greater than 65,535. It comes in both big-endian and
little-endian formats. The endianness is normally indicated by an initial byte order
mark. That is, the first character in the file is the zero-width nonbreaking space, code
point 65,279. In big-endian UTF-16, this is the two bytes 0xFEFF (in hexadecimal). In
little-endian UTF-16, this is the reverse, 0xFFFE.
UTF-16 encodes characters with code points from 0 to 65,535 (the Basic Multilingual
Plane, or BMP for short) as 2-byte unsigned ints. Characters from beyond the BMP
are encoded as surrogate pairs made up of four bytes: first a high surrogate, then a
low surrogate. The Java char data type is really a big-endian UTF-16 code point, not
a Unicode character, though the difference is significant only for characters from
outside the BMP.
To see how this works, consider a character from outside the BMP in a typical UCS-4
(4-byte) big-endian representation. This is composed of four bytes of eight bits each.
I will label the bits as x0 through x31:
x31
x30
x29
x28
x27
x26
x25
x24
x23
x22
x21
x20
x19
x18
x17
x16
x15
x14
x13
x12
x11
x10
x9
x8
x7
x6
x5
x4
x3
x2
x1
x0
In reality, the high-order byte is always 0. The first three bits of the second byte are
also always 0, so these don't need to be encoded. Only bits x0 through x20 need to
be encoded. These are encoded in four bytes, like this:
1
1
0
1
1
0
w1
w2
w3
w4
x15
x14
x13
x12
x11
x10
1
1
0
1
1
1
x9
x8
x7
x6
x5
x4
x3
x2
x1
x0
Here, w1w2w3w4 is the 4-byte number formed by subtracting 1 from the 5-bit
number x20x19x18x17x16. There are simple, efficient algorithms for breaking up
non-BMP characters into these surrogate pairs and recomposing them. Most of the
time you'll let the Reader and Writer classes do this for you automatically. The main
thing you need to remember is that a Java char is really a UTF-16 code point, and
while 99% of the time this is the same as one Unicode character, there are cases
where it takes two chars to make a single character.

19.3. UTF-8
UTF-8 is the preferred encoding of Unicode for most scenarios that don't require fast
random indexing into a string. It has a number of nice characteristics, including
robustness and compactness compared to other Unicode encodings.
UTF-8 encodes the ASCII characters in a single byte, characters between 128 and
2,047 in two bytes, other characters in the BMP in three bytes, and characters from
outside the BMP in four bytes. Java .class files use UTF-8 to store string literals,
identifiers, and other text data in compiled byte code.
To better understand UTF-8, consider a typical Unicode character from the Basic
Multilingual Plane as a sequence of 16 bits:
x15
x14
x13
x12
x11
x10
x9
x8
x7
x6
x5
x4
x3
x2
x1
x0
Each ASCII character (each character between 0 and 127) has its upper nine bits
equal to 0:
0
0
0
0
0
0
0
0
0
x6
x5
x4
x3
x2
x1
x0
Therefore, it's easy to encode an ASCII character as a single byte. Just drop the highorder byte:
0
x6
x5
x4
x3
x2
x1
x0
Now consider characters between 128 and 2,047. These all have their top five bits
equal to 0, as shown here:
0
0
0
0
0
x10
x9
x8
x7
x6
x5
x4
x3
x2
x1
x0
These characters are encoded into two bytes, but not in the most obvious fashion.
The 11 significant bits of the character are broken up like this:
1
1
0
x10
x9
x8
x7
x6
1
0
x5
x4
x3
x2
x1
x0
Neither of the bytes that make up this number begins with a 0 bit. Thus, you can
distinguish between bytes that are part of a 2-byte character and bytes that
represent 1-byte characters (which all begin with 0).
The remaining characters in the BMP have values between 2,048 and 65,535. Any or
all of the bits in these characters may take on the value of either 0 or 1. Thus, they
are encoded in three bytes, like this:
1
1
1
0
x15
x14
x13
x12
1
0
x11
x10
x9
x8
x7
x6
1
0
x5
x4
x3
x2
x1
x0
Within this scheme, any byte beginning with a 0 bit must be a 1-byte ASCII character
between 1 and 127. Any byte beginning with the three bits 110 must be the first
byte of a 2-byte character. Any byte beginning with the four bits 1110 must be the
first byte of a 3-byte character. Finally, any byte beginning with the two bits 10 must
be the second or third byte of a multibyte character.
The DataOutputStream class provides a writeUTF( ) method that encodes a string in a
slight variation of UTF-8. It first writes the number of encoded bytes in the string (as
an unsigned short), followed by the UTF-8-encoded format of the string:
public final void writeUTF(String s) throws IOException
The DataInputStream class provides two corresponding readUTF( ) methods to read such
a string from its underlying input stream:
public final String readUTF( ) throws IOException
public static final String readUTF(DataInput in) throws IOException
Each of these first reads a 2-byte unsigned short that tells it how many more bytes
to read. These bytes are then read and decoded into a Java Unicode string. An
EOFException is thrown if the stream ends before all the expected bytes have been
read. If the bytes read cannot be interpreted as a valid UTF-8 string, a
UTFDataFormatException is thrown.
However, DataInputStream and DataOutputStream diverge from the official UTF-8 format in
one respect: they encode the null character (0x00) in two bytes rather than one.
This makes it slightly easier for C code that expects null-terminated strings to parse
Java .class files. On the other hand, it makes the data written by writeUTF( )
incompatible with most other libraries. The Reader and Writer classes discussed in the
next chapter read and write true UTF-8 with 1-byte nulls, and these should be
preferred for almost all use cases other than parsing Java byte code.
19.4. Other Encodings
Unicode support is growing, but there will doubtless be legacy data in other
encodings that must be read for centuries to come. Such encodings include ASCII
and Latin-1, as well as less common encoding schemes such as EBCDIC and
MacRoman. There are multiple encodings in use for Arabic, Turkish, Hebrew, Greek,
Cyrillic, Chinese, Japanese, Korean, and many other languages and scripts. The Reader
and Writer classes allow you to read and write data in these different character sets.
The String class also has a number of methods that convert between different
encodings (though a String object itself is always represented in Unicode).
Modern desktop and server Java environments are pretty well guaranteed to have
these six character sets available:
US-ASCII
ISO-8859-1
UTF-8
UTF-16BE
UTF-16LE
UTF-16
All other encodings are optional and may not be supported in any given VM. Most
VMs will have many more encodings as well, but only these six are almost certain to
be present. They're likely to be more interoperable, not just with Java but with other
programs written in other languages. Some VMs, especially on Windows, omit some
of the more obscure or larger encodings to save space. J2ME VMs will likely include
many fewer to save space, and they don't have the java.nio.charsets package at all.
If you've installed Sun's JRE/JDK, a basic set of encodings is included in the standard
rt.jar file along with all the other classes from the Java class library. There may also
be a charsets.jar file that includes several dozen additional encodings, such as
MacRoman and SJIS.
19.5. Converting Between Byte Arrays and Strings
I/O is about bytes. Disks and networks understand bytes, not characters.
Nonetheless, much actual programming is modeled in terms of characters and text.
When reading in data, it's generally necessary to convert the bytes into characters.
When writing out data, it's necessary to convert the characters into bytes. The Reader
and Writer classes can perform the conversions implicitly, which is normally the
simplest approach when you only need to work on text. However, when working with
mixed formats such as FITS, GIF, or XOP that contain both text and binary data, it's
normally necessary to explicitly convert the text to or from bytes in some encoding.
19.5.1. The String Class
The java.lang.String class has several constructors that form a string from a byte
array and several methods that return a byte array corresponding to a given string.
There's no unique way to do this. There are multiple encodings of characters into
bytes. Anytime a string is converted to bytes or vice versa, that conversion happens
according to a certain encoding. The same string can produce different byte arrays
when converted into different encodings.
Six constructors form a new String object from a byte array:
public String(byte[] ascii, int highByte)
public String(byte[] ascii, int highByte, int offset, int length)
public String(byte[] data, String encoding)
throws UnsupportedEncodingException
public String(byte[] data, int offset, int length, String encoding)
throws UnsupportedEncodingException
public String(byte[] data)
public String(byte[] data, int offset, int length)
The first two constructors, the ones with the highByte argument, are leftovers from
Java 1.0 that are deprecated in Java 1.1 and later. These two constructors do not
accurately translate non-Latin-1 character sets into Unicode. Instead, they read each
byte in the ascii array as the low-order byte of a 2-byte character and fill in the highorder byte with the highByte argument. For example:
byte[] isoLatin1 = new byte[256];
for (int i = 0; i < 256; i++) isoLatin1[i] = (byte) i;
String s = new String(isoLatin1, 0);
Frankly, this is a kludge. It's deprecated for good reason. This scheme works quite
well for Latin-1 data with a high byte of 0. However, it's extremely difficult to use for
character sets where different characters need to have different high bytes, and it's
completely unworkable for character sets like MacRoman that also need to adjust
bits in the low-order byte to conform to Unicode. The only approach that genuinely
works for the broad range of character sets Java programs may be asked to handle is
table lookup. Each supported character encoding requires a table mapping
characters in the set to Unicode characters. These tables are hidden inside the sun.io
package, but they are present, and they are how the next four constructors translate
from various encodings to Unicode.
The third and fourth constructors allow the client programmer to specify not only the
byte data but also the encoding table to be used when converting these bytes to
Unicode chars. The third constructor converts the entire array from the specified
encoding into Unicode. The fourth one converts only the specified subarray of data
starting at offset and continuing for length bytes. Otherwise, they're identical. The
first argument is the data to be converted. The final argument is the encoding
scheme to be used to perform the conversion. For example:
byte[] isoLatin1 = new byte[256];
for (int i = 0; i < 256; i++) isoLatin1[i] = (byte) i;
String s = new String(isoLatin1, "8859_1");
The fifth and sixth constructors are similar to the third and fourth. However, they
always use the host platform's default encoding, as specified by the system property
file.encoding. If this is ISO 8859-1, you may write:
byte[] isoLatin1 = new byte[256];
for (int i = 0; i < 256; i++) isoLatin1[i] = (byte) i;
String s = new String(isoLatin1);
This code fragment produces different results on platforms with different default
encodings.
The three getBytes( ) methods go the other direction, converting the Unicode string
into an array of bytes in a particular encoding:
public void getBytes(int srcBegin, int srcEnd, byte[] dst, int
dstBegin)
public byte[] getBytes( )
public byte[] getBytes(String encoding) throws
UnsupportedEncodingException
Once again, the first method is deprecated. The byte array it returns contains only
the low-order bytes of the 2-byte characters in the string (starting at srcBegin and
continuing through srcEnd). This works well enough for ASCII and Latin-1 but fails
miserably for pretty much all other character sets. The no-arg getBytes( ) method
properly converts the Unicode characters in the string into a byte array in the
platform's default encodingassuming a full conversion is possible (and it isn't always;
you cannot, for example, convert a string of Chinese ideographs into Latin-1). The
byte array returned contains the converted characters. The third and final getBytes( )
method specifies the encoding to be used to make the conversion. For example, this
statement converts the Greek word andr s (man) into its byte equivalent using the
MacGreek encoding:
byte[] man = "andr s".getBytes("MacGreek");
This method throws an UnsupportedEncodingException if the Java virtual machine does not
supply the requested encoding.
19.5.2. The Charset Class
Char-to-byte conversion through the String class is relatively indirect and not always
as efficient as one would like. In Java 1.4, the java.nio.charsets package provides
classes for efficient conversion of large chunks of text to and from any encoding Java
supports. This is in fact a more direct interface to the character conversion code
that's used by the String class and has been present in the JDK since Java 1.1.
Charset is an abstract class that represents a character set such as US-ASCII, ISO8859-1, or SJIS. Each Charset object defines a mapping between the characters in that
set and some subset of Unicode. The mapping is sometimes implemented
algorithmically, sometimes as simple table lookup, and sometimes as a combination
of both, but the details need not concern you. The Charset abstraction hides all this.
19.5.2.1. Retrieving Charset objects
The one constructor in the Charset class is protected:
protected Charset(String canonicalName,String[] aliases)
While you might invoke this if adding support for an encoding Java doesn't support
out of the box, that usage is rare. Much more commonly, you'll call the
Charset.forName( ) factory method to ask Java for one of the encodings it supports:
public static Charset forName(String charsetName)
throws IllegalCharsetNameException, UnsupportedCharsetException
For example, this statement requests the Charset object for the Big5 encoding for
Chinese:
Charset big5 = Charset.forName("Big5");
Character set names are case insensitive. Charset.forName("BIG5") returns the same
Charset object as Charset.forName("Big5").
If the local JDK supports the requested encoding, Charset.forName( ) returns a Charset
object. Otherwise, it throws an UnsupportedCharsetException. This is a runtime exception,
so you don't need to explicitly handle it as long as you're confident the runtime
contains the requested character set. Charset.forName( ) may also throw an
IllegalCharsetNameException if the name contains spaces, non-ASCII characters, or
punctuation marks other than the hyphen, period, colon, and underscore.
Java 5 adds one more way to get a Charset. The static Charset.defaultCharset( ) method
returns the current system's default character set:
public static Charset defaultCharset( )
This code prints the name of the platform's default character set:
System.out.println(Charset.defaultCharset( ));
When I tested this, the default on Mac OS X was MacRoman, on Windows it was
windows-1252, and on Linux it was UTF-8. These were all U.S.-localized systems.
Systems localized for other countries, especially outside Western Europe and the
Americas, would probably show something different.
19.5.2.2. Character set info
The static Charset.isSupported( ) method checks whether an encoding is available in
the current VM:
public static boolean isSupported(String charsetName)
For example, if you wanted to use Big5 if possible but fall back to UTF-8 if it wasn't,
you might write code like this:
Charset cs;
if (Charset.isSupported("Big5") cs = Charset.forName("Big5");
else cs = Charset.forName("UTF-8");
The static Charset.availableCharsets( ) method enables you to inquire which character
sets are installed in the local VM:
public static SortedMap availableCharsets( )
The keys in the returned method are the character set names. The values are the
Charset objects themselves. In Java 5, a genericized signature makes this more
explicit:
public static SortedMap <String, Charset> availableCharsets( )
Example 19-1 is a simple program to list all the available character sets:
Example 19-1. List available character sets
import java.nio.charset.*;
import java.util.*;
class CharsetLister {
public static void main(String[] args) {
Map charsets = Charset.availableCharsets( );
Iterator iterator = charsets.keySet().iterator( );
while (iterator.hasNext( )) {
System.out.println(iterator.next( ));
}
}
}
When run on the Apple Java VM 1.4, it found 64 character sets, including the
following:
$ java CharsetLister
Big5
Big5-HKSCS
EUC-JP
EUC-KR
GB18030
GBK
ISO-2022-JP
ISO-2022-KR
ISO-8859-1
ISO-8859-13
ISO-8859-15
ISO-8859-2
...
x-MS950-HKSCS
x-mswin-936
x-windows-949
x-windows-950
The Java 5 VM has 85 more. Character set availability varies from one VM vendor
and version to the next. In general, I recommend sticking to UTF-8 if at all possible
for new data. UTF-8 should always be supported. Legacy protocols, formats, and
data may require occasional use of US-ASCII, ISO-8859-1, or other encodings, but
new text data should be encoded in UTF-8.
Many character sets are commonly known by more than one name. For instance,
UTF-8 is also referred to as UTF8 and unicode-1-1-utf-8. The names shown in the
program's output are the canonical names of the character sets. The name( ) instance
method returns the canonical name of a given Charset object:
public String name( )
The aliases( ) method returns all the aliases for a given character set, not including
its canonical name:
public final Set aliases( )
The values in the set are strings. In Java 5, a genericized signature makes this more
explicit:
public final Set<String> aliases( )
Character sets may also have display names that can be localized and may contain
non-ASCII characters:
public String displayName( )
The display name is usually the same as the canonical name, but specific
implementations may instead return a localized value that can contain spaces and
non-ASCII characters. The display name is meant for showing to people, not for
looking up character sets.
For interoperability, character set names and aliases should be registered with the
Internet Assigned Number Authority (IANA) and listed in the registry at
http://www.iana.org/assignments/character-sets. The isRegistered( ) method returns
true if the character set has been so registered:
public final boolean isRegistered( )
Many of the character sets shipped with the JDK have not been registered. You may
need to use these character sets to decode existing data, but you should not
generate any new data in an unregistered character set.
Example 19-3 is a slightly more complex program that lists all the available
character sets by their display names, canonical names, and aliases.
Example 19-2. List different names for character sets
import java.nio.charset.*;
import java.util.*;
class AliasLister {
public static void main(String[] args) {
Map charsets = Charset.availableCharsets( );
Iterator iterator = charsets.values().iterator( );
while (iterator.hasNext( )) {
Charset cs = (Charset) iterator.next( );
System.out.print(cs.displayName( ));
if (cs.isRegistered( )) {
System.out.print(" (registered): ");
}
else {
System.out.print(" (unregistered): ");
}
System.out.print(cs.name( ) );
Iterator names = cs.aliases().iterator( );
while (names.hasNext( )) {
System.out.print(", ");
System.out.print(names.next( ));
}
System.out.println( );
}
}
}
Here's a sample of the output from the Apple Java VM 1.4:
$ java AliasLister
Big5 (registered): Big5, csBig5
Big5-HKSCS (registered): Big5-HKSCS, big5-hkscs, Big5_HKSCS, big5hkscs
EUC-JP (registered): EUC-JP, eucjis, x-eucjp, csEUCPkdFmtjapanese,
eucjp,
Extended_UNIX_Code_Packed_Format_for_Japanese, x-euc-jp, euc_jp
EUC-KR (registered): EUC-KR, ksc5601, 5601, ksc5601_1987, ksc_5601,
ksc5601-1987,
euc_kr, ks_c_5601-1987, euckr, csEUCKR
GB18030 (registered): GB18030, gb18030-2000
...
x-MS950-HKSCS (unregistered): x-MS950-HKSCS, MS950_HKSCS
x-mswin-936 (unregistered): x-mswin-936, ms936, ms_936
x-windows-949 (unregistered): x-windows-949, windows949, ms_949, ms949
x-windows-950 (unregistered): x-windows-950, windows-950, ms950
19.5.2.3. Encoding and decoding
Of course, the primary purpose of a Charset object is to encode and decode text. The
encode( ) and decode( ) methods do this:
public final CharBuffer decode(ByteBuffer buffer)
public final ByteBuffer encode(CharBuffer buffer)
public final ByteBuffer encode(String s)
You can encode either a String or a CharBuffer. Decoding operates on a ByteBuffer and
produces a CharBuffer. These methods do not throw exceptions. If they encounter a
character they cannot convert, they replace it with the replacement character
(normally a question mark).
All character sets support decoding, and most but not all support encoding. The
canEncode( ) method returns true if the Charset supports encoding and false if it doesn't:
public boolean canEncode( )
A few special sets automatically detect the encoding of an incoming stream and set
the decoder appropriately. In the VM I use, there are exactly two such nonencoding
charsets: csISO2022CN and JISAutoDetect. If you try to encode text with a Charset
that does not support encoding, the encode( ) method throws an
UnsupportedOperationException.
Example 19-5 is a simple program that reads a stream in one encoding and writes it
out in another encoding. A Charset object converts between the two encodings. The
user interface implemented in the main( ) method simply reads the names of the
encodings to convert to and from the command-line arguments. Input is read from
System.in and written to System.out, mostly because I didn't want to spend a lot of lines
parsing command-line arguments. However, the convert( ) method is more general
and can operate on any streams you pass in.
Example 19-3. Converting encodings
import java.io.*;
import java.nio.charset.*;
import java.nio.*;
import java.nio.channels.*;
public class Recoder {
public static void main(String[] args) {
if (args.length != 2) {
System.err.println(
"Usage: java Recoder inputEncoding outputEncoding
<inFile >outFile");
return;
}
try {
Charset inputEncoding = Charset.forName(args[0]);
Charset outputEncoding = Charset.forName(args[1]);
convert(inputEncoding, outputEncoding, System.in,
System.out);
}
catch (UnsupportedCharsetException ex) {
System.err.println(ex.getCharsetName( ) + " is not
supported by this VM.");
}
catch (IllegalCharsetNameException ex) {
System.err.println(
"Usage: java Recoder inputEncoding outputEncoding
<inFile >outFile");
}
catch (IOException ex) {
System.err.println(ex.getMessage( ));
}
}
private static void convert(Charset inputEncoding, Charset
outputEncoding,
InputStream inStream, OutputStream outStream) throws
IOException {
ReadableByteChannel in = Channels.newChannel(inStream);
WritableByteChannel out = Channels.newChannel(outStream);
for (ByteBuffer inBuffer = ByteBuffer.allocate(4096);
in.read(inBuffer) != -1;
inBuffer.clear( )) {
inBuffer.flip( );
CharBuffer cBuffer = inputEncoding.decode(inBuffer);
ByteBuffer outBuffer = outputEncoding.encode(cBuffer);
while (outBuffer.hasRemaining( )) out.write(outBuffer);
}
}
}
The convert( ) method wraps a channel around the InputStream and another channel
around the OutputStream. Data is read from the input channel into a ByteBuffer. Next,
this buffer is flipped and decoded into a CharBuffer using the input Charset. That
CharBuffer is then reencoded into a new ByteBuffer using the output encoding. Finally,
this byte buffer is written onto the output channel.
Example 19-5 is simple, but it has one inobvious bug. What if the input data in the
buffer does not contain a complete multibyte character? That is, what if it reads in
only the first byte of a 2-byte or longer character? In this case, that character is
replaced by the replacement character (usually a question mark). However, suppose
you have a long stream that requires multiple reads from the channel into the
bufferthat is, say the entire stream can't fit into the buffer at once. Or suppose the
channel is nonblocking and the first couple of bytes of a 3- or 4-byte character have
arrived, but the last bytes haven't. In other words, suppose the data in the buffer is
malformed, even though the stream itself isn't. The encode( ) method does not leave
anything in the buffer. It will drain the buffer completely and use replacement
characters at the end if necessary. This has the potential to corrupt good data, and it
can be a very hard bug to diagnose because 99% of the time you're not going to hit
the fencepost condition that triggers the bug. (One way to make it a little more likely
to show up is to reduce the size of the buffer to something quite small, even three or
four bytes.)
You can avoid this problem by using a CharsetDecoder object directly to fill the buffer
with data repeatedly, and decode it only once all the data has been placed in the
buffer.
19.5.3. CharsetEncoder and CharsetDecoder
The decode( ) and encode( ) methods suffice for most simple use cases (as do the
String constructors and the getBytes( ) method). However, for more sophisticated
needs, you may wish to use an explicit CharsetEncoder or CharsetDecoder. These aren't as
simple as the previous methods, but they allow greater customization. For example,
you can configure them to throw an exception if they encounter an unencodable
character rather than replacing it with a question mark. Let's address the encoder
first. The decoder is similar, except it runs in the opposite direction.
19.5.3.1. Encoding
The constructor in the CharsetEncoder class is protected. Encoders are created by first
getting a Charset object for the encoding and then invoking its newEncoder( ) method:
public abstract CharsetEncoder newEncoder( )
throws UnupportedOperationException
This method throws an UnupportedOperationException if this is one of those uncommon
character sets that does not support encoding. For example:
Charset utf8 = Charset.forName("UTF-8");
CharsetEncoder encoder = utf8.newEncoder( );
The encoder encodes bytes from a CharBuffer into a ByteBuffer:
public final CoderResult encode(CharBuffer in, ByteBuffer out,
boolean endOfInput)
encode( ) reads as much data as possible from the CharBuffer and writes the encoded
bytes into the ByteBuffer. You normally call this method repeatedly, passing in more
data each time. All but the last time, you pass false as the final argument, indicating
that this is not the end of the input data. The last time you call encode( ), you pass
true. (If necessary, you can encode until there are no bytes remaining while passing
false and then encode zero bytes while passing TRue, but you do need to pass TRue the
last and only the last time you call the method.) Finally, you invoke the flush( )
method to write any last bytes that need to be written. The output buffer can then
be flipped and drained somewhere else.
For example, this method converts a string into a ByteBuffer containing the UTF-8
encoding of the string:
public static ByteBuffer convertToUTF8(String s) {
CharBuffer input = CharBuffer.wrap(s);
Charset utf8 = Charset.forName("UTF-8");
CharsetEncoder encoder = utf8.newEncoder( );
ByteBuffer output = ByteBuffer.allocate(s.length( )*3);
while (input.hasRemaining( )) {
encoder.encode(input, output, false);
}
encoder.encode(input, output, true);
encoder.flush(output);
output.flip( );
return output;
}
In UTF-8, each char in the string is encoded into at most three bytes in the output
array, so there's no possibility of underflow or overflow. However, there is a small
chance of the data being malformed if surrogate characters are used incorrectly in
the input string. Java doesn't check for this. To check for it (and you should, or this
code could get caught in an infinite loop), you need to inspect the return value from
encode( ). The return value is a CoderResult object that has five methods to tell you
what happened:
public boolean isError( )
public boolean isUnderflow( )
public boolean isOverflow( )
public boolean isMalformed( )
public boolean isUnmappable( )
(There's no result for success. If the encoding succeeded, these five methods each
return false.) Inspecting the result, and throwing an error if the encoding failed for
any reason, the convertToUTF8( ) method now becomes this:
public static ByteBuffer convertToUTF8(String s) throws IOException {
CharBuffer input = CharBuffer.wrap(s);
Charset utf8 = Charset.forName("UTF-8");
CharsetEncoder encoder = utf8.newEncoder( );
ByteBuffer output = ByteBuffer.allocate(s.length( )*3);
while (input.hasRemaining( )) {
CoderResult result = encoder.encode(input, output, false);
if (result.isError( )) throw new IOException("Could not encode " +
s);
}
encoder.encode(input, output, true);
encoder.flush(output);
output.flip( );
return output;
}
CharsetEncoder also has a convenience method that encodes all the remaining text in a
character buffer and returns a ByteBuffer of the necessary size:
public final ByteBuffer encode(CharBuffer in) throws
CharacterCodingException
This avoids problems with underflow and overflow. However, if the data is malformed
or a character cannot be converted into the output character set, it may throw a
CharacterCodingException. (This is configurable with the onMalformedInput( ) and
onUnmappableCharacter( ) methods.)
You can use a single CharsetEncoder object to encode multiple buffers in sequence. If
you do this, you will need to call the reset( ) method between buffers:
public final CharsetEncoder reset( )
This returns the same CharsetEncoder object to enable method invocation chaining.
19.5.3.2. Decoding
The CharsetDecoder class is almost a mirror image of CharsetEncoder. It converts from
bytes to characters rather than from characters to bytes. The constructor in the
CharsetDecoder class is protected too. Instead, an encoder for a character is created by
first getting a Charset object for the encoding and then invoking its newDecoder( )
method:
Charset utf8 = Charset.forName("UTF-8");
CharsetDecoder decoder = utf8.newDecoder( );
The decoder decodes bytes from a ByteBuffer into a CharBuffer:
public final CoderResult decode(ByteBuffer in, CharBuffer out, boolean
endOfInput)
As much data as possible is read from the ByteBuffer, converted into chars, and
written into the CharBuffer. You call this method repeatedly, passing in more data
each time. All but the last time, you pass false as the final argument. The last time
you call decode( ), pass TRue. Finally, invoke the flush( ) method to clear any last state.
At this point, the final data is flushed into the output buffer, which can be flipped and
drained somewhere else. For example, this method converts a byte array containing
UTF-8 text into a string:
public static String convertFromUTF8(byte[] data) throws IOException {
ByteBuffer input = ByteBuffer.wrap(data);
Charset utf8 = Charset.forName("UTF-8");
CharsetDecoder decoder = utf8.newDecoder( );
CharBuffer output = CharBuffer.allocate(data.length);
while (input.hasRemaining( )) {
CoderResult result = decoder.decode(input, output, false);
if (result.isError()) throw new IOException( );
}
decoder.decode(input, output, true);
decoder.flush(output);
output.flip( );
return output.toString( );
}
CharsetDecoder also has a convenience method that decodes all the remaining data in
a byte buffer and returns a CharBuffer of the necessary size:
public final CharBuffer decode(ByteBuffer in) throws
CharacterCodingException
This avoids problems with underflow and overflow. However, if the data is malformed
or a character cannot be converted into the output character set, it may throw a
CharacterCodingException. (This is configurable with the onMalformedInput( ) and
onUnmappableCharacter( ) methods.)
You can reuse a single CharsetDecoder object to decode multiple buffers in sequence. If
you do this, you will need to call the reset( ) method between buffers:
public final CharsetDecoder reset( )
19.5.3.3. Error handling
Each call to encode( ) or decode( ) returns a CoderResult object. This object tells you
whether the encoding succeeded, and, if so, how many bytes were encoded.
Normally, all you care about is whether the encoding succeeded or not. This is
revealed by the isError( ) method:
public boolean isError( )
However, if you care about why the encoding failed, several more methods in
CoderResult reveal the reason. Encoding can fail because there were insufficient
characters to encode into bytes:
public boolean isUnderflow( )
This might happen if only the first half of a surrogate pair were supplied at the end
of the input buffer.
Encoding or decoding can fail because there are too many characters to encode into
the output buffer:
public boolean isOverflow( )
Decoding can fail because the data is malformed in some way:
public boolean isMalformed( )
For instance, this might happen in UTF-8 if the bytes of a multibyte character were
shuffled.
Encoding can fail because the character you're trying to encode is unmappable:
public boolean isUnmappable( )
For instance, this would happen if you were trying to encode the Greek letter a using
the ISO-8859-1 character set because this character set does not contain the letter
a.
Some charsets may also tell you the length of the bad data that caused the
encoding or decoding to fail:
public int length( ) throws UnsupportedOperationException

However, not all will, and this method may throw an UnsupportedOperationException.
The whole idea of returning a special object to specify the error is a little strange for
Java. This is exactly what exceptions were designed to replace. If you like, you can
cause the CoderResult to throw an equivalent exception instead, using the
throwException( ) method:
public void throwException( ) throws CharacterCodingException
Depending on the type of the error, this throws a BufferUnderflowException,
BufferOverflowException, MalformedInputException, or UnmappableCharacterException. For
example:
CoderResult result = decoder.decode(input, output, false);
if (result.isError()) result.throwException( );
Sometimes you want to throw the exception and then stop reading or writing. For
example, this would be appropriate if you were feeding data to an XML parser.
However, if you're in the less draconian world of HTML, you might want to just keep
on trucking. To loosen up this way, you can set the action for malformed input and/or
unmappable characters to CodingErrorAction.IGNORE or CodingErrorAction.REPLACE with
onUnmappableCharacter( ) and onMalformedInput( ):
public final CharsetEncoder onMalformedInput(CodingErrorAction action)
public final CharsetEncoder onUnmappableCharacter(CodingErrorAction
action)
Ignoring simply drops bad data while replacing changes the bad data to a specified
replacement character (usually the question mark, by default). There's no separate
method for overflow and underflow errors. They count as malformed input. For
example, these statements tell a CharsetEncoder to drop malformed input and to
replace unmappable characters:
encoder.onMalformedInput(CodingErrorAction.IGNORE);
encoder.onUnmappableCharacter(CodingErrorAction.REPLACE);
You can also set the action to CodingErrorAction.REPORT. This is usually the default and
simply indicates that the encoder or decoder should return an error in a CoderResult or
throw a CharacterCodingException.
The replaceWith( ) method changes the replacement bytes the encoder uses when it
encounters an unmappable character while operating in replace mode:
public final CharsetEncoder replaceWith(byte[] replacement)
throws IllegalArgumentException
Not all byte sequences are legal here. The replacement array must contain
characters allowed in the encoding. If not, this method throws an
IllegalArgumentException.
There's also a getter method for this value:
public final byte[] replacement( )
The CharsetDecoder class has similar methods, except that it uses a string replacement
value instead of a byte replacement value:
public final CharsetDecoder replaceWith(String newReplacement)
public final String replacement( )
19.5.3.4. Measurement
A CharsetEncoder can estimate the number of bytes that will be required for each char
that's encoded:
public final float averageBytesPerChar( )
This may be exact for some encodings, but for variable-width encodings such as
UTF-8 it's only approximate. Java estimates UTF-8 as averaging 1.1 bytes per
character, but the exact ratio can vary widely from one string to the next.
A CharsetEncoder can also tell you the theoretical maximum number of bytes that will
be needed for each character:
public final float maxBytesPerChar( )
Both of these values can be useful in choosing the size of the ByteBuffer to encode
into.
19.5.3.5. Encodability
Encoders have the useful ability to tell whether or not a particular character, string,
or character sequence can be encoded in a given encoding:
public boolean canEncode(char c)
public boolean canEncode(CharSequence cs)
For example, this is very useful for XML serializers writing non-Unicode encodings.
These need to know whether any given string can be written directly or needs to be
escaped with a numeric character reference such as &#xA0; or &#160;. Serializers that
operate in Java 1.3 and earlier have to either use undocumented classes in the sun
packages, use really ugly hacks where they first convert the bytes into a string and
then look to see if a replacement character was used, or implement their own lookup
tables for all this data. In Java 1.4 and later, by contrast, serializers can just create
an appropriate encoder and then call canEncode( ).


# 20. Readers and Writers

he java.io.Writer
class is modeled on the java.io.OutputStream class. The java.io.Reader class is modeled
on the java.io.InputStream class.

Reader and Writer are abstract superclasses that identify
common functionality in the concrete subclasses. The difference between readers
and writers and input and output streams is that streams are fundamentally bytebased while readers and writers are fundamentally character-based. Where an input
stream reads a byte, a reader reads a character. Where an output stream writes a
byte, a writer writes a character.
While bytes are a more or less universal concept, characters are not. As you learned
in the last chapter, the same character can be encoded differently in different
character sets, and different character sets include different characters. Characters
can even have different sizes in different character sets. For example, ASCII and
Latin-1 use 1-byte characters. UTF-8 uses characters of varying width between one
and four bytes.
A language that supports international text must separate the reading and writing of
raw bytes from the reading and writing of characters. Classes that read characters
must be able to parse a variety of character encodings, not just ASCII, and translate
them into the language's native character set. Classes that write characters must be
able to translate the language's native character set into a variety of formats and
write those. In Java, this task is performed by the InputStreamReader and
OutputStreamWriter classes.
20.1. The java.io.Writer Class
The Writer class is abstract, just like OutputStream is abstract. You won't have any pure
instances of Writer that are not also instances of some concrete subclass of Writer.
However, many of the subclasses of Writer differ primarily in the targets of the text
they write, just as many concrete subclasses of OutputStream differ only in the targets
of the data they write. Most of the time you don't care about the difference between
FileOutputStream and ByteArrayOutputStream. Similarly, most of the time you won't care
about the difference between FileWriter and StringWriter. You'll just use the methods
of the common superclass, java.io.Writer.
You use a writer almost exactly like you use an output stream. Rather than writing
bytes, you write chars. The write( ) method writes a subarray from the char array text
starting at offset and continuing for length characters:
public abstract void write(char[] text, int offset, int length)
throws IOException
For example, given some Writer object w, you can write the string Testing 1-2-3 like
this:
char[] test = {'T', 'e', 's', 't', 'i', 'n', 'g', ' ',
'1', '-', '2', '-', '3'};
w.write(test, 0, test.length);
This method is abstract. Concrete subclasses that convert chars into bytes according
to a specified encoding and write those bytes onto an underlying stream must
override this method. An IOException may be thrown if the underlying stream's write( )
method throws an IOException. You can also write a single character, an entire array of
characters, a string, or a substring:
public void write(int c) throws IOException
public void write(char[] text) throws IOException
public void write(String s) throws IOException
public void write(String s, int offset, int length) throws IOException
The default implementations of these four methods convert their first argument into
an array of chars and pass that to write(char[] text, int offset, int length). Specific
subclasses may provide more efficient implementations of these methods.
This is one of the few instances where the general structure of
the Writer and the OutputStream classes diverge, though not in a
very significant way. In OutputStream, the fundamental, abstract
method that must be overridden by subclasses is the write( )
method that writes a single byte. OutputStream's multibyte write( )
methods are implemented in terms of the single-byte write( )
method whereas Writer's single-character write( ) method is
implemented in terms of a multicharacter write( ) method.
Beginning in Java 5, the Writer class implements the Appendable interface. This gives it
three more methods:
public W0riter append(char c) throws IOException // Java 5
public Writer append(CharSequence sequence)throws IOException // Java 5
public Writer append(CharSequence sequence,int start,int end)// Java 5
throws IOException
The append(char) method behaves the same as write(char) with the single difference
that it returns this Writer object to allow method invocation chaining. The other two
methods behave the same as the equivalent write(String) and write(String, int, int)
methods. However, they accept any class that implements CharSequence, not just
String.
Like output streams, writers may be buffered. To force the write to take place, call
flush( ):
public abstract void flush( ) throws IOException
The close( ) method closes the writer and releases any resources associated with it:
public abstract void close( ) throws IOException
This flushes the writer and closes the underlying output stream.
In Java 5, Writer implements Flushable and Closeable. However, it
still has these two methods in 1.4 and earlier.
20.2. The OutputStreamWriter Class
java.io.Writer is an abstract class. Its most basic concrete subclass is
OutputStreamWriter:
public class OutputStreamWriter extends Writer
Its constructor connects a character writer to an underlying output stream:
public OutputStreamWriter(OutputStream out)
public OutputStreamWriter(OutputStream out, String encoding) throws
UnsupportedEncodingException
The first constructor configures the writer to encode text in the platform's default
encoding. The second constructor specifies an encoding. For example, this code
attaches an OutputStreamWriter to System.out with the default encoding:
OutputStreamWriter osw = new OutputStreamWriter(System.out);
On U.S. and Western European systems, the default encoding is usually Cp1252 on
Windows, ISO 8859-1 (Latin-1) on Unix and Linux, and MacRoman on Macs. More
recent Linuxes may use UTF-8 everywhere. Whatever the default is, you can read it
from the system property file.encoding:
String defaultEncoding = System.getProperty("file.encoding");
On the other hand, if you want to write a file encoded in ISO 8859-7 (ASCII plus
Greek) you'd have to do this:
FileOutputStream fos = new FileOutputStream("greek.txt");
OutputStreamWriter greekWriter = new OutputStreamWriter(fos, "8859_7");
You should almost never use the default encoding. It's likely to cause problems as
files are moved between platforms and countries, especially if the document format
contains no means of indicating the encoding. If the file format does not specify a
different encoding, choose UTF-8:
FileOutputStream fos = new FileOutputStream("data.txt");
OutputStreamWriter utfWriter = new OutputStreamWriter(fos, "UTF-8");
There are reasons to pick other encodings, especially when dealing with legacy
software and formats that mandate something else. However, unless specified
otherwise, you should choose UTF-8. It has the best mix of interoperability,
robustness, compactness, and script support.
The write( ) methods convert characters to bytes according to the specified
character encoding and write those bytes onto the underlying output stream:
public void write(int c) throws IOException
public void write(char[] text, int offset, int length) throws
IOException
public void write(String s, int offset, int length) throws IOException
Once the Writer is constructed, writing the characters is easy:
String arete = "\u03B1\u03C1\u03B5\u03C4\u03B7";
greekWriter.write(arete, 0, arete.length( ));
The String variable arete contains the Unicode-escaped encoding of ar th, the Greek
word for excellence. The second line writes this word in the ISO 8859-7 character
set. In this encoding, these five Unicode characters (10 bytes) become the five bytes
225, 241, 229, 244, 231. You don't have to worry about exactly how this conversion
is performed. You just have to construct the writer, write the string, and let Java do
the grunt work of figuring out which Unicode characters map to which 8859-7
characters.
Unicode is a fairly large character set. Most other character sets don't have all the
characters in Unicode. Writing a character that doesn't exist in the current character
set produces a substitution character, usually a question mark.
The getEncoding( ) method returns a string containing the name of the encoding used
by this writer:
public String getEncoding( )
Example 20-1 loops through every nonsurrogate, defined character in the Basic
Multilingual Plane (BMP) and writes each one into the file given on the command
line, using the specified character encoding. If no character encoding is specified,
the platform's default encoding is used. If no file is specified, System.out is used.
Example 20-1. Printing all the BMP characters
import java.io.*;
public class UnicodeBMPTable {
public static void main(String[] args) throws IOException {
// Use platform default with a fallback to Latin-1 if
necessary
String encoding = System.getProperty("file.encoding",
"ISO-8859-1");
String lineSeparator =
System.getProperty("line.separator", "\r\n");
OutputStream target = System.out;
if (args.length > 0) target = new
FileOutputStream(args[0]);
if (args.length > 1) encoding = args[1];
OutputStreamWriter out = null;
try {
out = new OutputStreamWriter(target, encoding);
}
catch (UnsupportedEncodingException ex) {
// use platform default encoding
out = new OutputStreamWriter(target);
}
try {
for (int i = Character.MIN_VALUE; i <=
Character.MAX_VALUE; i++) {
// Skip undefined code points; these are not
characters
if (!Character.isDefined(i)) continue;
char c = (char) i;
// Surrogates are not full characters so skip them;
// this requires Java 5
if (Character.isHighSurrogate(c) ||
Character.isLowSurrogate(c)) continue;
out.write(i + ":\t" + c + lineSeparator);
}
}
finally {
out.close( );
}
}
}
Here's a sample of the file this program writes when the MacRoman encoding is
specified:
213: Õ
214: Ö
215: ?
216: Ø
217: Ù
218: Ú
219: û
220: Ü
221: ?
222: ?
223: ß
224: à
MacRoman is a one byte encoding so it can only hold about 256 different characters.
The remaining characters are all replaced by the substitution character, a question
mark. Unicode characters 215, 221, and 222 just don't exist in this character set.
20.3. The java.io.Reader Class
You use a reader almost exactly as you use an input stream. Rather than reading
bytes, you read characters. The basic read( ) method reads a specified number of
characters from the underlying input stream into an array starting at a given offset:
public abstract int read(char[] text, int offset, int length) throws IOException
This read( ) method returns the number of characters actually read. As with input
streams reading bytes, there may not be as many characters available as you
requested. Also like the read( ) method of an input stream, it returns -1 when it
detects the end of the data.
This read( ) method is abstract. Concrete subclasses that read bytes from some
source must override this method. An IOException may be thrown if the underlying
stream's read( ) method throws an IOException or an encoding error is detected.
You can also fill an array with characters using this method:
public int read(char[] text) throws IOException
This is equivalent to invoking read(text, 0, text.length). Thus, it also returns the
number of characters read and throws an IOException when the underlying stream
throws an IOException or when an encoding error is detected. The following method
reads a single character and returns it: public int read( ) throws IOException
Although an int is returned, this int is always between 0 and 65,535 and may be cast
to a char without losing information. All three read( ) methods block until some input
is available, an I/O error occurs, or the end of the stream is reached.
In Java 5, Reader implements the java.lang.Readable interface which requires the ability
to read directly into a CharBuffer starting at the buffer's current position:
public int read(CharBuffer target) // Java 5
throws IOException, NullPointerException, ReadOnlyBufferException
Like the other read( ) methods, this method returns the number of characters read,
or -1 on end of stream.
You can skip a certain number of characters. This method also blocks until some
characters are available. It returns the number of characters skipped or -1 if the end
of stream is reached.
public long skip(long n) throws IOException
The ready( ) method returns TRue if the reader is ready to be read from or false if it
isn't. Generally, this means the underlying stream has available data.
public boolean ready( ) throws IOException
This is not quite the same as InputStream's available( ) method. available( ) returns an
int specifying how many bytes are available to be read. However, it's not always
possible to tell how many characters are available in a stream without actually
reading them, particularly with encodings that use characters of different widths
(such as UTF-8, where a character may be one, two, three, or four bytes long).
Like input streams, some readers support marking and resetting, and some don't.
The markSupported( ) method returns true if the Reader supports marking and resetting
or false if it doesn't.
public boolean markSupported( )
public void mark(int readAheadLimit) throws IOException
public void reset( ) throws IOException
The close( ) method closes the Reader and releases any resources the reader held:
public abstract void close( ) throws IOException
20.4. The InputStreamReader Class
The most important concrete subclass of Reader is InputStreamReader:
public class InputStreamReader extends Reader
The constructor connects a character reader to an underlying input stream:
public InputStreamReader(InputStream in)
public InputStreamReader(InputStream in, String encoding)
throws UnsupportedEncodingException
The first constructor uses the platform's default encoding, as given by the system
property file.encoding. The second one uses the specified encoding. For example, to
attach an InputStreamReader to System.in with the default encoding:
InputStreamReader isr = new InputStreamReader(System.in);
If you want to read a file encoded in Latin-5 (ASCII plus Turkish, as specified by ISO
8859-9), you might do this:
FileInputStream fin = new FileInputStream("turkish.txt");
InputStreamReader isr = new InputStreamReader(fin, "8859_9");
In Java 1.4 and later, you can specify the encoding as a Charset or CharsetDecoder object
instead:
public InputStreamReader(InputStream in, Charset encoding) // Java 1.4
public InputStreamReader(InputStream in, CharsetDecoder decoder) //
Java 1.4
The read( ) methods read bytes from an underlying input stream and convert those
bytes to characters according to the specified encoding:
public int read( ) throws IOException
public int read(char[] text, int offset, int length) throws IOException
public int read(CharBuffer target) // Java 5
throws IOException, NullPointerException, ReadOnlyBufferException
The getEncoding( ) method returns a string containing the name of the encoding used
by this reader:
public String getEncoding( )
The remaining two methods just override methods from java.io.Reader but behave
identically from the perspective of the programmer:
public boolean ready( ) throws IOException
public void close( ) throws IOException
The close( ) method does close the underlying input stream.
InputStreamReader does not itself support marking and resetting, though it can be
chained to a reader that does.
Example 20-2 uses an InputStreamReader to read a file in a user-specified encoding. The
FileConverter reads the name of the input file, the name of the output file, the input
encoding, and the output encoding. Characters that are not available in the output
character set are replaced by the substitution character.
Example 20-2. CharacterSetConverter
import java.io.*;
public class StreamRecoder {
public static void main(String[] args) {
if (args.length < 2) {
System.err.println(
"Usage: java StreamRecoder "
+ "infile_encoding outfile_encoding infile outfile");
return;
}
InputStreamReader isr = null;
OutputStreamWriter osw = null;
try {
File infile = new File(args[2]);
File outfile = new File(args[3]);
if (outfile.exists( )
&&
infile.getCanonicalPath().equals(outfile.getCanonicalPath( )))
{
System.err.println("Can't convert file in place");
return;
}
FileInputStream fin = new FileInputStream(infile);
FileOutputStream fout = new FileOutputStream(outfile);
isr = new InputStreamReader(fin, args[0]);
osw = new OutputStreamWriter(fout, args[1]);
while (true) {
int c = isr.read( );
if (c == -1) break; // end of stream
osw.write(c);
}
osw.close( );
isr.close( );
}
catch (IOException ex) {
System.err.println(ex);
ex.printStackTrace( );
}
finally {
if (isr != null) {
try {
isr.close( );
} catch (IOException ex) {
ex.printStackTrace( );
}
}
if (osw != null) {
try {
osw.close( );
}
catch (IOException ex) {
ex.printStackTrace( );
}
}
}
}
}
Since this is just a simple example, I haven't put a lot of effort into the user
interface. A more realistic command-line interface would provide a set of flags and
sensible defaults. Even better would be a graphical user interface. I'll demonstrate
that at the end of the chapter, when we return to the file viewer program.
Example 20-2 is very similar to the Recoder class in Example 19-3 in the previous
chapter. However, that class accessed the CharsetEncoder and CharsetDecoder more
directly. This is a higher level approach that hides a lot of the implementation detail,
which makes it much simpler and easier to understand. Most of the time in
streaming situations, it's going to be a lot easier to use InputStreamReader and/or
OutputStreamWriter than Charset or CharsetEncoder/CharsetDecoder. Charset, CharsetEncoder, and
CharsetDecoder fit better when you have one large block of text or bytes to encode or
decode rather than an ongoing stream. Charset, CharsetEncoder, and CharsetDecoder also
offer a few more configuration options, especially for handling encoding errors in the
input data. However, usually the way InputStreamReader and OutputStreamWriter handle
this (replacing each malformed byte with the default substitution character) is fine.

20.5. Encoding Heuristics
There's no 100% guaranteed way to determine the encoding of an arbitrary file or
stream. However, you can make some reasonable guesses that are likely to be more
correct than not.
Unicode files are especially easy to detect because most such files begin with a byte
order mark. This is the Unicode zero-width nonbreaking space character that has
code point 0xFEFF. The byte-swapped character 0xFFFE is never a legal Unicode
character. Furthermore the single bytes 0xFF and 0XFE are uncommon in most
single-byte encodings like Latin-1 and MacRoman, unlikely to occur in sequence, and
unlikely to occur at the beginning of a stream. Therefore, a stream that begins with
the two bytes 0XFF and 0xFE in that order is almost certainly encoded in big-endian
UTF-16. A stream that starts with the opposite order (0XFE and 0xFF) is almost
certainly encoded in little-endian UTF-16.
In UTF-8, the zero-width nonbreaking space is represented by the three bytes 0xEF
0xBB 0xBF, always in that order. Thus, any file that begins with these three bytes is
almost certainly UTF-8. However, not all UTF-8 files begin with a byte order mark,
but UTF-8 is a very picky standard, and it's unlikely that any non-UTF-8 file will
accidentally parse correctly as UTF-8. If you think a file might be UTF-8, try reading a
few hundred characters as UTF-8. If there are no exceptions, chances are very good
it is UTF-8.
Some other encodings of Unicode such as UTF-32 can be also be detected by
inspecting the byte order mark. However, these are mostly of theoretical interest.
I've never encountered one in the wild.
If a file isn't Unicode, life is tougher. Most single-byte character sets are supersets of
ASCII, so even if you guess wrong, the majority of the text is likely to come through
unchanged. Latin-1 misread as MacRoman or vice versa isn't pretty. However, it is
intelligible in most cases.
If you have some idea of the file type, there may be other ways to guess the
encoding. For instance, all XML documents that are not written in Unicode must
begin with an XML declaration that includes an encoding declaration:
<?xml version="1.0" encoding="SJIS"?>
Other than Unicode and EBCDIC, most character sets are supersets of ASCII so you
can assume the encoding is ASCII, read far enough in the stream to find the
encoding declaration, then back up and reread the document with the correct
encoding. To detect Unicode, look for a byte order mark. To detect EBCDIC-encoded
XML, look for the initial four bytes 0x4C 0x6F 0xA7 0x94 in that order. This is "<?xm"
in all EBCDIC variants.
HTML is similar. You treat the file as ASCII or EBCDIC just long enough to read the
encoding meta tag:
<meta http-equiv="content-type" content="text/html; charset=sjis" />
However unlike XML, HTML is case-insensitive, so you also need to look for variants
like this:
<META Http-equiv="content-type" Content="text/html; charset=sjis" />
Either way, once you've found the meta element, you back up and start over once you
know the encoding. (mark( ) and reset( ) are very helpful here.)
Sometimes there's metadata outside the file or stream that can help you. For
instance, HTTP servers normally send a Content-type header that may include a
charset parameter like this one:
Content-type: text/html; charset=sjis
If there's no explicit parameter, the protocol may give you enough information. For
instance, HTTP specifies that all text/* documents are assumed to be Latin-1 (ISO8859-1) unless explicitly specified otherwise.
Following these rules along with a smattering of local knowledge will probably suffice
most of the time. If it's not enough, there are still more sophisticated tricks you can
try. For instance, you can spellcheck a document in a variety of encodings and see
which one generates the fewest errors for words containing non-ASCII characters. Of
course, this requires you to know or make a reasonable guess at the language. That
too can be done based on the stream contents if necessary. Honestly, though, very
few programs need to make this level of effort.
20.6. Character Array Readers and Writers
The ByteArrayInputStream and ByteArrayOutputStream classes use stream methods to read
and write arrays of bytes. The CharArrayReader and CharArrayWriter classes use Reader and
Writer methods to read and write arrays of chars. Since char arrays are purely internal
to Java, this is one of the few uses of readers and writers where you don't need to
concern yourself with conversions between different encodings. If you want to read
arrays of text encoded in some non-Unicode encoding, you should chain a
ByteArrayInputStream to an InputStreamReader instead. Similarly, to write text into a byte
array in a non-Unicode encoding, just chain an OutputStreamWriter to a
ByteArrayOutputStream.
20.6.1. The CharArrayWriter Class
A CharArrayWriter maintains an internal array of chars into which successive characters
are written. The array is expanded as needed. This array is stored in a protected
field called buf:
protected char[] buf
The no-argument constructor creates a CharArrayWriter object with a 32-character
buffer. This is on the small side, so you can expand it with the second constructor:
public CharArrayWriter( )
public CharArrayWriter(int initialSize)
The write( ) methods write their characters into the buffer. If there's insufficient
space in buf to hold the characters, it's expanded to at least the amount necessary
to hold the extra text.
The buffer can be read in several ways. The writeTo( ) method copies the text in the
buffer onto another Writer object:
public void writeTo(Writer out) throws IOException
The toCharArray( ) method returns a copy of the text in the buffer:
public char[] toCharArray( )
Changes to the copy do not affect the CharArrayWriter's internal data and vice versa.
The toString( ) method returns a string initialized from the characters stored in the
buffer:
public String toString( )
The size( ) method returns the number of characters currently stored in the buffer:
public int size( )
Finally, the reset( ) method empties the buffer so that the writer can be reused for
new data:
public void reset( )
However, the internal buffer is not freed and still occupies memory. It will only be
garbage collected when the writer itself is.
There is a close( ) method, but it's a no-op. In fact, you can continue writing to a
CharArrayWriter after it's been closed. This should probably be classified as a bug.
For example, the following code fragment fills a char array with the Unicode Basic
Multilingual Plane:
CharArrayWriter caw = new CharArrayWriter(65536);
for (int i = 0; i < 65536; i++) {
caw.write(i);
}
caw.close( );
char[] unicode = caw.toCharArray( );
20.6.2. The CharArrayReader Class
A CharArrayReader uses an array of chars as the underlying source of text to read. It is
one of the few readers that does not have an underlying input stream; it has an
underlying char array instead. This array is set in the constructor. Either an entire
array may be used or a specified subarray beginning at offset and continuing for
length characters:
public CharArrayReader(char[] text)
public CharArrayReader(char[] text, int offset, int length)
The CharArrayReader class stores a reference to the text array in a protected field called
buf. A separate copy is not made. Modifying this array from outside the class can
violate data encapsulation and potentially cause thread synchronization problems.
The reader also stores the current position in the array (the index of the next array
component that will be returned by read( )), the number of chars in the array, and the
current mark, if any.
protected char[] buf
protected int pos
protected int count
protected int markedPos
The read( ) methods read text from the buf array, updating the pos field as they do so.
These methods behave like any other reader's read( ) methods. If the end of the
array is reached, they return -1.
CharArrayReaders support marking and resetting to the limit of the length of the array.
markSupported( ) returns true. mark( ) marks the current position in the stream by
setting markedPos equal to pos. The readAheadLimit argument is for compatibility; its value
is ignored. The reset( ) method sets pos equal to markedPos.
Finally, the close( ) method sets buf to null so that the array can be garbage
collected. Attempts to read from a CharArrayReader after it's been closed throw
IOExceptions.
20.7. String Readers and Writers
The java.io.StringReader and java.io.StringWriter classes allow programmers to use
Reader and Writer methods to read and write strings. Like char arrays, Java strings are
composed of pure Unicode characters. Therefore, they're good sources of data for
readers and good targets for writers. This is the other common case where readers
and writers don't need to convert between different encodings.
20.7.1. String Writers
This class would more accurately be called StringBufferWriter, but StringWriter is more
poetic. A StringWriter maintains an internal StringBuffer to which it appends
characters. This buffer can easily be converted to a string as necessary. StringWriter
has a no-args constructor:
public StringWriter( )
There is also a constructor that allows you to specify the initial size of the internal
string buffer. This isn't too important because string buffers (and, by extension,
string writers) are expanded as necessary. Still, if you can estimate the size of the
string in advance, it's more efficient to select a size big enough to hold all characters
that will be written:
public StringWriter(int initialSize)
The StringWriter class has the usual collection of write( ) methods, all of which just
append their data to the StringBuffer.
There are flush( ) and close( ) methods, but both have empty method bodies, as
string writers operate completely internal to Java and do not require flushing or
closing. You can continue to write to a StringWriter even after it's been closed. This
should probably be classified as a bug, and I don't recommend that you write code
that relies on this behavior.
There are two ways to get the current contents of the StringWriter's internal buffer.
The toString( ) method returns it as a new String object while the getBuffer( ) method
returns the actual buffer:
public String toString( )
public StringBuffer getBuffer( )
Strings are immutable, but changes to the buffer object returned by getBuffer( )
change the state of the StringWriter.
The following code fragment creates a string containing the printable ASCII
character set:
StringWriter sw = new StringWriter(128);
for (int i = 32; i < 127; i++) {
sw.write(i);
}
String ascii = sw.toString( );
20.7.2. String Readers
A StringReader uses the methods of the Reader class to get characters from a string.
This is useful when you want to process each character in a string in sequential
order. This class replaces the deprecated StringBufferInputStream class:
public class StringReader extends Reader
The single constructor sets the string that's the source of data for this reader:
public StringReader(String s)
Since string objects are immutable, the data in the string may not be changed after
the StringReader is constructed.
Of course, the class has the usual read( ) methods, all of which read as many
characters as requested from the string. These methods return -1 if the end of the
string has been reached. They throw an IOException if the reader has been closed.
The ready( ) method returns TRue. Strings are always ready to be read.
String readers support marking and resetting to the limit of the string's length.
markSupported( ) returns TRue. mark( ) marks the current position in the stream. (The
readAheadLimit argument is for compatibility only; its value is ignored.) The reset( )
method moves backward in the string to the marked position.
Finally, the close( ) method sets the internal string data to null. Attempts to read
from a StringReader after it's been closed throw IOExceptions.
Here's a simple method that uses StringReader to break a string into its separate
characters and print them:
public static void printCharacters(String s) {
StringReader sr = new StringReader(s);
try {
int c;
while ((c = sr.read( )) != -1) {
System.out.println((char) c);
}
}
catch (IOException ex) {
// should not happen; StringReaders do not throw exceptions
}
return;
}
Admittedly, this is a contrived example. If you really needed to do this, you could
just loop through the string itself using its charAt( ) method.
20.8. Reading and Writing Files
You've already learned how to chain an OutputStreamWriter to a FileOutputStream and an
InputStreamReader to a FileInputStream. Although this isn't hard, Java provides two simple
utility classes that take care of the details, java.io.FileWriter and java.io.FileReader .
20.8.1. FileWriter
The FileWriter class is a subclass of OutputStreamWriter that writes text files using the
platform's default character encoding and buffer size. If you need to change these
values, construct an OutputStreamWriter on a FileOutputStream instead.
public class FileWriter extends OutputStreamWriter
This class has four constructors:
public FileWriter(String fileName) throws IOException
public FileWriter(String fileName, boolean append) throws IOException
public FileWriter(File file) throws IOException
public FileWriter(FileDescriptor fd)
The first constructor opens a file and positions the file pointer at the beginning of the
file. Any text in the file is overwritten. For example:
FileWriter fw = new FileWriter("36.html");
The second constructor allows you to specify that new text is appended to the
existing contents of the file rather than overwriting it by setting the second
argument to TRue. For example:
FileWriter fw = new FileWriter("36.html", true);
The third and fourth constructors use a File object and a FileDescriptor, respectively,
instead of a filename to identify the file to be written to. Any preexisting contents in
a file so opened are overwritten.
You use the standard Writer methods like write( ), flush( ), and close( ) to write the
text in the file.
20.8.2. FileReader
The FileReader class is a subclass of InputStreamReader that reads text files using the
platform's default character encoding. If you need to change the encoding, construct
an InputStreamReader chained to a FileInputStream instead.
public class FileReader extends InputStreamReader
This class has three constructors that differ only in how the file to be read is
specified:
public FileReader(String fileName) throws FileNotFoundException
public FileReader(File file) throws FileNotFoundException
public FileReader(FileDescriptor fd)
Only the constructors are declared in this class. You use the standard Reader methods
like read( ), ready( ), and close( ) to read the text in the file.
FileReader and FileWriter always use the local default encoding for
converting characters to and from bytes. This is rarely what you
want. You should almost always specify the encoding explicitly, or
perhaps autodetect it. FileReader and FileWriter are a minor
convenience at most. Instead of using them, you can easily chain
an InputStreamReader to a FileInputStream or an OutputStreamWriter to a
FileOutputStream. Even if you know you want the default encoding,
you're better off requesting it explicitly to make your intention
clear.
20.9. Buffered Readers and Writers
Input and output can be time-consuming operations. It's often quicker to read or
write text in large chunks rather than in many separate smaller pieces, even when
you only process the text in the smaller pieces. The java.io.BufferedReader and
java.io.BufferedWriter classes provide internal character buffers. Text that's written to
a buffered writer is stored in the internal buffer and only written to the underlying
writer when the buffer fills up or is flushed. Likewise, reading text from a buffered
reader may cause more characters to be read than were requested; the extra
characters are stored in an internal buffer. Future reads first access characters from
the internal buffer and only access the underlying reader when the buffer is
emptied.
Even if the underlying stream is buffered, it still pays to buffer
the reader or writer too. Many character conversions can be done
more quickly in blocks than they can on individual characters,
irrespective of I/O speed. That is, for maximum performance use
a BufferedReader and a BufferedInputStream or a BufferedWriter and a
BufferedOutputStream.
20.9.1. Buffering Writes
The java.io.BufferedWriter class is a subclass of java.io.Writer that you chain to another
Writer class to buffer characters. This allows more efficient writing of text.
public class BufferedWriter extends Writer
There are two constructors. One has a default buffer size (8192 characters); the
other lets you specify the buffer size:
public BufferedWriter(Writer out)
public BufferedWriter(Writer out, int size)
For example:
BufferedWriter bw = new BufferedWriter(new FileWriter("37.html"));
BufferedWriter overrides most of its superclass's methods, but all changes are purely
internal. write( ), flush( ), close( ), etc. are all used exactly as they are for any writer
object.
There is one new method in this class, newLine( ). This method writes a platformdependent line terminator string: \n on Unix, \r on the Mac, \r\n on Windows. The
value of this string is taken from the system property line.separator.
public String newLine( ) throws IOException
Do not use the newLine( ) method if you're writing network code
such as an HTTP server. Instead, explicitly write the carriage
return/linefeed pair. Most network protocols specify a \r\n line
separator, regardless of host-platform conventions.
Example 20-3 is a revised version of Example 20-1 that uses a BufferedWriter to
increase efficiency and handle platform-dependent line separators.
Example 20-3. BufferedUnicodeTable
import java.io.*;
public class BufferedBMPTable {
public static void main(String[] args) throws IOException {
// Use platform default with a fallback to Latin-1 if
necessary
String encoding = System.getProperty("file.encoding",
"ISO-8859-1");
String lineSeparator =
System.getProperty("line.separator", "\r\n");
OutputStream target = System.out;
if (args.length > 0) target = new
FileOutputStream(args[0]);
if (args.length > 1) encoding = args[1];
BufferedWriter out = null;
try {
out = new BufferedWriter(new OutputStreamWriter(target,
encoding));
}
catch (UnsupportedEncodingException ex) { // platform
default encoding
out = new BufferedWriter(new
OutputStreamWriter(target));
}
try {
for (int i = Character.MIN_VALUE; i <=
Character.MAX_VALUE; i++) {
if (!Character.isDefined(i)) continue;
char c = (char) i;
if (Character.isHighSurrogate(c) ||
Character.isLowSurrogate(c)) continue;
out.write(i + ":\t" + c);
out.newLine( );
}
}
finally {
out.close( );
}
}
}
This is actually not the fastest you can go. BufferedWriter is internally synchronized.
Each call to one of its methods is atomic. If two threads try to write onto the same
BufferedWriter at the same time, one of them blocks. This prevents the threads from
corrupting the data. However, this synchronization has a performance cost, even
when only one thread has access to the writer. You can often improve performance
by replacing the stock BufferedWriter from java.io with an unsynchronized version such
as shown in Example 20-4. When you don't need to worry about synchronization,
this version can increase speed by 30-50%, though as always exact performance
gains are likely to vary from one VM to the next.
Example 20-4. UnsynchronizedBufferedWriter
package com.elharo.io;
import java.io.*;
public class UnsynchronizedBufferedWriter extends Writer {
private final static int CAPACITY = 8192;
private char[] buffer = new char[CAPACITY];
private int position = 0;
private Writer out;
private boolean closed = false;
public UnsynchronizedBufferedWriter(Writer out) {
this.out = out;
}
public void write(char[] text, int offset, int length)
throws IOException {
checkClosed( );
while (length > 0) {
int n = Math.min(CAPACITY - position, length);
System.arraycopy(text, offset, buffer, position, n);
position += n;
offset += n;
length -= n;
if (position >= CAPACITY) flushInternal( );
}
}
public void write(String s) throws IOException {
write(s, 0, s.length( ));
}
public void write(String s, int offset, int length) throws
IOException {
checkClosed( );
while (length > 0) {
int n = Math.min(CAPACITY - position, length);
s.getChars(offset, offset + n, buffer, position);
position += n;
offset += n;
length -= n;
if (position >= CAPACITY) flushInternal( );
}
}
public void write(int c) throws IOException {
checkClosed( );
if (position >= CAPACITY) flushInternal( );
buffer[position] = (char) c;
position++;
}
public void flush( ) throws IOException {
flushInternal( );
out.flush( );
}
private void flushInternal( ) throws IOException {
if (position != 0) {
out.write(buffer, 0, position);
position = 0;
}
}
public void close( ) throws IOException {
closed = true;
this.flush( );
out.close( );
}
private void checkClosed( ) throws IOException {
if (closed) throw new IOException("Writer is closed");
}
}
All characters are first written into an internal byte array of length 8192. Only when
that buffer fills up is it flushed to the underlying writer. The java.io.BufferedWriter class
is organized very much like this, except that it also has a number of synchronized
blocks to permit threadsafe usage.
20.9.2. Buffering Reads
BufferedReader is a subclass of Reader that is chained to another Reader class to buffer
input. This allows more efficient reading of characters and lines.
public class BufferedReader extends Reader
For example:
BufferedReader br = new BufferedReader(new FileReader("37.html"));
There are two constructors. One has a default buffer size (8192 characters); the
other requires the programmer to specify the buffer size:
public BufferedReader(Reader in, int buffer_size)
public BufferedReader(Reader in)
In a BufferedReader, the two multicharacter read( ) methods try to completely fill the
specified array or subarray of text by reading repeatedly from the underlying reader.
They return only when the requested number of characters have been read, the end
of the data is reached, or the underlying reader would block. This is not the case for
most readers which attempt only one read from the underlying data source before
returning.
BufferedReader does support marking and resetting, at least up to the length of the
buffer. Another reason to use a BufferedReader is to enable marking and resetting on a
reader that otherwise wouldn't support it, such as an InputStreamReader.
Besides buffering, BufferedReader is notable for its readLine( ) method that allows you
to read text a line at a time. This replaces the common but deprecated readLine( )
method in DataInputStream.
public String readLine( ) throws IOException
This method returns a string that contains a line of text from a text file. \r, \n, and
\r\n are assumed to be line breaks and are not included in the returned string. This
method is often used when reading user input from System.in since most platforms
only send the user's input to the running program after the user has typed a full line
(that is, hit the Enter key).
readLine( ) can hang if the last character of the stream is not a
carriage return or a linefeed and the sender does not close the
stream. This problem tends to arise on network connections
where the client or server keeps a socket open for a response
after sending its data. For this reason, readLine( ) should not be
used in network programming.
Example 20-5 uses a BufferedReader and readLine( ) to read all files named on the
command line, line by line, and copy them to System.out. In essence it implements the
Unix cat or the DOS type utility.
Example 20-5. The cat program
import java.io.*;
class Cat {
public static void main (String[] args) {
String thisLine;
for (int i=0; i < args.length; i++) {
try {
BufferedReader br = new BufferedReader(new
FileReader(args[i]));
while ((thisLine = br.readLine( )) != null) {
System.out.println(thisLine);
} // end while
} // end try
catch (IOException ex) {System.err.println(ex);}
} // end for
} // end main
}
20.9.3. Line Numbering
LineNumberReader is a subclass of BufferedReader that keeps track of which line it's
currently reading. It also has methods to get and set the line number. This class
replaces the deprecated LineNumberInputStream class.
public class LineNumberReader extends BufferedReader
This class has two constructors. Both chain this reader to an underlying reader; the
second also sets the size of the buffer.
public LineNumberReader(Reader in)
public LineNumberReader(Reader in, int size)
LineNumberReader has all the methods of BufferedReader, including readLine( ). These are
overridden to keep track of the line number. The behavior of these methods is not
changed.
LineNumberReader also introduces two methods for inspecting and changing the line
number:
public int getLineNumber( )
public void setLineNumber(int lineNumber)
The setLineNumber( ) method does not change the line that you're reading in the file. It
just changes the value getLineNumber( ) returns. For example, it would allow you to
start counting from -5 if you knew there were six lines of header data, and you
wanted line 1 to be the first line of the body text.
Example 20-6 uses a LineNumberReader and readLine( ) to read all files named on the
command line, line by line, and copy them to System.out, prefixing each line with its
line number.
Example 20-6. The LineCat Program
import java.io.*;
class LineCat {
public static void main (String[] args) {
String thisLine;
for (int i=0; i < args.length; i++) {
try {
LineNumberReader br = new LineNumberReader(new
FileReader(args[i]));
while ((thisLine = br.readLine( )) != null) {
System.out.println(br.getLineNumber( ) + ": " +
thisLine);
} // end while
} // end try
catch (IOException ex) {System.err.println(ex);}
} // end for
} // end main
}

20.10. Print Writers
The java.io.PrintWriter class is a subclass of java.io.Writer that contains the familiar
print( ) and println( ) methods from System.out and other instances of PrintStream. In
Java 5 and later, it also has all the format( ) and printf( ) methods introduced in
Chapter 7. It's deliberately similar to the java.io.PrintStream class.
The main difference between PrintStream and PrintWriter is that PrintWriter handles
multiple-byte and other non-Latin-1 character sets properly. The other, more minor
difference is that automatic flushing is performed only when println( ) is invoked, not
every time a newline character is seen. Sun would probably like to deprecate
PrintStream and use PrintWriter instead, but that would break too much existing code.
(In fact, Sun did deprecate the PrintStream( ) constructors in Java 1.1, but they
undeprecated them in 1.2.)
There are four constructors in this class:
public PrintWriter(Writer out)
public PrintWriter(Writer out, boolean autoFlush)
public PrintWriter(OutputStream out)
public PrintWriter(OutputStream out, boolean autoFlush)
The PrintWriter can send text either to an output stream or to another writer. If
autoFlush is set to true, the PrintWriter is flushed every time println( ) is invoked.
The PrintWriter class implements the abstract write( ) method from java.io.Writer and
overrides five other methods:
public void write(int c)
public void write(char[] text)
public void write(String s)
public void write(String s, int offset, int length)
public void flush( )
public void close( )
These methods are used almost identically to their equivalents in any other Writer
class. The one difference is that none of them throw IOExceptions; in fact, no method
in the PrintWriter class ever throws an IOException. If the underlying output stream or
writer throws an IOException, it's caught inside PrintWriter and an error flag is set. Read
the status of this flag with the checkError( ) method:
public boolean checkError( )
Since checkError( ) returns a boolean, it only tells you that an I/O error has occurred; it
does not tell you what that error was. Furthermore, once an error has occurred,
checkError( ) always returns truethere is no way to reset it so you can test for later
errors.
The main advantages of the PrintWriter class are the 9-way overloaded print( )
method and the 10-way overloaded println( ) method. Any Java object, variable, or
literal can be printed by passing it to a print( ) or println( ) method. The println( )
method follows its argument with a platform-dependent line separator (such as \r\n)
and then flushes the output if autoFlush is enabled. The print( ) method does not.
Otherwise, these methods are the same.
public void print(boolean b)
public void print(char c)
public void print(int i)
public void print(long l)
public void print(float f)
public void print(double d)
public void print(char[] text)
public void print(String s)
public void print(Object 0)
public void println( )
public void println(boolean b)
public void println(char c)
public void println(int i)
public void println(long l)
public void println(float f)
public void println(double d)
public void println(char[] text)
public void println(String s)
public void println(Object o)
You should never use println( ), either the PrintWriter or the PrintStream version, in
networking code. Most network protocols like HTTP expect to see a carriage
return/linefeed pair as the line separator character. If you use println( ), your
network programs may run on Windows, but they'll have problems on most other
platforms. Furthermore, these problems can be hard to diagnose because some
servers and clients are more forgiving of improper line-ending conventions than
others.
Java 5 adds two format( ) and two printf( ) methods:
public PrintWriter printf(String format, Object... args)
throws IllegalFormatException, NullPointerException
public PrintWriter printf(Locale l, String format, Object... args)
throws IllegalFormatException, NullPointerException
public PrintWriter format(String format, Object... args)
throws IllegalFormatException, NullPointerException
public PrintWriter format(Locale l, String format, Object... args)
throws IllegalFormatException, NullPointerException
T
h
e
s
e
m
e
t
h
o
d
s
h
a
v
e
t
h
e
s
a
m
e
b
e
h
a
vio
r
a
s
t
h
e
simila
rly
n
a
m
e
d
m
e
t
h
o
d
s in
t
h
e
P
r
i
n
t
S
t
r
e
a
m
cla
s
s
dis
c
u
s
s
e
d in
C
h
a
p
t
e
r
7.
20.11. Piped Readers and Writers
Piped readers and writers do for character streams what piped input and output
streams do for byte streams: they allow two threads to communicate. Character
output from one thread becomes character input for the other thread:
public class PipedWriter
extends Writer
public class PipedReader
extends Reader
The PipedWriter class has two constructors. The first constructs an unconnected
PipedWriter object. The second constructs one that's connected to the PipedReader
object sink: public PipedWriter( ) public PipedWriter(PipedReader sink) throws
IOException
The PipedReader class also has two constructors. Again, the first constructor creates an
unconnected PipedReader object. The second constructs one that's connected to the
PipedWriter object source: public PipedReader( ) public PipedReader(PipedWriter
source) throws IOException
Piped readers and writers are normally created in pairs. The piped writer becomes
the underlying source for the piped reader. This is one of the few cases where a
reader does not have an underlying input stream. For example: PipedWriter pw =
new PipedWriter( ); PipedReader pr = new PipedReader(pw);
This simple example is a little deceptive because these lines of code will normally be
in different methods and perhaps even different classes. Some mechanism must be
established to pass a reference to the PipedWriter into the thread that handles the
PipedReader, or you can create them in the same thread and pass a reference to the
connected stream into a separate thread.
Alternatively, you can start with a PipedReader and then wrap it with a PipedWriter:
PipedReader pr = new PipedReader( );
PipedWriter pw = new PipedWriter(pr);
Or you can create them both unconnected and use one or the other's connect( )
method to link them:
public void connect(PipedReader sink) throws IOException
public void connect(PipedWriter source) throws IOException
PipedWriter's connect( ) method takes as an argument the PipedReader to connect to.
PipedReader's connect( ) argument takes as an argument the PipedWriter to connect to:
PipedReader pr = new PipedReader( ); PipedWriter pw = new PipedWriter( );
pr.connect(pw);
or:
PipedReader pr = new PipedReader( );
PipedWriter pw = new PipedWriter( );
pw.connect(pr);
Neither a PipedWriter nor a PipedReader can be connected to more than one reader or
writer. Attempts to do so throw IOExceptions. Furthermore, once connected, a
PipedWriter/PipedReader pair may not be disconnected. Otherwise, these classes have
the usual read( ), write( ), flush( ), close( ), and ready( ) methods like all reader and
writer classes.
When characters are written on the PipedWriter, that text becomes available as input
to be read by the connected PipedReader. If a PipedReader tries to read characters, but
its connected PipedWriter hasn't yet provided it with any, the PipedReader blocks.
Closing either a PipedReader or a PipedWriter also closes the reader or writer it's
connected to.
20.12. Filtered Readers and Writers
The java.io.FilterReader and java.io.FilterWriter classes are abstract classes that read
characters and filter them in some way before passing the text along. You can
imagine a FilterReader that converts all characters to uppercase.
public abstract class FilterReader extends Reader
public abstract class FilterWriter extends Writer
Although FilterReader and FilterWriter are modeled after java.io.FilterInputStream and
java.io.FilterOutputStream, they are much less commonly used than those classes.
There are no concrete subclasses of FilterWriter in the java packages and only one
concrete subclass of FilterReader (PushbackReader). These classes exist so you can write
your own filters.
20.12.1. The FilterReader Class
FilterReader has a single constructor, which is protected:
protected FilterReader(Reader in)
The in argument is the Reader to which this filter is chained. This reference is stored in
a protected field called in from which text for this filter is read and is null after the
filter has been closed.
protected Reader in
Since FilterReader is an abstract class, only subclasses can be instantiated. Therefore,
it doesn't matter that the constructor is protected since it may only be invoked from
subclass constructors.
FilterReader provides the usual collection of read( ), skip( ), ready( ), markSupported( ),
mark( ), reset( ), and close( ) methods. These all simply invoke the equivalent method
in the in field with the same arguments. For example, the skip( ) method works like
this:
public long skip(long n) throws IOException {
return in.skip(n);
}
Each subclass usually overrides at least these two read( ) methods to perform the
filtering:
public int read( ) throws IOException
public int read(char[] text, int offset, int length) throws IOException
In FilterReader, neither method invokes the other. You must override each of them,
even if it's only to call the other one.
Java source code can include Unicode escapes for characters not available in the
current character set. An escape sequence is a \u followed by the four-hexadecimaldigit equivalent of the Unicode character. As an example, I'll write a FilterReader
subclass that reads a \u-escaped file and converts it to pure Unicode. This is a much
trickier problem than it first appears. First, there's not a fixed ratio between the
number of bytes and number of chars. Most of the time one byte is one char, but
some of the time five bytes are one char. The second difficulty is ensuring that \u09EF
is recognized as Unicode escape while \\u09EF is not. In other words, only a u
preceded by an odd number of slashes is a valid Unicode escape. A u preceded by an
even number of slashes should be passed along unchanged. Example 20-7 shows a
solution.
Example 20-7. SourceReader
package com.elharo.io;
import java.io.*;
public class SourceReader extends FilterReader {
public SourceReader(Reader in) {
super(in);
}
private int buffer = -1;
public int read( ) throws IOException {
if (this.buffer != -1) {
int c = this.buffer;
this.buffer = -1;
return c;
}
int c = in.read( );
if (c != '\\') return c;
int next = in.read( );
if (next != 'u' ) { // This is not a Unicode escape
this.buffer = next;
return c;
}
// Read next 4 hex digits
// If the next four chars do not make a valid hex digit
// this is not a valid .java file.
StringBuffer sb = new StringBuffer( );
sb.append((char) in.read( ));
sb.append((char) in.read( ));
sb.append((char) in.read( ));
sb.append((char) in.read( ));
String hex = sb.toString( );
try {
return Integer.valueOf(hex, 16).intValue( );
}
catch (NumberFormatException ex) {
throw new IOException("Bad Unicode escape: \\u" + hex);
}
}
private boolean endOfStream = false;
public int read(char[] text, int offset, int length) throws
IOException {
if (endOfStream) return -1;
int numRead = 0;
for (int i = offset; i < offset+length; i++) {
int temp = this.read( );
if (temp == -1) {
this.endOfStream = true;
break;
}
text[i] = (char) temp;
numRead++;
}
return numRead;
}
public long skip(long n) throws IOException {
char[] c = new char[(int) n];
int numSkipped = this.read(c);
return numSkipped;
}
}
20.12.2. The FilterWriter Class
The FilterWriter class has a single constructor and no other unique methods:
protected FilterWriter(Writer out)
The out argument is the writer to which this filter is chained. This reference is stored
in a protected field called out to which text sent through this filter is written.
protected Writer out
Since FilterWriter is an abstract class, only subclasses may be instantiated.
Therefore, it doesn't matter that the constructor is protected since it may only be
invoked from subclass constructors anyway. FilterWriter provides the usual collection
of write( ), close( ), and flush( ) methods. These all simply invoke the equivalent
method in the out field with the same arguments. For example, the close( ) method
works like this:
public void close( ) throws IOException {
out.close( );
}
Each subclass has to override at least these three write( ) methods to perform the
filtering:
public void write(int c) throws IOException
public void write(char[] text, int offset, int length) throws
IOException
public void write(String s, int offset, int length) throws IOException
In FilterWriter, these methods do not invoke each other. You must override each of
them, even if it's only to call one of the other two.
There are no subclasses of FilterWriter in the core API. Example 20-8, SourceWriter, is
an example of a FilterWriter that converts Unicode text to \u-escaped ASCII. The big
question is what to do if the input text contains an unescaped backslash. The
simplest and most robust solution is to replace it with \u005C, the Unicode escape for
the backslash itself.
Example 20-8. SourceWriter
package com.elharo.io;
import java.io.*;
public class SourceWriter extends FilterWriter {
public SourceWriter(Writer out) {
super(out);
}
public void write(char[] text, int offset, int length)
throws IOException {
for (int i = offset; i < offset+length; i++) {
this.write(text[i]);
}
}
public void write(String s, int offset, int length) throws
IOException {
for (int i = offset; i < offset+length; i++) {
this.write(s.charAt(i));
}
}
public void write(int c) throws IOException {
// We have to escape the backslashes below.
if (c == '\\') out.write("\\u005C");
else if (c < 128) out.write(c);
else {
String s = Integer.toHexString(c);
// Pad with leading zeroes if necessary.
if (c < 256) s = "00" + s;
else if (c < 4096) s = "0" + s;
out.write("\\u");
out.write(s);
}
}
}
20.12.3. PushbackReader
The PushbackReader class is a filter that provides a pushback buffer around a given
reader. This allows a program to "unread" the last character it read. It's similar to
PushbackInputStream discussed in Chapter 6, but instead of pushing back bytes, it
pushes back chars. Both PushbackReader and BufferedReader use buffers, but only
PushbackReader allows unreading and only BufferedReader allows marking and resetting.
The first difference is that pushing back characters allows you to unread characters
after the fact. Marking and resetting requires you to mark in advance the location
you want to reset to. The second difference is that you can push back a character
that was never on the stream in the first place. Marking and resetting only allows
you to reread the same characters, not add new characters to the stream.
PushbackReader has two constructors, both of which take an underlying reader as an
argument. The first uses a one-character pushback buffer; the second sets the
pushback buffer to a specified size:
public PushbackReader(Reader in)
public PushbackReader(Reader in, int size)
The PushbackReader class has the usual collection of read( ) methods. These methods
first try to read the requested characters from the pushback buffer and only read
from the underlying reader if the pushback buffer is empty or has too few
characters.
PushbackReader also has ready( ), markSupported( ), and close( ) methods. The ready( ) and
close( ) methods merely invoke the ready( ) and close( ) methods of the underlying
reader. The markSupported( ) method returns false; pushback readers do not support
marking and resetting.
Three unread( ) methods push back specific characters. The first pushes back the
character c, the second pushes back the text array, and the third pushes back the
subarray of text beginning at offset and continuing for length chars.
public void unread(int c) throws IOException
public void unread(char[] text) throws IOException
public void unread(char[] text, int offset, int length) throws
IOException
The unread characters aren't necessarily the same as the characters that were read.
The client programmer can insert text as the stream is read. The number of
characters you can push back onto the stream is limited by the size of the buffer set
in the constructor. Attempts to unread more characters than can fit in the buffer
throw an IOException. An IOException is also thrown if you try to unread a closed reader;
once a PushbackReader has been closed, it can be neither read nor unread.


# 1. Introducing I/O

// a stream is a sequence of bytes of indeterminate length

// byte-oriented i/o is handled by input and output streams while character i/o is handled by readers and writers.


// STREAMS

// a stream is an ordered sequence of bytes of indeterminate length

// input streams move bytes of data into a Java program from some generally external source while output streams move bytes of data from Java to some generally external target

an input stream may read from a finite source of bytes such as a file or an unlimited source of bytes such as System.in

Similarly, an output stream may have a definite number of bytes to output or an indefinite number of bytes.]


The first source of input most programmers encounter is System.in (i.e. standard input).

The console is also available for output through the static field out in the
java.lang.System class, that is, System.out 
stderr is available as System.err .

Both System.out and System.err are print streamsthat is, instances of java.io.PrintStream.

Files are another common source of input and destination for output. File input
streams provide a stream of data that starts with the first byte in a file and finishes
with the last byte in that file.

File output streams write data into a file, either by
erasing the file s contents and starting from the beginning or by appending data to
the file.

Network connections provide streams too. When you connect to a web server, FTP
server, or some other kind of server, you read the data it sends from an input stream
connected from that server and write data onto an output stream connected to that
server

The two main classes are java.io.InputStream and java.io.OutputStream . These are
abstract base classes for many different subclasses with more specialized abilities.

Input streams read bytes and output streams write bytes. Readers read characters
and writers write characters.


Input and output streams are fundamentally byte-based. Readers and writers are based on characters, which can have varying widths depending on the character set.

Since characters are ultimately composed of bytes, readers take their input from
streams. However, they convert those bytes into chars according to a specified
encoding format before passing them along. Similarly, writers convert chars to bytes
according to a specified encoding before writing them onto some underlying stream.

The java.io.Reader and java.io.Writer classes are abstract superclasses for classes that
read and write character-based data. The subclasses are notable for handling the
conversion between different character sets.



1.5. Buffers and Channels

In nonblocking I/O, streams
are relegated mostly to a supporting role while the real work is done by channels
and buffers. Input buffers are filled with data from the channel and then drained of
data by the application. Output buffers work in reverse: the application fills them
with data that is subsequently drained out by the target. The design is such that the
writer and reader don't always have to operate in lockstep with each other. Most
importantly, the client application can queue reads and writes to each channel. It
does not have to stop processing simply because the other end of the channel isn't
quite ready. This enables one thread to service many different channels
simultaneously, dramatically reducing the load on the virtual machine.
Channels and buffers are also used to enable memory-mapped I/O. In memorymapped I/O, files are treated as large blocks of memory, essentially as big byte
arrays. Particular parts of a mapped file can be read with statements such as int x =
file.getInt(1067) and written with statements such as file.putInt(x, 1067). The data is
stored directly to disk at the right location without having to read or write all the
data that precedes or follows the section of interest.
Channels and buffers are a little more complex than streams and bytes. However, for
certain kinds of I/O-bound applications, the performance gains are dramatic and
worth the added complexity




1.6. The Ubiquitous IOException

Almost every method that performs input or output is declared to throw a checked IOException.

The only real exceptions to this rule are the
PrintStream and PrintWriter classes which catch and eat any exceptions thrown inside a print() or println( ) method.

IOException has many subclasses and methods often throw a more
specific exception that subclasses IOException; for instance, EOFException on an unexpected end of file or UnsupportedEncodingException when you try read text in an
unknown character set.

1.7. The Console: System.out, System.in, and
System.err

The console is the default destination for output written to System.out or System.err and
the default source of input for System.in.

System.out is the first instance of the OutputStream class most programmers encounter.
In fact, it's often encountered before students know what a class or an output stream
is. Specifically, System.out is the static out field of the java.lang.System class. It's an
instance of java.io.PrintStream, a subclass of java.io.OutputStream.
System.out corresponds to stdout in Unix or C. Normally, output sent to System.out
appears on the console. As a general rule, the console converts the numeric byte
data System.out sends to it into ASCII or Latin-1 text. Thus, the following lines write the
string "Hello World!" on the console:
byte[] hello = {72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100, 33,
10,
13};
System.out.write(hello);



try {
// Do something that may throw an exception.
}
catch (Exception ex) {
System.err.println(ex);
}




System.in is the input stream connected to the console, much as System.out is the output stream connected to the console.


It's sometimes convenient to be able to redirect System.out, System.in, and System.err
from inside the running program.
public static void setIn(InputStream in)
public static void setOut(PrintStream out)
public static void setErr(PrintStream err)

System.setIn(new FileInputStream("yankees99.tab"));
System.setOut(new PrintStream(new FileOutputStream("yankees99.out")));








# 2. Output Streams

The java.io.OutputStream class declares the three basic methods you need to write bytes of data onto a stream. It also has methods for closing and flushing streams:

public void flush( ) throws IOException
public void close( ) throws IOException

OutputStream is an abstract class. Subclasses provide implementations of the abstract write(int b) method.


The fundamental method of the OutputStream class is write():

public abstract void write(int b) throws IOException

This method writes a single unsigned byte of data whose value should be between 0 and 255. If you pass a number larger than 255 or smaller than 0, it's reduced modulo 256 before being written.

```java
public class AsciiChart {
public static void main(String[] args) {

for (int i = 32; i < 127; i++) {
System.out.write(i);
// break line after every eight characters.
if (i % 8 == 7) System.out.write('\n');
else System.out.write('\t');
}
System.out.write('\n');
}
}

try {
for (int i = 32; i <= 127; i++) out.write(i);
}
catch (IOException ex) {
System.err.println(ex);
}
```

It's often faster to write data in large chunks than it is to write it byte by byte. Two
overloaded variants of the write( ) method do this:
public void write(byte[] data) throws IOException
public void write(byte[] data, int offset, int length) throws
IOException

The first variant writes the entire byte array data. The second writes only the subarray
of data starting at offset and continuing for length bytes.

```java
String s = "How are streams treating you?";
byte[] data = s.getBytes();
System.out.write(data);

public class AsciiArray {
public static void main(String[] args) {
byte[] b = new byte[(127-31)*2];
int index = 0;
for (int i = 32; i < 127; i++) {
b[index++] = (byte) i;
// Break line after every eight characters.
if (i % 8 == 7) b[index++] = (byte) '\n';
else b[index++] = (byte) '\t';
}
b[index++] = (byte) '\n';
try {
System.out.write(b);
}
catch (IOException ex) {
System.err.println(ex);
}
}
}
```

When you're through with a stream, you should close it.

To close a stream, invoke its close( ) method:

public void close( ) throws IOException


Not all streams need to be closedbyte array output streams do not need to be
closed, for example. However, streams associated with files and network
connections should always be closed when you're done with them.

```java
OutputStream out = null;

try {
out = new FileOutputStream("numbers.dat");
// Write to the stream...
} catch (IOException ex) {
System.err.println(ex);
} finally {
if (out != null) {
try {
out.close( );
}
catch (IOException ex) {
System.err.println(ex);
}
}
}


Java 5 added a Closeable interface that the OutputStream class implements:
package java.io



Many output streams buffer writes to improve performance. Rather than sending
each byte to its destination as it's written, the bytes are accumulated in a memory
buffer ranging in size from several bytes to several thousand bytes. When the buffer
fills up, all the data is sent at once. The flush( ) method forces the data to be written
whether or not the buffer is full:
public void flush( ) throws IOException
This is not the same as any buffering performed by the operating system or the
hardware. These buffers will not be emptied by a call to flush( ). (Then sync( )
method in the FileDescriptor class, discussed in Chapter 17, can sometimes empty
these buffers.)
If you use a stream for only a short time, you don't need to flush it explicitly. It
should flush automatically when the stream is closed. This should happen when the
program exits or when the close( ) method is invoked. You flush an output stream
explicitly only if you want to make sure data is sent before you're through with the
stream. For example, a program that sends bursts of data across the network
periodically should flush after each burst of data is written to the stream.
Flushing is often important when you're trying to debug a crashing program. All
streams flush automatically when their buffers fill up, and all streams should be
flushed when a program terminates normally. If a program terminates abnormally,
however, buffers may not get flushed. In this case, unless there is an explicit call to
flush( ) after each write, you can't be sure the data that appears in the output
indicates the point at which the program crashed. In fact, the program may have
continued to run for some time past that point before it crashed.
System.out, System.err, and some (but not all) other print streams automatically flush
after each call to println( ) and after each time a new line character ('\n') appears in
the string being written. You can enable or disable auto-flushing in the PrintStream
constructor.
2.4.1. The Flushable Interface
Java 5 added a Flushable interface that the OutputStream class implements:
package java.io;
public interface Flushable {
void flush( ) throws IOException;
}
Formatter and various other things that can be flushed also implement this interface.
I've never figured out the use case that justifies this extra interface either, but it's
there if for some reason you want to write a method that accepts only objects that
can be flushed as arguments, or some such.



2.5. Subclassing OutputStream
OutputStream is an abstract class that mainly describes the operations available with
any OutputStream object. Specific subclasses know how to write bytes to particular
destinations. For instance, a FileOutputStream uses native code to write data in files. A
ByteArrayOutputStream uses pure Java to write its output in an expanding byte array.
Recall that there are three overloaded variants of the write( ) method in OutputStream,
one abstract, two concrete:
public abstract void write(int b) throws IOException
public void write(byte[] data) throws IOException
public void write(byte[] data, int offset, int length) throws
IOException
Subclasses must implement the abstract write(int b) method. They often also
override the third variant, write(byte[], data int offset, int length), to improve
performance. The implementation of the three-argument version of the write( )
method in OutputStream simply invokes write(int b) repeatedlythat is:
public void write(byte[] data, int offset, int length) throws
IOException {
for (int i = offset; i < offset+length; i++) write(data[i]);
}
Most subclasses can provide a more efficient implementation of this method. The
one-argument variant of write( ) merely invokes write(data, 0, data.length); if the
three-argument variant has been overridden, this method will perform reasonably
well. However, a few subclasses may override it anyway.
Example 2-3 is a simple program called NullOutputStream that mimics the behavior of
/dev/null on Unix operating systems. Data written into a null output stream is lost.
Example 2-3. The NullOutputStream class
package com.elharo.io;
import java.io.*;
public class NullOutputStream extends OutputStream {
private boolean closed = false;
public void write(int b) throws IOException {
if (closed) throw new IOException("Write to closed
stream");
}
public void write(byte[] data, int offset, int length)
throws IOException {
if (data == null) throw new NullPointerException("data is
null");
if (closed) throw new IOException("Write to closed
stream");
}
public void close( ) {
closed = true;
}
}
The no-op flush( ) method inherited from the superclass is good enough here since
this stream really doesn't need flushing. However, note that this class does need to
check whether the stream is closed before writing anything, and check whether the
array passed in to write( ) is null. Most subclasses will need to make similar checks.
By redirecting System.out and System.err to a null output stream in the shipping version
of your program, you can disable any debugging messages that might have slipped
through quality assurance. For example:
OutputStream out = new NullOutputStream( );
PrintStream ps = new PrintStream(out);
System.setOut(ps);
System.setErr(ps);
```


# 3. Input Streams

```java
The java.io.InputStream is the abstract superclass for all input streams.

// read() reads a single single unsigned byte of data and returns the integer value of the unsigned byte or -1 if it encounters the end of the input stream
try {
  InputStream in = new FileInputStream("file.txt");

  int[] data = new int[10];

  for (int i = 0; i < data.length; i++) {
    int datum = in.read( );
    if (datum == -1) break;
    data[i] = datum;
  }
} catch (IOException ex) {
  System.err.println(ex.getMessage( ));
}

// read() reads bytes into an array and tries to read enough data to fill the array and returns the number of bytes read or -1 on end of stream
try {
byte[] b = new byte[10];
System.in.read(b);
}
catch (IOException ex) {
System.err.println("Couldn't read from System.in!");
}

// read length bytes of data starting at position offset into the
array and returns the number of bytes read or -1 on end of stream
try {
byte[] b = new byte[100];
int offset = 0;
while (offset < b.length) {
int bytesRead = System.in.read(b, offset, b.length - offset);
if (bytesRead == -1) break; // end of stream
offset += bytesRead;
}
} catch (IOException ex) {
  System.error.println("Couldn't read from stdin");
}

// available()
public int available( ) throws IOException

try {
  byte[] b = new byte[100]; int offset = 0;
  
  while (offset < b.length) {
    int a = System.in.available();

    int bytesRead = System.in.read(b, offset, a);
    
    if (bytesRead == -1) break; // end of stream
    
    offset += bytesRead
  }
} catch (IOException ex) {
  System.err.println("Couldn't read from System.in!");
}

// skip()
public long skip(long bytesToSkip) throws IOException


try {
long bytesSkipped = 0; long bytesToSkip
= 80; while (bytesSkipped < bytesToSkip)
{
long n = in.skip(bytesToSkip -
bytesSkipped); if (n == -1) break;
bytesSkipped += n; }
}
catch (IOException ex) {
System.err.println(ex); }


// close()
public void close( ) throws IOException

InputStream in = null;
try {
URL u = new URL("http://www.msf.org/");
in = u.openStream( ); // Read from the
stream...
}
catch (IOException ex) {
System.err.println(ex); }
finally {
if (in != null) {
try {
in.close( ); }
catch (IOException ex) {
System.err.println(ex); }
}
}


3.6. Marking and Resetting

// some input streams allow you to mark a particular position in the stream and then return to it

public void mark(int readLimit)
public void reset( ) throws IOException
public boolean markSupported()

// markSupported() is used to check if this stream supports marking

// mark() marks the current position in the stream (and erases any previously marked position)

// reset() rewinds to the previously marked position as long as you haven't read more than readLimit bytes


package com.elharo.io;
import java.util.*;
import java.io.*;
public class RandomInputStream extends
InputStream {
private Random generator = new
Random( ); private boolean closed = false;
public int read( ) throws IOException {
checkOpen( );
int result = generator.nextInt( ) % 256; if
(result < 0) result = -result; return result;
}
public int read(byte[] data, int offset, int
length) throws IOException {
checkOpen( );
byte[] temp = new byte[length];
generator.nextBytes(temp);
System.arraycopy(temp, 0, data, offset,
length); return length;
}
public int read(byte[] data) throws
IOException {
checkOpen( );
generator.nextBytes(data); return
data.length;
}
public long skip(long bytesToSkip) throws
IOException {
checkOpen( );
// It's all random so skipping has no
effect.
return bytesToSkip;
}
public void close( ) {
this.closed = true;
}
private void checkOpen( ) throws
IOException {
if (closed) throw new IOException("Input
stream closed"); }
public int available( ) {
// Limited only by available memory and
the size of an array.
return Integer.MAX_VALUE; }
}

The no-argument read( ) method returns a
random int in the range of an unsigned
byte (0 to 255). The other two read( )
methods fill a specified part of an array
with random bytes. They return the
number of bytes read (in this case the
number of bytes created).


package com.elharo.io;
import java.io.*;
public class StreamCopier {
public static void main(String[] args) {
try {
copy(System.in, System.out); }
catch (IOException ex) {
System.err.println(ex); }
}
public static void copy(InputStream in,
OutputStream out) throws IOException {
byte[] buffer = new byte[1024]; while
(true) {
int bytesRead = in.read(buffer); if
(bytesRead == -1) break; out.write(buffer,
0, bytesRead); }
}
}
```

# 4. File Streams

The java.io.FileInputStream and java.io.FileOutputStream classes provide methods for reading and writing data in files

java.io.FileInputStream is a concrete subclass of java.io.InputStream. It provides an input
stream connected to a particular file.

public FileInputStream(String fileName) throws IOException
public FileInputStream(File file) throws FileNotFoundException
public FileInputStream(FileDescriptor fdObj)

The first constructor uses a string containing the name of the file. The second constructor uses a java.io.File object. The third constructor uses a
java.io.FileDescriptor object.

try {
FileInputStream fis = new FileInputStream("README.TXT");
for (int n = fis.read(); n != -1; n = fis.read( )) {
System.out.write(n);
}
}
catch (IOException ex) {
System.err.println(ex);
}
System.out.println( );

Java looks for files in the current working directory


For maximum cross-platform awareness, you should use File objects
instead.

One particularly important trick is to create multisegment paths by
successively appending new File objects for each directory like so:
File root = new File("/");
File dir = new File(root, "etc");
File child = new File(dir, "hosts");
FileInputStream fis = new FileInputStream(child);

However, this still assumes that the root of the filesystem is named "/", which isn't
likely to be a true on a non-Unix system. It's better to use the File.listRoots( )
method:
File[] roots = File.listRoots( )
File dir = new File(roots[0], "etc");
File child = new File(dir, "hosts");
FileInputStream fis = new FileInputStream(child);


Create a temporary file with the File.createTempFile( ) method.
Find the user's home directory with System.getProperty("user.home").
Find the current working directory with System.getProperty("user.dir").

import java.io.*;
import com.elharo.io.*;
public class FileTyper {
public static void main(String[] args) throws IOException {
if (args.length != 1) {
System.err.println("Usage: java FileTyper filename");
return;
}
typeFile(args[0]);
}
public static void typeFile(String filename) throws
IOException {
FileInputStream fin = new FileInputStream(filename);
try {
StreamCopier.copy(fin, System.out);
}
finally {
fin.close( );
}
}
}

The FileInputStream class has one method that's not declared in the InputStream
superclass: getFD( ).

This method returns the java.io.FileDescriptor object associated with this stream.

It is possible to open multiple input streams to the same file at the same time,
though it's rarely necessary to do so. Each stream maintains a separate pointer that
points to the current position in the file. Reading from the file does not change the
file in any way.



4.2. Writing Files
The java.io.FileOutputStream class is a concrete subclass of java.io.OutputStream that
provides output streams connected to files. This class has all the usual methods of
output streams, such as write( ), flush( ), and close( ), which are used exactly as
they are for any other output stream.
FileOutputStream( ) has three main constructors, differing primarily in how the file is
specified:
public FileOutputStream(String filename) throws IOException
public FileOutputStream(File file) throws IOException
public FileOutputStream(FileDescriptor fd)
The first constructor uses a string containing the name of the file; the second
constructor uses a java.io.File object; the third constructor uses a
java.io.FileDescriptor object. To write data to a file, just pass the name of the file to
the FileOutputStream( ) constructor, then use the write( ) methods as usual. If the file
does not exist, all three constructors will create it. If the file does exist, any data
inside it will be overwritten.
A fourth constructor also lets you specify whether the file's contents should be
erased before data is written into it (append == false) or whether data is to be tacked
onto the end of the file (append == true):
public FileOutputStream(String name, boolean append) throws IOException
Output streams created by the other three constructors simply overwrite the file;
they do not provide an option to append data to the file.
Java looks for files in the current working directory. You can write to a file in a
different directory by passing a full or relative path to the file from the current
working directory. For example, to append data to the \Windows\java\javalog.txt file
no matter which directory is current, you would do this:
FileOutputStream fout =
new FileOutputStream("/Windows/java/javalog.txt", true);
Although Windows uses a backslash as the directory separator, Java still expects you
to use a forward slash as in Unix. Hardcoded pathnames are dangerously platformdependent. Using this constructor automatically classifies your program as impure
Java. As with input streams, a slightly less dangerous alternative builds a File object
a piece at a time like so:
File[] roots = File.listRoots( )
File windows = new File(roots[0], "Windows");
File java = new File(windows, "java");
File javalog = new File(java, "javalog.txt");
FileInputStream fis = new FileInputStream(javalog);
Untrusted code is normally not allowed to write files either. If an applet tries to
create a FileOutputStream, the constructor throws a SecurityException.
The FileOutputStream class has one method that's not declared in java.io.OutputStream:
getFD( ).
public final FileDescriptor getFD( ) throws IOException
This method returns the java.io.FileDescriptor object associated with this stream.
Example 4-2 reads two filenames from the command line, then copies the first file
into the second file. The StreamCopier class from Example 3-3 in the previous chapter
does the actual reading and writing.
Example 4-2. The FileDumper program
import java.io.*;
import com.elharo.io.*;
public class FileCopier {
public static void main(String[] args) {
if (args.length != 2) {
System.err.println("Usage: java FileCopier infile
outfile");
}
try {
copy(args[0], args[1]);
}
catch (IOException ex) {
System.err.println(ex);
}
}
public static void copy(String inFile, String outFile)
throws IOException {
FileInputStream fin = null;
FileOutputStream fout = null;
try {
fin = new FileInputStream(inFile);
fout = new FileOutputStream(outFile);
StreamCopier.copy(fin, fout);
}
finally {
try {
if (fin != null) fin.close( );
}
catch (IOException ex) {
}
try {
if (fout != null) fout.close( );
}
catch (IOException ex) { }
}
}
}
Since we're no longer writing to System.out and reading from System.in, it's important to
make sure the streams are closed when we're done. This is a good use for a finally
clause, as we need to make sure the files are closed whether the reads and writes
succeed or not.
Java is better about closing files than most languages. As long as the VM doesn't
terminate abnormally, the files will be closed when the program exits. Still, if this
class is used inside a long-running program like a web server, waiting until the
program exits isn't a good idea; other threads and processes may need access to the
files.
Example 4-2 has one bug: the program does not behave well if the input and output
files are the same. While it would be straightforward to compare the two filenames
before copying, this is not safe enough. Once aliases, shortcuts, symbolic links, and
other factors are taken into account, a single file may have multiple names. The full
solution to this problem will have to wait until Chapter 17, where I discuss canonical
paths and temporary files.
000 234 127 034 234 234 000 000 000
002 004 070 000 234 127 098
CA FE BA BE 07 89 9A 65 45 65 43 6F F6
7F 8F EE E5 67 63 26 98 9E 9C
import java.io.*;
import com.elharo.io.*;
public class FileDumper {
public static final int ASC = 0; public
static final int DEC = 1; public static final
int HEX = 2; public static void
main(String[] args) {
if (args.length < 1) {
System.err.println("Usage: java
FileDumper [-ahd] file1 file2..."); return;
}
int firstArg = 0;
int mode = ASC;
if (args[0].startsWith("-")) {
firstArg = 1;
if (args[0].equals("-h")) mode = HEX;
else if (args[0].equals("-d")) mode = DEC;
}
for (int i = firstArg; i < args.length; i++)
{
try {
if (mode == ASC) dumpAscii(args[i]);
else if (mode == HEX) dumpHex(args[i]);
else if (mode == DEC)
dumpDecimal(args[i]); }
catch (IOException ex) {
System.err.println("Error reading from "
+ args[i] + ": "
+ ex.getMessage( ));
}
if (i < args.length-1) { // more files to
dump System.out.println("\r\n------------------
--------------------\r\n"); }
}
}
public static void dumpAscii(String
filename) throws IOException {
FileInputStream fin = null; try {
fin = new FileInputStream(filename);
StreamCopier.copy(fin, System.out); }
finally {
if (fin != null) fin.close( ); }
}
public static void dumpDecimal(String
filename) throws IOException {
FileInputStream fin = null; byte[] buffer =
new byte[16]; boolean end = false;
try {
fin = new FileInputStream(filename);
while (!end) {
int bytesRead = 0;
while (bytesRead < buffer.length) {
int r = fin.read(buffer, bytesRead,
buffer.length - bytesRead); if (r == -1) {
end = true;
break;
}
bytesRead += r;
}
for (int i = 0; i < bytesRead; i++) {
int dec = buffer[i];
if (dec < 0) dec = 256 + dec; if (dec <
10) System.out.print("00" + dec + " ");
else if (dec < 100) System.out.print("0" +
dec + " "); else System.out.print(dec + "
"); }
System.out.println( ); }
}
finally {
if (fin != null) fin.close( ); }
}
public static void dumpHex(String
filename) throws IOException {
FileInputStream fin = null; byte[] buffer =
new byte[24]; boolean end = false;
try {
fin = new FileInputStream(filename);
while (!end) {
int bytesRead = 0;
while (bytesRead < buffer.length) {
int r = fin.read(buffer, bytesRead,
buffer.length - bytesRead); if (r == -1) {
end = true;
break;
}
bytesRead += r;
}
for (int i = 0; i < bytesRead; i++) {
int hex = buffer[i];
if (hex < 0) hex = 256 + hex; if (hex >=
16)
System.out.print(Integer.toHexString(hex)
+ " "); else System.out.print("0" +
Integer.toHexString(hex) + " "); }
System.out.println( ); }
}
finally {
if (fin != null) fin.close( ); }
}
}
D:\JAVA\ioexamples\04> <b>java
FileDumper -h FileDumper.class</b> ca fe
ba be 00 00 00 2e 00 78 0a 00 22 00 37
09 00 38 00 39 08 00 3a 0a 00 3b 00 3c
08 00 3d 0a 00 3e 00 3f 08 00 40 0a 00
3e 00 41 08 00 42 0a 00 21 00 43 0a 00
21 00 44 0a 00 21 00 45 09 00 38 00 46
08 00 47 07 00
48 0a 00 0f 00 49 0a 00 4a 00 4b 07 00 4c
0a 00 3b 00 4d 0a 00 0f 00 4e ...
In later chapters, I'll add a graphical user
interface and many more possible
interpretations of the data in the file,
including floating-point, big- and littleendian integer, and various text
encodings.


# 5. Network Streams

From its first days, Java, more than any other common programming language, has
had the network in mind. Java is the first programming language to provide as much
support for network I/O as it does for file I/O, perhaps even more (Java's URL,
URLConnection, Socket, and ServerSocket classes are all fertile sources of streams). The
exact type of the stream used by a network connection is typically hidden inside the
undocumented sun classes. Thus, network I/O relies primarily on the basic InputStream
and OutputStream methods, which you can wrap with any higher-level stream that suits
your needs: buffering, cryptography, compression, or whatever your application
requires.
5.1. URLs
The java.net.URL class represents a Uniform Resource Locator such as
http://www.cafeaulait.org/books/javaio2/. Each URL unambiguously identifies the
location of a resource on the Internet. The URL class has six constructors. All are
declared to throw MalformedURLException, a subclass of IOException.
public URL(String url) throws MalformedURLException
public URL(String protocol, String host, String file)
throws MalformedURLException
public URL(String protocol, String host, int port, String file)
throws MalformedURLException
public URL(String protocol, String host, int port, String file,
URLStreamHandler handler) throws MalformedURLException
public URL(URL context, String url) throws MalformedURLException
public URL(URL context, String url, URLStreamHandler handler)
throws MalformedURLException
Each constructor throws a MalformedURLException if its arguments do not specify a valid
URL. Often, this means a particular Java implementation does not have the right
protocol handler installed. Thus, given a complete absolute URL such as
http://www.poly.edu/schedule/fall2006/bgrad.html#cs, you construct a URL object like
so:
URL u = null;
try {
u = new URL("http://www.poly.edu/schedule/fall2006/bgrad.html#cs");
}
catch (MalformedURLException ex) {
// this shouldn't happen for a syntactically correct http URL
}
You can also construct the URL object by passing its pieces to the constructor:
URL u = new URL("http", "www.poly.edu", "/schedule/
fall2006/bgrad.html#cs");
You don't normally need to specify a port for a URL. Most protocols have default
ports. For instance, the HTTP port is 80. Sometimes the port used does change, and
in that case you can use this constructor:
URL u = new URL("http", "www.poly.edu", 80, "/schedule/
fall2006/bgrad.html#cs ");
Finally, many HTML files contain relative URLs. The last two constructors create URLs
relative to a given URL and are particularly useful when parsing HTML. For example,
the following code creates a URL pointing to the file 08.html, taking the rest of the
URL from u1:
URL u1 = new URL("http://www.cafeaualit.org/course/week12/07.html");
URL u2 = new URL(u1, "08.html");
Once a URL object has been constructed, you can retrieve its data in two ways. The
openStream( ) method returns a raw stream of bytes from the source. The getContent( )
method returns a Java object that represents the data. When you call getContent( ),
Java looks for a content handler that matches the MIME type of the data. It is the
openStream( ) method that is of concern in this book.
The openStream( ) method makes a socket connection to the server and port specified
in the URL. It returns an input stream from which you can read the data at that URL.
Any headers that come before the actual data or file requested are stripped off
before the stream is opened. You only get the raw data.
public InputStream openStream( ) throws IOException
Example 5-1 shows you how to connect to a URL entered on the command line,
download its data, and copy that to System.out.
Example 5-1. The URLTyper program
import java.net.*;
import java.io.*;
public class URLTyper {
public static void main(String[] args) throws IOException {
if (args.length != 1) {
System.err.println("Usage: java URLTyper url");
return;
}
InputStream in = null;
try {
URL u = new URL(args[0]);
in = u.openStream( );
for (int c = in.read(); c != -1; c = in.read( )) {
System.out.write(c);
}
}
catch (MalformedURLException ex) {
System.err.println(args[0] + " is not a URL Java
understands.");
}
finally {
if (in != null) in.close( );
}
}
}
For example, here are the first few lines you see when you connect to
http://www.oreilly.com/:
$ java URLTyper http://www.oreilly.com/
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en-US" xml:lang="enUS">
<head>
<title>oreilly.com -- Welcome to O'Reilly Media, Inc. -- computer
books, softwar
e conferences, online publishing</title>
<meta name="keywords" content="O'Reilly, oreilly, computer books,
technical books, UNIX, unix, Perl, Java, Linux, Internet, Web, C, C++,
Windows,
Windows NT, Security, Sys Admin, System Administration, Oracle, PL/SQL,
online b
ooks, books online, computer book online, e-books, ebooks, Perl
Conference, Open
Source Conference, Java Conference, open source, free software, XML,
Mac OS X,
.Net, dot net, C#, PHP, CGI, VB, VB Script, Java Script, javascript,
Windows 200
0, XP, bioinformatics, web services, p2p" />
...
Most network connections, even on LANs, are slower and less reliable sources of
data than files. Connections across the Internet are even slower and less reliable,
and connections through a modem are slower and less reliable still. One way to
enhance performance under these conditions is to buffer the data: to read as much
data as you can into a temporary storage array inside the class, then parcel it out as
needed. In the next chapter, you'll learn about the BufferedInputStream class that does
exactly this.
Untrusted code running under the control of a security managere.g., applets that run
inside a web browserare normally allowed to connect only to the host they were
downloaded from. This host can be determined from the URL returned by the
getCodeBase( ) method of the Applet class. Attempts to connect to other hosts throw
security exceptions. You can create URLs that point to other hosts, but you may not
download data from them using openStream( ) or any other method. (This security
restriction for applets applies to any network connection, regardless of how you get
it.)

5.2. URL Connections
URL connections are closely related to URLs, as their name implies. Indeed, you get
a reference to a URLConnection by using the openConnection( ) method of a URL object; in
many ways, the URL class is only a wrapper around the URLConnection class. URL
connections provide more control over the communication between the client and
the server. In particular, URL connections provide not just input streams by which
the client can read data from the server, but also output streams to send data from
the client to the server.
The java.net.URLConnection class is an abstract class that handles communication with
different kinds of servers, such as FTP servers and web servers. Protocol-specific
subclasses of URLConnection, which are hidden inside the sun packages, handle different
kinds of servers.
5.2.1. Reading Data from URL Connections
URL connections take place in five steps:
1. The URL object is constructed.
2. The openConnection( ) method of the URL object creates the URLConnection
object.
3. The parameters for the connection and the request properties that
the client sends to the server are set up.
4. The connect( ) method makes the connection to the server, perhaps
using a socket for a network connection or a file input stream for a
local connection. The response header information is read from the
server.
5. Data is read from the connection using the input stream returned by
getInputStream( ) or a content handler returned by getContent( ). Data can
be sent to the server using the output stream provided by
getOutputStream( ).
This scheme is very much based on the HTTP protocol. It does not fit other schemes
that have a more interactive "request, response, request, response, request,
response" pattern instead of HTTP/1.0's "single request, single response, close
connection" pattern. In particular, FTP doesn't really fit this pattern.
URLConnection objects are not constructed directly in your own programs. Instead, you
create a URL for the particular resource and call that URL's openConnection( ) method.
This gives you a URLConnection. Then the getInputStream( ) method returns an input
stream that reads data from the URL. (The openStream( ) method of the URL class is just
a thin veneer over the getInputStream( ) method of the URLConnection class.) If the
connection cannot be opened, for example because the remote host is unreachable,
connect( ) throws an IOException. For example, this code repeats the main body of
Example 5-1 using a URLConnection to open the stream:
URL u = new URL(args[0]);
URLConnection connection = u.openConnection( );
in = connection.getInputStream( );
for (int c = in.read(); c != -1; c = in.read( )) {
System.out.write(c);
}
5.2.2. Writing Data on URL Connections
Writing data to a URLConnection is similar to reading data. However, you must first
inform the URLConnection that you plan to use it for output. Then, instead of getting the
connection's input stream and reading from it, you get the connection's output
stream and write to it. This is commonly used for HTTP POST and PUT. Here are the
steps for writing data on a URLConnection:
1. Construct the URL object.
2. Call the openConnection( ) method of the URL object to create the
URLConnection object.
3. Pass true to setDoOutput( ) to indicate that this URLConnection will be used for
output.
4. If you also want to read input from the stream, invoke setDoInput(true) to
indicate that this URLConnection will be used for input.
5. Create the data you want to send, preferably as a byte array.
6. Call getOutputStream( ) to get an output stream object. Write the byte
array calculated in step 5 onto the stream.
7. Close the output stream.
8. Call getInputStream( ) to get an input stream object. Read and write it as
usual.
Example 5-2 uses these steps to implement a simple mail client. It forms a mailto
URL from an email address entered on the command line. Input for the message is
copied from System.in onto the output stream of the URLConnection using a StreamCopier.
The end-of-stream character signals the end of the message.
Example 5-2. The MailClient class
import java.net.*;
import java.io.*;
public class MailClient {
public static void main(String[] args) {
if (args.length == 0) {
System.err.println("Usage: java MailClient
username@host.com");
return;
}
try {
URL u = new URL("mailto:" + args[0]);
URLConnection uc = u.openConnection( );
uc.setDoOutput(true);
uc.connect( );
OutputStream out = uc.getOutputStream( );
for (int c = System.in.read(); c != -1; c =
System.in.read( )) {
out.write(c);
}
out.close( );
}
catch (IOException ex) {
System.err.println(ex);
}
}
}
For example, to send email to the author of this book:
$ java MailClient elharo@metalab.unc.edu
hi there!
^D
MailClient suffers from a few restrictions. The proper way to detect the end of the
message is to look for a period on a line by itself. Proper or not, that style of user
interface is really antiquated, so I didn't bother to implement it. To do so properly,
you'll need to use a Reader or a Writer; they're discussed in Chapter 20. Furthermore,
it works only in Java environments that support the mailto protocol; thus, it works
under Sun's JDK but may not work in other VMs. It also requires that the local host be
running an SMTP server, or that the system property mail.host must contain the name
of an accessible SMTP server, or that a machine in the local domain named mailhost
be running an SMTP server. Finally, the security manager must permit network
connections to that server, although this is not normally a problem in an application.
5.3. Sockets
Before data is sent across the Internet from one host to another, it is split into
packets of varying but finite size called datagrams . Datagrams range in size from a
few dozen bytes to about 60,000 bytes. Anything larger, and often things smaller,
must be split into smaller packets before it's transmitted. The advantage of this
scheme is that if one packet is lost, it can be retransmitted without requiring
redelivery of all other packets. Furthermore, if packets arrive out of order, they can
be reordered at the receiving end of the connection.
Fortunately, packets are invisible to the Java programmer. The host's native
networking software splits data into packets on the sending end and reassembles
packets on the receiving end. Instead, the Java programmer is presented with a
higher-level abstraction called a socket. The socket provides a reliable connection for
the transmission of data between two hosts. It isolates you from the details of packet
encodings, lost and retransmitted packets, and packets that arrive out of order. A
socket performs four fundamental operations:
Connects to a remote machine
Sends data
Receives data
Closes the connection
A socket may not connect to more than one remote host. However, a socket may
both send data to and receive data from the remote host it's connected to.
The java.net.Socket class is Java's interface to a network socket and allows you to
perform all four fundamental socket operations. It provides raw, uninterpreted
communication between two hosts. You can connect to remote machines; you can
send data; you can receive data; you can close the connection. No part of the
protocol is abstracted out, as is the case with URL and URLConnection. The programmer
is completely responsible for the interaction between the client and the server.
To open a connection, call one of the Socket constructors, specifying the host to which
you want to connect. Each Socket object is associated with exactly one remote host.
To connect to a different host, you must create a new Socket object:
public Socket(String host, int port
throws UnknownHostException, IOException
public Socket(InetAddress address, int port) throws IOException
public Socket(String host, int port, InetAddress localAddress,
int localPort)
throws IOException
public Socket(InetAddress address, int port, InetAddress localAddress,
int localPort) throws IOException
The host argument is a string like "www.oreilly.com" or "duke.poly.edu" that specifies
the particular host to connect to. It may even be a numeric, dotted quad string such
as "199.1.32.90". This argument may also be passed as a java.net.InetAddress object.
The port argument is the port on the remote host to connect to. A computer's
network interface is logically subdivided into 65,536 different ports. As data
traverses the Internet in packets, each packet carries both the address of the host
it's going to and a port number on that host. A host reads the port number from
each packet it receives to decide which program should receive that chunk of data.
Many services run on well-known ports. For example, HTTP servers generally listen
on port 80.
The optional localAddress and localPort arguments specify which address and port on
the local host the socket connects from, assuming more than one is available. Most
hosts have many available ports but only one address. These two arguments are
optional. If they're left out, the constructor will choose reasonable values.
Data is sent across the socket via streams. These are the methods to get both
streams for the socket:
public InputStream getInputStream( ) throws IOException
public OutputStream getOutputStream( ) throws IOException
There's also a method to close the socket:
public void close( ) throws IOException
This closes the socket's input and output streams as well. Any attempt to read from
or write to them after the socket is closed throws an IOException.
Example 5-3 is yet another program that connects to a web server and downloads a
specified URL. However, since this one uses raw sockets, it needs to both send the
HTTP request and read the headers in the response. These are not parsed away as
they are by the URL and URLConnection classes; you use an output stream to send the
request explicitly and an input stream to read the dataincluding HTTP headersback.
Only HTTP URLs are supported.
Example 5-3. The SocketTyper program
import java.net.*;
import java.io.*;
public class SocketTyper {
public static void main(String[] args) throws IOException {
if (args.length != 1) {
System.err.println("Usage: java SocketTyper url1");
return;
}
URL u = new URL(args[0]);
if (!u.getProtocol( ).equalsIgnoreCase("http")) {
System.err.println("Sorry, " + u.getProtocol( )
+ " is not supported");
return;
}
String host = u.getHost( );
int port = u.getPort( );
String file = u.getFile( );
if (file == null) file = "/";
// default port
if (port <= 0) port = 80;
Socket s = null;
try {
s = new Socket(host, port);
String request = "GET " + file + " HTTP/1.1\r\n"
+ "User-Agent: SocketTyper\r\n"
+ "Accept: text/*\r\n"
+ "Host: " + host + "\r\n"
+ "\r\n";
byte[] b = request.getBytes("US-ASCII");
OutputStream out = s.getOutputStream( );
InputStream in = s.getInputStream( );
out.write(b);
out.flush( );
for (int c = in.read(); c != -1; c = in.read( )) {
System.out.write(c);
}
}
finally {
if (s != null && s.isConnected()) s.close( );
}
}
}
For example, when SocketTyper connects to http://www.oreilly.com/, here is what you
see:
$ java SocketTyper http://www.oreilly.com/
HTTP/1.1 200 OK
Date: Mon, 23 May 2005 14:03:17 GMT
Server: Apache/1.3.33 (Unix) PHP/4.3.10 mod_perl/1.29
P3P: policyref="http://www.oreillynet.com/w3c/p3p.xml",CP="CAO DSP COR
CURa ADMa
DEVa TAIa PSAa PSDa IVAa IVDa CONo OUR DELa PUBi OTRa IND PHY ONL UNI
PUR COM N
AV INT DEM CNT STA PRE"
Last-Modified: Mon, 23 May 2005 08:20:30 GMT
ETag: "20653-db8c-4291924e"
Accept-Ranges: bytes
Content-Length: 56204
Content-Type: text/html
X-Cache: MISS from www.oreilly.com
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en-US" xml:lang="enUS">
<head>
...
Notice the header lines here, which you didn't see in Example 5-1. When you use the
URL class to download a web page, the associated protocol handler consumes the
HTTP header before you get a stream.
5.4. Server Sockets
Each connection has two ends: the client, which initiates the connection, and the
server, which responds to the connection. So far, I've only discussed the client side
and assumed that a server existed out there for the client to talk to. To implement a
server, you need to write a program that waits for other hosts to connect to it. A
server socket binds to a particular port on the local machine (the server). Then it
listens for incoming connection attempts from remote machines (the clients). When
the server detects a connection attempt, it accepts the connection. This creates a
socket between the two machines over which the client and the server
communicate.
Multiple clients can connect to a server simultaneously. Incoming data is
distinguished by the port to which it is addressed and the client host and port from
which it came. The server can tell which service (such as HTTP or FTP) the data is
intended for by checking the port at which it arrives. It knows where to send any
response by looking at the client address and port stored with the data.
No more than one server socket can listen to a particular port at one time.
Therefore, since a server may need to handle many connections at once, server
programs tend to be multithreaded. (Alternately, they can use nonblocking I/O. We'll
explore this starting in Chapter 16.) Generally, the server socket listening on the
port only accepts the connections. It passes off the actual processing of each
connection to a separate thread. Incoming connections are stored in a queue until
the server can accept them. On most systems, the default queue length is between
5 and 50. Once the queue fills up, further incoming connections are refused until
space in the queue opens up.
The java.net.ServerSocket class represents a server socket. The constructors receive
the port to bind to, the queue length for incoming connections, and the IP address:
public ServerSocket(int port) throws IOException
public ServerSocket(int port, int backlog) throws IOException
public ServerSocket(int port, int backlog, InetAddress bindAddr)
throws IOException
Normally, you only specify the port you want to listen on:
ServerSocket ss = new ServerSocket(80);
When you create a ServerSocket object, it attempts to bind to the port given by the
port argument. If another server socket is already listening to the port, the
constructor throws an IOExceptionmore specifically, a java.net.BindException. Only one
server socket can listen to a particular port at a time. This includes server sockets
opened by non-Java programs. For example, if there's already an HTTP server
running on port 80, you won't be able to bind to port 80.
On Unix systems, including Mac OS X but not Windows, the
program must be running as root to bind to a port between 1 and
1023. Otherwise, accept( ) tHRows a BindException.
0 is a special port number. It tells Java to pick an available port. You can then find
out which port it has picked with the getLocalPort( ) method:
public int getLocalPort( )
This is useful if the client and the server have already established a separate
channel of communication over which the chosen port number can be
communicated. For example, the FTP protocol uses two sockets. The client makes
the initial connection to the server on a socket it will use to send commands. The
client also opens a server socket on a random port on the local host. One of the
commands it sends tells the server the port number on which the client is listening.
The server then opens a socket to the client's server port, which it uses to send files.
Because commands and data are sent over two different sockets, a long file doesn't
tie up the command channel.
Once you have a ServerSocket, you wait for incoming connections by calling the accept(
) method. This method blocks until a connection attempt occurs and then returns a
Socket that you can use to communicate with the client.
public Socket accept( ) throws IOException
The close( ) method terminates the ServerSocket:
public void close( ) throws IOException
That's pretty much all there is to the ServerSocket, except for a few methods dealing
with socket options and some other details. In particular, there aren't methods for
getting input and output streams. Instead, accept( ) returns a client Socket object: this
Socket's getInputStream( ) or getOutputStream( ) methods return the streams used to
communicate. For example:
ServerSocket ss = new ServerSocket(2345);
Socket s = ss.accept( );
OutputStream out = s.getOutputStream( );
// send data to the client...
s.close( );
Notice in this example, I closed the Socket s, not the ServerSocket ss. ss is still bound to
port 2345. You get a new socket for each connection and reuse the server socket.
For example, the next code fragment repeatedly accepts connections:
ServerSocket ss = new ServerSocket(2345);
while (true) {
Socket s = ss.accept( );
OutputStream out = s.getOutputStream( );
// send data to the client...
s.close( );
}
The program in Example 5-4 listens for incoming connections on port 2345. When it
detects one, it answers with the client's address and port and its own. Then it closes
the connection.
Example 5-4. The HelloServer program
import java.net.*;
import java.io.*;
public class HelloServer {
public static void main(String[] args) throws IOException {
int port = 2345;
ServerSocket ss = new ServerSocket(port);
while (true) {
try {
Socket s = ss.accept( );
String response = "Hello " + s.getInetAddress( ) + "
on port "
+ s.getPort( ) + "\r\n";
response += "This is " + s.getLocalAddress( ) + " on
port "
+ s.getLocalPort( ) + "\r\n";
OutputStream out = s.getOutputStream( );
out.write(response.getBytes("US-ASCII"));
out.flush( );
s.close( );
}
catch (IOException ex) {
// This is an error on one connection. Maybe the
client crashed.
// Maybe it broke the connection prematurely. Whatever
happened,
// it's not worth shutting down the server for.
}
} // end while
} // end main
} // end HelloServer
Here's some output from this server. The server is running on utopia.poly.edu. The
client is connecting from titan.oit.unc.edu. Note how the port from which the
connection comes changes each time; like most client programs, the telnet program
picks an arbitrary local port for outgoing connections:
$ telnet utopia.poly.edu
Trying 128.238.3.21...
Connected to utopia.poly.edu.
Escape character is '^]'.
Hello titan.oit.unc.edu/152.2.22.14 on port 50361
This is utopia.poly.edu/128.238.3.21 on port 2345
Connection closed by foreign host.
% telnet utopia.poly.edu
Trying 128.238.3.21...
Connected to utopia.poly.edu.
Escape character is '^]'.
Hello titan.oit.unc.edu/152.2.22.14 on port 50362
This is utopia.poly.edu/128.238.3.21 on port 2345
Connection closed by foreign host.
If you aren't able to make a connection to this server, check your
firewall rules. For security, most modern networks install firewalls
in either the router, the local host, or both that prevent all
connections to unrecognized services and ports. You may need to
configure your firewall(s) to allow connections to port 2345 to run
this program.
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;
import java.net.*;
import com.elharo.io.ui.*;
public class URLViewer extends JFrame
implements ActionListener {
private JTextField theURL = new
JTextField( ); private JButton loadButton =
new JButton("Load"); private
JStreamedTextArea theDisplay = new
JStreamedTextArea(60, 72); public
URLViewer( ) {
super("URL Viewer");
this.getContentPane(
).add(BorderLayout.NORTH, theURL);
JScrollPane pane = new
JScrollPane(theDisplay);
this.getContentPane(
).add(BorderLayout.CENTER, pane); Panel
south = new Panel( );
south.add(loadButton);
this.getContentPane(
).add(BorderLayout.SOUTH, south);
theURL.addActionListener(this);
loadButton.addActionListener(this);
this.setLocation(50, 50);
this.setDefaultCloseOperation(JFrame.EXIT
_ON_CLOSE); this.pack( );
}
public void actionPerformed(ActionEvent
event) {
try {
URL u = new URL(theURL.getText( ));
InputStream in = u.openStream( );
OutputStream out =
theDisplay.getOutputStream( );
theDisplay.setText(""); for (int c =
in.read(); c != -1; c = in.read( )) {
out.write(c);
}
in.close( );
}
catch (IOException ex) {
theDisplay.setText("Invalid URL: " +
ex.getMessage( )); }
}
public static void main(String args[]) {
final URLViewer me = new URLViewer( );
// To avoid deadlock don't show frames on
the main thread
SwingUtilities.invokeLater(
new Runnable( ) {
public void run( ) {
me.show( );
}
}
);
}
}
The URLViewer class itself extends JFrame.
The constructor builds the interface, which
consists of a JTextField to type the URL
into, a JStreamedTextArea component from
Chapter 2 that is placed inside a
JScrollPane, and a Load button that can be
pressed to download the content of the
URL.
The streamed text area is filled when the
user clicks the Load button or presses
Enter inside the URL text field. The
URLViewer object listens to both of these
components. The URLViewer
's
actionPerformed( ) method constructs a
URL from the text in the text field, then
opens an input stream from the URL in the
text field. Data from the URL's input
stream pours into the text area's output
stream. When that's finished, the input
stream is closed. The output stream,
however, is left open so the user can view
new URLs.


P



# 17. Working with Files

The java.io.File class attempts to provide a platform-independent abstraction
for common file operations and metainformation.

17.1. Understanding Files
As far as a Java program knows, a file is a sequential set of bytes stored on a
random access medium such as a hard disk or CD. There is a first byte in the file, a
second byte, and so on, until the end of the file. In this way, a file is similar to a
stream. However, a program can jump around in a file, reading first one part of a file
and then another. This isn't possible with a stream.
17.1.1. Filenames
Every file has a name. The format of the filename is determined by the operating
system. For example, in DOS and Windows 3.1, filenames are 8 ASCII characters
long with a 3-letter extension. README.TXT is a valid DOS filename, but Read me
before you run this program or your hard drive will get trashed is not. All ASCII
characters from 32 up (that is, noncontrol characters), except for the 15 punctuation
characters (+=/][":;,?*\<>|) and the space character, may be used in filenames.
Filenames are case-insensitive (though generally rendered as all capitals).
README.TXT and readme.txt are the same filename. A period may be used only as a
separator between the 8-character name and the 3-letter extension. Furthermore,
the complete path to the file, including the disk drive and all directories, may not
exceed 80 characters in length.
On the other hand, Read me before you run this program or your hard drive will get
trashed is a valid Win32 (Windows 95 and later) filename. On those systems
filenames may contain up to 255 characters, though room also has to be left for the
path to the file. The full pathname may not exceed 255 characters. Furthermore,
Win32 systems allow any Unicode character with value 32 or above in filenames,
except \/*<>:?" and |. In particular, the +,;=][ characters, forbidden in DOS and
Windows 3.1, are legal in Win32 filenames.
Win32 also makes short versions of the filename that conform to
the DOS 8.3 format available to non-32-bit applications that don't
understand the long filenames. Java understands the long
filenames and uses them in preference to the short form.
Read me before you run this program or your hard drive will get trashed is not a
valid Mac OS 9 filename because on Mac OS 9 file and directory names cannot be
longer than 31 bytes. Volume names cannot be longer than 27 bytes. However,
there's no fixed length to a full path name. The exact number of characters allowed
in a name depends on the number of bytes per character used by the local
encoding. Read me or your HD will be trashed only contains 27 bytes in most
encodings and is thus a valid Macintosh file, directory, and volume name. Mac OS 9
filenames can contain slashes and backslashes (unlike Windows filenames) but may
not contain colons. Otherwise, any ASCII characters, as well as 8-bit MacRoman
characters like ® and p, can be used in a Mac filename.
Of course today most Mac users are running Mac OS X, which is a version of Unix.
Just as Windows converts names to 8.3 filenames as necessary to support older
applications, so too does Mac OS X convert really long filenames to shorter ones for
older apps. Java programs running on Mac OS X only see the longer Unix style
names.
Pretty much all modern Unix systems including Linux and Mac OS X allow at least
255 characters in a filename, and none of those 255 characters needs to be left for a
path. Just about any ASCII character except the forward slash (/) and the null
character (ASCII 0) are valid in a Unix filename. However, because Unix makes
heavy use of a command line, filenames containing spaces, single quotation marks,
double quotes, hyphens, or other characters interpreted by the Unix shell are often
inconvenient. Underscores (which aren't interpreted by the Unix shell) are safe and
often used in place of problematic characters (for example,
Read_me_or_your_HD_will_be_trashed.)
Character sets are an issue for filenames too. Some Unixes use ISO 8859-1, some
use ASCII only, and some use Unicode. Worse yet, the names of the files can change
from one user to the next depending on how they've configured their locale.
American Mac OS 9 filenames are given in the 8-bit MacRoman character set, but
internationalized versions of the Mac OS use different character sets. Mac OS X uses
Unicode throughout. However, some bugs in Apple's Java implementation prevent it
from reading or writing files whose names contain characters from outside the Basic
Multilingual Plane. Windows 95 and later, fortunately, use Unicode exclusively, and it
pretty much works. However, the reliable lowest common denominator character set
for filenames is still ASCII.
Case sensitivity is a problem too. Readme.txt and README.TXT are the same file on
Mac OS 9 and Windows but represent two different files on Unix. Mac OS X is
basically Unix, but in this respect it's actually more similar to Windows and the
classic Mac OS. Mac OS X filenames are case insensitive. (Actually case sensitive
filenames are an option when a disk is formatted, but the default that almost
everyone uses is case insensitive.)
Handling different filename conventions is one of the difficulties of doing real crossplatform work. For best results:
Use only printable ASCII characters, periods, and underscores in filenames.
Avoid punctuation characters in filenames where possible, especially forward
and back slashes.
Never begin a filename with a period, a hyphen, or an @.
Avoid extended character sets and accented characters like ü, ç, and é.
Use mixed-case filenames (since they're easier to read), but do not assume
case alone will distinguish between filenames.
Try to keep your filenames to 32 characters or less.
If a filename can be stored in a DOS-compatible 8.3 format without excessive
effort, you might as well do so. However, Java itself assumes a system on
which files have long names with four- and five-character extensions, so don't
go out of your way to do this.
17.1.2. File Attributes
Most operating systems also store a series of attributes describing each file. The
exact attributes a file possesses are platform-dependent. For example, on Unix a file
has an owner ID, a group ID, a modification time, and a series of read, write, and
execute flags that determine who is allowed to do what with the file. If an operating
system supports multiple types of filesystems (and most modern desktop and server
operating systems do), the attributes of a file may vary depending on what kind of
filesystem it resides on.
Many Mac files also have a type code and a creator code as well as a potentially
unlimited number of attributes that determine whether a file is a bundle or not, is an
alias or not, has a custom icon or not, and various other characteristics mostly
unique to the Mac platform.
DOS filesystems store a file's last modification date, the actual size of the file, the
number of allocation blocks the file occupies, and essentially boolean information
about whether or not a file is hidden, read-only, a system file, or whether the file has
been modified since it was last backed up.
Modern versions of Windows support multiple kinds of filesystems including FAT (the
basic DOS-compatible filesystem) and NTFS (NT File System). Each of these
filesystems supports a slightly different set of attributes. They all support a superset
of the basic DOS file attributes, including creation time, modification time, access
time, allocation size, file size, and whether the file is read-only, system, hidden,
archive, or control.
Any cross-platform library like the java.io package is going to have trouble
supporting all these attributes. Java can read a fairly broad cross-section of these
possible attributes for which most platforms have some reasonable equivalent. It
does not allow you easy access to platform-specific attributes, like Mac file types and
creator codes, Windows' archive attributes, or Unix group IDs.
17.1.3. Filename Extensions and File Types
Filename extensions often indicate the type of a file. For example, a file that ends
with the four-letter extension .java is presumed to be a text file containing Java
source code; a file ending in the five-letter extension .class is assumed to contain
compiled Java byte code; a file ending in the 3-letter extension .gif is assumed to
contain a GIF image.
What does your computer do when you double-click on the file panther.gif? If your
computer is a Macintosh, it opens the file in the program that created the file. That's
because the Mac stores a four-letter creator code for every file on the disk. Assuming
the application associated with that creator code can be found (it can't always,
though), the file panther.gif is opened in the creating program. On the other hand, if
your computer is a Windows PC or a Unix workstation, the creating program is not
necessarily opened. Instead, whichever program is registered as the viewer of .gif
files is launched and used to view the file. In command-line environments, like the
Unix shell, this isn't really an issue because you begin by specifying the program to
run (that is, you type xv panther.gif, not simply panther.gif) but in GUI environments,
the program that's opened may not be the program you want to use.
File extensions have the further disadvantage that they do not really guarantee the
content type of their document and are an unreliable means of determining the type
of a file. Users can easily change them. For example, the simple DOS command copy
HelloWorld.java HelloWorld.gif causes a text file to be misinterpreted as a GIF image.
Filename extensions are only as reliable as the user who assigned them. What's
more, it's hard to distinguish between files that belong to different applications that
have the same type. For instance, many users are surprised to discover that after
installing Firefox, all their HTML files appear to belong to Firefox instead of Internet
Explorer.
The Macintosh solved this problem over two decades ago. Almost every Mac file has
a four-letter type code like "TEXT" and a four-letter creator code like "R*ch". Since
each file has both a type code and a creator code, a Mac can distinguish between
files that belong to different applications but have the same type. Installing Firefox
doesn't mean that Firefox suddenly thinks it owns all your Internet Explorer
documents. Software vendors register codes with Apple so that companies don't
accidentally step on each other's toes. Since codes are almost never seen by end
users, there's not a huge rush to snap up all the good ones like "TEXT" and "HTML".
Overall, this is a pretty good system that's worked incredibly well for more than
twenty years. Apple actually tried to get rid of it in favor of Unix/DOS style file
extensions when they moved to Mac OS X, but backed down after massive outcries
from developers and users alike. Neither Windows nor Unix has anything nearly as
simple and trouble-free. However, because Windows and Unix have not adopted
Mac-style type and creator codes, Java does not have any standard means for
accessing them.
The com.apple.eio.FileManager class included with Apple's port of the
JDK 1.4 and 1.5 provides access to Mac-specific type and creator
codes and other file attributes. Steve Roy's open source
MRJAdapter library (https://mrjadapter.dev.java.net/) provides this
for almost every version of Java Apple has ever shipped.
None of these solutions are perfect. On a Mac, you're likely to want to use Photoshop
to create GIF files but Preview or Firefox to view them. Furthermore, it's relatively
hard to say that you want all text files opened in BBEdit. On the other hand, the
Windows solution is prone to user error; filename extensions are too exposed. For
example, novice HTML coders often can't understand why their HTML files
painstakingly crafted in Notepad open as plaintext in Internet Explorer. Notepad
surreptitiously inserts a.txt extension on all the files it saves unless the filename is
enclosed in double quote marks. For instance, a file saved as HelloWorld.html
actually becomes HelloWorld.html.txt while a file saved as "HelloWorld.html" is
saved with the expected name. Furthermore, filename extensions make it easy for a
user to lie about the contents of a file, potentially confusing and crashing
applications. (You can lie about a file type on a Mac too, but it takes a lot more
work.) Finally, Windows provides absolutely no support for saying that you want one
group of GIF images opened in Photoshop and another group opened in Paint.
Some algorithms can attempt to determine a file's type from its contents, though
these are also error-prone. Many file formats begin with a particular magic number
that uniquely identifies the format. For instance, all compiled Java class files begin
with the number 0xCAFEBABE (in hexadecimal). If the first four bytes of a file aren't
0xCAFEBABE, it's definitely not a Java class file. Furthermore, barring deliberate
fraud, there's only about a one in four billion chance that a random, non-Java file will
begin with those four bytes. Unfortunately, only a few file formats require magic
numbers. Text files, for instance, can begin with any four ASCII characters. You can
apply some heuristics to identify such files. For example, a file of pure ASCII should
not contain any bytes with values between 128 and 255 and should have a limited
number of control characters with values less than 32. But such algorithms are
complicated to devise and imperfect. Even if you are able to identify a file as ASCII
text, how would you determine whether it contains Java source code or a letter to
your mother? Worse yet, how could you tell whether it contains Java source code or
C source code? It's not impossible, barring deliberately perverse files like a
concatenation of a C program with a Java program, but it's difficult and often not
worth your time.
17.2. Directories and Paths
Modern operating systems organize files into hierarchical directories. Directories are
also called folders, especially by Mac users. Each directory contains zero or more
files or other directories. Like files, directories have names and attributes,
thoughdepending on the operating systemthose names and attributes may be
different from the attributes allowed for files.
17.2.1. Paths and Separators
To specify a file completely, you don't just give its name. You also give the directory
the file lives in. Of course, that directory may itself be inside another directory,
which may be in another directory, until you reach the root of the filesystem. The
complete list of directories from the root to a specified file plus the name of the file
itself is called the absolute path to the file. The exact syntax of absolute paths varies
from system to system. Here are a few examples: DOS
C:\PUBLIC\HTML\JAVAFAQ\INDEX.HTM Win32 C:\public\html\javafaq\index.html Mac
OS 9 Macintosh HD:public:html:javafaq:index.html Unix/Linux/Mac OS X
/Volumes/Macintosh HD/public/html/javafaq/index.html
All of these strings reference a file named index.html on the primary hard drive in
the javafaq directory, which is itself in the html directory, which is in the public
directory. One obvious difference is the file separator character. Unix (including Linux
and Mac OS X) use a forward slash (/) to separate directories. DOS-based
filesystems, including the variants of Windows and OS/2, use a backslash (\). Other
platforms may use something completely different.
The separator used on a given system is available from the mnemonic constants
java.io.File.separator and java.io.File.separatorChar. File.separatorChar is the first
character of the string File.separator. All operating systems I'm familiar with use a
single character separator string, so these two variables are essentially the same.
The File.separator variable is set from the system property file.separator: public static
final String separator = System.getProperty("file.separator"); public static final char
separatorChar = separator.charAt(0);
System Properties
Several system properties provide full paths to directories according to the
local filesystem conventions. The security manager permitting, you can
use these to construct cross-platform filenames. Such properties include:
java.home
The directory where Java is installed, e.g., /usr/local/java on many
Unix systems.
java.class.path
The classpath contains many directories separated by the path
separator character.
user.home
The user's home directory.
user.dir
The current working directory.
There are also two related mnemonic constants, File.pathSeparator and
File.pathSeparatorChar. The path separator string is set from the system property
path.separator. As with the separator character, File.pathSeparatorChar is the first
character in File.pathSeparator.
public static final String pathSeparator
= System.getProperty("path.separator");
public static final char pathSeparatorChar = pathSeparator.charAt(0);
The path separator is used to separate two files (generally with complete
pathnames) in a list of paths such as a classpath. For example, with a separator of a
slash and a path separator of a colon, my classpath looks like this:
.:/usr/local/java/lib:/home/users/elharo/:/home/users/elharo/JavaDis/
Now the bad news: although Java has a fairly powerful abstraction layer so that
programmers don't need to hardcode explicit separators and path separators, few
programmers actually use this. Many programmers simply assume that the file
separator is a slash and the path separator is a colon and hardcode those constants
as "/" and ":". Therefore, to avoid breaking all this third-party code, Java passes
pathnames through a normalization phase that attempts to recognize the separator
conventions and convert those to the conventions of the local platform.
You probably don't need to know about the encoding at this level of detail unless
you're trying to manipulate filenames manuallyfor example, walking directories by
looking for separator characters rather than calling getParent( ). The more you let
Java do the work for you, the better off you'll be. As long as you use the methods of
the File class rather than parsing pathnames as strings, the details should be
transparent.
17.2.2. Relative versus Absolute Paths
There are two ways to reference a file, relative and absolute. Absolute addressing
gives a complete path to a file, starting with the disk or root of the filesystem and
working its way down. C:\PUBLIC\HTML\JAVAFAQ\INDEX.HTM, Macintosh
HD:public:html:javafaq:index.htm, and /public/html/javafaq/index.htm are all
examples of absolute paths. Relative addressing does not use a complete path to a
file; instead, it specifies the path relative to the current working directory. A relative
pathname may point to a file in the current working directory by giving its name
alone; other times it may point to a file in a subdirectory of the current working
directory by giving the name of the subdirectory and the name of the file, and it may
point to the parent of the current working directory with the double period (..).
17.2.2.1. Absolute paths
On Unix, all mounted disks, whether local or mounted over the network, are
combined into a single virtual filesystem. The root of this filesystem is the directory
called /. You generally do not need to concern yourself with which physical disk any
particular directory resides on, as long as that disk has sufficient space. Absolute
paths always begin with the root directory, /.
On Windows and Mac OS 9, there is no root directory. Each mounted disk partition or
network server is a separate and independent filesystem. On Windows, these disks
are assigned drive letters. A: is normally the floppy drive. B: is the second floppy
drive (uncommon these days), C: is the primary boot disk. D: is often the CD-ROM,
though it can be an additional hard disk or partition as well. E: through Z: can be
used for further disks, partitions, or network servers. A full pathname begins with the
drive letter where the file resides, e.g., C:\PUBLIC\HTML\JAVAFAQ\INDEX.HTM.
Windows can also refer to remote machines on the network by specifying an
additional level like this: \\BIO\C\2PUBLIC\HTML\JAVAFAQ\INDEX.HTM. This path refers
to a file called INDEX.HTM in the directory JAVAFAQ in the directory HTML in the
directory PUBLIC on the C drive of the machine BIO.
For these reasons and more, absolute pathnames are a royal pain to work with
across platforms. You should avoid hardcoding them in your programs whenever
possible. Instead, you should calculate them at runtime from system properties and
user input.
17.2.2.2. Relative paths
The following are some examples of relative paths: Unix html/index.html DOS
HTML\INDEX.HTM Win32 html\index.html Mac OS 9 :html:index.html Unix index.html
DOS INDEX.HTM Win32 index.html Mac OS 9 index.html
Note that a filename in isolation constitutes a relative path on all platforms.
Generally, the running application identifies one directory as the current working
directory. Relative pathnames are interpreted relative to the working directory.
Normally, the current working directory is the directory in which the application was
launched. For example, if you started your program from the command line in the
/home/users/elharo directory, a relative path of classes/juggler.class would point to a
file with the absolute path /home/users/elharo/classes/juggler.class. On the
Macintosh, the current working directory is generally whichever one the application
lives in.
The current working directory is fixed once a program starts running. Java provides
no means to change it.
Because the current working directory is unpredictable, you should not hardcode
relative pathnames into your application. A common solution is to distribute your
program as a JAR archive, store the data files in the JAR file, and retrieve them with
the various geTResource( ), geTResourceAsStream( ), and findResource( ) methods of
java.lang.Class or java.lang.ClassLoader. This works irrespective of the current working
directory as long as the JAR archive has been placed somewhere in the classpath.
17.3. The File Class
Instances of the java.io.File class represent filenames on the local system, not actual
files. Occasionally, this distinction is crucial. For instance, File objects can represent
directories as well as files. Also, you cannot assume that a file exists just because you
have a File object for a file.
public class File extends Object implements Serializable, Comparable
Although there are no guarantees that a file named by a File object actually exists, the
File class does contain many methods for getting information about the attributes of a
file and for manipulating those files. The File class attempts to account for systemdependent features like the file separator character and file attributes.
Each File object contains a single String field called path that contains either a relative or
absolute path to the file, including the name of the file or directory itself:
private String path
Many methods in this class work solely by looking at this string. They do not necessarily
look at any part of the filesystem.
17.3.1. Constructing File Objects
The java.io.File class has three constructors. Each accepts some variation of a filename
as an argument. This one is the simplest:
public File(String path)
The path argument should be either an absolute or relative path to the file in a format
understood by the host operating system. For example, using Unix filename conventions:
File uf1 = new File("25.html");
File uf2 = new File("course/week2/25.html");
File uf3 = new File("/public/html/course/week2/25.html");
Much poorly written Java code implicitly assumes Unix filename conventions, and most
VMs take this into account. Therefore, code that assumes Unix conventions is likely to
produce reasonable results on most operating systems. Windows VMs generally allow you
to use Windows conventions instead. For example:
File wf1 = new File("25.htm");
File wf2 = new File("course\\week2\\25.html");
File wf3 = new File("D:\\public\\html\\course\\week2\\25.htm");
The double backslashes are merely the escape sequence for the single backslash in a
string literal. Otherwise, attempts to compile this code would generate an "Invalid escape
character" error message. Remember that \t is a tab, \n a linefeed, and so on. Here,
however, we need a backslash to simply be a backslash.
The second File constructor specifies an absolute or relative pathname and a filename:
public File(String directory, String filename)
For example:
File f2 = new File("course/week2", "25.html");
This produces a File object with the path field set to course/week2/25.html. The constructor is
smart enough to handle the case of directories with and without trailing separators. The
third constructor is identical to the second, except that the first argument is a File object
instead of a string.
public File(File directory, String filename)
This third constructor is the most robust of the lot, provided the filename is only a
filename like readme.txt and not a relative path like cryptozip/readme.txt. The reason is
that this constructor guarantees the use of the local path separator character and is thus
more platform-independent. You can use this to build a file structure that works on all
platforms regardless of path separators or normalization routines. For example, suppose
you want to build a File object that points to the file com/elharo/io/StreamCopier.class.
The following four lines do this without reference to the file separator character:
File temp = new File("com");
temp = new File(temp, "elharo");
temp = new File(temp, "io");
File scfile = new File(temp, "StreamCopier.class");
None of these constructors throw any exceptions. All the constructor does is set the path
field; Java never checks to see whether the file named by path actually exists or even
whether the name passed to the constructor is a valid filename. For example, the
following File object causes problems on Unix, OS/2, Mac OS 9, Mac OS X, and Windows,
but you can still construct it:
File f = new File("-This is not a /nice\\ file:\r\nno it isn't");
Some methods in other classes also return File objects, most notably the
java.awt.FileDialog and javax.swing.JFileChooser methods discussed in the next chapter. Using
file dialogs or choosers to ask the user for a filename is preferable to hardcoding them or
reading them from the command line because file dialogs properly handle cross-platform
issues and the distinctions between relative and absolute paths.
One thing you may not have noticed about these constructors: since
a File object does not represent a file as much as a filename, these
constructors do not actually create files. To create a new file with
Java, you can open a file output stream to the file or invoke the
createNewFile( ) method.
In Java 1.2 and later, construction of a File object includes normalization. This process
reads hardcoded pathnames and attempts to convert them to the conventions of the
local platform. This improves compatibility with code that's making assumptions about
filenames. For instance, if a Windows VM is asked to create a File object with the path
/public/html/javafaq/course/week2/index.html, it actually sets the path field to \public\
html\ javafaq\ course\week2\ index.html. The reverse process happens on Unix;
backslashes are converted to forward slashes. Because it can only really normalize
separators, not filesystem roots, this scheme works better for relative pathnames than
absolute ones.
17.3.2. Listing the Roots
The static File.listRoots( ) method returns an array containing the roots of the filesystem
as File objects:
public static File[] listRoots( )
On Unix, this array is likely to have length 1 and contain the single root /. On Windows, it
probably contains all the drive letters mapped to one device or another, whether or not
there's actually any media in the drive, e.g., A:\, C:\, D:\, E:\, F:\, G:\. If the security
manager does not allow the program to read a particular root, that root is not included in
the returned list. If the security manager does not allow the program to read any root, the
returned list will have length zero. Do not assume the array returned by listRoots( )
necessarily has any members! null is returned if the list can't be determined at all. This is
not the same thing as a zero-length array.
The list of roots may or may not contain drives that are mounted over the network. If the
drive is mounted in such a fashion that it pretends to be a local drive, it probably will be
in the list. If the filesystem does not look like a local drive, it probably won't appear in the
list. For instance, on Windows, network drives mapped to letters appear, but drives with
UNC pathnames do not. Example 17-1 is a very simple program to list the roots and print
them.
Example 17-1. RootLister
import java.io.*;
public class RootLister {
public static void main(String[] args) {
File[] roots = File.listRoots( );
for (int i = 0; i < roots.length; i++) {
System.out.println(roots[i]);
}
}
}
Here's the output produced by RootLister on my Windows NT system. A: is the floppy drive.
This system doesn't have a second floppy, which would normally be B:. C:, D:, E:, and F:
are all partitions of the primary hard drive that appear to be separate drives. G: is an
external hard drive, and H: is the CD-ROM. I: is a Macintosh drive mounted over the LAN.
D:\JAVA\ioexamples\17>java RootLister
A:\
C:\
D:\
E:\
F:\
G:\
H:\
I:\
The output on Unix (including Mac OS X) is much simpler and is virtually guaranteed to
look like this:
$ java RootLister
/
17.3.3. Listing Information about a File
The File class contains many methods that return particular information about the file.
Most of this information can be gleaned from the path field alone without accessing the
filesystem. Therefore, most of these methods do not throw IOExceptions.
17.3.3.1. Does the file exist? Is it a normal file? Is it a directory?
Since a File object does not necessarily correspond to a real file on the disk, the first
question you'll probably want to ask is whether the file corresponding to the File object
actually exists. This is especially important if you're relying on a user to type a filename
rather than select it from a dialog because users routinely mistype filenames. The exists(
) method returns true if the file named in this file object's path field exists or false if it
doesn't:
public boolean exists( )
There are two other ways to ask this question. The isFile( ) method returns TRue if the file
exists and is not a directory. On the other hand, the isDirectory( ) method returns true if
the file exists and is a directory.
public boolean isFile( )
public boolean isDirectory( )
The isDirectory( ) method considers Unix symbolic links and Mac aliases to directories to
be directories themselves; it does not consider Windows shortcuts to directories to be
directories. All three of these methods throw a security exception if the security manager
does not allow the specified file to be read. In fact, if the file couldn't be read if it did
exist, isDirectory( ) tHRows an exception whether the file actually exists or not. Revealing
whether certain files exist can be a security violation.
17.3.3.2. Filename and path
The getName( ) method takes no arguments and returns the name of the file as a string:
public String getName( )
The name does not include any part of the directory in which the file lives. That is, you
get back index.html instead of /public/html/javafaq/index.html. If the file is a directory like
/public/html/javafaq/, only the last name is returned (javafaq in this example).
The getPath( ) method returns the complete path to the file:
public String getPath( )
This simply returns the path field. Therefore, the path is relative if the File object was
constructed with a relative path and absolute if the File object was constructed with an
absolute path. Furthermore, this method never throws IOExceptions. Consider Example 17-
2. This simple program constructs two File objects, one with a relative path and one with
an absolute path, and prints the name and path of each object.
Example 17-2. Paths
import java.io.*;
public class Paths {
public static void main(String[] args) {
File absolute = new File("/public/html/javafaq/index.html");
File relative = new File("html/javafaq/index.html");
System.out.println("absolute: ");
System.out.println(absolute.getName( ));
System.out.println(absolute.getPath( ));
System.out.println("relative: ");
System.out.println(relative.getName( ));
System.out.println(relative.getPath( ));
}
}
When the program is run on Unix, here's the output:
$ java Paths
absolute:
index.html
/public/html/javafaq/index.html
relative:
index.html
html/javafaq/index.html
On Windows the output is a little different because the File constructor normalizes the file
separator character to the backslash:
D:\JAVA\ioexamples\17>java Paths
absolute:
index.html
\public\html\javafaq\index.html
relative:
index.html
html\javafaq\index.html
17.3.3.3. Absolute paths
The getAbsolutePath( ) method returns the complete path to the file starting from a
filesystem root:
public String getAbsolutePath( )
Examples of absolute paths include /public/html/javafaq/index.html and
D:\JAVA\ioexamples\17 but not html/javafaq/index.html or ioexamples\17. If the File
object's path field is already an absolute path, its value is returned. Otherwise, a separator
character and the value of the path field are appended to the value of the system property
user.dir, which refers to the current working directory. This method throws a security
exception when run from untrusted code because untrusted code cannot normally read
the user.dir property.
If you need to know whether a file is specified by a relative or absolute path, you can call
isAbsolute( ):
public boolean isAbsolute( )
This does not throw any security exceptions because it does not need to go outside the
class to determine whether or not a pathname is absolute. Instead, the check is
performed by looking at the first few characters of the path field. On Unix, an absolute
path begins with a /. On Windows or OS/2, an absolute path begins with a capital letter
followed by a colon and a backslash, like C:\.
17.3.3.4. Canonical paths
Exactly what a canonical path is, and how it differs from an absolute path, is systemdependent, but it tends to mean that the path is somehow more real than the absolute
path. Typically, if the full path contains aliases, shortcuts, shadows, or symbolic links of
some kind, the canonical path resolves those aliases to the actual directories they refer
to. The canonical path is returned by the getCanonicalPath( ) method:
public String getCanonicalPath( ) throws IOException
For example, suppose /bin/perl is a symbolic link to the real file at /usr/local/bin/perl, and
you construct a File object perlLink like this:
File perlLink = new File("/bin/perl");
perlLink.getAbsolutePath( ) returns /bin/perl, but perlLink.getCanonicalPath( ) returns
/usr/local/bin/perl.
getCanonicalPath( ) only resolves symbolic links. It does not resolve
hard links. That is, it resolves links created with "ln -s file link" but
not "ln file link."
getCanonicalPath( ) also removes relative references like the double period (..), which refers
to the parent directory in paths. For instance, suppose the current working directory is
/home/elharo/javaio/ioexamples/17.Then you create a File object like this:
File f = new File("../11/index.html");
String absolutePath = f.getAbsolutePath( );
String canonicalPath = f.getCanonicalPath( );
absolutePath is now /home/elharo/javaio/ioexamples/17/../11/index.html. However,
canonicalPath is /home/elharo/javaio/ioexamples/11/index.html.
On Windows, getCanonicalPath( ) normalizes the case of two paths so that
C:\Documents\Books and C:\DOCUMENTS\BOOKS are recognized as the same path. Mac
OS X also normalizes the case. Other Unixes with case sensitive filesystems do not.
Usually, the normalized form is whatever was initially provided for the file's name.
One use for canonical paths is to test whether two files are the same. You might need to
do this if you're reading from an input file and writing to an output file. While it might
occasionally be possible to read from and write to the same file, doing so always requires
special care. For example, the FileCopier program from Example 4-2 in Chapter 4 failed
when the source and destination were the same file. Now we can use canonical paths to
correct that flaw by testing whether two files are the same before copying, as shown in
Example 17-3. If the files are the same, no copy needs to take place.
Example 17-3. Safe FileCopier
import java.io.*;
public class SafeFileCopier {
public static void main(String[] args) throws IOException {
if (args.length != 2) {
System.err.println("Usage: java FileCopier infile outfile");
}
else copy(new File(args[0]), new File(args[1]));
}
public static void copy(File inFile, File outFile) throws
IOException {
if (inFile.getCanonicalPath().equals(outFile.getCanonicalPath(
))) {
// inFile and outFile are the same;
// hence no copying is required.
return;
}
InputStream in = null;
OutputStream out = null;
try {
in = new BufferedInputStream(new FileInputStream(inFile));
out = new BufferedOutputStream(new
FileOutputStream(outFile));
for (int c = in.read(); c != -1; c = in.read( )) {
out.write(c);
}
}
finally {
if (in != null) in.close( );
if (out != null) out.close( );
}
}
}
I could test the files themselves, but since a single file may have multiple paths through
aliases or parent links, I'm still not guaranteed that the inFile and outFile aren't the same.
But each file has exactly one unique canonical path, so if inFile's canonical path is not
equal to outFile's canonical path, they can't possibly be the same file. Conversely, if
inFile's canonical path is equal to outFile's canonical path, they must be the same file.
The getCanonicalFile( ) method acts just like getCanonicalPath( ), except that it returns a new
File object instead of a string:
public File getCanonicalFile( ) throws IOException
The File object returned has a path field that's the canonical path of the file. Both
getCanonicalPath( ) and getCanonicalFile( ) can throw IOExceptions because both need to read
the filesystem to resolve aliases, shadows, symbolic links, shortcuts, and parent directory
references.
17.3.3.5. Parents
The getParent( ) method returns a string containing everything before the last file
separator in the path field:
public String getParent( )
For example, if a File object's path field is
/home/users/elharo/javaio/ioexamples/11/index.html, getParent( ) returns
/home/users/elharo/javaio/ioexamples/11. If a File object's path field is 11/index.html, getParent( )
returns 11. If a File object's path field is index.html, getParent( ) returns null. Filesystem
roots have no parent directories. For these files, getParent( ) returns null.
The getParentFile( ) method does the same thing, except that it returns the parent as a
new File object instead of a string:
public File getParentFile( )
17.3.3.6. File attributes
The File class has several methods that return information about the file, such as its
length, the time it was last modified, whether it's readable, whether it's writable, and
whether it's hidden.
The canWrite( ) method indicates whether the program can write into the file referred to
by this File object. The canRead( ) method indicates whether the program can read from
the file.
public boolean canRead( )
public boolean canWrite( )
Both these methods perform two checks. The first check determines whether Java's
security manager allows the file in question to be read or written; the second determines
whether the operating system allows the file to be read or written. If Java's security
manager disallows the access, a security exception is thrown. If the OS disallows the
access, the method returns false but does not throw any exceptions. However,
attempting to read from or write to such a file will almost certainly throw an IOException.
Java 6 adds a canExecute( ) method that tests whether the current application can execute
the file represented by the File object:
public boolean canExecute( ) // Java 6
Like canRead( ) and canWrite( ), this method does not merely check the execute bit. The
question is whether the current program can launch the file (e.g., by Runtime's exec( )
methods).
The isHidden( ) method returns true if the file exists but is hidden; that is, it does not
appear in normal displays or listings. It returns false if the file isn't hidden or doesn't
exist.
public boolean isHidden( )
Exactly how a file is hidden varies from platform to platform. On Unix, any file whose
name begins with a period is hidden. On Windows, hidden files are identified by particular
attributes. This method throws a security exception if the security manager doesn't allow
the file to be read.
The lastModified( ) method returns a long indicating the last time this file was modified:
public long lastModified( )
The time is the number of milliseconds since midnight, January 1, 1970, Greenwich Mean
Time. However, in older VMs the conversion between this long and a real date is platformdependent, so it's only useful for comparing the modification dates of different files, not
for determining the absolute time a file was modified. This method throws a security
exception if the security manager doesn't allow the file to be read. It returns 0 if the file
doesn't exist or the last modified date can't be determined.
Finally, the length( ) method returns the number of bytes in the file or 0 if the file does not
exist:
public long length( )
This method throws a security exception if the security manager doesn't allow the file to
be read.
17.3.3.7. An example
Example 17-4 is a character-mode program that lists all the available information about
files named on the command line. Names may be given as absolute or relative paths.
Example 17-4. The FileSpy program
import java.io.*;
import java.util.*;
public class FileSpy {
public static void main(String[] args) {
for (int i = 0; i < args.length; i++) {
File f = new File(args[i]);
if (f.exists( )) {
System.out.println("Name: " + f.getName( ));
System.out.println("Absolute path: " + f.getAbsolutePath(
));
try {
System.out.println("Canonical path: " +
f.getCanonicalPath( ));
}
catch (IOException ex) {
System.out.println("Could not determine the canonical
path.");
}
String parent = f.getParent( );
if (parent != null) {
System.out.println("Parent: " + f.getParent( ));
}
if (f.canWrite()) System.out.println(f.getName( ) + " is
writable.");
if (f.canRead()) System.out.println(f.getName( ) + " is
readable.");
if (f.isFile( )) {
System.out.println(f.getName( ) + " is a file.");
}
else if (f.isDirectory( )) {
System.out.println(f.getName( ) + " is a directory.");
}
else {
System.out.println("What is this?");
}
if (f.isAbsolute( )) {
System.out.println(f.getPath( ) + " is an absolute
path.");
}
else {
System.out.println(f.getPath( ) + " is not an absolute
path.");
}
long lm = f.lastModified( );
if (lm != 0) System.out.println("Last Modified at " + new
Date(lm));
long length = f.length( );
if (length != 0) {
System.out.println(f.getName( ) + " is " + length + "
bytes long.");
}
}
else {
System.out.println("I'm sorry. I can't find the file " +
args[i]);
}
}
}
}
Here's the result of running FileSpy on itself:
D:\JAVA\ioexamples\17>java FileSpy FileSpy.java
Name: FileSpy.java
Absolute path: D:\JAVA\ioexamples\17\FileSpy.java
Canonical path: D:\Java\ioexamples\17\FileSpy.java
FileSpy.java is writable.
FileSpy.java is readable.
FileSpy.java is a file.
FileSpy.java is not an absolute path.
Last Modified at Fri Sep 11 15:11:24 PDT 1998
FileSpy.java is 1846 bytes long.
17.3.4. Manipulating Files
The File class has methods to create, move, rename, and delete files. A method to copy
files is a noticeable omission.
17.3.4.1. Creating files
The createNewFile( ) method creates the file referenced by the File object:
public boolean createNewFile( ) throws IOException
This method checks to see whether the file exists and creates the file if it doesn't already
exist. It returns true if the file was created and false if it wasn't created, either because it
couldn't be created or because the file already existed. For example:
File f = new File("output.dat");
boolean success = f.createNewFile( );
if (success) {
//...
}
else { //...
This method throws an IOException if an I/O error occurs. It throws a security exception if
the security manager vetoes the creation of the file.
17.3.4.2. Moving and renaming files
The renameTo( ) method changes the name of a file:
public boolean renameTo(File destination)
For example, to change the name of the file src.txt in the current working directory to
dst.txt, you would write:
File src = new File("src.txt");
File dst = new File("dst.txt");
src.renameTo(dst);
If a file already exists with the destination name, the existing file may be overwritten or
the rename may fail and return false. This varies from one platform and VM to another.
If the destination file is in a different directory than the source file, the renameTo( ) may
move the source file from its original directory to the directory specified by the destination
argument. For example, to move a file src to the directory /usr/tmp on a Unix system
without changing the file's name, do this:
File dest = new File("/usr/tmp/" + src.getName( ));
src.renameTo(dest);
However, this behavior is unreliable and platform-dependent. For instance, renameTo( )
moves files if, and only if, the directory structure specified in the dest File object already
exists. I've also seen this code work on some Unix versions with some versions of the JDK
and fail on others. It's best not to rely on this method for more than renaming a file in the
same directory.
If src is successfully renamed, the method returns true. If the security manager doesn't
allow the program to write to both the source file and the destination file, renameTo( )
throws a security exception. Otherwise, it returns false. Be sure to check this. Renaming is
one of the more flaky areas of Java.
Copying Files
There is no copy( ) method that merely copies a file to a new location without
removing the original. However, you can open a file output stream to the copy,
open a file input stream from the original file, and copy the data byte by byte
from the original into the copy. For example, to copy the file src to the file dst:
FileInputStream in = new FileInputStream(src);
FileOutputStream out = new FileOutputStream(dst);
for (int c = in.read(); c != -1; c = in.read( )) {
out.write(c);
}
in.close( );
out.close( );
There are some serious problems with this code. First of all, it assumes that
both src and dst refer to files, not directories. Second, it only copies the
contents of the files. If the file is associated with metainformation or extra
data, that data is lost.
17.3.4.3. Deleting files
The delete( ) method removes files from the filesystem permanently:
public boolean delete( )
This method returns true if the file existed and was deleted. (You can't delete a file that
doesn't exist.) If the security manager disallows this action, a security exception is
thrown. Otherwise, delete( ) returns false.
17.3.4.4. Changing file attributes
The setLastModified( ) method changes a file's last modified time:
public boolean setLastModified(long time)
The time argument is the number of milliseconds since midnight, GMT, January 1, 1970.
This is converted to the format necessary for a particular platform's file modification
times. If the platform does not support millisecond-accurate file modification times, the
time is rounded to the nearest time the host platform does support. This method throws
an IllegalArgumentException if time is negative; it throws a SecurityException if the security
manager disallows write access to the file.
The setReadOnly( ) method marks the file so that writing to the file is disallowed:
public boolean setReadOnly( )
Java 6 adds several more methods for changing a file's attributes. You can mark a file
readable, writable, or executable:
public boolean setReadable(boolean executable) // Java 6
public boolean setWritable(boolean executable) // Java 6
public boolean setExecutable(boolean executable) // Java 6
Passing true makes the file readable, writable, and executable by the file's owner; passing
false does the opposite. Changing these attributes may not always be possible. These
methods return TRue if the file now has the requested attribute value or false if it doesn't.
These methods can also throw a SecurityException if the security manager disallows access
to the file.
You can pass false as the second argument to these methods to indicate that the file
should be readable, writable, and executable by everyone, not just the file's owner:
public boolean setReadable(boolean executable, boolean ownerOnly) //
Java 6
public boolean setWritable(boolean executable, boolean ownerOnly) //
Java 6
public boolean setExecutable(boolean executable, boolean ownerOnly) //
Java 6
Java has no concept of Unix group access, though.
17.3.5. Temporary Files
The File class provides two methods that create temporary files that exist only as long as
the program runs:
public static File createTempFile(String prefix, String suffix)
throws IOException
public static File createTempFile(String prefix, String suffix,
File directory) throws IOException
The createTempFile( ) methods create a file with a name that begins with the specified
prefix and ends with the specified suffix. The prefix is a string used at the beginning of all
temporary filenames; the suffix is appended to the end of all temporary filenames. The
suffix may be null. If so, .tmp is used as the suffix. The same run of the same VM does
not create two files with the same name. For example, consider this for loop:
for (int i=0; i < 10; i++) {
File.createTempFile("mail", ".tem");
}
When run, it creates files named something like mail30446.tem, mail30447.tem, etc.
through mail30455.tem.
By default, temporary files are placed in the directory named by the java.io.tmpdir
property. On Unix, this is likely to be /tmp or /var/tmp. On Windows, it's probably C:\temp
or C:\Windows\Temp. On Mac OS X, it's probably /private/tmp. You can specify a different
directory using the third argument to createTempFile( ). For instance, this code fragment
creates a temporary file in the current working directory:
File cwd = new File(System.getProperty("user.dir"));
File temp = File.createTempFile("rus", ".tmp", cwd);
You often want to delete temporary files when your program exits. You can accomplish
this by passing them to the deleteOnExit( ) method:
public void deleteOnExit( )
For example:
File temp = File.createTempFile("mail", ".tem");
temp.deleteOnExit( );
This method works on any File object, not just temporary files. Be careful because there's
no good way to cancel a request to delete files.
Temporary files are useful when you need to operate on a file in place. You can do this in
two passes. In the first pass, read from the file you're converting and write into the
temporary file. In the second pass, read from the temporary file and write into the file
you're converting. Here's an example:
File infile = new File(args[2]);
File outfile = new File(args[3]);
boolean usingTempFile = false;
if (infile.getCanonicalPath().equals(outfile.getCanonicalPath( ))) {
outfile = File.createTempFile("temp", null);
outfile.deleteOnExit( );
usingTempFile = true;
}
// perform operations as normal, then close both files...
if (usingTempFile) {
FileInputStream fin = new FileInputStream(outfile);
FileOutputStream fout = new FileOutputStream(infile);
for (int c = fin.read(); c != -1; c = fin.read( )) {
fout.write(c);
}
fin.close( );
fout.close( );
}
17.3.6. Checking for Free Space/Java 6
Java 6 adds three methods to inspect the amount of available and used space on a
particular partition. A File object is used to choose the partition but otherwise it has no
effect on the value returned. Two files on the same partition would give the same
answers.
The getTotalSpace( ) method returns the size of the file's partition in bytes:
public long getTotalSpace( )
The getFreeSpace( ) method returns the total amount of empty space on the file's partition
in bytes:
public long getFreeSpace( )
If the file does not exist, these methods return 0. They do not throw an exception. The
number returned by this method is approximate. Depending on the nature of the
filesystem, you may not be able to use all the bytes for a single file. For instance, some
filesystems have maximum file sizes. The getUsableSpace( ) method makes a slightly better
effort to find out how much space you can actually use.
public long getUsableSpace( )
It accounts for details like read-only filesystems that getFreeSpace( ) may not. However, the
number it returns is still only approximate.
Even Java 6 doesn't have any reliable means to list all the partitions on a disk or to
determine which partition you're on. If you happen to know the locations of files on each
partition, these methods tell you how much space is left on each one. Normally, you have
a single directory where a file will be saved and what you want to know is how much
space is left on that directory's partition.
Example 17-5 is a simple program that lists the total, free, and usable space on the
partition that contains the current working directory:
Example 17-5. Listing available space on the current partition
import java.io.*;
public class CWDSpace {
public static void main(String[] args) {
File cwd = new File(".");
System.out.println("Total space on current partition: "
+ cwd.getTotalSpace( ) / (1024 * 1024) + " MB\t");
System.out.println("Free space on current partition: "
+ cwd.getFreeSpace( ) / (1024 * 1024) + " MB\t");
System.out.println("Usable space on current partition: "
+ cwd.getUsableSpace( ) / (1024 * 1024) + " MB");
}
}
Here's the output when I ran this on my Linux box from a directory in the /home partition:
$ java CWDSpace
Total space on current partition: 6053 MB
Free space on current partition: 2601 MB
Usable space on current partition: 2293 MB
If I had to save a large file in this directory, I could save around two gigabytes. Anything
much larger and I'd have to free up some space first.
17.3.7. Directories
A File object can represent a directory as easily as a file. Most of the File methods like
getName( ), canWrite( ), and getPath( ) behave exactly the same for a directory as they do for
a file. However, a couple of methods in the File class behave differently when they
operate on directories than they do when operating on ordinary files.
The delete( ) method only works on empty directories. If a directory contains even one
file, it can't easily be deleted. If you attempt to delete a nonempty directory, delete( )
fails and returns false. No exception is thrown.
The renameTo( ) method works on both empty and nonempty directories. Howeverwhether
a directory is empty or notrenameTo( ) can only rename it, not move it to a different
directory. If you attempt to move a directory into another directory, renameTo( ) fails and
returns false. No exception is thrown.
The File class also has several methods that just work with directories, not with regular
files.
17.3.7.1. Creating directories
The createNewFile( ) doesn't work for directories. For that purpose, the File class has a
mkdir( ) method:
public boolean mkdir( )
The mkdir( ) method attempts to create a directory with the path specified in the path
field. If the directory is created, the method returns TRue. For example:
File f = new File("tmp/");
f.mkdir( );
The trailing slash is optional, but it helps you to remember that you're dealing with a
directory rather than a plain file. If the security manager does not allow the directory to
be created, mkdir( ) throws a security exception. If the directory cannot be created for any
other reason, mkdir( ) returns false. The mkdir( ) method only works for single directories.
Trying to create a directory like com/elharo/io/ with mkdir( ) only works if com/elharo
already exists.
The mkdirs( ) method creates every directory in a path that doesn't already exist:
public boolean mkdirs( )
For example:
File f = new File("com/elharo/io/");
f.mkdirs( );
mkdirs( ) returns true if all directories in this path are created or already exist and false if
only some or none of them are created. If mkdirs( ) returns false, you need to test each
directory in the path to see whether it was created because the invocation could have
been partially successful.
One reason mkdir( ) and mkdirs( ) may return false (fail to create a directory) is that a file
already exists with the name the directory has. Neither mkdir( ) nor mkdirs( ) will overwrite
an existing file or directory.
17.3.7.2. Listing directories
The list( ) method returns an array of strings containing the names of each file in the
directory referred to by the File object:
public String[] list( )
This method returns null if the File object doesn't point to a directory. It throws a security
exception if the program isn't allowed to read the directory being listed. An alternative
version of list( ) uses a FilenameFilter object (discussed later in the chapter) to restrict
which files are included in the list:
public String[] list(FilenameFilter filter)
Example 17-6 is a simple character-mode program that recursively lists all the files in a
directory, and all the files in directories in the directory, and all the files in directories in
the directory, and so on. Files are indented two spaces for each level deep they are in the
hierarchy.
Example 17-6. The DirList program
import java.io.*;
import java.util.*;
public class DirList {
private File directory;
private int indent = 2;
private static List seen = new ArrayList( );
public static void main(String[] args) throws IOException {
DirList dl = new DirList(args[0]);
dl.list( );
}
public DirList(String name) throws IOException {
this(new File(name), 2);
}
public DirList(File f) throws IOException {
this(f, 2);
}
public DirList(File directory, int indent) throws IOException {
if (directory.isDirectory( )) {
this.directory = new File(directory.getCanonicalPath( ));
}
else {
throw new IOException(directory.toString( ) + " is not a
directory");
}
this.indent = indent;
String spaces = "";
for (int i = 0; i < indent-2; i++) spaces += " ";
System.out.println(spaces + directory + File.separatorChar);
}
public void list( ) throws IOException {
if (!seen.contains(this.directory)) {
seen.add(this.directory);
String[] files = directory.list( );
String spaces = "";
for (int i = 0; i < indent; i++) spaces += " ";
for (int i = 0; i < files.length; i++) {
File f = new File(directory, files[i]);
if (f.isFile( )) {
System.out.println(spaces + f.getName( ));
}
else { // it's another directory
DirList dl = new DirList(f, indent + 2);
dl.list( );
}
}
}
}
}
Special care has to be taken to make sure this program doesn't get caught in an infinite
recursion. If a directory contains an alias, shadow, shortcut, or symbolic link that points to
one of its own parents, there's potential for infinite recursion. To avoid this possibility, all
paths are converted to canonical paths in the constructor, and these paths are stored in
the static list seen. A directory is listed only if it has not yet been traversed by this
program.
17.3.7.3. The listFiles( ) methods
The two list( ) methods return arrays of strings. The strings contain the names of files.
You can use these to construct File objects. Java allows you to eliminate the intermediate
step of creating File objects by providing two listFiles( ) methods that return arrays of
File objects instead of arrays of strings.
public File[] listFiles( )
public File[] listFiles(FilenameFilter filter)
public File[] listFiles(FileFilter filter)
The no-argument variant of listFiles( ) simply returns an array of all the files in the given
directory. The other two variants return the files that pass through their filters. File and
filename filters will be discussed shortly.
17.3.8. File URLs
File URLs locate a file on the local filesystem. (Very early web browsers used file URLs to
refer to FTP sites. However, that usage has long since disappeared.) They have this basic
form:
file://<host>/<path>
<host> should be the fully qualified domain name of the system on which the <path> is
found, though if it's omitted, the local host is assumed. <path> is the hierarchical path to
the file, using a forward slash as a directory separator (regardless of host filename
conventions) and URL encoding of any special characters in filenames that would
normally be encoded in a URL. Examples of file URLs include:
file:///C|/docs/JCE%201.2%20beta%201/guide/API_users_guide.html
file:///D:/JAVA/
file:///usr/local/java/docs/JCE%201.2%20beta%201/guide/API_users_guide.html
file:///D%7C/JAVA/
file:///Macintosh%20HD/Java/Cafe%20%au%20%Lait/course/week4/01.5.html
file:/Users/elharo/Documents/books/Java%20IO%20 2/
Many web browsers allow other, nonstandard formats like:
file:///C|/jdk2beta4/docs/JCE 1.2 beta 1/guide/API_users_guide.html
file:///C:\jdk1.2beta4\docs\JCE 1.2 beta 1\guide\API_users_guide.html
file:/D:/Java/ioexamples/17/FileDialogApplet.html
file:/Users/elharo/Documents/books/Java IO 2/
Because of the differences between file and directory names from one computer to the
next, the exact syntax of file URLs is unpredictable from platform to platform and web
browser to web browser. The File class has a toURL( ) method that returns a file URL that's
appropriate for the local platform:
public URL toURL( ) throws MalformedURLException
However, this method does not properly escape non-ASCII and non-URL-legal characters
such as the space so it's been deprecated as of Java 1.4 and replaced by the toURI( )
method:
public URI toURI( )
toURI( ) isn't perfect, but it does a better job than toURL( ), and you should use it if it's
available.
17.4. Filename Filters
You often want to look for a particular kind of filefor example, text files. To do this,
you need a FilenameFilter object that specifies which files you'll accept. FilenameFilter
is an interface in the java.io package:
public interface FilenameFilter
This interface declares a single method, accept( ):
public abstract boolean accept(File directory, String name);
The directory argument is a File object pointing to a directory, and the name argument
is the name of a file. The method should return true if a file with this name in this
directory passes through the filter and false if it doesn't. Example 17-7 is a class that
filters out everything that is not an HTML file.
Example 17-7. HTMLFilter
import java.io.*;
public class HTMLFilter implements FilenameFilter {
public boolean accept(File directory, String name) {
if (name.endsWith(".html")) return true;
if (name.endsWith(".htm")) return true;
return false;
}
}
Files can be filtered using any criteria you like. An accept( ) method may test
modification date, permissions, file size, and any attribute Java supports. This accept(
) method tests whether the file ends with .html and is in a directory where the
program can read files:
public boolean accept(File directory, String name) {
if (name.endsWith(".html") && directory.canRead( )) {
return true;
}
return false;
}
Filename filters are primarily intended for the use of file dialogs, which will be
discussed in the next chapter. However, the listFiles( ) method can take a
FilenameFilter as an argument:
public File[] listFiles(FilenameFilter filter)
This method assumes that the File object represents a directory. The array of File
objects returned by listFiles( ) only contains those files that passed the filter. For
example, the following lines of code list HTML files in the /public/html/javafaq
directory using the HTMLFilter of Example 17-7:
File dir = new File("/public/html/javafaq");
File[] htmlFiles = dir.listFiles(new HTMLFilter( ));
for (int i = 0; i < htmlFiles.length; i++) {
System.out.println(htmlFiles[i]);
}
17.5. File Filters
The FileFilter interface is very similar to FilenameFilter:
public abstract interface FileFilter
The accept( ) method of FileFilter takes a single File object as an argument, rather
than two strings giving the directory and path:
public boolean accept(File pathname)
Example 17-8 is a filter that only passes HTML files. Its logic is essentially the same
as the filter of Example 17-7.
Example 17-8. HTMLFileFilter
import java.io.*;
public class HTMLFileFilter implements FileFilter {
public boolean accept(File pathname) {
if (pathname.getName( ).endsWith(".html")) return true;
if (pathname.getName( ).endsWith(".htm")) return true;
return false;
}
}
This class appears as an argument in one of the listFiles( ) methods of java.io.File:
public File[] listFiles(FileFilter filter)
Example 17-9 uses the HTMLFileFilter to list the HTML files in the current working
directory.
Example 17-9. List HTML files
import java.io.*;
public class HTMLFiles {
public static void main(String[] args) {
F
i
l
e
c
w
d
=
n
e
w
F
i
l
e
(
S
y
s
t
e
m.g
e
t
P
r
o
p
e
r
t
y
(
"
u
s
e
r.d
i
r
"
)
)
;
F
i
l
e
[
]
h
t
m
l
F
i
l
e
s
=
c
w
d.l
i
s
t
F
i
l
e
s
(
n
e
w
H
T
M
L
F
i
l
e
F
i
l
t
e
r
(
)
)
;
f
o
r
(
i
n
t
i
=
0
;
i
<
h
t
m
l
F
i
l
e
s.l
e
n
g
t
h
;
i
+
+
)
{
S
y
s
t
e
m.o
u
t.p
r
i
n
t
l
n
(
h
t
m
l
F
i
l
e
s
[
i
]
)
;
}
}
}
17.6. File Descriptors
As I've said several times so far, the existence of a java.io.File object doesn't imply
the existence of the file it represents. A java.io.FileDescriptor object does, however,
refer to an actual file:
public final class FileDescriptor extends Object
A FileDescriptor object is an abstraction of an underlying machine-specific structure
that represents an open file. While file descriptors are very important for the
underlying OS and filesystem, their only real use in Java is to guarantee that data
that's been written to a stream is in fact committed to disk, that is, to synchronize
between the program and the hardware.
In addition to open files, file descriptors can also represent open sockets. There are
also three file descriptors for the console: System.in, System.out, and System.err. These
are available as the three mnemonic constants FileDescriptor.in, FileDescriptor.out,
and FileDescriptor.err:
public static final FileDescriptor in
public static final FileDescriptor out
public static final FileDescriptor err
Because file descriptors are very closely tied to the native operating system, you
never construct your own file descriptors. Various methods in other classes that refer
to open files or sockets may return them. Both the FileInputStream and FileOutputStream
classes and the RandomAccessFile class have a getFD( ) method that returns the file
descriptor associated with the open stream or file:
public final FileDescriptor getFD( ) throws IOException
Since file descriptors are only associated with open files and sockets, they become
invalid as soon as the file or socket is closed. You can test whether a file descriptor is
still valid with the valid( ) method:
public native boolean valid( )
This returns TRue if the descriptor is still valid or false if it isn't.
The one real use to which a client programmer can put a file descriptor object is to
sync a file. This is accomplished with the aptly named sync( ) method:
public native void sync( ) throws SyncFailedException
The sync( ) method forces the system buffers to write all the data they contain to the
actual hardware. Generally, you'll want to flush the stream before syncing it.
Flushing clears out Java's internal buffers. Syncing clears out the operating system's,
device driver's, and hardware's buffers. If synchronization does not succeed, sync( )
throws a java.io.SyncFailedException, a subclass of IOException.
17.7. Random-Access Files
File input and output streams require you to start reading or writing at the beginning
of a file and then read or write the file in order, possibly skipping over some bytes or
backing up but mostly moving from start to finish. Sometimes, however, you need to
read parts of a file in a more or less random order, where the data near the
beginning of the file isn't necessarily read before the data nearer the end. Other
times you need to both read and write the same file. For example, in record-oriented
applications like databases, the actual data may be indexed; you would use the
index to determine where in the file to find the record you need to read or write.
While you could do this by constantly opening and closing the file and skipping to
the point where you needed to read, this is far from efficient. Writes are even worse
since you would need to read and rewrite the entire file, even to change just one
byte of data.
Random-access files can be read from or written to or both from a particular byte
position in the file. A single random-access file can be both read and written. The
position in the file where reads and writes start from is indicated by an integer called
the file pointer. Each read or write advances the file pointer by the number of bytes
read or written. Furthermore, the programmer can reposition the file pointer at
different bytes in the file without closing the file.
In Java, random file access is performed through the java.io.RandomAccessFile class.
This is not a subclass of java.io.File:
public class RandomAccessFile extends Object implements DataInput,
DataOutput
Among other differences between File objects and RandomAccessFile objects, the
RandomAccessFile constructors actually open the file in question and throw an IOException
if it doesn't exist:
public RandomAccessFile(String filename, String mode) throws
FileNotFoundException
public RandomAccessFile(File file, String mode) throws IOException
The first argument to the constructor is the file you want to access. The second
argument is the mode for access. The mode can be "r" for read-only access or "rw",
"rws", or "rwd" for read/write access. Java does not support write-only access. For
example:
RandomAccessFile raf = new RandomAccessFile("29.html", "r");
The rw mode is regular buffered read-write access. Changes may not be immediately
written to the file. This can lose data in the event of a system crash. In rws mode,
Java writes all data to the disk immediately and is safer if slower. In rwd mode, Java
writes all content immediately but may buffer changes to the file's metadata (its
name, permissions, and so on).
An IllegalArgumentException is thrown if anything other than these four strings is
specified as the mode. (In Java 1.3 and earlier, only "rw" and "r" were allowed.) A
security exception is thrown if the security manager does not allow the requested
file to be read. A security exception is also thrown if you request read/write access,
but only read access is allowed. Security checks are made only when the object is
constructed. It is assumed that the security manager's policy won't change while the
program is running. Finally, an IOException is thrown if the operating system doesn't
allow the file to be accessed or some other I/O problem occurs.
The getFilePointer( ) and seek( ) methods allow you to query and change the position
in the file at which reads and writes occur. Attempts to seek (position the file
pointer) past the end of the file just move the file pointer to the end of the file.
Attempts to write from the end of the file extend the file.
public native long getFilePointer( ) throws IOException
public native void seek(long pos) throws IOException
Attempts to read from the end of the file throw an EOFException (a subclass of
IOException). You can determine the length of the file with the length( ) method:
public native long length( ) throws IOException
The RandomAccessFile class implements both the DataInput and DataOutput interfaces.
Therefore, reads and writes use methods exactly like the methods of the
DataInputStream and DataOutputStream classes, such as read( ), readFully( ), readInt( ),
writeInt( ), readBoolean( ), writeBoolean( ), and so on.
Finally, there are a few miscellaneous methods. The getFD( ) method simply returns
the file descriptor for this file:
public final FileDescriptor getFD( ) throws IOException
The skipBytes( ) method attempts to reposition the file pointer n bytes further in the
file from where it is now. It returns the number of bytes actually skipped, which may
be less than n:
public int skipBytes(int n) tHRows IOException
The seek( ) method jumps to an absolute position in the file starting from 0, whereas
skipBytes( ) moves n bytes past wherever the file pointer is now:
public void seek(long position) throws IOException
Finally, the close( ) method closes the file:
public native void close( ) throws IOException
Once the file is closed, it may not be read from, though a new RandomAccessFile object
that refers to the same file can be created.
17.8. General Techniques for Cross-Platform File
Access Code
It's hard to write truly cross-platform file manipulation code. The NIO working group
was supposed to fix this years ago. However, they spent so much time on channels
and buffers that they never fulfilled their mandate to design a decent, platformindependent filesystem API. Now it looks like it's not going to make it into Java 6
either. Maybe we'll finally get one in Java 7. In the meantime, to help you achieve
greater serenity and overall cross-platform nirvana, I've summarized some basic
rules from this chapter to help you write file manipulation code that's robust across a
multitude of platforms:
Never, never, never hardcode pathnames in your application.
Ask the user to name your files. If you must provide a name for a file, try to
make it fit in an 8.3 DOS filename with only pure ASCII characters.
Do not assume the file separator is "/" (or anything else). Use File.separatorChar
instead.
Do not parse pathnames to find directories. Use the methods of the java.io.File
class instead.
Do not use renameTo( ) for anything except renaming a file. In particular, do not
use it to move a file.
Try to avoid moving and copying files from within Java programs if at all
possible.
Do not use . to refer to the current directory. Use System.getProperty("user.dir")
instead.
Do not use .. to refer to the parent directory. Use getParent( ) instead.
Place any data files your program requires in JAR archives rather than directly
in the filesystem and load them as resources from the classpath.
When in doubt, it never hurts to convert filenames to canonical form.
Do not assume anything about filesystem conventions. Some platform
somewhere will surprise you. (Have you tested your program on BeOS yet?)
Test your code on as many different filesystems as you can get your hands on.
Despite all the problems I've pointed out, it is possible to write robust file access
code that works across all platforms where Java runs, but doing so requires
understanding, effort, and thought. You cannot simply write for Windows or Unix and
hope things will work out for the best on other platforms. You must plan to handle a
wide range of filesystems and filename conventions.


