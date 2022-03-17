package main

import (
	"fmt"
	"sync"
	"time"
)

// fan-in multiplexes multiple input channels onto one output channel

func FanIn(ins ...<-chan int) <-chan int {
	out := make(chan int)

	var wg sync.WaitGroup

	wg.Add(len(ins))
	
	for _, in := range ins {
		go func(ch <-chan int) {
			defer wg.Done()

			for n := range ch {
				out <- n
			}
		}(in)
	}

	// close out after all sources close
	go func() {
		wg.Wait()
		close(out)
	}()

	return out
}

func main() {
	ins := make([]<-chan int, 0)

	for i := 0; i < 3; i++ {
		ch := make(chan int)

		ins = append(ins, ch)

		go func() {
			defer close(ch);

			for i := 1; i <= 5; i++ {
				ch <- i
				time.Sleep(time.Second)
			}
		}()
	}

	out := FanIn(ins...)

	for val := range out {
		fmt.Println(val)
	}
}
