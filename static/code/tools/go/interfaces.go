// an interface is a named collection of method signatures
type Shape interface {
  Area() float64
}

type Circle struct {
  radius float64
}

func (c Circle) Area() float64 {
  return 3.14 * c.radius * c.radius
}

func measure(shape Shape) {
  fmt.Println(shape.Area())
}

// ...

c := Circle{2}
measure(c)



type Person struct {
 FirstName string
 LastName string
 Age int
}

This should be read as declaring a user-defined type with the name Person to have the underlying type of the struct literal that follows.

In addition to struct literals, you can use any primitive type or compound type literal to define a concrete type.

type Score int
type Converter func(string)Score
type TeamScores map[string]Score


Go allows you to declare a type at any block level, from the package block down.

Go supports methods on user-defined types.

type Person struct {
 FirstName string
 LastName string
 Age int
}
func (p Person) String() string {
 return fmt.Sprintf("%s %s, age %d", p.FirstName, p.LastName, p.Age)
}

The receiver appears between the keyword func and the name of the method.

Methods must be
declared in the same package as their associated type; Go doesn’t allow you to add
methods to types you don’t control.


p := Person {
 FirstName: "Fred",
 LastName:"Fredson",
 Age: 52,
}
output := p.String()



// Pointer Receivers and Value Receivers

Method receivers, too. They can be pointer receivers (the type is a pointer) or value receivers (the type is a value type). The following rules help you determine when to use each
kind of receiver:

• If your method modifies the receiver, you must use a pointer receiver.
• If your method needs to handle nil instances then it must use a pointer receiver.
• If your method doesn’t modify the receiver, you can use a value receiver.

Whether or not you use a value receiver for a method that doesn’t modify the receiver
depends on the other methods declared on the type. When a type has any pointer
receiver methods, a common practice is to be consistent and use pointer receivers for
all methods, even the ones that don’t modify the receiver.
Here’s some simple code to demonstrate pointer and value receivers. We’ll start with a
type that has two methods on it, one using a value receiver, the other with a pointer
receiver:
type Counter struct {
 total int
 lastUpdated time.Time
}
func (c *Counter) Increment() {
 c.total++
 c.lastUpdated = time.Now()
}
func (c Counter) String() string {
 return fmt.Sprintf("total: %d, last updated: %v", c.total, c.lastUpdated)
}
We can then try out these methods with the following code. You can run it yourself
on The Go Playground:
Methods | 131
var c Counter
fmt.Println(c.String())
c.Increment()
fmt.Println(c.String())
You should see the following output:
total: 0, last updated: 0001-01-01 00:00:00 +0000 UTC
total: 1, last updated: 2009-11-10 23:00:00 +0000 UTC m=+0.000000001
One thing you might notice is that we were able to call the pointer receiver method
even though c is a value type. When you use a pointer receiver with a local variable
that’s a value type, Go automatically converts it to a pointer type. In this case,
c.Increment() is converted to (&c).Increment().
However, be aware that the rules for passing values to functions still apply. If you pass
a value type to a function and call a pointer receiver method on the passed value, you
are invoking the method on a copy. You can try out the following code on The Go
Playground:
func doUpdateWrong(c Counter) {
 c.Increment()
 fmt.Println("in doUpdateWrong:", c.String())
}
func doUpdateRight(c *Counter) {
 c.Increment()
 fmt.Println("in doUpdateRight:", c.String())
}
func main() {
 var c Counter
 doUpdateWrong(c)
 fmt.Println("in main:", c.String())
 doUpdateRight(&c)
 fmt.Println("in main:", c.String())
}
When you run this code, you’ll get the output:
in doUpdateWrong: total: 1, last updated: 2009-11-10 23:00:00 +0000 UTC
 m=+0.000000001
in main: total: 0, last updated: 0001-01-01 00:00:00 +0000 UTC
in doUpdateRight: total: 1, last updated: 2009-11-10 23:00:00 +0000 UTC
 m=+0.000000001
