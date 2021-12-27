package main

// a slice is a contiguous sequence of values

func main() {


	// the type of a slice is defined by the type of the elements it contains and so the length of a slice is not part of its type


	// a slice literal can be used to create a slice
	primes := []rune{2, 3, 5, 6}

	primes[3] = 7
	fmt.Println(primes[3])

	// the built-in len() function returns the length of a slice
	fmt.Println(len(primes))


	// we can specify the values at particular indices
	counts := []int{1, 5: 4, 6, 10: 7, 9}


	// a nil slice contains no elements
	var empty []string
	
	// slices cannot be compared using the == and != operators though we can check if a slice is nil
	fmt.Println(empty == nil)


	// the built-in append() function takes a slice and some values, makes a copy of the slice, adds the values to the copy of the slice and returns the copy
	
	fmt.Println(append(primes, 11))
	fmt.Println(append(primes, 15, 17, 19))

	// 




	// the built-in copy() function 
}









// slices are typed only by the elements they contain
s := make([]string, 3)

s[0] = "a"
s[1] = "b"
s[2] = "c"

fmt.Println(s[2])    // c
fmt.Println(len(s))  // 3

s = append(s, "d")
s = append(s, "e", "f")
fmt.Println(s)  // [a b c d e f]

// slice literal
s := []string{"a", "b", "c", "d", "e", "f"}

fmt.Println(s)       // [a b c d e f]
fmt.Println(s[2:5])  // [c d e]
fmt.Println(s[:5])   // [a b c d e]
fmt.Println(s[2:])   // [c d e f]

// slices describe a section of an underlying array
s := [4]string{"a", "b", "c", "d"}

a := s[0:2]
b := s[1:3]

b[0] = "x"

fmt.Println(a)  // [a x]
fmt.Println(b)  // [x c]
fmt.Println(s)  // [a x c d]






We can write a single function that processes slices of any size and we can grow slices as needed.



You can simulate multidimensional slices and make a slice of slices:
var x [][]int

// the zero-value for a slice is nil
var x []int


A slice is the first type we’ve seen that isn’t comparable. It is a compile-time error to use == to see if two slices are identical or != to see if they are different. The only thing
you can compare a slice with is nil:

fmt.Println(x == nil) // prints true



// a slice is appended to another slice by expanding it into individual values

y := []int{20, 30, 40}
x = append(x, y...)



Every slice has a capacity, which is the number of consecutive memory locations reserved. This can be larger than the length. Each time you append to a slice, one or
more values is added to the end of the slice. Each value added increases the length by
one. When the length reaches the capacity, there’s no more room to put values. If you
try to add additional values when the length equals the capacity, the append function
uses the Go runtime to allocate a new slice with a larger capacity. The values in the
original slice are copied to the new slice, the new values are added to the end, and the
new slice is returned.


the built-in cap function returns the current capacity of a slice

var x []int
fmt.Println(x, len(x), cap(x))
x = append(x, 10)
fmt.Println(x, len(x), cap(x))
x = append(x, 20)
fmt.Println(x, len(x), cap(x))
x = append(x, 30)
fmt.Println(x, len(x), cap(x))
x = append(x, 40)
fmt.Println(x, len(x), cap(x))
x = append(x, 50)
fmt.Println(x, len(x), cap(x))


If you know how many things you plan to put into a slice, create the slice with the correct initial capacity. We do that with the make function.

It allows us to
specify the type, length, and, optionally, the capacity.

x := make([]int, 5)

This creates an int slice with a length of 5 and a capacity of 5. Since it has a length of 5, x[0] through x[4] are valid elements, and they are all initialized to 0.

We can also specify an initial capacity with make:

x := make([]int, 5, 10)

You can also create a slice with zero length, but a capacity that’s greater than zero:

x := make([]int, 0, 10)

In this case, we have a non-nil slice with a length of 0, but a capacity of 10. Since the
length is 0, we can’t directly index into it, but we can append values to it:

x := make([]int, 0, 10)
x = append(x, 5,6,7,8)

The value of x is now [5 6 7 8], with a length of 4 and a capacity of 10.


If you use a variable to specify a capacity that’s smaller than the length, your program will panic at runtime.



// slice expressions

A slice expression creates a slice from a slice.

If you leave off the starting offset, 0 is assumed. Likewise, if you leave off the ending offset, the end of the
slice is substituted.


x := []int{1, 2, 3, 4}
y := x[:2]
z := x[1:]
d := x[1:3]
e := x[:]
fmt.Println("x:", x)
fmt.Println("y:", y)
fmt.Println("z:", z)
fmt.Println("d:", d)
fmt.Println("e:", e)


When you take a slice from a slice, you are not making a copy of the data. Instead, you now have two variables that are sharing memory. This means that changes to an
element in a slice affect all slices that share that element. 

x := []int{1, 2, 3, 4}
y := x[:2]
z := x[1:]
x[1] = 20
y[0] = 10
z[1] = 30

