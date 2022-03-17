
Retry
Retry accounts for a possible transient fault in a distributed system by transparently
retrying a failed operation.
Applicability
Transient errors are a fact of life when working with complex distributed systems.
These can be caused by any number of (hopefully) temporary conditions, especially if
the downstream service or network resource has protective strategies in place, such as
throttling that temporarily rejects requests under high workload, or adaptive strate‐
gies like autoscaling that can add capacity when needed.
These faults typically resolve themselves after a bit of time, so repeating the request
after a reasonable delay is likely (but not guaranteed) to be successful. Failing to
account for transient faults can lead to a system that’s unnecessarily brittle. On the
other hand, implementing an automatic retry strategy can considerably improve the
stability of the service that can benefit both it and its upstream consumers.
Participants
This pattern includes the following participants:
Effector
The function that interacts with the service.
Retry
A function that accepts Effector and returns a closure with the same function sig‐
nature as Effector.
Implementation
This pattern works similarly to Circuit Breaker or Debounce in that there is a type,
Effector, that defines a function signature. This signature can take whatever form is
appropriate for your implementation, but when the function executing the
84 | Chapter 4: Cloud Native Patterns
potentially-failing operation is implemented, it must match the signature defined by
Effector.
The Retry function accepts the user-defined Effector function and returns an Effector
function that wraps the user-defined function to provide the retry logic. Along with
the user-defined function, Retry also accepts an integer describing the maximum
number of retry attempts that it will make, and a time.Duration that describes how
long it’ll wait between each retry attempt. If the retries parameter is 0, then the
retry logic will effectively become a no-op.
Although not included here, retry logic will typically include some
kind of a backoff algorithm.
Sample code
The signature for function argument of the Retry function is Effector. It looks
exactly like the function types for the previous patterns:
type Effector func(context.Context) (string, error)
The Retry function itself is relatively straightforward, at least when compared to the
functions we’ve seen so far:
func Retry(effector Effector, retries int, delay time.Duration) Effector {
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
You may have already noticed what it is that keeps the Retry function so slender:
although it returns a function, that function doesn’t have any external state. This
means we don’t need any elaborate mechanisms to support concurrency.
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