in main: total: 1, last updated: 2009-11-10 23:00:00 +0000 UTC m=+0.000000001
The parameter in doUpdateRight is of type *Counter, which is a pointer instance. As
you can see, we can call both Increment and String on it. Go considers both pointer
and value receiver methods to be in the method set for a pointer instance. For a value
instance, only the value receiver methods are in the method set. This seems like a
132 | Chapter 7: Types, Methods, and Interfaces
pedantic detail right now, but we’ll come back to it when talking about interfaces in
just a bit.
One final note: do not write getter and setter methods for Go structs, unless you need
them to meet an interface (we’ll start covering interfaces in “A Quick Lesson on Inter‐
faces” on page 141). Go encourages you to directly access a field. Reserve methods for
business logic. The exceptions are when you need to update multiple fields as a single
operation or when the update isn’t a straightforward assignment of a new value. The
Increment method defined earlier demonstrates both of these properties.
Code Your Methods for nil Instances
We just mentioned pointer instances, which might make you wonder what happens
when you call a method on a nil instance. In most languages, this produces some
sort of error. (Objective-C allows you to call a method on a nil instance, but it always
does nothing.)
Go does something a little different. It actually tries to invoke the method. If it’s a
method with a value receiver, you’ll get a panic (we discuss panics in “panic and
recover” on page 174), as there is no value being pointed to by the pointer. If it’s a
method with a pointer receiver, it can work if the method is written to handle the
possibility of a nil instance.
In some cases, expecting a nil receiver actually makes the code simpler. Here’s an
implementation of a binary tree that takes advantage of nil values for the receiver:
type IntTree struct {
 val int
 left, right *IntTree
}
func (it *IntTree) Insert(val int) *IntTree {
 if it == nil {
 return &IntTree{val: val}
 }
 if val < it.val {
 it.left = it.left.Insert(val)
 } else if val > it.val {
 it.right = it.right.Insert(val)
 }
 return it
}
func (it *IntTree) Contains(val int) bool {
 switch {
 case it == nil:
 return false
 case val < it.val:
 return it.left.Contains(val)
Methods | 133
 case val > it.val:
 return it.right.Contains(val)
 default:
 return true
 }
}
The Contains method doesn’t modify the *IntTree, but it is
declared with a pointer receiver. This demonstrates the rule men‐
tioned previously about supporting a nil receiver. A method with a
value receiver can’t check for nil and as mentioned earlier, panics
if invoked with a nil receiver.
The following code uses the tree. You can try it out on The Go Playground:
func main() {
 var it *IntTree
 it = it.Insert(5)
 it = it.Insert(3)
 it = it.Insert(10)
 it = it.Insert(2)
 fmt.Println(it.Contains(2)) // true
 fmt.Println(it.Contains(12)) // false
}
It’s very clever that Go allows you to call a method on a nil receiver, and there are
situations where it is useful, like our tree node example. However, most of the time it’s
not very useful. Pointer receivers work just like pointer function parameters; it’s a
copy of the pointer that’s passed into the method. Just like nil parameters passed to
functions, if you change the copy of the pointer, you haven’t changed the original.
This means you can’t write a pointer receiver method that handles nil and makes the
original pointer non-nil. If your method has a pointer receiver and won’t work for a
nil receiver, check for nil and return an error (we discuss errors in Chapter 8).
Methods Are Functions Too
Methods in Go are so much like functions that you can use a method as a replace‐
ment for a function any time there’s a variable or parameter of a function type.
Let’s start with this simple type:
type Adder struct {
 start int
}
func (a Adder) AddTo(val int) int {
 return a.start + val
}
134 | Chapter 7: Types, Methods, and Interfaces
We create an instance of the type in the usual way and invoke its method:
myAdder := Adder{start: 10}
fmt.Println(myAdder.AddTo(5)) // prints 15
We can also assign the method to a variable or pass it to a parameter of type
func(int)int. This is called a method value:
f1 := myAdder.AddTo
fmt.Println(f1(10)) // prints 20
A method value is a bit like a closure, since it can access the values in the fields of the
instance from which it was created.
You can also create a function from the type itself. This is called a method expression:
f2 := Adder.AddTo
fmt.Println(f2(myAdder, 15)) // prints 25
In the case of a method expression, the first parameter is the receiver for the method;
our function signature is func(Adder, int) int.
Method values and method expressions aren’t just clever corner cases. We’ll see one
way to use them when we look at dependency injection in “Implicit Interfaces Make
Dependency Injection Easier” on page 155.
Functions Versus Methods
Since you can use a method as a function, you might wonder when you should
declare a function and when you should use a method.
The differentiator is whether or not your function depends on other data. As we’ve
covered several times, package-level state should be effectively immutable. Any time
your logic depends on values that are configured at startup or changed while your
program is running, those values should be stored in a struct and that logic should be
implemented as a method. If your logic only depends on the input parameters, then it
should be a function.
Types, packages, modules, testing, and dependency injection are interrelated con‐
cepts. We’ll look at dependency injection later in this chapter. You can find out more
about packages and modules in Chapter 9 and testing in Chapter 13.
Type Declarations Aren’t Inheritance
In addition to declaring types based on built-in Go types and struct literals, you can
also declare a user-defined type based on another user-defined type:
type HighScore Score
type Employee Person
Methods | 135
There are many concepts that can be considered “object-oriented,” but one stands
out: inheritance. This is where the state and methods of a parent type are declared to
be available on a child type and values of the child type can be substituted for the par‐
ent type. (For the computer scientists in the audience, I realize that subtyping is not
inheritance. However, most programming languages use inheritance to implement
subtyping so the definitions are often conflated in popular usage.)
Declaring a type based on another type looks a bit like inheritance, but it isn’t.
The two types have the same underlying type, but that’s all. There is no hierarchy
between these types. In languages with inheritance, a child instance can be used
anywhere the parent instance is used. The child instance also has all the methods
and data structures of the parent instance. That’s not the case in Go. You can’t
assign an instance of type HighScore to a variable of type Score or vice versa
without a type conversion, nor can you assign either of them to a variable of type int
without a type conversion. Furthermore, any methods defined on Score aren’t
defined on HighScore:
// assigning untyped constants is valid
var i int = 300
var s Score = 100
var hs HighScore = 200
hs = s // compilation error!
s = i // compilation error!
s = Score(i) // ok
hs = HighScore(s) // ok
For user-defined types whose underlying types are built-in types, a user-declared type
can be used with the operators for those types. As we see in the preceding code, they
can also be assigned literals and constants compatible with the underlying type.
A type conversion between types that share an underlying type
keeps the same underlying storage but associates different
methods.
Types Are Executable Documentation
While it’s well understood that you should declare a struct type to hold a set of related
data, it’s less clear when you should declare a user-defined type based on other builtin types or one user-defined type that’s based on another user-defined type. The short
answer is that types are documentation. They make code clearer by providing a name
for a concept and describing the kind of data that is expected. It’s clearer for someone
reading your code when a method has a parameter of type Percentage than of type
int, and it’s harder for it to be invoked with an invalid value.
136 | Chapter 7: Types, Methods, and Interfaces
The same logic applies when declaring one user-defined type based on another userdefined type. When you have the same underlying data, but different sets of opera‐
tions to perform, make two types. Declaring one as being based on the other avoids
some repetition and makes it clear that the two types are related.








