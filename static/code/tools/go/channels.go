package main

import "fmt"

func main() {

	// declare a bidirectional channel of type string
	var c1 chan string

	// we can instantiate a channel using the built-in make() function
	c1 = make(chan string)


	// we use the <- operator to write values to and read values from a channel
	c2 := make(chan string)

	go func() {
	c2 <- "hello"
	}()

	// writes to full channels will block and reads from empty channels will block
	fmt.Println(<-c2)  // hello


	// a unidirectional channel can either only read values (<-chan) or write values (chan<-)
	var c3 <-chan int
	var c4 chan<- int

	// bidirectional channels are implicitly converted to unidirectional channels when needed
	c5 := make(chan int)

	c3 = c5
	c4 = c5

	go func() {
		c4 <- 5
	}

	fmt.Println(<-c3)  // 5


	// closing a channel indicates that there will be no more values written to the channel
	c6 := make(chan bool)
	close(c6)

	// reads from a closed channel will return the zero value for the type of the channel
	fmt.Println(<-c6)  // false

	// the second value returned by a read will be false if the channel has been closed
	if v, ok := <-c6; !ok {
		fmt.Println("closed")  // closed
	}

	// closing a channel that has already been closed will panic
	// close(c6)  // panic


	// a for-range loop can be used to read from a channel and break when the channel is closed
	c7 := make(chan int)

	go func() {
		defer close(c7)
		for i := 1; i <= 5; i++ {
			c7 <- i
		}
	}()

	for v := range c7 {
		fmt.Println(v)
	}


	// buffered channels are channels which have a specified capacity (un-buffered channels essentially have a capacity of zero)
	var c8 chan rune
	c8 = make(chan rune, 3)

	// we can write values to a buffered channel in absence of any reads without blocking until the capacity is reached
	c8 <- 'A'
	c8 <- 'B'
	c8 <- 'C'
	// c8 <- 'D'  // block

	// values are read from a buffered channel in fifo order
	for v := range c8 {
		fmt.Println(v)
	}


	// the default value for a channel is nil
	var c9 chan interface{}

	// reading from a nil channel will block
	// <-c9  // block

	// writes to a nil channel will also block
	// c9 <- struct{}{}  // block

	// closing a nil channel will panic
	// close(c9)  // panic
}