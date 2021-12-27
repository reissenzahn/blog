package main

import "fmt"

// the for-range loop can be used to iterate over the built-in compound types

func main() {

  // the first variable is the position/key while the second is the value corresponding to that position/key
  primes := []int{2, 3, 5, 7, 11}
  for i, v := range primes {
    fmt.Printf("%d: %d\n", i, v)
  }


  // the value from the compound type is copied to the value variable at each iteration
  for _, v := range primes {
    v *= 2
  }
  fmt.Println(primes)  // []


  // an underscore can be used if the key is not required
  for _, v := range primes {
  fmt.Println(v)  // 
  }

  // the second variable can be omitted if the value is not required
  for k := range primes {
  fmt.Println(k)
  }


  // the order of the key-value pairs varies for security reasons
  counts := map[string]int{"apple": 2, "banana": 5, "orange": 3}
  for i := 0; i < 3; i++ {
    for k, v := range counts {
      fmt.Printf("%s: %d\n", k, v)
    }

    fmt.Println()
  }


  // a for-range iterates over the runes of a string rather than its bytes so the the key is the number of bytes from the beginning of the string but the value is a rune
  message := "pi_π!"
  for i, r := range message {
    fmt.Printf("%d: %c (%d)\n", i, r, r)
  }
}