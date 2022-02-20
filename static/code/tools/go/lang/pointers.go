package main

import "fmt"

func zero(p *int) {
  *p = 0
}

func main() {
  // a pointer is simply a variable that holds the location in memory where a value is stored
  i := 1
  
  // the address of operator returns the address of a variable
  p := &i

  // the dereference operator return the value pointed to by a pointer
  fmt.Println(*p)  // 1

  // the dereference operator can be used to modify the value pointed to by a pointer
  *p = 2

  // call by value with a pointer allows the pointed to value to be modified
  zero(&i)
  fmt.Println(i)  // 0


}


var pointerZ *string

Every pointer, no matter what type it is pointing to, is always the same size: a number that holds the location in memory where the data is stored.

The zero value for a pointer is nil.


nil is an untyped identifier that represents the lack of a value for certain types

// the address operator returns the address of the memory location where a variable is stored
x := "hello"
pointerToX := &x

// the indirection operator returns the value pointed to by a pointer (i.e. dereferences the pointer)
x := 10
pointerToX := &x
fmt.Println(pointerToX) // prints a memory address
fmt.Println(*pointerToX) // prints 10
z := 5 + *pointerToX
fmt.Println(z) // prints 15

// attempting to dereference a nil pointer will cause a panic
var x *int
fmt.Println(x == nil) // prints true
fmt.Println(*x) // panics


// A pointer type is a type that represents a pointer. It is written with a * before a type name. A pointer type can be based on any type:

x := 10
var pointerToX *int
pointerToX = &x

The built-in function new creates a pointer variable. It returns a pointer to a zero value instance of the provided type:

var x = new(int)
fmt.Println(x == nil) // prints false
fmt.Println(*x) // prints 0

The new function is rarely used. For structs, use an & before a struct literal to create a
pointer instance.

You can’t use an & before a primitive literal (numbers, booleans, and strings) or a constant because they don’t have memory addresses; they exist only at
compile time.


When you need a pointer to a primitive type, declare a variable and point to it:

x := &Foo{}
var y string
z := &y

Not being able to take the address of a constant is sometimes inconvenient. If you
have a struct with a field of a pointer to a primitive type, you can’t assign a literal
directly to the field:

type person struct {
 FirstName string
 MiddleName *string
 LastName string
}
p := person{
 FirstName: "Pat",
 MiddleName: "Perry", // This line won't compile
 LastName: "Peterson",
}

Compiling this code returns the error:
cannot use "Perry" (type string) as type *string in field value
If you try to put an & before "Perry", you’ll get the error message:
cannot take the address of "Perry"

There are two ways around this problem. The first is to do what we showed previously, introduce a variable to hold the constant value. The second way is to write a
helper function that takes in a boolean, numeric, or string type and returns a pointer
to that type:

func stringp(s string) *string {
 return &s
}

With that function, you can now write:
p := person{
 FirstName: "Pat",
 MiddleName: stringp("Perry"), // This works
 LastName: "Peterson",
}

Why does this work? When we pass a constant to a function, the constant is copied to
a parameter, which is a variable. Since it’s a variable, it has an address in memory. The
function then returns the variable’s memory address.
Use a helper function to turn a constant value into a pointer.




Go gives you the choice to use pointers or values for both primitives and structs.


Go developers use pointers to indicate that a parameter is mutable.
Since Go is a call by value language, the values passed to functions are copies. For nonpointer types like primitives, structs, and arrays, this means that the called function cannot modify the original. Since the called function has a copy of the original data, the immutability of the original data is guaranteed.

However, if a pointer is passed to a function, the function gets a copy of the pointer.
This still points to the original data, which means that the original data can be modi‐
fied by the called function.



When you pass a nil pointer to a function, you cannot make the value non-nil. You can only reassign the value if there was a value already assigned to the pointer. While confusing at first, it makes sense. Since the memory location was passed to the function via call-by-value, we can’t change the memory address, any more than we could change the value of an int parameter.

func failedUpdate(g *int) {
 x := 10
 g = &x
}
func main() {
 var f *int // f is nil
 failedUpdate(f)
 fmt.Println(f) // prints nil
}

