func add(x, y int) int {
  return x + y
}

// multiple return
func swap(x, y string) (string, string) {
  return y, x
}

// named return parameters
func split(sum int) (x, y string) {
  x = sum * 4 / 9
  y = sum - a
  return
}

// recursion
func factorial(n int) int {
  if n == 0 {
    return 1
  }

  return n * factorial(n-1)
}





// a function declartion includes the name of the function, the input parameters, and the return type

func div(numerator int, denominator int) int {
 if denominator == 0 {
 return 0
 }
 return numerator / denominator
}


func main() {
 result := div(5, 2)
 fmt.Println(result)
}


When you have multiple input parameters of the same type, you
can write your input parameters like this:

func div(numerator, denominator int) int {


// Multiple Return Values

Go allows for multiple return values.

func divAndRemainder(numerator int, denominator int) (int, int, error) {
 if denominator == 0 {
 return 0, 0, errors.New("cannot divide by zero")
 }
 return numerator / denominator, numerator % denominator, nil
}

func main() {
 result, remainder, err := divAndRemainder(5, 2)
 if err != nil {
 fmt.Println(err)
 os.Exit(1)
 }
 fmt.Println(result, remainder)
}


If a
function returns multiple values, but you don’t need to read one or more of the val‐
ues, assign the unused values to the name _.

result, _, err := divAndRemain
der(5, 2).

Surprisingly, Go does let you implicitly ignore all of the return values for a function.
You can write divAndRemainder(5,2) and the returned values are dropped.



// Named Return Values

Go also allows you to specify names for your return values

func divAndRemainder(numerator int, denominator int) (result int, remainder int,
 err error) {
 if denominator == 0 {
 err = errors.New("cannot divide by zero")
 return result, remainder, err
 }
 result, remainder = numerator/denominator, numerator%denominator
 return result, remainder, err
}

When you supply names to your return values, what you are doing is pre-declaring variables that you use within the function to hold the return values.

x, y, z := divAndRemainder(5, 2)
fmt.Println(x, y, z)

If you only want to name some of the return values, you can do so
by using _ as the name for any return values you want to remain
nameless.

The other problem with named return values is that you don’t have to return them.

func divAndRemainder(numerator, denominator int) (result int, remainder int,
 err error) {
 // assign some values
 result, remainder = 20, 30
 if denominator == 0 {
 return 0, 0, errors.New("cannot divide by zero")
 }
 return numerator / denominator, numerator % denominator, nil
}


Notice that we assigned values to result and remainder and then returned different
values directly. Before running this code, try to guess what happens when we pass 5
and 2 to this function. The result might surprise you:
2 1
The values from the return statement were returned even though they were never
assigned to the named return parameters. That’s because the Go compiler inserts code
that assigns whatever is returned to the return parameters. The named return param‐
eters give a way to declare an intent to use variables to hold the return values, but
don’t require you to use them.



Blank Returns—Never Use These!

If you have named return values, you can
just write return without specifying the values that are returned. This returns the last
values assigned to the named return values.

func divAndRemainder(numerator, denominator int) (result int, remainder int,
 err error) {
 if denominator == 0 {
 err = errors.New("cannot divide by zero")
 return
 }
 result, remainder = numerator/denominator, numerator%denominator
 return
}

Using blank returns makes a few additional changes to our function. When there’s
invalid input, we return immediately. Since no values were assigned to result and
remainder, their zero values are returned. If you are returning the zero values for
your named return values, be sure they make sense. Also notice that we still have to
put a return at the end of the function. Even though we are using blank returns, this
function returns values. It is a compile-time error to leave return out.
At first, you might find blank returns handy since they allow you to avoid some typ‐
ing. However, most experienced Go developers consider blank returns a bad idea
because they make it harder to understand data flow. Good software is clear and read‐
able; it’s obvious what is happening. When you use a blank return, the reader of your
code needs to scan back through the program to find the last value assigned to the
return parameters to see what is actually being returned.
If your function returns values, never use a blank return. It can
make it very confusing to figure out what value is actually returned.





Functions Are Values

functions are values

the type of a function (called its signature) is built out of the keyword func and the types of the parameters and return values

any function that has the exact same number and types of parameters and return values meets the type signature


func add(i int, j int) int { return i + j }
func sub(i int, j int) int { return i - j }
func mul(i int, j int) int { return i * j }
func div(i int, j int) int { return i / j }

var opMap = map[string]func(int, int) int{
 "+": add,
 "-": sub,
 "*": mul,
 "/": div,
}

func main() {
 expressions := [][]string{
 []string{"2", "+", "3"},
 []string{"2", "-", "3"},
 []string{"2", "*", "3"},
 []string{"2", "/", "3"},
 []string{"2", "%", "3"},
 []string{"two", "+", "three"},
 []string{"5"},
 }
 for _, expression := range expressions {
 if len(expression) != 3 {
 fmt.Println("invalid expression:", expression)
 continue
 }
 p1, err := strconv.Atoi(expression[0])
 if err != nil {
 fmt.Println(err)
 continue
 }
 op := expression[1]
 opFunc, ok := opMap[op]
 if !ok {
 fmt.Println("unsupported operator:", op)
 continue
 }
 p2, err := strconv.Atoi(expression[2])
 if err != nil {
 fmt.Println(err)
 continue
 }
 result := opFunc(p1, p2)
 fmt.Println(result)
 }
}


Function Type Declarations

Just like you can use the type keyword to define a struct, you can use it to define a
function type, too (we’ll go into more details on type declarations in Chapter 7):
type opFuncType func(int,int) int
We can then rewrite the opMap declaration to look like this:
var opMap = map[string]opFuncType {
 // same as before
}
We don’t have to modify the functions at all. Any function that has two input parame‐
ters of type int and a single return value of type int automatically meets the type and
can be assigned as a value in the map.
What’s the advantage of declaring a function type? One use is documentation. It’s use‐
ful to give something a name if you are going to refer to it multiple times. We will see
another use in “Function Types Are a Bridge to Interfaces” on page 154.
Anonymous Functions
Not only can you assign functions to variables, you can also define new functions
within a function and assign them to variables.
These inner functions are anonymous functions; they don’t have a name. You don’t
have to assign them to a variable, either. You can write them inline and call them
immediately. Here’s a simple example that you can run on The Go Playground:
func main() {
 for i := 0; i < 5; i++ {
96 | Chapter 5: Functions
 func(j int) {
 fmt.Println("printing", j, "from inside of an anonymous function")
 }(i)
 }
}
You declare an anonymous function with the keyword func immediately followed by
the input parameters, the return values, and the opening brace. It is a compile-time
error to try to put a function name between func and the input parameters.
Just like any other function, an anonymous function is called by using parenthesis. In
this example, we are passing the i variable from the for loop in here. It is assigned to
the j input parameter of our anonymous function.
Running the program gives the following output:
printing 0 from inside of an anonymous function
printing 1 from inside of an anonymous function
printing 2 from inside of an anonymous function
printing 3 from inside of an anonymous function
printing 4 from inside of an anonymous function
Now, this is not something that you would normally do. If you are declaring and exe‐
cuting an anonymous function immediately, you might as well get rid of the anony‐
mous function and just call the code. However, there are two situations where
declaring anonymous functions without assigning them to variables is useful: defer
statements and launching goroutines. We’ll talk about defer statements in a bit.
Goroutines are covered in Chapter 10.








Go Is Call By Value


// arguments are passed by value and so a copy is made of the value of a variable used as an argument to a function call

type person struct {
 age int
 name string
}

func modifyFails(i int, s string, p person) {
 i = i * 2
 s = "Goodbye"
 p.name = "Bob"
}

func main() {
 p := person{}
 i := 2
 s := "Hello"
 modifyFails(i, s, p)
 fmt.Println(i, s, p)
}

This isn’t just true for primitive types.

The behavior is a little different for maps and slices.

func modMap(m map[int]string) {
 m[2] = "hello"
 m[3] = "goodbye"
 delete(m, 1)
}
func modSlice(s []int) {
 for k, v := range s {
 s[k] = v * 2
 }
 s = append(s, 10)
}

func main() {
 m := map[int]string{
 1: "first",
 2: "second",
 }
 modMap(m)
 fmt.Println(m)
 s := []int{1, 2, 3}
 modSlice(s)
 fmt.Println(s)
}

For the map, it’s easy to explain what happens: any changes made to a map parameter
are reflected in the variable passed into the function. For a slice, it’s more complicated. You can modify any element in the slice, but you can’t lengthen the slice. This is
true for maps and slices that are passed directly into functions as well as map and
slice fields in structs.

This program leads to the question: why do maps and slices behave differently than other types? It’s because maps and slices are both implemented with pointers.

Every type in Go is a value type. It’s just that sometimes the value is a pointer.


Go is a call by value language. Every time you pass a parameter to a function, Go makes a copy of the value that’s passed in.
