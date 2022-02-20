package main

// Circuit Breaker automatically degrades service functions in response to a likely fault, preventing larger or cascading failures by eliminating recurring errors and providing reasonable error responses.

import (
	"context"
	"sync"
	"time"
	"errors"
	"log"
)

type Circuit func(context.Context) (string, error)

func Breaker(circuit Circuit, failureThreshold uint) Circuit {
	var m sync.RWMutex
	consecutiveFailures := 0
	lastAttempt := time.Now()
	
	return func(ctx context.Context) (string, error) {
		m.RLock()

 		d := consecutiveFailures - int(failureThreshold)
 		
		if d >= 0 {
 			shouldRetryAt := lastAttempt.Add(time.Second * 2 << d)
 			
			if !time.Now().After(shouldRetryAt) {
				m.RUnlock()
				return "", errors.New("service unreachable")
 			}
 		}

		m.RUnlock()
		
		res, err := circuit(ctx)

		m.Lock()
		defer m.Unlock()
		
		lastAttempt = time.Now()
		
		if err != nil {
			consecutiveFailures++
			return res, err
		}

		consecutiveFailures = 0
		
		return res, nil
	}
}

func unreliable(ctx context.Context) (string, error) {
	return "", errors.New("something went wrong")
}

func main() {	
	wrapped := Breaker(unreliable, 5)

	for i := 0; i < 10; i++ {
		res, err := wrapped(context.Background())
		if err != nil {
			log.Print(err)
		}
	}
}