Use Embedding for Composition
The software engineering advice “Favor object composition over class inheritance”
dates back to at least the 1994 book Design Patterns by Gamma, Helm, Johnson, and
Vlissides (Addison-Wesley), better known as the Gang of Four book. While Go
doesn’t have inheritance, it encourages code reuse via built-in support for composi‐
tion and promotion:
type Employee struct {
 Name string
 ID string
}
func (e Employee) Description() string {
 return fmt.Sprintf("%s (%s)", e.Name, e.ID)
}
type Manager struct {
 Employee
 Reports []Employee
}
func (m Manager) FindNewEmployees() []Employee {
 // do business logic
}
Note that Manager contains a field of type Employee, but no name is assigned to that
field. This makes Employee an embedded field. Any fields or methods declared on an
embedded field are promoted to the containing struct and can be invoked directly on
it. That makes the following code valid:
m := Manager{
 Employee: Employee{
 Name: "Bob Bobson",
 ID: "12345",
 },
 Reports: []Employee{},
}
fmt.Println(m.ID) // prints 12345
fmt.Println(m.Description()) // prints Bob Bobson (12345)
You can embed any type within a struct, not just another struct.
This promotes the methods on the embedded type to the contain‐
ing struct.
Use Embedding for Composition | 139
If the containing struct has fields or methods with the same name as an embedded
field, you need to use the embedded field’s type to refer to the obscured fields or
methods. If you have types defined like this:
type Inner struct {
 X int
}
type Outer struct {
 Inner
 X int
}
You can only access the X on Inner by specifying Inner explicitly:
o := Outer{
 Inner: Inner{
 X: 10,
 },
 X: 20,
}
fmt.Println(o.X) // prints 20
fmt.Println(o.Inner.X) // prints 10
Embedding Is Not Inheritance
Built-in embedding support is rare in programming languages (I’m not aware of
another popular language that supports it). Many developers who are familiar with
inheritance (which is available in many languages) try to understand embedding by
treating it as inheritance. That way lies tears. You cannot assign a variable of type
Manager to a variable of type Employee. If you want to access the Employee field
in Manager, you must do so explicitly. You can run the following code on
The Go Playground:
var eFail Employee = m // compilation error!
var eOK Employee = m.Employee // ok!
You’ll get the error:
cannot use m (type Manager) as type Employee in assignment
Furthermore, there is no dynamic dispatch for concrete types in Go. The methods on
the embedded field have no idea they are embedded. If you have a method on an
embedded field that calls another method on the embedded field, and the containing
struct has a method of the same name, the method on the embedded field will not
invoke the method on the containing struct. This behavior is demonstrated in the fol‐
lowing code, which you can run on The Go Playground:
140 | Chapter 7: Types, Methods, and Interfaces
type Inner struct {
 A int
}
func (i Inner) IntPrinter(val int) string {
 return fmt.Sprintf("Inner: %d", val)
}
func (i Inner) Double() string {
 return i.IntPrinter(i.A * 2)
}
type Outer struct {
 Inner
 S string
}
func (o Outer) IntPrinter(val int) string {
 return fmt.Sprintf("Outer: %d", val)
}
func main() {
 o := Outer{
 Inner: Inner{
 A: 10,
 },
 S: "Hello",
 }
 fmt.Println(o.Double())
}
Running this code produces the output:
Inner: 20
While embedding one concrete type inside another won’t allow you to treat the outer
type as the inner type, the methods on an embedded field do count toward the
method set of the containing struct. This means they can make the containing struct
implement an interface.
A Quick Lesson on Interfaces
While Go’s concurrency model (which we cover in Chapter 10) gets all of the publi‐
city, the real star of Go’s design is its implicit interfaces, the only abstract type in Go.
Let’s see what makes them so great.
We’ll start by taking a quick look at how to declare interfaces. At their core, interfaces
are simple. Like other user-defined types, you use the type keyword.
A Quick Lesson on Interfaces | 141
Here’s the definition of the Stringer interface in the fmt package:
type Stringer interface {
 String() string
}
In an interface declaration, an interface literal appears after the name of the interface
type. It lists the methods that must be implemented by a concrete type to meet the
interface. The methods defined by an interface are called the method set of the
interface.
Like other types, interfaces can be declared in any block.
Interfaces are usually named with “er” endings. We’ve already seen fmt.Stringer, but
there are many more, including io.Reader, io.Closer, io.ReadCloser, json.Mar
shaler, and http.Handler.
Interfaces Are Type-Safe Duck Typing
So far, nothing that’s been said is much different from interfaces in other languages.
What makes Go’s interfaces special is that they are implemented implicitly. A concrete
type does not declare that it implements an interface. If the method set for a concrete
type contains all of the methods in the method set for an interface, the concrete type
implements the interface. This means that the concrete type can be assigned to a vari‐
able or field declared to be of the type of the interface.
This implicit behavior makes interfaces the most interesting thing about types in Go,
because they enable both type-safety and decoupling, bridging the functionality in
both static and dynamic languages.
To understand why, let’s talk about why languages have interfaces. Earlier we men‐
tioned that Design Patterns taught developers to favor composition over inheritance.
Another piece of advice from the book is “Program to an interface, not an implemen‐
tation.” Doing so allows you to depend on behavior, not on implementation, allowing
you to swap implementations as needed. This allows your code to evolve over time, as
requirements inevitably change.
Dynamically typed languages like Python, Ruby, and JavaScript don’t have interfaces.
Instead, those developers use “duck typing,” which is based on the expression “If it
walks like a duck and quacks like a duck, it’s a duck.” The concept is that you can pass
an instance of a type as a parameter to a function as long as the function can find a
method to invoke that it expects:
class Logic:
def process(self, data):
 # business logic
