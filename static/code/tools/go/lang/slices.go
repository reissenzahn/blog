package main

// a slice is a contiguous sequence of values whose type is defined by the type of the elements it contains (and not its length)

func main() {

	// a slice literal can be used to create a slice
	primes := []rune{2, 3, 5, 6}

	primes[3] = 7
	fmt.Println(primes[3])

	// the built-in len() function returns the length of a slice
	fmt.Println(len(primes))

	// we can specify the values at particular indices
	counts := []int{1, 5: 4, 6, 10: 7, 9}
	fmt.Println(counts)


	// a nil slice contains no elements
	var empty []string
	
	// slices cannot be compared using the == and != operators though we can check if a slice is nil
	fmt.Println(empty == nil)


	// the built-in append() function takes a slice and some values, makes a copy of the slice, adds the values to the copy of the slice and returns the copy
	fmt.Println(append(primes, 11))
	fmt.Println(append(primes, 15, 17, 19))

	// a slice is appended to another slice by expanding it into individual values
	fmt.Println(append(primes, []int{15, 17, 19}...))


	// a slice has an underlying capacity which allows the slice to increase in length until the length exceeds the capacity in which case the runtime allocates a new slice with a larger capacity and copies the original slice into the new slice
	var evens []int
	fmt.Println(evens, len(evens), cap(evens))
	evens = append(evens, 2)
	fmt.Println(evens, len(evens), cap(evens))
	evens = append(evens, 4)
	fmt.Println(evens, len(evens), cap(evens))
	evens = append(evens, 6)
	fmt.Println(evens, len(evens), cap(evens))
	evens = append(evens, 8)
	fmt.Println(evens, len(evens), cap(evens))
	evens = append(evens, 19)
	fmt.Println(evens, len(evens), cap(evens))


	// the built-in make() function can be used to create a slice of a given length
	letters := make([]rune, 3)

	fmt.Println(letters)

	letters[0] = 'A'
	letters[1] = 'B'
	letters[2] = 'C'

	fmt.Println(letters)

	// we can also specify an initial capacity
	// letters := make([]rune, 5, 10)


	// a slice expression creates a slice from a slice
	nums := []int{1, 2, 3, 4}

	fmt.Println(nums[:2])
	fmt.Println(nums[1:])
	fmt.Println(nums[1:3])
	fmt.Println(nums[:])

	// taking a slice from a slice does not make a copy but rather gives you two variables that are sharing memory
	odds := []int{1, 3, 5, 7}

	upper := odds[:2]
	lower := odds[1:]

	odds[1] = 20
	upper[0] = 10
	lower[1] = 30

	fmt.Println(odds)
	fmt.Println(upper)
	fmt.Println(lower)

	// we can also take a slice from an array with a slice expression (this has the same memory-sharing properties as taking a slice from a slice)
	x := [4]int{5, 6, 7, 8}
	y := x[:2]
	z := x[2:]
	x[0] = 10
	fmt.Println("x:", x)
	fmt.Println("y:", y)
	fmt.Println("z:", z)


	// the built-in copy() function copies as many values as it can from a source to destination slice (limited by whichever slice is smaller) and returns the number of elements copied
	src1 := []int{1, 2, 3, 4}

	dst1 := make([]int, 4)
	n := copy(dst1, src1)
	fmt.Println(dst1)
	fmt.Println(n)

	// we can copy only a portion of a slice by making the length of the destination slice smaller
	dst2 := make([]int, 2)
	copy(dst2, src1)
	fmt.Println(dst2)

	// using slice expressions we can copy from somewhere in the middle of a slice
	dst3 := make([]int, 2)
	copy(dst3, src1[2:])
	fmt.Println(dst3)

	// we are also able to copy between two slices that cover overlapping sections of an underlying slice
	copy(src1[:3], src1[1:])
	fmt.Println(src1)

	// you can use copy with arrays by taking a slice of the array
	src2 := [4]int{5, 6, 7, 8}
	dst3 := make([]int, 2)
	copy(dst3, d[:])
	fmt.Println(dst3)

	dst4 := [4]int{5, 6, 7, 8}
	copy(dst4[:], src2)
	fmt.Println(dst4)
}










