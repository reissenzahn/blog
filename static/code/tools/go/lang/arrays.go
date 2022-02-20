package main

import "fmt"

func main() {
	// an array is a fixed-size contiguous sequence of elements of the same type

	// array elements are assigned the suitable zero-value if initial values are not specified
	var counts [3]int
	fmt.Println(counts)       // [0 0 0]

	// arrays are read and written using bracket syntax
	counts[0] = 1
	counts[1] = 2
	counts[2] = 3
	fmt.Println(counts[1])  // 2

	// the built-in len() function returns the length of an array
	fmt.Println(len(counts))  // 3


	// an out-of-bounds read or write at runtime will panic
	n := 5
	fmt.Println(counts[n])  // 2


	// an array literal can be used to provide initial value for the array in which case we can omit the length of the array
	var letters = [...]rune{'a', 'b', 'c'}
	fmt.Println(letters)

	// an array literal can specify the indices of array elements
	var sparse = [10]int{1, 3: 4, 7, 8: 9, 6}
	fmt.Println(sparse)

	// we can declare multidimensional arrays
	var matrix [2][3]int

	for i := 0; i < 2; i++ {
		for j := 0; j < 3; j++ {
			matrix[i][j] = i + j
		}
	}

	fmt.Println(matrix)

	// the length of an array is part of the type of the array and so you cannot use a variable to specify the size of an array as types must be resolved at compile time
}