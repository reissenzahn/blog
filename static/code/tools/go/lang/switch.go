package main

func main() {

}


// you can switch on any type that can be compared using == (which excludes slices, maps, channels, functions and structs that contain fields of these types)
answer := 'y'

switch answer {
case 'y':
  fmt.Println("Yes!")
case 'n':
  fmt.Println("No!")
default:
  fmt.Println("Huh?")
}

switch time.Now().Weekday() {
// a single case can list multiple values
case time.Saturday, time.Sunday:
  fmt.Println("It's the weekend.")
default:
  fmt.Println("It's a weekday.")
}



// 
switch os := runtime.GOOS; os {
case "darwin":
  fmt.Println("OS X")
case "linux":
  fmt.Println("Linux")
default:
  fmt.Printf("%s.\n", os)
}

// a blank switch allows you to use any boolean comparison for each case
t := time.Now()
switch {
// a case can also evaluate an expression
case t.Hour() < 12:
  fmt.Println("It's before noon")
default:
  fmt.Println("It's after noon")
}

// a black switch also allows variables to be defined
switch n := rand.Intn(10); {
case n < 3:
  fmt.Println("too small")
case n > 9:
  fmt.Println("too big", n)
default:
  fmt.Println("good")
}


switch size := len(word); size {
case 1, 2, 3, 4:
  fmt.Println("short")
case 5:
  // a case is a new block so variables can be declared that are only visible within that block
  msg := "exactly right!"
  fmt.Println(msg)
 case 6, 7, 8, 9:
 default:
 fmt.Println(word, "is a long word!")
 }
}

// the fallthrough keyword causes execution to continue in the next branch


// cases statements do not require an explicit break though the break keyword can be used to exit early


 

