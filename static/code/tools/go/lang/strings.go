package main

import "fmt"

// a string is represented as a sequence of bytes and several library functions as well as the for-range loop assume that a string is composed of a sequence of UTF-8-encoded code points

func main() {

	// string literals are written in UTF-8 (unless you use hexadecimal escapes)
	var s string = "hello 🦆"
	fmt.Println(s)  // hello

	// the built-in len() function returns the length of the string in bytes
	fmt.Println(s)


	// we can extract a single byte from a string using an index expression
	var b byte = s[1]
	fmt.Println(b)  //

	// slice expression notation also works with strings
	fmt.Println(s[2:5])
	fmt.Println(s[:4])
	fmt.Println(s[3:])

	// we need to be careful as a UTF-8 code point can be anywhere from one to four bytes long
	fmt.Println(s[6])
	fmt.Println(s[7])
	fmt.Println(s[8])
	fmt.Println(s[9])


	// a single rune or byte can be converted to a string
	var x rune = 'x'
	fmt.Println(string(x))

	var y byte = 'y'
	fmt.Println(string(y))

	// though we must be wary of trying to convert an int to a string
	var z int = 67
	fmt.Println(string(z))


	// a string can be converted back and forth to a slice of bytes or a slice of runes
	fmt.Println([]byte(s))
	fmt.Println([]rune(s))
}