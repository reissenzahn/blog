

Generics is the concept that it is sometimes useful to write functions or structs where the specific type of a parameter or field is specified when it is used.

Go uses interfaces to specify which types can be used. If any type is usable, this is specified with the new universe block identifier any, which is exactly equivalent to interface{}, but is only valid within a type constraint.

// type parameters 
type Stack[T any] struct {
 vals []T
}

func (s *Stack[T]) Push(val T) {
 s.vals = append(s.vals, val)
}

func (s *Stack[T]) Pop() (T, bool) {
 if len(s.vals) == 0 {
 var zero T
 return zero, false
 }
 top := s.vals[len(s.vals)-1]
 s.vals = s.vals[:len(s.vals)-1]
 return top, true
}


Just like we replaced interface{} with T
in our vars declaration, we do the same here. We also refer to the type in the receiver
section with Stack[T] instead of Stack.

Finally, generics make zero value handling a little interesting. In Pop, we can’t just
return nil, because that’s not a valid value for a value type, like int. The easiest way
to get a zero value for a generic is to simply declare a variable with var and return it,
since by definition, var always initializes its variable to the zero value if no other
value is assigned.

func main() {
 var s Stack[int]
 s.Push(10)
 s.Push(20)
 s.Push(30)
 v, ok := s.Pop()
 fmt.Println(v, ok)
}


The only difference is that when we declare our variable, we include the type that we
want to use with our Stack

Furthermore, if you try to
push a string onto our stack, the compiler will catch it. Adding the line:
s.Push("nope")


func (s Stack[T]) Contains(val T) bool {
 for _, v := range s.vals {
 if v == val {
 return true
 }
 }
 return false
}

Unfortunately, this does not compile. It gives the error:
cannot compare v == val (operator == not defined for T)
Just as interface{} doesn’t say anything, neither does any. We can only store values
of any type and retrieve them. 

To use ==, we need a different type. Since nearly all Go
types can be compared with == and !=, a new built-in interface called comparable is
defined in the universe block. If we change our definition of Stack to use
comparable:

type Stack[T comparable] struct {
 vals []T
}
we can then use our new method:
func main() {
 var s Stack[int]
 s.Push(10)
 s.Push(20)
 s.Push(30)
 fmt.Println(s.Contains(10))
 fmt.Println(s.Contains(5))
}
This prints out:
true
false


You can try out this updated stack.
Let’s go back to our binary tree and see about making it generic. We want to limit it to
types that implement Orderable, and we want to enforce type safety at compile time.
We do this by adding type parameters. First, let’s modify the Orderable interface:
type Orderable[T any] interface {
 Order(T) int
}
This declaration says that the Orderable interface defines a single type parameter
named T that can be any type. For a type to implement this interface, the input
parameter for the Order method must be of type T. We then change the Order
method for OrderableInt to be compatible with this new generic interface:
func (oi OrderableInt) Order(val OrderableInt) int {
 return int(oi - val)
}
This is simpler than our previous method definition, which had an input parameter
of type interface{} and required a type assertion. We make a similar change to
OrderableString:
func (os OrderableString) Order(val OrderableString) int {
 return strings.Compare(string(os), string(val))
}
And now we update our Tree definition to refer to our generic Orderable interface:
type Tree[T Orderable[T]] struct {
 val T
 left, right *Tree[T]
}
This definition might look a little complicated, but all it says is that our Tree struct
can only contain types that implement Orderable. We also update the Tree method
definitions. Insert is now:
func (t *Tree[T]) Insert(val T) *Tree[T]
and Contains is now:
func (t *Tree[T]) Contains(val T) bool
We also have to make a small change to the body of Insert. When we instantiate a
new Tree, we now use:
return &Tree[T]{val: val}
We declare our generic tree with a type parameter:
var it *Tree[OrderableInt]
Introducing Generics in Go | 331
While we can assign untyped constants without an explicit type conversion, assigning
an int variable requires a type conversion (because Go doesn’t convert types
automatically):
a := 10
it = it.Insert(OrderableInt(a))
Also, notice that if we pass an OrderableString to Insert, we now get a compiletime error:
cannot use OrderableString("nope") (constant "nope" of type OrderableString)
 as OrderableInt value in argument
