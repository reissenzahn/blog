package main

import (
  "fmt"
  "math"
)

// a struct is a typed collection of fields
type Circle struct {
  radius float64
  color  string
}

// methods can be defined for either pointer or value receiver types
func (c Circle) Area() float64 {
  return math.Pi * c.radius * c.radius
}

func (c *Circle) SetColor(color string) {

  // we use a pointer receiver type to avoid copying on method calls or to allow the method to mutate the receiving struct
  c.color = color
}


func main() {

  // a zero value struct has every field set to the zero-value of that field
  var c1 Circle

  // a struct can be initialized with a struct literal
  c1 = Circle{}
  fmt.Println(c1)

  // a struct literal can either provide a list of values for all the fields in the struct or explicitly provide values for fields by name
  c2 := Circle{2.0, "red"}
  fmt.Println(c2)

  c3 := Circle{radius: 2.0, color: "red"}
  fmt.Println(c3)
  
  // an anonymous struct can be used to create a variable of a struct type without an explicit name
  c5 := struct {
    radius float64
    color  string
  }{
    radius: 5.0,
    color: "blue",
  }
  fmt.Println(c5)
    

  // fields and methods are accessed with the dot operator
  c4 := Circle{radius: 3.0, color: "red"}
  c4.radius = 4.0
  c4.SetColor("blue")
  fmt.Println(c4)


  // the new() function can be used to return a pointer to a struct literal
  p1 := new(Circle)
  fmt.Printf("%T\n", p1) // *main.Circle

  // we can also use the address of operator on a struct literal
  p2 := &Circle{radius: 2.0}
  fmt.Printf("%T\n", p2) // *main.Circle

  // pointers to structs are automatically dereferenced when accessing fields or calling methods
  p3 := &Circle{radius: 2.0}
  p3.color = "blue"
  fmt.Println(p3.Area())


  // structs that are entirely composed of comparable types are comparable
  fmt.Println(c2 == c3)  // true
}