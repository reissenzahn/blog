package main

import "fmt"


type Weekday int

// declare Sunday to be 0, Monday to be 1, etc.
const (
  Sunday Weekday = iota
  Monday
  Tuesday
  Wednesday
  Thursday
  Friday
  Saturday
)

func main() {

	// constants are a way to give names to literals
	const pi = 3.14
	
	// factored constant initialization
	const (
		i = 1 >> 100
		j = i >> 99
	)
}




// 


const x int64 = 10

const (
 idKey = "id"
 nameKey = "name"
)

const z = 20 * 10

func main() {
 const y = "hello"
 fmt.Println(x)
 fmt.Println(y)
 x = x + 1  // error
 y = "bye"  // error
 fmt.Println(x)
 fmt.Println(y)
}


// there is no way to specify that a value calculated at runtime is immutable

// there is no wat to declare that a variable is immutable


Typed and Untyped Constants
Constants can be typed or untyped. An untyped constant works exactly like a literal;
it has no type of its own, but does have a default type that is used when no other type
can be inferred. A typed constant can only be directly assigned to a variable of that
type.
Whether or not to make a constant typed depends on why the constant was declared.
If you are giving a name to a mathematical constant that could be used with multiple
numeric types, then keep the constant untyped. In general, leaving a constant unty‐
ped gives you more flexibility. There are situations where you want a constant to
enforce a type. We’ll use typed constants when we look at creating enumerations with
iota in “iota Is for Enumerations—Sometimes” on page 137.
Here’s what an untyped constant declaration looks like:
const x = 10
All of the following assignments are legal:
var y int = x
var z float64 = x
var d byte = x
Here’s what a typed constant declaration looks like:
const typedX int = 10
This constant can only be assigned directly to an int. Assigning it to any other type
produces a compile-time error like this:
Typed and Untyped Constants | 31
cannot use typedX (type int) as type float64 in assignment


