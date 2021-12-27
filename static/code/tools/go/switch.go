switch answer {
case 'y':
  fmt.Println("Yes!")
case 'n':
  fmt.Println("No!")
default:
  fmt.Println("Huh?")
}

switch time.Now().Weekday() {
case time.Saturday, time.Sunday:
  fmt.Println("It's the weekend.")
default:
  fmt.Println("It's a weekday.")
}

t := time.Now()
switch {  // switch true
case t.Hour() &lt; 12:
  fmt.Println("It's before noon")
default:
  fmt.Println("It's after noon")
}

switch os := runtime.GOOS; os {
case "darwin":
  fmt.Println("OS X")
case "linux":
  fmt.Println("Linux")
default:
  fmt.Printf("%s.\n", os)
}




// switch statements

words := []string{"a", "cow", "smile", "gopher", "octopus", "anthropologist"}
for _, word := range words {
 switch size := len(word); size {
 case 1, 2, 3, 4:
 fmt.Println(word, "is a short word!")
 case 5:
 wordLen := len(word)
 fmt.Println(word, "is exactly the right length:", wordLen)
 case 6, 7, 8, 9:
 default:
 fmt.Println(word, "is a long word!")
 }
}

the case clauses (and the optional default clause) are contained inside a set of
braces. But you should note that you don’t put braces around the contents of the case
clauses. You can have multiple lines inside a case (or default) clause and they are all
considered to be part of the same block.
Inside case 5:, we declare wordLen, a new variable. Since this is a new block, you can
declare new variables within it. Just like any other block, any variables declared
within a case clause’s block are only visible within that block.

By default, cases in switch statements in Go don’t fall through.

This prompts the question: if cases don’t fall through, what do you do if there are
multiple values that should trigger the exact same logic? In Go, you separate multiple
matches with commas.

an empty case means nothing happens.

Go does include a fallthrough keyword, which lets one case continue on to the next one

You can switch on any type that can be compared with ==, which includes
all of the built-in types except slices, maps, channels, functions, and structs that con‐
tain fields of these types.

Even though you don’t need to put a break statement at the end of each case clause,
you can use them in situations where you want to exit early from a case

func main() {
loop:
 for i := 0; i < 10; i++ {
 switch {
 case i%2 == 0:
 fmt.Println(i, "is even")
 case i%3 == 0:
 fmt.Println(i, "is divisible by 3 but not 2")
 case i%7 == 0:
 fmt.Println("exit the loop!")
80 | Chapter 4: Blocks, Shadows, and Control Structures
 break loop
 default:
 fmt.Println(i, "is boring")
 }
 }
}

you can write a switch statement that doesn’t specify the value that you’re comparing against. This is called a
blank switch. A regular switch only allows you to check a value for equality. A blank
switch allows you to use any boolean comparison for each case.

words := []string{"hi", "salutations", "hello"}
for _, word := range words {
 switch wordLen := len(word); {
 case wordLen < 5:
 fmt.Println(word, "is a short word!")
 case wordLen > 10:
 fmt.Println(word, "is a long word!")
 default:
 fmt.Println(word, "is exactly the right length.")
 }
}

Just like a regular switch statement, you can optionally include a short variable decla‐
ration as part of your blank switch.

switch n := rand.Intn(10); {
case n == 0:
 fmt.Println("That's too low")
case n > 5:
 fmt.Println("That's too big:", n)
default:
 fmt.Println("That's a good number:", n)
}