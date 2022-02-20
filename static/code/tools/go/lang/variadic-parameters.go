package main

import "fmt"

// variadic parameters must be the last (or only) parameter in the input parameter list
func sum(values ...int) {
  total := 0

  // the variable created in the function is a slice of the specified type
  for _, value := range values {
    total += value
  }

  fmt.Println(total)
}

func main() {

  // you to supply however many values you want for a variadic parameter (or no values at all)
  fmt.Println(sum())        // 0
  fmt.Println(sum(1))       // 1
  fmt.Println(sum(1, 2, 3)) // 6

  // a slice literal can be passed as the input to a variadic parameter
  values := []int{1, 2, 3}
  sum(values...)  // 6
}