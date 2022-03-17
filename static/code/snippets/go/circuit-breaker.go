package main

import (
	"context"
	"errors"
	"fmt"
	"time"
	"sync"
)

// circuit breaker automatically degrades service functions in response to a likely fault, preventing larger or cascading failures by eliminating recurring errors and providing reasonable error responses

// signature of the function that is interacting with the service
type Circuit func(ctx context.Context, args interface{}) (interface{}, error)

func WithBreaker(circuit Circuit, failureThreshold int) Circuit {
	var mutex sync.RWMutex
	
	consecutiveFailures := 0
	lastAttempt := time.Now()

	return func(ctx context.Context, args interface{}) (interface{}, error) {
		mutex.RLock()
		
		// check if failure threshold exceeded
		if consecutiveFailures >= failureThreshold {
			// timeout is calculated with an exponential backoff in which the duration of the delays betweens retries roughly doubles each attempt
			shouldRetryAt := lastAttempt.Add(time.Second * 2 << (consecutiveFailures - failureThreshold))

			// a reset mechanism allows requests to call circuit again after a timeout
			if !time.Now().After(shouldRetryAt) {
				mutex.RUnlock()
				return nil, errors.New("service unreachable")
			}
		}
	
		mutex.RUnlock()

		result, err := circuit(ctx, args)

		mutex.Lock()
		defer mutex.Unlock()

		lastAttempt = time.Now()

		if err != nil {
			consecutiveFailures++
			return result, err
		}

		// any successful calls to circuit cause the consecutive failure count to be reset
		consecutiveFailures = 0

		return result, nil
	}
}

func Broken(context context.Context, args interface{}) (interface{}, error) {
	return nil, errors.New("something went wrong")
}

func main() {
	broken := WithBreaker(Broken, 3)

	for i := 0; i < 10; i++ {
		_, err := broken(context.Background(), nil)
		if err != nil {
			fmt.Println(err)
		}
	}
}