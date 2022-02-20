package main

import "fmt"

func main() {

	// a select simultaneously considers the channel reads and writes specified by its case statements to see if any of them are ready in which case it executes the corresponding statements
	c1 := make(<-chan string)
	c2 := make(chan<- string)

	select {
	// a read operation is ready if the channel has a value to read or is closed
	case v := <-c1:
		fmt.Printf("read: %s\n", v)
	// a write operation is ready if the channel has not reached capacity
	case c3<-"hello":
		fmt.Println("wrote: hello")
	}


	// if none of the channels are ready then the select blocks until one of the channels is ready
	c3 := make(<-chan string)
	
	select {
	case <-c3:
	case <-time.After(1 * time.Second):
		fmt.Println("timed out!")
	}


	// if there are multiple ready channels then each case statement will have an equal chance to be selected
	c4 := make(chan interface{});
	c5 := make(chan interface{});

	close(c4)
	close(c5)

	var n, m int

	for i := 1000; i >= 0; i-- {
		select {
		case <-c4:
			n++
		case <-c5:
			m++
		}
	}

	fmt.Println(n)  // 
	fmt.Println(m)  // 


	// a default clause can be included that will execute if all the channels are blocking
	done := make(chan interface{})
	count := 0
	
	go func() {
	 time.Sleep(2 * time.Second)
	 close(done)
	}()
	
	loop:
	for {
		select {
		case <-done:
			break loop
		default:
		}
	
		// do some work
	
		count++
		time.Sleep(1 * time.Second)
	}
	
	fmt.Printf(count)
}