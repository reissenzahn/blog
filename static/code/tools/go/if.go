package main

import (
	"fmt"
	"math/rand"
)

func main() {
	// if-else statement
	i := rand.Intn(10)
	if i > 5 {
		fmt.Println("too big")
	} else if i == 5 {
		fmt.Println("perfect!")
	} else {
		fmt.Println("too small")
	}

	// we can declare variables that are scoped to the condition and to the if and else blocks
	if i := rand.Intn(10); i%2 == 0 {
		fmt.Printf("%d is even\n", i)
	} else {
		fmt.Printf("%d is odd\n", i)
	}
}