package main

import "fmt"

// iota lets you assign an increasing value to a set of constants

// we typically first define a type based on int that will represent all of the valid values
type Status int

// then we use a const block to define a set of values for the type
const (
	// the first constant in the const block has the type specified and its value is set to iota
	Loading Status = iota
	
	// every subsequent line gets the same type and assignment where iota is incrmeneted on each line
	Ready
	Working
	Done
)


// nevertheless there is nothing stopping you from creating additional values of your type

// if you insert a new identifier in the middle of your list of literals then all of the subsequent ones will be renumbered

// iota based enumerations only make sense when you care about being able to differentiate between a set of values but do not care what the values actually are

type BitField int
const (
 Field1 BitField = 1 << iota // assigned 1
 Field2 // assigned 2
 Field3 // assigned 4
 Field4 // assigned 8
)

// iota starts numbering from 0

// if zero is nonsensical then a common pattern is to assign the first iota value in the constant block to _ or to a constant that indicates the value is invalid


