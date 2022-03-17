package main

import (
	"fmt"
	"sync"
	"time"
)

// fan-out evenly distributes messages from an input channel to multiple output channels

func FanOut(in <-chan int, n int) []<-chan int {
	outs := make([]<-chan int, 0)

	// we create n out channels to forward values to
	for i := 0; i < n; i++ {
		ch := make(chan int)
		outs = append(outs, ch)

		// each out channel has a go routine that competes for reads from the in channel
		go func() {
			// if source is closed then we close all out channels
			defer close(ch)

			for val := range in {
				ch <- val
			}
		}()
	}

	return outs
}

func main() {
	in := make(chan int)

	outs := FanOut(in, 5)

	go func() {
		for i := 1; i <= 10; i++ {
			in <- i
			time.Sleep(time.Second)
		}

		close(in)
	}()

	var wg sync.WaitGroup

	wg.Add(len(outs))

	for i, out := range outs {
		go func(i int, ch <-chan int) {
			defer wg.Done()

			for val := range ch {
				fmt.Printf("(%d) %d\n", i, val)
			}
		}(i, out)
	}

	wg.Wait()
}