We start with a nil variable f in main. When we call failedUpdate, we copy the value
of f, which is nil, into the parameter named g. This means that g is also set to nil.
We then declare a new variable x within failedUpdate with the value 10. Next, we
change g in failedUpdate to point to x. This does not change the f in main, and
when we exit failedUpdate and return to main, f is still nil.
The second implication of copying a pointer is that if you want the value assigned to a
pointer parameter to still be there when you exit the function, you must dereference
the pointer and set the value. If you change the pointer, you have changed the copy,
not the original. Dereferencing puts the new value in the memory location pointed to
by both the original and the copy. Here’s a short program that shows how this works:
func failedUpdate(px *int) {
 x2 := 20
 px = &x2
}
func update(px *int) {
 *px = 20
}
func main() {
 x := 10
 failedUpdate(&x)
 fmt.Println(x) // prints 10
 update(&x)
 fmt.Println(x) // prints 20
}
The flow through this code is shown in Figure 6-4.
In this example, we start with x in main set to 10. When we call failedUpdate, we
copy the address of x into the parameter px. Next, we declare x2 in failedUpdate, set
to 20. We then point px in failedUpdate to the address of x2. When we return to
main, the value of x is unchanged. When we call update, we copy the address of x into
px again. However, this time we change the value of what px in update points to, the
variable x in main. When we return to main, x has been changed.
Pointers Indicate Mutable Parameters | 115
Figure 6-4. The wrong way and the right way to update a pointer



Pointers Are a Last Resort
Rather than populating a struct by passing a pointer to it into a function, have
the function instantiate and return the struct (see Example 6-3 and Example 6-4).

Example 6-3. Don’t do this
func MakeFoo(f *Foo) error {
 f.Field1 = "val"
 f.Field2 = 20
 return nil
}

Example 6-4. Do this
func MakeFoo() (Foo, error) {
 f := Foo{
 Field1: "val",
 Field2: 20,
 }
 return f, nil
}
The only time you should use pointer parameters to modify a variable is when the
function expects an interface.

f := struct {
 Name string `json:"name"`
 Age int `json:"age"`
}
err := json.Unmarshal([]byte(`{"name": "Bob", "age": 30}`), &f)
The Unmarshal function populates a variable from a slice of bytes containing JSON. It
is declared to take a slice of bytes and an interface{} parameter. The value passed in
for the interface{} parameter must be a pointer. If it is not, an error is returned.
This pattern is used because Go doesn’t have generics. That means there isn’t a conve‐
nient way to pass a type into a function to specify what to unmarshal into nor is there
a way to specify a different return type for different types.
Because JSON integration is so common, this API is sometimes treated as a common
case by new Go developers, instead of the exception that it should be.
Pointers Are a Last Resort | 117
There is a way to represent a type in Go in a variable, by using the
Type type in the reflect package. The reflect package is reserved
for situations where there is no other way to accomplish a task.
We’ll look at reflection in Chapter 14.
When returning values from a function, you should favor value types. Only use a
pointer type as a return type if there is state within the data type that needs to be
modified. When we look at I/O in “io and Friends” on page 233, we’ll see that with
buffers for reading or writing data. In addition, there are data types that are used with
concurrency that must always be passed as pointers. We’ll see those in Chapter 10.



