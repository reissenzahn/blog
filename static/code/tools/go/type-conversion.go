

// there is no automatic type promotion that allows numeric types to be automatically converted as needed

a type conversion when variable types do not match.

Even different-sized integers and floats must be converted to the same type to interact

var x int = 10
var y float64 = 30.2
var z float64 = float64(x) + y
var d int = x + int(y)


you cannot treat another type as a boolean

no other type can be implicitly or explicitly converted to a bool

, . If you want to convert
from another data type to boolean, you must use one of the comparison operators
(==, !=, >, <, <=, or >=). For example, to check if variable x is equal to 0, the code
would be x == 0. If you want to check if string s is empty, use s == "".






Go does allow you to perform a type conversion from one struct type to
another if the fields of both structs have the same names, order, and types. Let’s see what
this means. Given this struct:

type firstPerson struct {
 name string
 age int
}
We can use a type conversion to convert an instance of firstPerson to
secondPerson, but we can’t use == to compare an instance of firstPerson and an
instance of secondPerson, because they are different types:
type secondPerson struct {
 name string
 age int
}
We can’t convert an instance of firstPerson to thirdPerson, because the fields are in
a different order:
type thirdPerson struct {
 age int
 name string
}
We can’t convert an instance of firstPerson to fourthPerson because the field
names don’t match:
type fourthPerson struct {
 firstName string
 age int
}
Structs | 59
Finally, we can’t convert an instance of firstPerson to fifthPerson because there’s
an additional field:
type fifthPerson struct {
 name string
 age int
 favoriteColor string
}
Anonymous structs add a small twist to this: if two struct variables are being com‐
pared and at least one of them has a type that’s an anonymous struct, you can com‐
pare them without a type conversion if the fields of both structs have the same names,
order, and types. You can also assign between named and anonymous struct types if
the fields of both structs have the same names, order, and types:
type firstPerson struct {
 name string
 age int
}
f := firstPerson{
 name: "Bob",
 age: 50,
}
var g struct {
 name string
 age int
}
// compiles -- can use = and == between identical named and anonymous structs
g = f
fmt.Println(f == g)