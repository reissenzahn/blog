
The Retry function accepts the user-defined Effector function and returns an Effector
function that wraps the user-defined function to provide the retry logic. Along with
the user-defined function, Retry also accepts an integer describing the maximum
number of retry attempts that it will make, and a time.Duration that describes how
long it’ll wait between each retry attempt. If the retries parameter is 0, then the
retry logic will effectively become a no-op.
Although not included here, retry logic will typically include some
kind of a backoff algorithm.


type Effector func(context.Context) (string, error)

func Retry(effector Effector, retries int, delay time.Duration) Effector {
	return func(ctx context.Context) (string, error) {
		for i := 0; ; i++ {
			resp, err = effector(ctx)
			if err == nil || i >= retries {
				return resp, err
			}
		}
	}
}


func Retry(effector Effector, retries int, delay time.Duration)Effector {
 return func(ctx context.Context) (string, error) {
 for r := 0; ; r++ {
 response, err := effector(ctx)
 if err == nil || r >= retries {
 return response, err
 }
 log.Printf("Attempt %d failed; retrying in %v", r + 1, delay)
 select {
 case <-time.After(delay):
 case <-ctx.Done():
 return "", ctx.Err()
 }
 }
 }
}

To use Retry, we can implement the function that executes the potentially-failing
operation and whose signature matches the Effector type; this role is played by
EmulateTransientError in the following example:
var count int
func EmulateTransientError(ctx context.Context) (string, error) {
 count++
 if count <= 3 {
 return "intentional fail", errors.New("error")
 } else {
 return "success", nil
 }
}
func main() {
 r := Retry(EmulateTransientError, 5, 2*time.Second)
 res, err := r(context.Background())
 fmt.Println(res, err)
}

In the main function, the EmulateTransientError function is passed to Retry, pro‐
viding the function variable r. When r is called, EmulateTransientError is called,
and called again after a delay if it returns an error, according to the retry logic shown
previously. Finally, after the fourth attempt, EmulateTransientError returns a nil
error and exits.