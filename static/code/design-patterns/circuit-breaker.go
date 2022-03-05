

We begin by creating a Circuit type that specifies the signature of the function that’s
interacting with your database or other upstream service. In practice, this can take
whatever form is appropriate for your functionality.
It should include an error in its
return list, however.


type Circuit func(context.Context) (string, error)

func Breaker(circuit Circuit, failureThreshold int) Circuit {
	var mutex sync.RWMutex
	consecutiveFailures := 0
	lastAttempt := time.Now()

	return func(ctx context.Context) (string, error) {
		mutex.RLock()

		d := consecutiveFailures - failureThreshold

		if d >= 0 {
			shouldRetryAt := lastAttemt.Add(time.Second * 2 << d)

			if !time.Now().After(shouldRetryAt) {
				mutex.RUnlock()
				return "", errors.New("service unreachable")
			}
		}

		mutex.RUnlock()

		resp, err := circuit(ctx)

		mutex.Lock()
		defer mutex.Unlock()

		lastAttempt = time.Now()
		
		if err != nil {
			consecutiveFailures++
			return resp, err
		}

		consecutiveFailures = 0

		return resp, 0
	}
}



The Breaker function constructs another function, also of type Circuit, which wraps
circuit to provide the desired functionality.


The closure works by counting the number of consecutive errors returned by
circuit. If that value meets the failure threshold, then it returns the error “service
unreachable” without actually calling circuit. Any successful calls to circuit cause
consecutiveFailures to reset to 0, and the cycle begins again.
The closure even includes an automatic reset mechanism that allows requests to call
circuit again after several seconds, with an exponential backoff in which the dura‐
tions of the delays between retries roughly doubles with each attempt. Though simple
and quite common, this actually isn’t the ideal backoff algorithm. 
