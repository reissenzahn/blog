// It is idiomatic to communicate errors via an explicit separate return value.

// by convention, errors are the last return value and have type error

// errors.New constructs a basic error value with the given error message

// a nil value in the error position indicates there was no error

// a custom type can be created by implementing the Error() method

// the error type is an interface type

// an error variable represents any value that can describe itself as a string

func divide(x int, int y) (int, error) {
	if y == 0 {
		return -1, errors.New("divide by zero")
	}

	return x / y, nil
}

if _, err := divide(1, 0); err != nil {
	panic(err)
}

// custom errors
type divideError struct {
	x int
	y int
}

func (e *divideError) Error() string {
	return fmt.Sprintf("can't divide %d by %d", e.x, e.y)
}

func divide(x int, int y) (int, error) {
	if y == 0 {
		return -1, &divideError{x, y}
	}

	return x / y, nil
}

if _, err := divide(1, 0); err != nil {
	panic(err)
}

_, err := divide(1, 0)

if de, ok := err.(*divideError); ok {
	fmt.Println(de.x)
	fmt.Println(de.y)
}

// panic exits the program with an error
if _, err := os.Create("/tmp/file"); err != nil {
  panic(err)
}


Go handles errors by returning a value of type error as the last return value for a function. This is entirely by convention

When a function executes as expected, nil is returned for the error parameter. If something goes wrong, an
error value is returned instead. The calling function then checks the error return
value by comparing it to nil, handling the error, or returning an error of its own.

func calcRemainderAndMod(numerator, denominator int) (int, int, error) {
 if denominator == 0 {
 return 0, 0, errors.New("denominator is 0")
 }
 return numerator / denominator, numerator % denominator, nil
}

A new error is created from a string by calling the New function in the errors package. Error messages should not be capitalized nor should they end with punctuation or a newline.

In most cases, you should set the other return values to their zero values when a non-nil error is returned.

