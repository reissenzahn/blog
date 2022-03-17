package main

// timeout allows a process to stop waiting for an answer once it is clear that an answer may not be coming

type SlowFunction func(string) (string, error)

type WithContext func(context.Context, string) (string, error)
func Timeout(f SlowFunction) WithContext {
 return func(ctx context.Context, arg string) (string, error) {
 chres := make(chan string)
 cherr := make(chan error)
 go func() {
 res, err := f(arg)
 chres <- res
 cherr <- err
 }()
 select {
Stability Patterns | 91
 case res := <-chres:
 return res, <-cherr
 case <-ctx.Done():
 return "", ctx.Err()
 }
 }
}
Within the function that Timeout constructs, Slow is run in a goroutine, with its
return values being sent into channels constructed for that purpose, if and when it
ever completes.
The following goroutine statement is a select block on two channels: the first of the
Slow function response channels, and the Context value’s Done channel. If the former
completes first, the closure will return the Slow function’s return values; otherwise it
returns the error provided by the Context.
Using the Timeout function isn’t much more complicated than consuming Slow
directly, except that instead of one function call, we have two: the call to Timeout to
retrieve the closure, and the call to the closure itself:

func main() {
	ctx, cancel := context.WithTimeout(context.Background(), 1 * time.Second)
	defer cancel()

}

func main() {
 ctx := context.Background()
 ctxt, cancel := context.WithTimeout(ctx, 1*time.Second)
 defer cancel()
 timeout := Timeout(Slow)
 res, err := timeout(ctxt, "some input")
 fmt.Println(res, err)
}

