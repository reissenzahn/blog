package main

import "fmt"

func main() {
	// condition-only for statement
	i := 0
	for i < 5 {
		fmt.Println(i) // 0 1 2 3 4
		i += 1
	}

	// traditional for statement
	for i := 0; i < 5; i++ {
		if i%2 != 0 {

			// continue skips to the next iteration
			continue
		}

		fmt.Println(i) // 0 2 4
	}

	// infinite for statement
	for {
		fmt.Println("again!") // again!

		// break exits the loop immediately
		break
	}

	// a for statement can be labeled to allow a break or continue to apply to apply to the outer loop
escape:
	for i := 0; i < 5; i++ {
		fmt.Println("outer")
		for j := 0; j < 5; j++ {
			fmt.Println("inner")
			break escape
		}
	}
	fmt.Println("escaped!")
}