You can look at our generic tree on The Go2Go Playground.
Use Type Lists to Specify Operators
While we can use our Tree with built-in types when there’s an associated type that
meets Orderable, it might be nice to use a Tree with built-in types that didn’t require
the wrappers. To do that, we need a way to specify that we can use the < operator. Go
generics do that with a type list, which is simply a list of types specified within an
interface:
type BuiltInOrdered interface {
 type string, int, int8, int16, int32, int64, float32, float64,
 uint, uint8, uint16, uint32, uint64, uintptr
}
When a type constraint is specified with an interface containing a type list, any listed
type can match. However, the allowed operators are the ones that apply for all of the
listed types. In this case, those are the operators ==, !=, >, <, >=, <=, and +. Let’s see
how this works with yet another version of Tree. First, we switch from Orderable to
BuiltInOrdered:
type Tree[T BuiltInOrdered] struct
Next, we change our switch statements in Insert and Contains back to the way they
looked in IntTree:
switch {
case val < t.val:
case val > t.val:
}
We can now use this Tree with the built-in int type (or any of the other types listed
in BuiltInOrdered):
func main() {
 var it *Tree[int]
 it = it.Insert(5)
 a := 10
332 | Chapter 15: A Look at the Future: Generics in Go
 it = it.Insert(a)
 // etc...
}
You can look at this generic tree variant on The Go2Go Playground.
Generic Functions Abstract Algorithms
We can also write generic functions. Earlier we mentioned that not having generics
made it difficult to write map, reduce, and filter implementations that work for all
types. Generics make it easy. Here are implementations from the type parameters
draft design:
// Map turns a []T1 to a []T2 using a mapping function.
// This function has two type parameters, T1 and T2.
// This works with slices of any type.
func Map[T1, T2 any](s []T1, f func(T1) T2) []T2 {
 r := make([]T2, len(s))
 for i, v := range s {
 r[i] = f(v)
 }
 return r
}
// Reduce reduces a []T1 to a single value using a reduction function.
func Reduce[T1, T2 any](s []T1, initializer T2, f func(T2, T1) T2) T2 {
 r := initializer
 for _, v := range s {
 r = f(r, v)
 }
 return r
}
// Filter filters values from a slice using a filter function.
// It returns a new slice with only the elements of s
// for which f returned true.
func Filter[T any](s []T, f func(T) bool) []T {
 var r []T
 for _, v := range s {
 if f(v) {
 r = append(r, v)
 }
 }
 return r
}
Functions place their type parameters after the function name and before the variable
parameters. Map and Reduce have two type parameters, both of any type, while Filter
has one. When we run the code:
Generic Functions Abstract Algorithms | 333
words := []string{"One", "Potato", "Two", "Potato"}
filtered := Filter(words, func(s string) bool {
 return s != "Potato"
})
fmt.Println(filtered)
lengths := Map(filtered, func(s string) int {
 return len(s)
})
fmt.Println(lengths)
sum := Reduce(lengths, 0, func(acc int, val int) int {
 return acc + val
})
fmt.Println(sum)
we get the output:
[One Two]
[3 3]
6
Try it for yourself.
Just as Go supports type inference when using the := operator, it also supports type
inference to simplify calls to generic functions. In some situations, type inference isn’t
possible (for example, when a type parameter is only used as a return value). When
that happens, all of the type arguments must be specified:
type Integer interface {
 type int, int8, int16, int32, int64, uint, uint8, uint16, uint32, uint64
}
func Convert[T1, T2 Integer](in T1) T2 {
 return T2(in)
}
func main() {
 var a int = 10
 b := Convert[int, int64](a)
 fmt.Println(b)
}
Type Lists Limit Constants and Implementations
Type lists also specify which constants can be assigned to variables of the generic
type. There are no constants that can be assigned to all of the listed types in
BuiltInOrdered, so you cannot assign a constant to a variable of that generic type. If
you use the Integer interface, the following code will not compile, because you can‐
not assign 1000 to an 8-bit integer:
// INVALID!
func PlusOneThousand[T Integer](in T) T {
334 | Chapter 15: A Look at the Future: Generics in Go
 return in + 1_000
}
However, this is valid:
// VALID
func PlusOneHundred[T Integer](in T) T {
 return in + 100
}
In addition to built-in types, type lists can also contain type literals for slices, maps,
arrays, channels, or structs. This is a bit contrived, but we can use this ability to write
a single function that can copy the values out of any map or slice:
type Rangeable[E comparable, T any] interface {
 type []T, map[E]T
}
func CopyVals[E comparable, T any, R Rangeable[E, T]](vals R) []T {
 var out []T
 for _, v := range vals {
 out = append(out, v)
 }
 return out
}
This example gets a little mind-bending, because we use type parameters on an inter‐
face that we use as a type parameter. Here’s how we use the CopyVals function with a
[]string and a map[string]float64:
func main() {
 x := []string{"a", "b", "c"}
 var out []string = CopyVals[interface{}, string, []string](x)
 fmt.Println(out)
 y := map[string]float64{"a": 1.2, "b": 3.1, "c": 10}
 var out2 []float64 = CopyVals[string, float64, map[string]float64](y)
 fmt.Println(out2)
}
Unfortunately, the current implementation of type inference can’t infer the type
parameters, so we have to supply them. This code prints out:
[a b c]
[3.1 10 1.2]
You can try this code for yourself.
A type list entry that’s a built-in type or type literal matches any type whose underly‐
ing type matches the built-in type or type literal. If you specify user-defined types on
a type list, only the specified types can be used for the type parameter.
Type Lists Limit Constants and Implementations | 335
If you specify both built-in types and user-defined types in your
type list, the interface can be met by any of the specified userdefined types or by any type whose underlying type matches the
built-in types.
Specifying a user-defined type does not give you access to the methods on the type.
Any methods on user-defined types that you want to access must be defined in the
interface, but all types specified in the type list must implement those methods, or
they will not match the interface. Here’s some code to make this clearer. Let’s define a
few types:
type MyInt int
func (mi MyInt) String() string {
 return strconv.Itoa(int(mi))
}
type AlsoMyInt int
func (ami AlsoMyInt) String() string {
 return "42"
}
type MyFloat float64
func (mf MyFloat) String() string {
 return strconv.FormatFloat(float64(mf), 'E', -1, 64)
}
Here’s a function that takes in a value of type T, doubles it, and returns the result of
calling the String method. We’re going to try a few different interfaces as a type
bound, so we’ll put ??? here for now (this is not valid Go syntax):
func DoubleString[T ???](val T) string {
 x := val * 2
 return x.String()
}
Our first type bound attempt is:
type IntOrFloat interface {
 type int, float64
}
If we try to use IntOrFloat as the type bound, the code fails to compile with:
x.String undefined (interface IntOrFloat has no method String)
Let’s try another type bound, this one using MyInt and MyFloat:
type MyTypes interface {
 type MyInt, MyFloat
}
336 | Chapter 15: A Look at the Future: Generics in Go
Because the methods on types in the type list are ignored, we get the same compila‐
tion error:
x.String undefined (interface MyTypes has no method String)
Now let’s try to include a method in our type bound:
type StringableIntOrFloat interface {
 type int, float64
 String() string
}
The function compiles if we use StringableIntOrFloat. Since int and float64 don’t
implement String (or any other methods), you cannot pass variables of those types
into DoubleString. If you do, you’ll get compile-time errors:
int does not satisfy StringableIntOrFloat (missing method String)
float64 does not satisfy StringableIntOrFloat (missing method String)
However, you can pass in MyInt, MyFloat, or AlsoMyInt.
Let’s try one last type bound:
type StringableMyType interface {
 type MyInt, MyFloat
 String() string
}
The function also compiles if we use StringableMyType. In this case, you can only
pass in variables of type MyInt or MyFloat. Attempting to pass in a variable of type
AlsoMyInt results in a compile-time error:
AlsoMyInt does not satisfy StringableMyType (AlsoMyInt or int not found in
 MyInt, MyFloat)
