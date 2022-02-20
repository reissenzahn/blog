



func main() {
  // a function declartion includes the name of the function, the input parameters, and the return type
  func add(x, y int) int {
    return x + y
  }

  fmt.Println(add(1, 2))
  
  // multiple return values can be specified
  func swap(x, y string) (string, string) {
    return y, x
  }

  // the name _ can be used to ignore the return value of a function
  second, _ = swap("abc", "xyz")
  fmt.Println(second)
  
  // return parameters can be named
  func divide(x, y int) (result, remainder int, err error) {
    if y == 0 {
      err = errors.New("divided by zero");
      return
    }
  
    result = x/y
    remainder = x%y
  
    // named return values allow us to return the last values assigned to the named return values using return without any values
    return
  }

  // the type of a function (called its signature) is built out of the keyword func and the types of the parameters and return values
  type operation func(int, int) int

  var op map[string]func(int, int) int = add

  fmt.Println(op(1, 2))

  // any function that has the exact same number and types of parameters and return values meets the type signature
  func sub(x, y int) int {
    return x - y
  }

  op = sub
  fmt.Println(op(1, 2))


  // anonymous functions are functions without names and they can be assigned to variables
  var greet = func(who string) {
    fmt.Println("Hello, ", who)
  }

  greet("Joe")


  // arguments are passed by value and so a copy is made of the value of a variable used as an argument to a function call
  type point struct {
    x int
    y int
  }
  
  func center(p point) {
    p.x = 0
    p.y = 0
  }
  
  p := person{}
  p.x = 1
  center(p)
  fmt.Println(p.x)

  // however sometimes the value is a pointer (such as in the case of maps and slices)
  func clear(s []int) {
    for k, v := range s {
      s[k] = 0
    }
  }

  s := []int{1, 2, 3}
  clear(s)
  fmt.Println(s)
}