func main() {
 numerator := 20
 denominator := 3
 remainder, mod, err := calcRemainderAndMod(numerator, denominator)
 if err != nil {
 fmt.Println(err)
 os.Exit(1)
 }
 fmt.Println(remainder, mod)
}
error is a built-in interface that defines a single method:
type error interface {
 Error() string
}
Anything that implements this interface is considered an error. The reason why we
return nil from a function to indicate that no error occurred is that nil is the zero
value for any interface type.
There are two very good reasons why Go uses a returned error instead of thrown
exceptions. First, exceptions add at least one new code path through the code. These
paths are sometimes unclear, especially in languages whose functions don’t include a
declaration that an exception is possible. This produces code that crashes in surpris‐
ing ways when exceptions aren’t properly handled, or, even worse, code that doesn’t
crash but whose data is not properly initialized, modified, or stored.
The second reason is more subtle, but demonstrates how Go’s features work together.
The Go compiler requires that all variables must be read. Making errors returned val‐
ues forces developers to either check and handle error conditions or make it explicit
that they are ignoring errors by using an underscore (_) for the returned error value.
As noted in Chapter 5, while you cannot ignore some values
returned from a function, you can ignore all of the return values
from a function. If you ignore all the return values, you would be
able to ignore the error, too. In most cases, it is very bad form to
ignore the values returned from a function. Please avoid this,
except for cases like fmt.Println.
Exception handling may produce shorter code, but having fewer lines doesn’t neces‐
sarily make code easier to understand or maintain. As we’ve seen, idiomatic Go favors
clear code, even if it takes more lines.
162 | Chapter 8: Errors
Another thing to note is how code flows in Go. The error handling is indented inside
an if statement. The business logic is not. This gives a quick visual clue to which
code is along the “golden path” and which code is the exceptional condition.
Use Strings for Simple Errors
Go’s standard library provides two ways to create an error from a string. The first is
the errors.New function. It takes in a string and returns an error. This string is
returned when you call the Error method on the returned error instance. If you pass
an error to fmt.Println, it calls the Error method automatically:
func doubleEven(i int) (int, error) {
 if i % 2 != 0 {
 return 0, errors.New("only even numbers are processed")
 }
 return i * 2, nil
}
The second way is to use the fmt.Errorf function. This function allows you to use all
of the formatting verbs for fmt.Printf to create an error. Like errors.New, this string
is returned when you call the Error method on the returned error instance:
func doubleEven(i int) (int, error) {
 if i % 2 != 0 {
 return 0, fmt.Errorf("%d isn't an even number", i)
 }
 return i * 2, nil
}
Sentinel Errors
Some errors are meant to signal that processing cannot continue due to a problem
with the current state. In his blog post “Don’t just check errors, handle them grace‐
fully”, Dave Cheney, a developer who has been active in the Go community for many
years, coined the term sentinel errors to describe these errors:
The name descends from the practice in computer programming of using a specific
value to signify that no further processing is possible. So to [sic] with Go, we use spe‐
cific values to signify an error.
—Dave Cheney
Sentinel errors are one of the few variables that are declared at the package level. By
convention, their names start with Err (with the notable exception of io.EOF). They
should be treated as read-only; there’s no way for the Go compiler to enforce this, but
it is a programming error to change their value.
Use Strings for Simple Errors | 163
Sentinel errors are usually used to indicate that you cannot start or continue process‐
ing. For example, the standard library includes a package for processing ZIP files,
archive/zip. This package defines several sentinel errors, including ErrFormat,
which is returned when data that doesn’t represent a ZIP file is passed in. Try out this
code on The Go Playground:
func main() {
 data := []byte("This is not a zip file")
 notAZipFile := bytes.NewReader(data)
 _, err := zip.NewReader(notAZipFile, int64(len(data)))
 if err == zip.ErrFormat {
 fmt.Println("Told you so")
 }
}
Another example of a sentinel error in the standard library is rsa.ErrMessageToo
Long in the crypto/rsa package. It indicates that a message cannot be encrypted
because it is too long for the provided public key. When we discuss the context in
Chapter 12, you’ll see another commonly used sentinel error, context.Canceled.
Be sure you need a sentinel error before you define one. Once you define one, it is
part of your public API and you have committed to it being available in all future
backward-compatible releases. It’s far better to reuse one of the existing ones in the
standard library or to define an error type that includes information about the condi‐
tion that caused the error to be returned (we’ll see how to do that in the next section).
But if you have an error condition that indicates a specific state has been reached in
your application where no further processing is possible and no contextual informa‐
tion needs to be used to explain the error state, a sentinel error is the correct choice.
How do you test for a sentinel error? As you can see in the preceding code sample,
use == to test if the error was returned when calling a function whose documentation
explicitly says it returns a sentinel error. In an upcoming section, we will discuss how
to check for sentinel errors in other situations.
Using Constants for Sentinel Errors
In Constant errors, Dave Cheney proposed that constants would make useful sentinel
errors. You’d have a type like this in a package (we’ll talk about creating packages in
Chapter 9):
package consterr
type Sentinel string
func(s Sentinel) Error() string {
 return string(s)
}
164 | Chapter 8: Errors
and then use it like this:
package mypkg
const (
 ErrFoo = consterr.Sentinel("foo error")
 ErrBar = consterr.Sentinel("bar error")
)
This looks like a function call, but it’s actually casting a string literal to a type that
implements the error interface. It would be impossible to change the values of Err
Foo and ErrBar. At first glance, this looks like a good solution.
However, this practice isn’t considered idiomatic. If you used the same type to create
constant errors across packages, two errors would be equal if their error strings are
equal. They’d also be equal to a string literal with the same value. Meanwhile, an error
created with errors.New is only equal to itself or to variables explicitly assigned its
value. You almost certainly do not want to make errors in different packages equal to
each other; otherwise, why declare two different errors? (You could avoid this by cre‐
ating a nonpublic error type in every package, but that’s a lot of boilerplate.)
The sentinel error pattern is another example of the Go design philosophy. Sentinel
errors should be rare, so they can be handled by convention instead of language rules.
Yes, they are public package-level variables. This makes them mutable, but it’s highly
unlikely someone would accidentally reassign a public variable in a package. In short,
it’s a corner case that is handled by other features and patterns. The Go philosophy is
that it’s better to keep the language simple and trust the developers and tooling than it
is to add additional features.
So far, all the errors that we’ve seen are strings. But Go errors can contain more infor‐
mation. Let’s see how.
Errors Are Values
Since error is an interface, you can define your own errors that include additional
information for logging or error handling. For example, you might want to include a
status code as part of the error to indicate the kind of error that should be reported
back to the user. This lets you avoid string comparisons (whose text might change) to
determine error causes. Let’s see how this works. First, define your own enumeration
to represent the status codes:
type Status int
const (
 InvalidLogin Status = iota + 1
 NotFound
)
Errors Are Values | 165
Next, define a StatusErr to hold this value:
type StatusErr struct {
 Status Status
 Message string
}
func (se StatusErr) Error() string {
 return se.Message
}
Now we can use StatusErr to provide more details about what went wrong:
func LoginAndGetData(uid, pwd, file string) ([]byte, error) {
 err := login(uid, pwd)
 if err != nil {
 return nil, StatusErr{
 Status: InvalidLogin,
 Message: fmt.Sprintf("invalid credentials for user %s", uid),
 }
 }
 data, err := getData(file)
 if err != nil {
 return nil, StatusErr{
 Status: NotFound,
 Message: fmt.Sprintf("file %s not found", file),
 }
 }
 return data, nil
}
Even when you define your own custom error types, always use error as the return
type for the error result. This allows you to return different types of errors from your
function and allows callers of your function to choose not to depend on the specific
error type.
If you are using your own error type, be sure you don’t return an uninitialized
instance. This means that you shouldn’t declare a variable to be the type of your cus‐
tom error and then return that variable. Let’s see what happens if you do. Try out the
following code on The Go Playground:
func GenerateError(flag bool) error {
 var genErr StatusErr
 if flag {
 genErr = StatusErr{
 Status: NotFound,
 }
 }
 return genErr
}
func main() {
166 | Chapter 8: Errors
 err := GenerateError(true)
 fmt.Println(err != nil)
 err = GenerateError(false)
 fmt.Println(err != nil)
}
Running this program produces the following output:
true
true
This isn’t a pointer type versus value type issue; if we declared genErr to be of type
*StatusErr, we’d see the same output. The reason why err is non-nil is that error is
an interface. As we discussed in “Interfaces and nil” on page 147, for an interface to
be considered nil, both the underlying type and the underlying value must be nil.
Whether or not genErr is a pointer, the underlying type part of the interface is not
nil.
There are two ways to fix this. The most common approach is to explicitly return nil
for the error value when a function completes successfully:
func GenerateError(flag bool) error {
 if flag {
 return StatusErr{
 Status: NotFound,
 }
 }
 return nil
}
This has the advantage of not requiring you to read through code to make sure that
the error variable on the return statement is correctly defined.
Another approach is to make sure that any local variable that holds an error is of
type error:
func GenerateError(flag bool) error {
 var genErr error
 if flag {
 genErr = StatusErr{
 Status: NotFound,
 }
 }
 return genErr
}
When using custom errors, never define a variable to be of the type
of your custom error. Either explicitly return nil when no error
occurs or define the variable to be of type error.
Errors Are Values | 167
As we covered in “Use Type Assertions and Type Switches Sparingly” on page 152,
don’t use a type assertion or a type switch to access the fields and methods of a cus‐
tom error. Instead, use errors.As, which we’ll look at in “Is and As” on page 170.
Wrapping Errors
When an error is passed back through your code, you often want to add additional
context to it. This context can be the name of the function that received the error or
the operation it was trying to perform. When you preserve an error while adding
additional information, it is called wrapping the error. When you have a series of
wrapped errors, it is called an error chain.
There’s a function in the Go standard library that wraps errors, and we’ve already seen
it. The fmt.Errorf function has a special verb, %w. Use this to create an error whose
formatted string includes the formatted string of another error and which contains
the original error as well. The convention is to write : %w at the end of the error for‐
mat string and make the error to be wrapped the last parameter passed to
fmt.Errorf.
The standard library also provides a function for unwrapping errors, the Unwrap
function in the errors package. You pass it an error and it returns the wrapped error,
if there is one. If there isn’t, it returns nil. Here’s a quick program that demonstrates
wrapping with fmt.Errorf and unwrapping with errors.Unwrap. You can run it on
The Go Playground:
func fileChecker(name string) error {
 f, err := os.Open(name)
 if err != nil {
 return fmt.Errorf("in fileChecker: %w", err)
 }
 f.Close()
 return nil
}
func main() {
 err := fileChecker("not_here.txt")
 if err != nil {
 fmt.Println(err)
 if wrappedErr := errors.Unwrap(err); wrappedErr != nil {
 fmt.Println(wrappedErr)
 }
 }
}
When you run this program, you see the following output:
in fileChecker: open not_here.txt: no such file or directory
open not_here.txt: no such file or directory
168 | Chapter 8: Errors
You don’t usually call errors.Unwrap directly. Instead, you use
errors.Is and errors.As to find a specific wrapped error. We’ll
talk about these two functions in the next section.
If you want to wrap an error with your custom error type, your error type needs to
implement the method Unwrap. This method takes in no parameters and returns an
error. Here’s an update to the error that we defined earlier to demonstrate how this
works:
type StatusErr struct {
 Status Status
 Message string
 err error
In some cases, expecting}
func (se StatusErr) Error() string {
 return se.Message
}
func (se StatusError) Unwrap() error {
 return se.err
}
Now we can use StatusErr to wrap underlying errors:
func LoginAndGetData(uid, pwd, file string) ([]byte, error) {
 err := login(uid,pwd)
 if err != nil {
 return nil, StatusErr {
 Status: InvalidLogin,
 Message: fmt.Sprintf("invalid credentials for user %s",uid),
 Err: err,
 }
 }
 data, err := getData(file)
 if err != nil {
 return nil, StatusErr {
 Status: NotFound,
 Message: fmt.Sprintf("file %s not found",file),
 Err: err,
 }
 }
 return data, nil
}
Not all errors need to be wrapped. A library can return an error that means process‐
ing cannot continue, but the error message contains implementation details that
aren’t needed in other parts of your program. In this situation it is perfectly
Wrapping Errors | 169
acceptable to create a brand-new error and return that instead. Understand the situa‐
tion and determine what needs to be returned.
If you want to create a new error that contains the message from
another error, but don’t want to wrap it, use fmt.Errorf to create
an error, but use the %v verb instead of %w:
err := internalFunction()
if err != nil {
 return fmt.Errorf("internal failure: %v", err)
}
Is and As
Wrapping errors is a useful way to get additional information about an error, but it
introduces problems. If a sentinel error is wrapped, you cannot use == to check for it,
nor can you use a type assertion or type switch to match a wrapped custom error. Go
solves this problem with two functions in the errors package, Is and As.
To check if the returned error or any errors that it wraps match a specific sentinel
error instance, use errors.Is. It takes in two parameters, the error that is being
checked and the instance you are comparing against. The errors.Is function returns
true if there is an error in the error chain that matches the provided sentinel error.
Let’s write a short program to see errors.Is in action. You can run it yourself on The
Go Playground:
func fileChecker(name string) error {
 f, err := os.Open(name)
 if err != nil {
 return fmt.Errorf("in fileChecker: %w", err)
 }
 f.Close()
 return nil
}
func main() {
 err := fileChecker("not_here.txt")
 if err != nil {
 if errors.Is(err, os.ErrNotExist) {
 fmt.Println("That file doesn't exist")
 }
 }
}
Running this program produces the output:
That file doesn't exist
170 | Chapter 8: Errors
By default, errors.Is uses == to compare each wrapped error with the specified
error. If this does not work for an error type that you define (for example, if your
error is a noncomparable type), implement the Is method on your error:
type MyErr struct {
 Codes []int
}
func (me MyErr) Error() string {
 return fmt.Sprintf("codes: %v", me.Codes)
}
func (me MyErr) Is(target error) bool {
 if me2, ok := target.(MyErr); ok {
 return reflect.DeepEqual(me, me2)
 }
 return false
}
(We mentioned reflect.DeepEqual back in Chapter 3. It can compare anything,
including slices.)
Another use for defining your own Is method is to allow comparisons against errors
that aren’t identical instances. You might want to pattern match your errors, specify‐
ing a filter instance that matches errors that have some of the same fields. Let’s define
a new error type, ResourceErr:
type ResourceErr struct {
 Resource string
 Code int
}
func (re ResourceErr) Error() string {
 return fmt.Sprintf("%s: %d", re.Resource, re.Code)
}
If we want two ResourceErr instances to match when either field is set, we can do so
by writing a custom Is method:
func (re ResourceErr) Is(target error) bool {
 if other, ok := target.(ResourceErr); ok {
 ignoreResource := other.Resource == ""
 ignoreCode := other.Code == 0
 matchResource := other.Resource == re.Resource
 matchCode := other.Code == re.Code
 return matchResource && matchCode ||
 matchResource && ignoreCode ||
 ignoreResource && matchCode
 }
 return false
}
Is and As | 171
Now we can find, for example, all errors that refer to the database, no matter the
code:
if errors.Is(err, ResourceErr{Resource: "Database"}) {
 fmt.Println("The database is broken:", err)
 // process the codes
}
You can see this code on The Go Playground.
The errors.As function allows you to check if a returned error (or any error it
wraps) matches a specific type. It takes in two parameters. The first is the error being
examined and the second is a pointer to a variable of the type that you are looking for.
If the function returns true, an error in the error chain was found that matched, and
that matching error is assigned to the second parameter. If the function returns
false, no match was found in the error chain. Let’s try it out with MyErr:
err := AFunctionThatReturnsAnError()
var myErr MyErr
if errors.As(err, &myErr) {
 fmt.Println(myErr.Code)
}
Note that you use var to declare a variable of a specific type set to the zero value. You
then pass a pointer to this variable into errors.As.
You don’t have to pass a pointer to a variable of an error type as the second parameter
to errors.As. You can pass a pointer to an interface to find an error that meets the
interface:
err := AFunctionThatReturnsAnError()
var coder interface {
 Code() int
}
if errors.As(err, &coder) {
 fmt.Println(coder.Code())
}
We’re using an anonymous interface here, but any interface type is acceptable.
If the second parameter to errors.As is anything other than a
pointer to an error or a pointer to an interface, the method panics.
Just like you can override the default errors.Is comparison with an Is method, you
can override the default errors.As comparison with an As method on your error.
Implementing an As method is nontrivial and requires reflection (we will talk about
172 | Chapter 8: Errors
reflection in Go we in Chapter 14). You should only do it in unusual circumstances,
such as when you want to match an error of one type and return another.
Use errors.Is when you are looking for a specific instance or spe‐
cific values. Use errors.As when you are looking for a specific
type.
Wrapping Errors with defer
Sometimes you find yourself wrapping multiple errors with the same message:
func DoSomeThings(val1 int, val2 string) (string, error) {
 val3, err := doThing1(val1)
 if err != nil {
 return "", fmt.Errorf("in DoSomeThings: %w", err)
 }
 val4, err := doThing2(val2)
 if err != nil {
 return "", fmt.Errorf("in DoSomeThings: %w", err)
 }
 result, err := doThing3(val3, val4)
 if err != nil {
 return "", fmt.Errorf("in DoSomeThings: %w", err)
 }
 return result, nil
}
We can simplify this code by using defer:
func DoSomeThings(val1 int, val2 string) (_ string, err error) {
 defer func() {
 if err != nil {
 err = fmt.Errorf("in DoSomeThings: %w", err)
 }
 }()
 val3, err := doThing1(val1)
 if err != nil {
 return "", err
 }
 val4, err := doThing2(val2)
 if err != nil {
 return "", err
 }
 return doThing3(val3, val4)
}
We have to name our return values so that we can refer to err in the deferred func‐
tion. If you name a single return value, you must name all of them, so we use an
underscore here for the string return value since we don’t explicitly assign to it.
Wrapping Errors with defer | 173
In the defer closure, we check if an error was returned. If so, we reassign the error to
a new error that wraps the original error with a message that indicates which function
detected the error.
This pattern works well when you are wrapping every error with the same message. If
you want to customize the wrapping error to provide more context about what
caused the error, then put both the specific and the general message in every
fmt.Errorf.
panic and recover
In previous chapters, we’ve mentioned panics in passing without going into any
details on what they are. Go generates a panic whenever there is a situation where the
Go runtime is unable to figure out what should happen next. This could be due to a
programming error (like an attempt to read past the end of a slice) or environmental
problem (like running out of memory). As soon as a panic happens, the current func‐
tion exits immediately and any defers attached to the current function start running.
When those defers complete, the defers attached to the calling function run, and so
on, until main is reached. The program then exits with a message and a stack trace.
If there are situations in your programs that are unrecoverable, you can create your
own panics. The built-in function panic takes one parameter, which can be of any
type. Usually, it is a string. Let’s make a trivial program that panics and run it on The
Go Playground:
func doPanic(msg string) {
 panic(msg)
}
func main() {
 doPanic(os.Args[0])
}
Running this code produces the following output:
panic: /tmpfs/play
goroutine 1 [running]:
main.doPanic(...)
 /tmp/sandbox567884271/prog.go:6
