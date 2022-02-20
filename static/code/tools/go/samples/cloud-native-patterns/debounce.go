package main

// Debounce limits the frequency of a function invocation so that only the first or last in a cluster of calls is actually performed.




On each call of the outer function—regardless of its outcome—a time interval is set. Any subsequent call made before that time interval expires is ignored; any call made afterwards is passed along to the inner function.

type Circuit func(context.Context) (string, error)

func DebounceFirst(circuit Circuit, d time.Duration) Circuit {
	var threshold time.Time
 var result string
 var err error
 var m sync.Mutex
 return func(ctx context.Context) (string, error) {
 m.Lock()
 defer func() {
 threshold = time.Now().Add(d)
 m.Unlock()
 }()
 if time.Now().Before(threshold) {
 return result, err
 }
 result, err = circuit(ctx)
 return result, err
 }
}



This implementation of DebounceFirst takes pains to ensure thread safety by wrap‐
ping the entire function in a mutex. While this will force overlapping calls at the start
of a cluster to have to wait until the result is cached, it also guarantees that circuit is
called exactly once, at the very beginning of a cluster. A defer ensures that the value
of threshold, representing the time when a cluster ends (if there are no further calls),
is reset with every call.
Our function-last implementation is a bit more awkward because it involves the use
of a time.Ticker to determine whether enough time has passed since the function
was last called, and to call circuit when it has. Alternatively, we could create a new
time.Ticker with every call, but that can get quite expensive if it’s called frequently:
82 | Chapter 4: Cloud Native Patterns
type Circuit func(context.Context) (string, error)
func DebounceLast(circuit Circuit, d time.Duration) Circuit {
 var threshold time.Time = time.Now()
 var ticker *time.Ticker
 var result string
 var err error
 var once sync.Once
 var m sync.Mutex
 return func(ctx context.Context) (string, error) {
 m.Lock()
 defer m.Unlock()
 threshold = time.Now().Add(d)
 once.Do(func() {
 ticker = time.NewTicker(time.Millisecond * 100)
 go func() {
 defer func() {
 m.Lock()
 ticker.Stop()
 once = sync.Once{}
 m.Unlock()
 }()
 for {
 select {
 case <-ticker.C:
 m.Lock()
 if time.Now().After(threshold) {
 result, err = circuit(ctx)
 m.Unlock()
 return
 }
 m.Unlock()
 case <-ctx.Done():
 m.Lock()
 result, err = "", ctx.Err()
 m.Unlock()
return
 }
 }
 }()
 })
 return result, err
 }
}
Stability Patterns | 83
Like DebounceFirst, DebounceLast uses a value called threshold to indicate the end
of a cluster of calls (assuming there are no additional calls). The similarity largely
ends there however.
You’ll notice that almost the entire function is run inside of the Do method of a
sync.Once value, which ensures that (as its name suggests) the contained function is
run exactly once. Inside this block, a time.Ticker is used to check whether
threshold has been passed and to call circuit if it has. Finally, the time.Ticker is
stopped, the sync.Once is reset, and the cycle is primed to repeat.