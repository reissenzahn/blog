package main

import "fmt"

func main() {

	// boolean (true or false)
	var t bool = true

	// signed integers
	var i8 int8 = 1
	var i16 int16 = 1
	var i32 int32 = 1
	var i64 int64 = 1
	
	// unsigned integers
	var u8 uint8 = 1
	var u16 uint16 = 1
	var u32 uint32 = 1
	var u64 uint64 = 1
	
	// floating point
	var f float32 = 3.14
	var d float64 = 2.71
	
	// complex numbers
	var u complex64 = complex(1, 2)
	var v complex128 = complex(3, 4)

	fmt.Println(u)
	fmt.Println(v)
	
	// unicode characters (alias for int32)
	var r rune = 'a'

	// strings
	var s string = "string"
	

	// a byte is an alias for uint8
	var b byte = 42

	// int is an alias for int32 on a 32-bit machine and an alias for int64 on a 64-bit machine
	var i int = 42

	// uint is similar to int except it is unsigned
	var u uint = 42

	// it is a compile-time error to assign, compare, or perform mathematical operations between an int and an int32 or int64 without a type conversion


	// a default zero value is assigned to any variable that is declared but not assigned a value (false zero bool, 0 for integers, 0.0 for floats, 0 + 0i for complex numbers, "" for strings)
	var zero int
	fmt.Println(int)  // 0


	// integer literals can be specified in binary, octal or hexadecimal
	var bin int = 0b10100110
	var hex int = 0xA6
	var oct int = 0o246

	// underscores can be used in integer literals for readability
	var million int = 1_000_000

	// floating point literals can use exponential notation
	var avogadro float64 = 6.03e23


}












Floating point literals have a default type of float64.
Integer literals default to being of int type.


There are two other special names for integer types, rune and uintptr.

Strings in Go are immutable.












Rune literals represent characters and are surrounded by single quotes

newline ('\n'), tab ('\t'), single quote ('\''), double quote ('\"'), and backslash ('\\').


There are two different ways to indicate string literals. Most of the time, you should
use double quotes to create an interpreted string literal

If you need to include backslashes, double quotes, or newlines in your string, use a
raw string literal. These are delimited with backquotes (`) and can contain any literal
character except a backquote. When using a raw string literal, we write our multiline
greeting like so:

// `Greetings and
// "Salutations"`


literals are untyped and they can interact with any variable that’s compatible with the literal

we can even use literals with user-defined types based on primitive types

Being untyped only goes so far; you can’t assign a literal
string to a variable with a numeric type or a literal number to a string variable, nor
can you assign a float literal to an int. These are all flagged by the compiler as errors.

it is a compile-time error to try to assign a literal whose value overflows the specified variable, such as
trying to assign the literal 1000 to a variable of type byte.

As you will see in the section on variable assignment, there are situations in Go where
the type isn’t explicitly declared. In those cases, Go uses the default type for a literal; if
there’s nothing in the expression that makes clear what the type of the literal is, the
literal defaults to a type. We will mention the default type for literals as we look at the
different built-in types.


