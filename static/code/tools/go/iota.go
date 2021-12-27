package main

import "fmt"


// iota lets you assign an increasing value to a set of constants

// we typically first define a type based on int that will represent all of the valid values
type Status int

// then we use a const block to define a set of values for the type
const (
 Loading Status = iota
 Ready
 Working
 Done
)

//  the first constant in the const block has the type specified and its value is set to iota


Every subsequent line has neither the type nor a value assigned to it. When the Go
compiler sees this, it repeats the type and the assignment to all of the subsequent con‐
stants in the block, and increments the value of iota on each line. This means that it
assigns 0 to the first constant (Uncategorized), 1 to the second constant (Personal),
and so on. When a new const block is created, iota is set back to 0.



// nevertheless there is nothing stopping 


The important thing to understand is that there is nothing in Go to stop you (or any‐
one else) from creating additional values of your type. Furthermore, if you insert a
new identifier in the middle of your list of literals, all of the subsequent ones will be
renumbered.

iotabased enumerations only make sense when you care about being able to differentiate
between a set of values, and don’t particularly care what the value is behind the
scenes

Because you can assign a literal expression to a constant, you’ll see
sample code that suggests you should use iota for cases like this:
type BitField int
const (
 Field1 BitField = 1 << iota // assigned 1
 Field2 // assigned 2
 Field3 // assigned 4
 Field4 // assigned 8
)
While this is clever, be careful when using this pattern. If you do so,
document what you are doing. As mentioned previously, using
iota with constants is fragile when you care about the value. You
don’t want a future maintainer to insert a new constant in the mid‐
dle of the list and break your code.

Be aware that iota starts numbering from 0. If you are using your set of constants to
represent different configuration states, the zero value might be useful. We saw this
earlier in our MailCategory type. When mail first arrives, it is uncategorized, so the
zero value makes sense. If there isn’t a sensical default value for your constants, a
common pattern is to assign the first iota value in the constant block to _ or to a
constant that indicates the value is invalid. This makes it easy to detect when a vari‐
able has not been properly initialized.