def program(logic):
142 | Chapter 7: Types, Methods, and Interfaces
 # get data from somewhere
 logic.process(data)
logicToUse = Logic()
program(logicToUse)
Duck typing might sound weird at first, but it’s been used to build large and success‐
ful systems. If you program in a statically typed language, this sounds like utter chaos.
Without an explicit type being specified, it’s hard to know exactly what functionality
should be expected. As new developers move on to a project or the existing develop‐
ers forget what the code is doing, they have to trace through the code to figure out
what the actual dependencies are.
Java developers use a different pattern. They define an interface, create an implemen‐
tation of the interface, but only refer to the interface in the client code:
public interface Logic {
 String process(String data);
}
public class LogicImpl implements Logic {
 public String process(String data) {
 // business logic
 }
}
public class Client {
 private final Logic logic;
 // this type is the interface, not the implementation
 public Client(Logic logic) {
 this.logic = logic;
 }
 public void program() {
 // get data from somewhere
 this.logic(data);
 }
}
public static void main(String[] args) {
 Logic logic = new LogicImpl();
 Client client = new Client(logic);
 client.program();
}
Dynamic language developers look at the explicit interfaces in Java and don’t see how
you can possibly refactor your code over time when you have explicit dependencies.
Switching to a new implementation from a different provider means rewriting your
code to depend on a new interface.
Interfaces Are Type-Safe Duck Typing | 143
Go’s developers decided that both groups are right. If your application is going to
grow and change over time, you need flexibility to change implementation. However,
in order for people to understand what your code is doing (as new people work on
the same code over time), you also need to specify what the code depends on. That’s
where implicit interfaces come in. Go code is a blend of the previous two styles:
type LogicProvider struct {}
func (lp LogicProvider) Process(data string) string {
 // business logic
}
type Logic interface {
 Process(data string) string
}
type Client struct{
 L Logic
}
func(c Client) Program() {
 // get data from somewhere
 c.L.Process(data)
}
main() {
 c := Client{
 L: LogicProvider{},
 }
 c.Program()
}
In the Go code, there is an interface, but only the caller (Client) knows about it;
there is nothing declared on LogicProvider to indicate that it meets the interface.
This is sufficient to both allow a new logic provider in the future and provide exe‐
cutable documentation to ensure that any type passed into the client will match the
client’s need.
Interfaces specify what callers need. The client code defines the
interface to specify what functionality it requires.
This doesn’t mean that interfaces can’t be shared. We’ve already seen several interfaces
in the standard library that are used for input and output. Having a standard interface
is powerful; if you write your code to work with io.Reader and io.Writer, it will
function correctly whether it is writing to a file on local disk or a value in memory.
144 | Chapter 7: Types, Methods, and Interfaces
Furthermore, using standard interfaces encourages the decorator pattern. It is com‐
mon in Go to write factory functions that take in an instance of an interface and
return another type that implements the same interface. For example, say you have a
function with the following definition:
func process(r io.Reader) error
You can process data from a file with the following code:
r, err := os.Open(fileName)
if err != nil {
 return err
}
defer r.Close()
return process(r)
return nil
The os.File instance returned by os.Open meets the io.Reader interface and can be
used in any code that reads in data. If the file is gzip-compressed, you can wrap the
io.Reader in another io.Reader:
r, err := os.Open(fileName)
if err != nil {
 return err
}
defer r.Close()
gz, err = gzip.NewReader(r)
if err != nil {
 return err
}
defer gz.Close()
return process(gz)
Now the exact same code that was reading from an uncompressed file is reading from
a compressed file instead.
If there’s an interface in the standard library that describes what
your code needs, use it!
It’s perfectly fine for a type that meets an interface to specify additional methods that
aren’t part of the interface. One set of client code may not care about those methods,
but others do. For example, the io.File type also meets the io.Writer interface. If
your code only cares about reading from a file, use the io.Reader interface to refer to
the file instance and ignore the other methods.
Interfaces Are Type-Safe Duck Typing | 145
Embedding and Interfaces
Just like you can embed a type in a struct, you can also embed an interface in an inter‐
face. For example, the io.ReadCloser interface is built out of an io.Reader and an
io.Closer:
type Reader interface {
 Read(p []byte) (n int, err error)
}
type Closer interface {
 Close() error
}
type ReadCloser interface {
 Reader
 Closer
}
Just like you can embed a concrete type in a struct, you can also
embed an interface in a struct. We’ll see a use for this in “Stubs in
Go” on page 286.
Accept Interfaces, Return Structs
You’ll often hear experienced Go developers say that your code should “Accept inter‐
faces, return structs.” What this means is that the business logic invoked by your
functions should be invoked via interfaces, but the output of your functions should
be a concrete type. We’ve already covered why functions should accept interfaces:
they make your code more flexible and explicitly declare exactly what functionality is
being used.
If you create an API that returns interfaces, you are losing one of the main advantages
of implicit interfaces: decoupling. You want to limit the third-party interfaces that
your client code depends on because your code is now permanently dependent on the
module that contains those interfaces, as well as any dependencies of that module,
and so on. (We talk about modules and dependencies in Chapter 9.) This limits
future flexibility. To avoid the coupling, you’d have to write another interface and do a
type conversion from one to the other. While depending on concrete instances can
lead to dependencies, using a dependency injection layer in your application limits
the effect. We’ll talk more about dependency injection in “Implicit Interfaces Make
Dependency Injection Easier” on page 155.
Another reason to avoid returning interfaces is versioning. If a concrete type is
returned, new methods and fields can be added without breaking existing code. The
146 | Chapter 7: Types, Methods, and Interfaces
same is not true for an interface. Adding a new method to an interface means that
you need to update all existing implementations of the interface, or your code breaks.
If you make a backward-breaking change to an API, you should increment your
major version number.
Rather than writing a single factory function that returns different instances behind
an interface based on input parameters, try to write separate factory functions for
each concrete type. In some situations (such as a parser that can return one or more
different kinds of tokens), it’s unavoidable and you have no choice but to return an
interface.
Errors are an exception to this rule. As we’ll see in Chapter 8, Go functions and meth‐
ods declare a return parameter of the error interface type. In the case of error, it’s
quite likely that different implementation of the interface could be returned, so you
need to use an interface to handle all possible options, as interfaces are the only
abstract type in Go.
There is one potential drawback to this pattern. As we discussed in “Reducing the
Garbage Collector’s Workload” on page 123, reducing heap allocations improves per‐
formance by reducing the amount of work for the garbage collector. Returning a
struct avoids a heap allocation, which is good. However, when invoking a function
with parameters of interface types, a heap allocation occurs for each of the interface
parameters. Figuring out the trade-off between better abstraction and better perfor‐
mance is something that should be done over the life of your program. Write your
code so that it is readable and maintainable. If you find that your program is too slow
and you have profiled it and you have determined that the performance problems are
due to a heap allocation caused by an interface parameter, then you should rewrite
the function to use a concrete type parameter. If multiple implementations of an
interface are passed into the function, this will mean creating multiple functions with
repeated logic.
Interfaces and nil
When discussing pointers in Chapter 6, we also talked about nil, the zero value for
pointer types. We also use nil to represent the zero value for an interface instance,
but it’s not as simple as it is for concrete types.
In order for an interface to be considered nil both the type and the value must be
nil. The following code prints out true on the first two lines and false on the last:
var s *string
fmt.Println(s == nil) // prints true
var i interface{}
fmt.Println(i == nil) // prints true
i = s
fmt.Println(i == nil) // prints false
Interfaces and nil | 147
You can run it for yourself on The Go Playground.
In the Go runtime, interfaces are implemented as a pair of pointers, one to the under‐
lying type and one to the underlying value. As long as the type is non-nil, the inter‐
face is non-nil. (Since you cannot have a variable without a type, if the value pointer
is non-nil, the type pointer is always non-nil.)
What nil indicates for an interface is whether or not you can invoke methods on it.
As we covered earlier, you can invoke methods on nil concrete instances, so it makes
sense that you can invoke methods on an interface variable that was assigned a nil
concrete instance. If an interface is nil, invoking any methods on it triggers a panic
(which we’ll discuss in “panic and recover” on page 174). If an interface is non-nil,
you can invoke methods on it. (But note that if the value is nil and the methods of
the assigned type don’t properly handle nil, you could still trigger a panic.)
Since an interface instance with a non-nil type is not equal to nil, it is not straight‐
forward to tell whether or not the value associated with the interface is nil when the
type is non-nil. You must use reflection (which we’ll discuss in “Use Reflection to
Check If an Interface’s Value Is nil” on page 306) to find out.



The Empty Interface Says Nothing

the empty interface represents that a variable could stora value of any type

var i interface{}
i = 20
i = "hello"
i = struct {
 FirstName string
 LastName string
} {"Fred", "Fredson"}






