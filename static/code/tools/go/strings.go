

a string 


e talked about slices, we can go back and look at strings again. You
might think that a string in Go is made out of runes, but that’s not the case. Under the
covers, Go uses a sequence of bytes to represent a string. These bytes don’t have to be
in any particular character encoding, but several Go library functions (and the forrange loop that we discuss in the next chapter) assume that a string is composed of a
sequence of UTF-8-encoded code points.

According to the language specification, Go source code is always
written in UTF-8. Unless you use hexadecimal escapes in a string
literal, your string literals are written in UTF-8.
Just like you can extract a single value from an array or a slice, you can extract a sin‐
gle value from a string by using an index expression:
var s string = "Hello there"
var b byte = s[6]
Like arrays and slices, string indexes are zero-based; in this example, b is assigned the
value of the seventh value in s, which is t.
The slice expression notation that we used with arrays and slices also works with
strings:
var s string = "Hello there"
var s2 string = s[4:7]
var s3 string = s[:5]
var s4 string = s[6:]
This assigns “o t” to s2, “Hello” to s3, and “there” to s4.
While it’s handy that Go allows us to use slicing notation to make substrings and use
index notation to extract individual entries from a string, you should be very careful
when doing so. Since strings are immutable, they don’t have the modification prob‐
lems that slices of slices do. There is a different problem, though. A string is com‐
posed of a sequence of bytes, while a code point in UTF-8 can be anywhere from one
to four bytes long. Our previous example was entirely composed of code points that
are one byte long in UTF-8, so everything worked out as expected. But when dealing
with languages other than English or with emojis, you run into code points that are
multiple bytes long in UTF-8:
var s string = "Hello "
var s2 string = s[4:7]
var s3 string = s[:5]
var s4 string = s[6:]
48 | Chapter 3: Composite Types
In this example, s3 will still be equal to “Hello.” The variable s4 is set to the sun emoji.
But s2 is not set to “o .” Instead, you get “o .” That’s because we only copied the
first byte of the sun emoji’s code point, which is invalid.
Go allows you to pass a string to the built-in len function to find the length of the
string. Given that string index and slice expressions count positions in bytes, it’s not
surprising that the length returned is the length in bytes, not in code points:
var s string = "Hello "
fmt.Println(len(s))
This code prints out 10, not 7, because it takes four bytes to represent the sun with
smiling face emoji in UTF-8.
Even though Go allows you to use slicing and indexing syntax with
strings, you should only use it when you know that your string only
contains characters that take up one byte.
Because of this complicated relationship between runes, strings, and bytes, Go has
some interesting type conversions between these types. A single rune or byte can be
converted to a string:
var a rune = 'x'
var s string = string(a)
var b byte = 'y'
var s2 string = string(b)
A common bug for new Go developers is to try to make an int into
a string by using a type conversion:
var x int = 65
var y = string(x)
fmt.Println(y)
This results in y having the value “A,” not “65.” As of Go 1.15, go
vet blocks a type conversion to string from any integer type other
than rune or byte.
A string can be converted back and forth to a slice of bytes or a slice of runes. Try
Example 3-9 out on The Go Playground.
Example 3-9. Converting strings to slices
var s string = "Hello, "
var bs []byte = []byte(s)
var rs []rune = []rune(s)
Strings and Runes and Bytes | 49
fmt.Println(bs)
fmt.Println(rs)
When you run this code, you see:
[72 101 108 108 111 44 32 240 159 140 158]
[72 101 108 108 111 44 32 127774]
The first output line has the string converted to UTF-8 bytes. The second has the
string converted to runes.
Most data in Go is read and written as a sequence of bytes, so the most common
string type conversions are back and forth with a slice of bytes. Slices of runes are
uncommon.



UTF-8

UTF-8 lets you use a single byte to represent the Unicode characters whose values are below 128 (which includes all of the letters, numbers, and punctuation commonly used in English), but expands to a maximum of four bytes to represent Unicode code points with larger values. The result is that the worst case for
UTF-8 is the same as using UTF-32.

You also don’t have to worry about little-endian versus big-endian. It also allows you to look at any byte in a sequence and tell if you are at the start of a
UTF-8 sequence, or somewhere in the middle.

The only downside is that you cannot randomly access a string encoded with UTF-8. While you can detect if you are in the middle of a character, you can’t tell how many characters in you are. You need to start at the beginning of the string and count.

Go doesn’t require a string to be written in UTF-8, but it strongly encourages it.

Rather than use the slice and index expressions with strings, you should extract substrings and code points from strings using the functions in the strings and unicode/utf8 packages in the standard library.

