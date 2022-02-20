package main

import (
	"fmt"
)

// generics

// accept a slice of any data type
func display[T any](s []T) {
	for _, v := range s {
		fmt.Print(v, " ")
	}

	fmt.Println()
}


// The [T any] part tells the compiler that the data type T is going to be determined at execution time.

// We are also free to use multiple data types using the [T, U, W any] notation.

// The any keyword tells the compiler that there are no constraints about the data type of T.

func main() {
	display(int[]{1, 2, 3})
	display(string[]{"a", "b", "c"})
	display([]float64{1.2, -2.33, 4.55})


}


// Go 1.18 is going to come with
// predefined constraints—one of them is called comparable and includes data types
// that can be compared for equality or inequality. 


// the comparable constraint is just a predefined interface that includes all data types that can be compared with == or !=

func same[T comparable](a, b T) bool {
	if a == b {
		return true
	}

	return false
 }


fmt.Println("4 = 3 is", Same(4,3))
fmt.Println("aa = aa is", Same("aa","aa"))
fmt.Println("4.1 = 4.15 is", Same(4.1,4.15))
fmt.Println(Same([]int{1,2},[]int{1,3}))


we can define the data types that are allowed to be passed as parameters to a generic function using an interface

type Numeric interface {
 type int, int8, int16, int32, int64, float64
}

In here, we define a new interface called Numeric that specifies the list of supported data types. You can use any data type you want as long as it can be used with the generic function that you are going to implement. In this case, we could have added
string or uint to the list of supported data types.

This is the definition of the generic function that uses the Numeric constraint.

func Add[T Numeric](a, b T) T {
 return a + b
}

fmt.Println("4 + 3 =", Add(4,3))
fmt.Println("4.1 + 3.2 =", Add(4.1,3.2))




// declare a new data type named TreeLast that uses generics.
type TreeLast[T any] []T

func (t TreeLast[T]) replaceLast(element T) (TreeLast[T], error) {
 if len(t) == 0 {
 return t, errors.New("This is empty!")
 }

 t[len(t) - 1] = element
 return t, nil
}

tempStr := TreeLast[string]{"aa", "bb"}

fmt.Println(tempStr)
tempStr.replaceLast("cc")
fmt.Println(tempStr)

tempInt := TreeLast[int]{12, -3}
fmt.Println(tempInt)
tempInt.replaceLast(0)
fmt.Println(tempInt)


type node[T any] struct {
	Data T
	next *node[T]
 }

 type list[T any] struct {
	start *node[T]
 }

func (l *list[T]) add(data T) {
	n := node[T]{
	Data: data,
	next: nil,
	}
 The add() function is generic in order to be able to work with all kinds of nodes.
 Apart from the signature of add(), all the remaining code is not associated with the
 use of generics.
	if l.start == nil {
	l.start = &n
	return
	}
 
	if l.start.next == nil {
	l.start.next = &n
	return
	}
 Go Generics
 [ 624 ]
 These two if blocks have to do with the adding of a new node to the linked list.
	temp := l.start
	l.start = l.start.next
	l.add(data)
	l.start = temp
 }
 The last part of add() has to do with defining the proper associations between nodes
 when adding a new node to the list.
 func main() {
	var myList list[int]
 First, we define a linked list of int values in main(), which is the linked list that we
 are going to work with.
	fmt.Println(myList)
 The initial value of myList is nil, as the list is empty and does not contain any nodes.
	myList.add(12)
	myList.add(9)
	myList.add(3)
	myList.add(9)
 In this first part, we add four elements to the linked list.
	// Print all elements
	for {
	fmt.Println("*", myList.start)
	if myList.start == nil {
	break
	}
	myList.start = myList.start.next
	}
 }
 The last part of main() is about printing all the elements of the list by traversing it
 with the help of the next field, which points to the next node in the list.
 