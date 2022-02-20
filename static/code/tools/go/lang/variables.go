package main

import "fmt"

// variables, constants, types, and functions declared outside of any functions are placed in the package block
var global int

func main() {
	// a variable that is declared without being assigned a value will be assigned the suitable zero-value
	var flag bool

	// every declared local variable must be read though unread package-level variables are allowed
	fmt.Println(flag) // false

	// a variable can be assigned a value when it is declared
	var count int = 1
	fmt.Println(count) // 1


	// we can declare multiple variables in one line
	var x, y int
	fmt.Println(x) // 0
	fmt.Println(y) // 0

	var pi, e float64 = 3.14, 2.71
	fmt.Println(pi) // 3.14
	fmt.Println(e)  // 2.71


	// types can be inferred from initialization
	var limit = 8
	fmt.Println(limit) // 8

	var n, msg = 5, "hello"
	fmt.Println(n)   // 5
	fmt.Println(msg) // hello


	// a shorthand for variable initialization can be used in function bodies
	i := 2
	fmt.Println(i) // 2

	result, ok := 7, true
	fmt.Println(result) // 7
	fmt.Println(ok)     // true

	// the shorthand form allows you to assign values to existing variables as long as there is one new variable being declared
	status := "none"
	output, status := 2, "success"
	fmt.Println(output) // 2
	fmt.Println(status) // success


	// factored variable initialization allows multiple variables to be declared
	var (
		valid    = true
		size     int
		hits, ch = 9, 'a'
	)
	fmt.Println(valid) // true
	fmt.Println(size)  // 0
	fmt.Println(hits)  // 9
	fmt.Println(ch)    // a


	// a shadowing variable has the same name as a variable in a containing block
	s := 3
	if s > 2 {
		// you cannot access a shadowed variable for as long as the shadowing variable exists
		s := 1
		s = 2
		fmt.Println(s) // 2
	}
	fmt.Println(s) // 3

	// the shorthand form only reuses variables declared in the current block and so can shadow variables in an outer block
	t := 3
	if t > 2 {
		t, u := 1, 4
		t = 2
		fmt.Println(t)
		fmt.Println(u)
	}
	fmt.Println(q)  // 3
}