fmt.Println("x:", x)
fmt.Println("y:", y)
fmt.Println("z:", z)

Slicing slices gets extra confusing when combined with append.
x := []int{1, 2, 3, 4}
y := x[:2]
fmt.Println(cap(x), cap(y))
y = append(y, 30)
fmt.Println("x:", x)
fmt.Println("y:", y)

4 4
x: [1 2 30 4]
y: [1 2 30]

What’s going on? Whenever you take a slice from another slice, the subslice’s capacity
is set to the capacity of the original slice, minus the offset of the subslice within the
original slice. This means that any unused capacity in the original slice is also shared
with any subslices.
When we make the y slice from x, the length is set to 2, but the capacity is set to 4, the
same as x. Since the capacity is 4, appending onto the end of y puts the value in the
third position of x.
This behavior creates some very odd scenarios, with multiple slices appending and
overwriting each other’s data. See if you can guess what the code in Example 3-7
prints out, then run it on The Go Playground to see if you guessed correctly.
Example 3-7. Even more confusing slices
x := make([]int, 0, 5)
x = append(x, 1, 2, 3, 4)
y := x[:2]
z := x[2:]
fmt.Println(cap(x), cap(y), cap(z))
y = append(y, 30, 40, 50)
x = append(x, 60)
z = append(z, 70)
fmt.Println("x:", x)
fmt.Println("y:", y)
fmt.Println("z:", z)

To avoid complicated slice situations, you should either never use append with a sub‐
slice or make sure that append doesn’t cause an overwrite by using a full slice expres‐
sion. This is a little weird, but it makes clear how much memory is shared between
the parent slice and the subslice. The full slice expression includes a third part, which
indicates the last position in the parent slice’s capacity that’s available for the subslice.
Subtract the starting offset from this number to get the subslice’s capacity.
Example 3-8 shows lines three and four from the previous example, modified to use
full slice expressions.
Example 3-8. The full slice expression protects against append
y := x[:2:2]
z := x[2:4:4]
Slices | 45
You can try out this code on The Go Playground. Both y and z have a capacity of 2.
Because we limited the capacity of the subslices to their lengths, appending additional
elements onto y and z created new slices that didn’t interact with the other slices.
After this code runs, x is set to [1 2 3 4 60], y is set to [1 2 30 40 50], and z is set to [3
4 70].
Be very careful when taking a slice of a slice! Both slices share the
same memory and changes to one are reflected in the other. Avoid
modifying slices after they have been sliced or if they were pro‐
duced by slicing. Use a three-part slice expression to prevent
append from sharing capacity between slices.
Converting Arrays to Slices
Slices aren’t the only thing you can slice. If you have an array, you can take a slice
from it using a slice expression. This is a useful way to bridge an array to a function
that only takes slices. However, be aware that taking a slice from an array has the
same memory-sharing properties as taking a slice from a slice. If you run the follow‐
ing code on The Go Playground:
x := [4]int{5, 6, 7, 8}
y := x[:2]
z := x[2:]
x[0] = 10
fmt.Println("x:", x)
fmt.Println("y:", y)
fmt.Println("z:", z)
you get the output:
x: [10 6 7 8]
y: [10 6]
z: [7 8]



// copy

If you need to create a slice that’s independent of the original, use the built-in copy function

x := []int{1, 2, 3, 4}
y := make([]int, 4)
num := copy(y, x)
fmt.Println(y, num)

The copy function takes two parameters. The first is the destination slice and the second is the source slice. It copies as many values as it can from source to destination,
limited by whichever slice is smaller, and returns the number of elements copied. The
capacity of x and y doesn’t matter; it’s the length that’s important.

You don’t need to copy an entire slice. The following code copies the first two elements of a four-element slice into a two-element slice

x := []int{1, 2, 3, 4}
y := make([]int, 2)
num = copy(y, x)
The variable y is set to [1 2] and num is set to 2.
You could also copy from the middle of the source slice:
x := []int{1, 2, 3, 4}
y := make([]int, 2)
copy(y, x[2:])
We are copying the third and fourth elements in x by taking a slice of the slice. Also
note that we don’t assign the output of copy to a variable. If you don’t need the num‐
ber of elements copied, you don’t need to assign it.
The copy function allows you to copy between two slices that cover overlapping sec‐
tions of an underlying slice:
x := []int{1, 2, 3, 4}
num = copy(x[:3], x[1:])
fmt.Println(x, num)
In this case, we are copying the last three values in x on top of the first three values of
x. This prints out [2 3 4 4] 3.
You can use copy with arrays by taking a slice of the array. You can make the array
either the source or the destination of the copy. You can try out the following code on
The Go Playground:
x := []int{1, 2, 3, 4}
d := [4]int{5, 6, 7, 8}
y := make([]int, 2)
copy(y, d[:])
fmt.Println(y)
copy(d[:], x)
fmt.Println(d)
The first call to copy copies the first two values in array d into slice y. The second
copies all of the values in slice x into array d. This produces the output:
[5 6]
[1 2 3 4]