One last note about type lists: an interface with a type list can only be used for a type
parameter.



Another useful feature that’s been left out of the initial Go generics implementation is
additional type parameters on methods. Looking back on the Map/Reduce/Filter
functions, you might think they’d be useful as methods, like this:

type functionalSlice[T any] []T
// THIS DOES NOT WORK
func (fs functionalSlice[T]) Map[E any](f func(T) E) functionalSlice[E] {
 out := make(functionalSlice[E], len(fs))
 for i, v := range fs {
 out[i] = f(v)
 }
 return out
}
// THIS DOES NOT WORK
func (fs functionalSlice[T]) Reduce[E any](start E, f func(E, T) E) E {
 out := start
 for _, v := range fs {
 out = f(out, v)
 }
 return out
}
which you could use like this:
var numStrings = functionalSlice[string]{"1", "2", "3"}
sum := numStrings.Map(func(s string) int {
 v, _ := strconv.Atoi(s)
 return v
}).Reduce(0, func(acc int, cur int) int {
 return acc + cur
})

Unfortunately for fans of functional programming, this does not work. Rather than
chaining method calls together, you need to either nest function calls or use the much
more readable approach of invoking the functions one at a time and assigning the
intermediate values to variables. The draft design goes into detail on the reason for
338 | Chapter 15: A Look at the Future: Generics in Go
excluding parameterized methods, but it is unclear how they would interact with a
type meeting an interface.
There are also no variadic type parameters. In “Build Functions with Reflection to
Automate Repetitive Tasks” on page 312, we wrote a wrapper function using reflec‐
tion to time any existing function. Those must still be handled via reflection as there’s
no way to do this with generics. Any time you use type parameters, you must explic‐
itly provide a name for each type you need, so you cannot represent a function with
any number of parameters of different types.
Other features left out of Go generics are more esoteric. These include:
Specialization
A function or method can be overloaded with one or more type-specific versions
in addition to the generic version. Since Go doesn’t have overloading, this feature
is not under consideration.
Currying
Allows you to declare a type based on a generic type that only specifies some of
the type parameters.
Metaprogramming
Allows you to specify code that runs at compile time to produce code that runs at
runtime.