The Zero Value Versus No Value
The other common usage of pointers in Go is to indicate the difference between a
variable or field that’s been assigned the zero value and a variable or field that hasn’t
been assigned a value at all. If this distinction matters in your program, use a nil
pointer to represent an unassigned variable or struct field.
118 | Chapter 6: Pointers
Because pointers also indicate mutability, be careful when using this pattern. Rather
than return a pointer set to nil from a function, use the comma ok idiom that we saw
for maps and return a value type and a boolean.
Remember, if a nil pointer is passed into a function via a parameter or a field on a
parameter, you cannot set the value within the function as there’s nowhere to store
the value. If a non-nil value is passed in for the pointer, do not modify it unless you
document the behavior.
Again, JSON conversions are the exception that proves the rule. When converting
data back and forth from JSON (yes, we’ll talk more about the JSON support in Go’s
standard library in “encoding/json” on page 241), you often need a way to differenti‐
ate between the zero value and not having a value assigned at all. Use a pointer value
for fields in the struct that are nullable.
When not working with JSON (or other external protocols), resist the temptation to
use a pointer field to indicate no value. While a pointer does provide a handy way to
indicate no value, if you are not going to modify the value, you should use a value
type instead, paired with a boolean.
The Dierence Between Maps and Slices
As we saw in the previous chapter, any modifications made to a map that’s passed to a
function are reflected in the original variable that was passed in. Now that we know
about pointers, we can understand why: within the Go runtime, a map is imple‐
mented as a pointer to a struct. Passing a map to a function means that you are copy‐
ing a pointer.
Because of this, you should avoid using maps for input parameters or return values,
especially on public APIs. On an API-design level, maps are a bad choice because
they say nothing about what values are contained within; there’s nothing that explic‐
itly defines what keys are in the map, so the only way to know what they are is to trace
through the code. From the standpoint of immutability, maps are bad because the
only way to know what ended up in the map is to trace through all of the functions
that interact with it. This prevents your API from being self-documenting. If you are
used to dynamic languages, don’t use a map as a replacement for another language’s
lack of structure. Go is a strongly typed language; rather than passing a map around,
use a struct. (We’ll learn another reason to prefer structs when we talk about memory
layout in “Reducing the Garbage Collector’s Workload” on page 123.)
The Dierence Between Maps and Slices | 119
Meanwhile, passing a slice to a function has more complicated behavior: any modifi‐
cation to the contents of the slice is reflected in the original variable, but using append
to change the length isn’t reflected in the original variable, even if the slice has a
capacity greater than its length. That’s because a slice is implemented as a struct with
three fields: an int field for length, an int field for capacity, and a pointer to a block
of memory. Figure 6-5 demonstrates the relationship.
Figure 6-5. The memory layout of a slice
When a slice is copied to a different variable or passed to a function, a copy is made
of the length, capacity, and the pointer. Figure 6-6 shows how both slice variables
point to the same memory.
Figure 6-6. The memory layout of a slice and its copy
Changing the values in the slice changes the memory that the pointer points to, so the
changes are seen in both the copy and the original. We see in Figure 6-7 how this
looks in memory.
120 | Chapter 6: Pointers
Figure 6-7. Modifying the contents of a slice
Changes to the length and capacity are not reflected back in the original, because they
are only in the copy. Changing the capacity means that the pointer is now pointing to
a new, bigger block of memory. In Figure 6-8 we show how each slice variable now
points to a different memory block.
Figure 6-8. Changing the capacity changes the storage
If the slice copy is appended to and there is enough capacity to not allocate a new
slice, the length changes in the copy and the new values are stored in the block of
memory that’s shared by the copy and the original. However, the length in the origi‐
nal slice remains unchanged. This means the Go runtime prevents the original slice
from seeing those values since they are beyond the length of the original slice.
The Dierence Between Maps and Slices | 121
Figure 6-9 highlights the values that are visible in one slice variable but not in the
other.
Figure 6-9. Changing the length is invisible in the original
The result is that a slice that’s passed to a function can have its contents modified, but
the slice can’t be resized. As the only usable linear data structure, slices are frequently
passed around in Go programs. By default, you should assume that a slice is not
modified by a function. Your function’s documentation should specify if it modifies
the slice’s contents.
The reason you can pass a slice of any size to a function is that the
data that’s passed to the function is the same for any size slice: two
int values and a pointer. The reason that you can’t write a function
that takes an array of any size is because the entire array is passed
to the function, not just a pointer to the data.
There’s another use for slices as input parameters: they are ideal for reusable buffers.
Slices as Buers
When reading data from an external resource (like a file or a network connection),
many languages use code like this:
r = open_resource()
while r.has_data() {
 data_chunk = r.next_chunk()
 process(data_chunk)
}
close(r)
122 | Chapter 6: Pointers
The problem with this pattern is that every time we iterate through that while loop,
we allocate another data_chunk even though each one is only used once. This creates
lots of unnecessary memory allocations. Garbage-collected languages handle those
allocations for you automatically, but the work still needs to be done to clean them up
when you are done processing.
Even though Go is a garbage-collected language, writing idiomatic Go means avoid‐
ing unneeded allocations. Rather than returning a new allocation each time we read
from a data source, we create a slice of bytes once and use it as a buffer to read data
from the data source:
file, err := os.Open(fileName)
if err != nil {
 return err
}
defer file.Close()
data := make([]byte, 100)
for {
 count, err := file.Read(data)
 if err != nil {
 return err
 }
 if count == 0 {
 return nil
 }
 process(data[:count])
}
Remember that we can’t change the length or capacity of a slice when we pass it to a
function, but we can change the contents up to the current length. In this code, we
create a buffer of 100 bytes and each time through the loop, we copy the next block of
bytes (up to 100) into the slice. We then pass the populated portion of the buffer to
process. We’ll look at more details about I/O in “io and Friends” on page 233.




// if a pointer to a local variable is returned then the value of the local variable is stored on the heap via escape analysis
type point struct {
  x, y int
}

func newPoint() *point {
  return &point{10, 20}
}