main.main()
 /tmp/sandbox567884271/prog.go:10 +0x5f
As you can see, a panic prints out its message followed by a stack trace.
Go provides a way to capture a panic to provide a more graceful shutdown or to pre‐
vent shutdown at all. The built-in recover function is called from within a defer to
check if a panic happened. If there was a panic, the value assigned to the panic is
174 | Chapter 8: Errors
returned. Once a recover happens, execution continues normally. Let’s take a look
with another sample program. Run it on The Go Playground:
func div60(i int) {
 defer func() {
 if v := recover(); v != nil {
 fmt.Println(v)
 }
 }()
 fmt.Println(60 / i)
}
func main() {
 for _, val := range []int{1, 2, 0, 6} {
 div60(val)
 }
}
There’s a specific pattern for using recover. We register a function with defer to han‐
dle a potential panic. We call recover within an if statement and check to see if a
non-nil value was found. You must call recover from within a defer because once a
panic happens, only deferred functions are run.
Running this code produces the following output:
60
30
runtime error: integer divide by zero
10
While panic and recover look a lot like exception handling in other languages, they
are not intended to be used that way. Reserve panics for fatal situations and use
recover as a way to gracefully handle these situations. If your program panics, be
very careful about trying to continue executing after the panic. It’s very rare that you
want to keep your program running after a panic occurs. If the panic was triggered
because the computer is out of a resource like memory or disk space, the safest thing
to do is use recover to log the situation to monitoring software and shut down with
os.Exit(1). If there’s a programming error that caused the panic, you can try to con‐
tinue, but you’ll likely hit the same problem again. In the preceding sample program,
it would be idiomatic to check for division by zero and return an error if one was
passed in.
The reason we don’t rely on panic and recover is that recover doesn’t make clear
what could fail. It just ensures that if something fails, we can print out a message and
continue. Idiomatic Go favors code that explicitly outlines the possible failure condi‐
tions over shorter code that handles anything while saying nothing.
There is one situation where recover is recommended. If you are creating a library
for third parties, do not let panics escape the boundaries of your public API. If a
panic and recover | 175
panic is possible, a public function should use a recover to convert the panic into an
error, return it, and let the calling code decide what to do with them.
While the HTTP server built into Go recovers from panics in han‐
dlers, David Symonds said in a GitHub comment that this is now
considered a mistake by the Go team.
Getting a Stack Trace from an Error
One of the reasons why new Go developers are tempted by panic and recover is that
they want to get a stack trace when something goes wrong. By default, Go doesn’t
provide that. As we’ve shown, you can use error wrapping to build a call stack by
hand, but there are third-party libraries with error types that generate those stacks
automatically (see Chapter 9 to learn how to incorporate third-party code in your
program). The best known third-party library provides functions for wrapping errors
with stack traces.
By default, the stack trace is not printed out. If you want to see the stack trace, use
fmt.Printf and the verbose output verb (%+v). Check the documentation to learn
more.
When you have a stack trace in your error, the output includes the
full path to the file on the computer where the program was com‐
piled. If you don’t want to expose the path, use the -trimpath flag
when building your code. This replaces the full path with the
package.
Wrapping Up
In this chapter, we learned about errors in Go, what they are, how to define your own,
and how to examine them. We also took a look at panic and recover. In the next
chapter, we discuss packages and modules, how to use third-party code in your pro‐
grams, and how to publish your own code for others to use.




func divAndRemainder(numerator int, denominator int) (int, int, error) {
	if denominator == 0 {
	return 0, 0, errors.New("cannot divide by zero")
	}
	return numerator / denominator, numerator % denominator, nil
 }
 
 There’s something else that we haven’t seen yet: creating and returning an error. If
 you want to learn more about errors, skip ahead to Chapter 8. For now, you only need
 to know that you use Go’s multiple return value support to return an error if some‐
 thing goes wrong in a function. If the function completes successfully, we return nil
 for the error’s value. By convention, the error is always the last (or only) value
 returned from a function.
 Calling our updated function looks like this:
 func main() {
	result, remainder, err := divAndRemainder(5, 2)
	if err != nil {
	fmt.Println(err)
	os.Exit(1)
	}
	fmt.Println(result, remainder)
 }
 