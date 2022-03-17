package main

import (
	"context"
	"fmt"
	"sync"
	"time"
)

// future provides a placeholder for a value that is still being generated by an asynchronous process

type Future interface {
	Result() (string, error)
}

type InnerFuture struct {
	once sync.Once
	wg sync.WaitGroup
	res string
	err error
	resCh <-chan string
	errCh <-chan error
}

func NewInnerFuture(resCh <-chan string, errCh <-chan error) *InnerFuture {
	return &InnerFuture{
		resCh: resCh,
		errCh: errCh,
	}
}

func (self *InnerFuture) Result() (string, error) {
	self.once.Do(func() {
		self.wg.Add(1)
		defer self.wg.Done()

		self.res = <-self.resCh
		self.err = <-self.errCh
	})

	self.wg.Wait()

	return self.res, self.err
}

func SlowFunction(ctx context.Context) Future {
	resCh := make(chan string)
	errCh := make(chan error)

	go func() {
		select {
		case <-time.After(time.Second * 2):
			resCh <- "done!"
			errCh <- nil
		case <-ctx.Done():
			resCh <- ""
			errCh <- ctx.Err()
		}
	}()

	return NewInnerFuture(resCh, errCh)
}

func main() {
	ctx := context.Background()

	future := SlowFunction(ctx)

	res, err := future.Result()
	if err != nil {
		panic(err)
	}

	fmt.Println(res)
}
