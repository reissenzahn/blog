package main

import "fmt"

func here() {
	i := 2

	// defer delays the invocation of a function until the surrounding function exits
	defer there(1)

	// you can defer multiple closures in which case they run in last-in-first-out order (the last defer registered runs first)
	defer there(i)

	// any variables passed into a deferred closure are not evaluated until the closure runs
	defer func() {
		fmt.Println("there!", i)
	}()

	fmt.Println("here!")
	i = 3

	// the code within defer closures runs after the return statement
	return
}

func there(i int) {
	fmt.Println("there!", i)
}

func main() {
	here()

	// here!
	// there! 3
	// there! 2
	// there! 1
}
