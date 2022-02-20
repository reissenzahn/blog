var val interface{}  = "hello"

v := i.(string)         // hello
v, ok := val.(string)   // hello true
v, ok := val.(float64)  // 0 false
v = val.(float64)       // panic

if v, ok := val.(string); ok {
  fmt.Println(v)  // hello
}

switch v := val.(type) {
case int:
  fmt.Printf("Twice %v is %v\n", v, v*2)
case string:
  fmt.Printf("%q is %v bytes long\n", v, len(v))  // "hello" is 3 bytes long
default:
  fmt.Printf("I don't know about type %T!\n", v)
}


// 

Go provides two ways to see if a variable of an interface type has a specific concrete type or if the concrete type implements another interface.

A type assertion names the concrete type that implemented the interface, or names another interface that is also implemented by the concrete type underlying
the interface.

type MyInt int
func main() {
 var i interface{}
 var mine MyInt = 20
 i = mine
 i2 := i.(MyInt)
 fmt.Println(i2 + 1)
}

In the preceding code, the variable i2 is of type MyInt.

You might wonder what happens if a type assertion is wrong. In that case, your code
panics.

i2 := i.(string)
fmt.Println(i2)

As we’ve already seen, Go is very careful about concrete types. Even if two types share
an underlying type, a type assertion must match the type of the underlying value. The
following code panics. You can try it out on The Go Playground:

i2 := i.(int)
fmt.Println(i2 + 1)

Obviously, crashing is not desired behavior. We avoid this by using the comma ok
idiom, just as we saw in “The comma ok Idiom” on page 54 when detecting whether
or not a zero value was in a map:

i2, ok := i.(int)
if !ok {
 return fmt.Errorf("unexpected type for %v",i)
}
fmt.Println(i2 + 1)

The boolean ok is set to true if the type conversion was successful. If it was not, ok is
set to false and the other variable (in this case i2) is set to its zero value. We then
handle the unexpected condition within an if statement, but in idiomatic Go, we
indent the error handling code. We’ll talk more about error handling in Chapter 8.


A type assertion is very different from a type conversion. Type conversions can be applied to both concrete types and interfaces and are checked at compilation time. Type assertions can only be
applied to interface types and are checked at runtime. Because they
are checked at runtime, they can fail.


When an interface could be one of multiple possible types, use a type switch instead

func doThings(i interface{}) {
 switch j := i.(type) {
 case nil:
 // i is nil, type of j is interface{}
 case int:
 // j is of type int
 case MyInt:
 // j is of type MyInt
 case io.Reader:
 // j is of type io.Reader
 case string:
 // j is a string
 case bool, rune:
 // i is either a bool or rune, so j is of type interface{}
 default:
 // no idea what i is, so j is of type interface{}
 }
}

Instead of specifying a boolean operation, you specify a variable of an interface type and follow it with .(type). Usually, you assign the variable being
checked to another variable that’s only valid within the switch.
Since the purpose of a type switch is to derive a new variable from
an existing one, it is idiomatic to assign the variable being switched
on to a variable of the same name (i := i.(type)), making this
one of the few places where shadowing is a good idea. To make the
comments more readable, our example doesn’t use shadowing.
The type of the new variable depends on which case matches. You can use nil for one
case to see if the interface has no associated type. If you list more than one type on a
case, the new variable is of type interface{}. Just like a switch statement, you can
have a default case that matches when no specified type does. Otherwise, the new
variable has the type of the case that matches.


One common use of a type assertion is to see if the concrete type behind the interface also
implements another interface. This allows you to specify optional interfaces. For
example, the standard library uses this technique to allow more efficient copies when
the io.Copy function is called. This function has two parameters of types io.Writer
and io.Reader and calls the io.copyBuffer function to do its work. If the io.Writer
parameter also implements io.WriterTo, or the io.Reader parameter also imple‐
ments io.ReaderFrom, most of the work in the function can be skipped:
// copyBuffer is the actual implementation of Copy and CopyBuffer.
// if buf is nil, one is allocated.
func copyBuffer(dst Writer, src Reader, buf []byte) (written int64, err error) {
 // If the reader has a WriteTo method, use it to do the copy.
 // Avoids an allocation and a copy.
 if wt, ok := src.(WriterTo); ok {
 return wt.WriteTo(dst)
 }
 // Similarly, if the writer has a ReadFrom method, use it to do the copy.
 if rt, ok := dst.(ReaderFrom); ok {
 return rt.ReadFrom(src)
 }
 // function continues...
}


Function Types Are a Bridge to Interfaces

Go allows methods on any user-defined type, including user-defined function types.
They allow
functions to implement interfaces. The most common usage is for HTTP handlers.
An HTTP handler processes an HTTP server request. It’s defined by an interface:

type Handler interface {
 ServeHTTP(http.ResponseWriter, *http.Request)
}

By using a type conversion to http.HandlerFunc, any function that has the signature
func(http.ResponseWriter,*http.Request) can be used as an http.Handler:
type HandlerFunc func(http.ResponseWriter, *http.Request)
func (f HandlerFunc) ServeHTTP(w http.ResponseWriter, r *http.Request) {
154 | Chapter 7: Types, Methods, and Interfaces
 f(w, r)
}
This lets you implement HTTP handlers using functions, methods, or closures using
the exact same code path as the one used for other types that meet the http.Handler
interface.

Functions in Go are first-class concepts, and as such, they are often passed as param‐
eters into functions. Meanwhile, Go encourages small interfaces, and an interface of
only one method could easily replace a parameter of function type. 

If your single function is likely to depend on many other functions or other state
that’s not specified in its input parameters, use an interface parameter and define a
function type to bridge a function to the interface. That’s what’s done in the http
package; it’s likely that a Handler is just the entry point for a chain of calls that needs
to be configured. However, if it’s a simple function (like the one used in sort.Slice),
then a parameter of function type is a good choice.

