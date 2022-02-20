package main

func main() {
  // a map is a collection that associates keys with values

  // the type of the value can be anything though the type of the key must be a type for which the equality operator is defined
  fruits := map[string]int{"apple": 2, "banana": 5, "orange": 3}
  
  fruits["apple"] = 1
  fruits["orange"]++
  fruits["pear"] = 4
  
  fmt.Println(fruits["orange"])  // 4

  // the built-in len() function returns the number of key-value pairs in the map
  fmt.Println(len(fruits))
  
  // the built-in delete() function deletes a key-value pair
  delete(fruits, "orange")

  // if the key is not in the map then nothing happens
  delete(fruits, "orange")


  // maps hold references to an underlying data structure and so if you pass a map to a function that changes the contents of the map then the changes will be visible in the caller
  func update(m map[string]int, k string, v int) {
    m[k] = v
  }
  
  update(fruits, "apple", 5)
  fmt.Println(fruits["apple"])


  // an empty map literal can be used to initialize an empty map
  letters := map[rune]int{}

  letters['a'] = 3
  letters['b'] = 4
  
  // reading the value assigned to a key that was never set returns the zero-value for the value type
  fmt.Println(letters['c'])

  // we can check if the the key was present in the map using the comma-ok idiom
  if v, ok := letters['c']; ok {
    fmt.Println(v)
  } else {
    fmt.Println("not found")
  }


  // the zero value for a map is nil
  var empty map[string]int

  // looking up a key in a nil map always returns the zero-value for the value type
  fmt.Println(empty["key"])  // 0

  // attempting to insert a value into a nil map causes a panic
  empty["key"] = 1


  // we can also use the built-in make() function to create a map
  set := make(map[int]bool)

  set[7] = true
  
  if set[7] {
    fmt.Println("in set")
  }


  // using make() allows us to create a map with a default size
  ages := make(map[int][]string, 10)

  // the map will still have a length of 0 and will still be able to grow past the initially specified size
  fmt.Println(len(ages)) // 0  
}