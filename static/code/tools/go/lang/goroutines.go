package main

import "fmt"

func count(j int) {
	for i := 0; i < j; i++ {
		fmt.Println(i)
	}
}

func main() {
	// a goroutine is a function that runs concurrently
	go count(5)

	// we can create a goroutine from an anonymous function
	go func(j int) {
		for i := 0; i < j; i++ {
			fmt.Println(i)
		}
	}(5)

	time.Sleep(time.Second)
}