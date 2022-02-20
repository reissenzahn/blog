package main

import "fmt"

func main() {

	// a goto statement causes execution to jump to a specified labeled line
	goto there
	fmt.Println("here")
there:
	fmt.Println("there")

	// a goto cannot skip over variable declarations or jump into an inner or parallel block
	// 	goto skip  // error
	// 	i := 1
	// skip:

	// 	if true {
	// 		goto block  // error
	// 	}

	// 	if false {
	// 	block:
	// 		fmt.Println(i)
	// 	}
}