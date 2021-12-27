

func sequence() func() int {
  i := 0
  return func() int {
    i++
    return i
  }
}

next := sequence()

fmt.Println(next())
fmt.Println(next())
fmt.Println(next())


// Closures

Functions declared inside of functions are special; they are closures. This means that functions declared inside of functions are able to
access and modify variables declared in the outer function.

Closures really become interesting when they are passed to other functions or
returned from a function. They allow you to take the variables within your function
and use those values outside of your function.


Since functions are values and you can specify the type of a function using its parameter and return types, you can pass functions as parameters into functions.

type Person struct {
 FirstName string
 LastName string
 Age int
}
people := []Person{
 {"Pat", "Patterson", 37},
 {"Tracy", "Bobbert", 23},
 {"Fred", "Fredson", 18},
}
fmt.Println(people)
Next, we’ll sort our slice by last name and print out the results:
// sort by last name
sort.Slice(people, func(i int, j int) bool {
 return people[i].LastName < people[j].LastName
})
fmt.Println(people)
The closure that’s passed to sort.Slice has two parameters, i and j, but within the
closure, we can refer to people so we can sort it by the LastName field. In computer
science terms, people is captured by the closure. Next we do the same, sorting by the
Age field:
// sort by age
sort.Slice(people, func(i int, j int) bool {
 return people[i].Age < people[j].Age
})
fmt.Println(people)
Running this code gives the following output:
[{Pat Patterson 37} {Tracy Bobbert 23} {Fred Fredson 18}]
[{Tracy Bobbert 23} {Fred Fredson 18} {Pat Patterson 37}]
[{Fred Fredson 18} {Tracy Bobbert 23} {Pat Patterson 37}]
The people slice is changed by the call to sort.Slice. We talk about this briefly in
“Go Is Call By Value” on page 104 and in more detail in the next chapter.
Passing functions as parameters to other functions is often useful
for performing different operations on the same kind of data.
Returning Functions from Functions
Not only can you use a closure to pass some function state to another function, you
can also return a closure from a function. Let’s show this off by writing a function
that returns a multiplier function. You can run this program on The Go Playground.
Here is our function that returns a closure:
func makeMult(base int) func(int) int {
 return func(factor int) int {
 return base * factor
 }
}
And here is how we use it:
func main() {
 twoBase := makeMult(2)
 threeBase := makeMult(3)
 for i := 0; i < 3; i++ {
 fmt.Println(twoBase(i), threeBase(i))
 }
}

Running this program gives the following output:
0 0
2 3
4 6