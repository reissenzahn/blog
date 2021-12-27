

The sync Package
The sync package contains the concurrency primitives that are most useful for lowlevel memory access synchronization. If you’ve worked in languages that primarily
handle concurrency through memory access synchronization, these types will likely
already be familiar to you. The difference between these languages in Go is that Go
has built a new set of concurrency primitives on top of the memory access synchroni‐
zation primitives to provide you with an expanded set of things to work with. As we
discussed in “Go’s Philosophy on Concurrency” on page 31, these operations have
their use—mostly in small scopes such as a struct. It will be up to you to decide
when memory access synchronization is appropriate. With that said, let’s begin taking
a look at the various primitives the sync package exposes.
WaitGroup
WaitGroup is a great way to wait for a set of concurrent operations to complete when
you either don’t care about the result of the concurrent operation, or you have other
means of collecting their results. If neither of those conditions are true, I suggest you
use channels and a select statement instead. WaitGroup is so useful, I’m introducing
it first so I can use it in subsequent sections. Here’s a basic example of using a
WaitGroup to wait for goroutines to complete:
var wg sync.WaitGroup
wg.Add(1)
go func() {
 defer wg.Done()
 fmt.Println("1st goroutine sleeping...")
 time.Sleep(1)
}()
wg.Add(1)
go func() {
 defer wg.Done()
 fmt.Println("2nd goroutine sleeping...")
 time.Sleep(2)
}()
The sync Package | 47
wg.Wait()
fmt.Println("All goroutines complete.")
Here we call Add with an argument of 1 to indicate that one goroutine is begin‐
ning.
Here we call Done using the defer keyword to ensure that before we exit the
goroutine’s closure, we indicate to the WaitGroup that we’ve exited.
Here we call Wait, which will block the main goroutine until all goroutines have
indicated they have exited.
This produces:
2nd goroutine sleeping...
1st goroutine sleeping...
All goroutines complete.
You can think of a WaitGroup like a concurrent-safe counter: calls to Add increment
the counter by the integer passed in, and calls to Done decrement the counter by one.
Calls to Wait block until the counter is zero.
Notice that the calls to Add are done outside the goroutines they’re helping to track. If
we didn’t do this, we would have introduced a race condition, because remember
from “Goroutines” on page 37 that we have no guarantees about when the goroutines
will be scheduled; we could reach the call to Wait before either of the goroutines
begin. Had the calls to Add been placed inside the goroutines’ closures, the call to Wait
could have returned without blocking at all because the calls to Add would not have
taken place.
It’s customary to couple calls to Add as closely as possible to the goroutines they’re
helping to track, but sometimes you’ll find Add called to track a group of goroutines
all at once. I usually do this before for loops like this:
hello := func(wg *sync.WaitGroup, id int) {
 defer wg.Done()
 fmt.Printf("Hello from %v!\n", id)
}
const numGreeters = 5
var wg sync.WaitGroup
wg.Add(numGreeters)
for i := 0; i < numGreeters; i++ {
 go hello(&wg, i+1)
}
wg.Wait()
48 | Chapter 3: Go’s Concurrency Building Blocks
This produces:
Hello from 5!
Hello from 4!
Hello from 3!
Hello from 2!
Hello from 1!
Mutex and RWMutex
If you’re already familiar with languages that handle concurrency through memory
access synchronization, then you’ll probably immediately recognize Mutex. If you
don’t count yourself among that group, don’t worry, Mutex is very easy to understand.
Mutex stands for “mutual exclusion” and is a way to guard critical sections of your
program. If you remember from Chapter 1, a critical section is an area of your pro‐
gram that requires exclusive access to a shared resource. A Mutex provides a
concurrent-safe way to express exclusive access to these shared resources. To borrow
a Goism, whereas channels share memory by communicating, a Mutex shares mem‐
ory by creating a convention developers must follow to synchronize access to the
memory. You are responsible for coordinating access to this memory by guarding
access to it with a mutex. Here’s a simple example of two goroutines that are attempt‐
ing to increment and decrement a common value; they use a Mutex to synchronize
access:
var count int
var lock sync.Mutex
increment := func() {
 lock.Lock()
 defer lock.Unlock()
 count++
 fmt.Printf("Incrementing: %d\n", count)
}
decrement := func() {
 lock.Lock()
 defer lock.Unlock()
 count--
 fmt.Printf("Decrementing: %d\n", count)
}
// Increment
var arithmetic sync.WaitGroup
for i := 0; i <= 5; i++ {
 arithmetic.Add(1)
 go func() {
 defer arithmetic.Done()
 increment()
 }()
The sync Package | 49
}
// Decrement
for i := 0; i <= 5; i++ {
 arithmetic.Add(1)
 go func() {
 defer arithmetic.Done()
 decrement()
 }()
}
arithmetic.Wait()
fmt.Println("Arithmetic complete.")
Here we request exclusive use of the critical section—in this case the count vari‐
able—guarded by a Mutex, lock.
Here we indicate that we’re done with the critical section lock is guarding.
This produces:
Decrementing: -1
Incrementing: 0
Decrementing: -1
Incrementing: 0
Decrementing: -1
Decrementing: -2
Decrementing: -3
Incrementing: -2
Decrementing: -3
Incrementing: -2
Incrementing: -1
Incrementing: 0
Arithmetic complete.
You’ll notice that we always call Unlock within a defer statement. This is a very com‐
mon idiom when utilizing a Mutex to ensure the call always happens, even when
panicing. Failing to do so will probably cause your program to deadlock.
Critical sections are so named because they reflect a bottleneck in your program. It is
somewhat expensive to enter and exit a critical section, and so generally people
attempt to minimize the time spent in critical sections.
One strategy for doing so is to reduce the cross-section of the critical section. There
may be memory that needs to be shared between multiple concurrent processes, but
perhaps not all of these processes will read and write to this memory. If this is the
case, you can take advantage of a different type of mutex: sync.RWMutex.
The sync.RWMutex is conceptually the same thing as a Mutex: it guards access to
memory; however, RWMutex gives you a little bit more control over the memory. You
50 | Chapter 3: Go’s Concurrency Building Blocks
can request a lock for reading, in which case you will be granted access unless the
lock is being held for writing. This means that an arbitrary number of readers can
hold a reader lock so long as nothing else is holding a writer lock. Here’s an example
that demonstrates a producer that is less active than the numerous consumers the
code creates:
producer := func(wg *sync.WaitGroup, l sync.Locker) {
 defer wg.Done()
 for i := 5; i > 0; i-- {
 l.Lock()
 l.Unlock()
 time.Sleep(1)
 }
}
observer := func(wg *sync.WaitGroup, l sync.Locker) {
 defer wg.Done()
 l.Lock()
 defer l.Unlock()
}
test := func(count int, mutex, rwMutex sync.Locker) time.Duration {
 var wg sync.WaitGroup
 wg.Add(count+1)
 beginTestTime := time.Now()
 go producer(&wg, mutex)
 for i := count; i > 0; i-- {
 go observer(&wg, rwMutex)
 }
 wg.Wait()
 return time.Since(beginTestTime)
}
tw := tabwriter.NewWriter(os.Stdout, 0, 1, 2, ' ', 0)
defer tw.Flush()
var m sync.RWMutex
fmt.Fprintf(tw, "Readers\tRWMutext\tMutex\n")
for i := 0; i < 20; i++ {
 count := int(math.Pow(2, float64(i)))
 fmt.Fprintf(
 tw,
 "%d\t%v\t%v\n",
 count,
 test(count, &m, m.RLocker()),
 test(count, &m, &m),
 )
}
The sync Package | 51
The producer function’s second parameter is of the type sync.Locker. This inter‐
face has two methods, Lock and Unlock, which the Mutex and RWMutex types sat‐
isfy.
Here we make the producer sleep for one second to make it less active than the
observer goroutines.
This produces:
Readers RWMutext Mutex
1 38.343µs 15.854µs
2 21.86µs 13.2µs
4 31.01µs 31.358µs
8 63.835µs 24.584µs
16 52.451µs 78.153µs
32 75.569µs 69.492µs
64 141.708µs 163.43µs
128 176.35µs 157.143µs
256 234.808µs 237.182µs
512 262.186µs 434.625µs
1024 459.349µs 850.601µs
2048 840.753µs 1.663279ms
4096 1.683672ms 2.42148ms
8192 2.167814ms 4.13665ms
16384 4.973842ms 8.197173ms
32768 9.236067ms 16.247469ms
65536 16.767161ms 30.948295ms
131072 71.457282ms 62.203475ms
262144 158.76261ms 119.634601ms
524288 303.865661ms 231.072729ms
You can see for this particular example that reducing the cross-section of our criticalsection really only begins to pay off around 213 readers. This will vary depending on
what your critical section is doing, but it’s usually advisable to use RWMutex instead of
Mutex when it logically makes sense.
Cond
The comment for the Cond type really does a great job of describing its purpose:
...a rendezvous point for goroutines waiting for or announcing the occurrence
of an event.
In that definition, an “event” is any arbitrary signal between two or more goroutines
that carries no information other than the fact that it has occurred. Very often you’ll
want to wait for one of these signals before continuing execution on a goroutine. If
we were to look at how to accomplish this without the Cond type, one naive approach
to doing this is to use an infinite loop:
52 | Chapter 3: Go’s Concurrency Building Blocks
for conditionTrue() == false {
}
However this would consume all cycles of one core. To fix that, we could introduce a
time.Sleep:
for conditionTrue() == false {
 time.Sleep(1*time.Millisecond)
}
This is better, but it’s still inefficient, and you have to figure out how long to sleep for:
too long, and you’re artificially degrading performance; too short, and you’re unnec‐
essarily consuming too much CPU time. It would be better if there were some kind of
way for a goroutine to efficiently sleep until it was signaled to wake and check its con‐
dition. This is exactly what the Cond type does for us. Using a Cond, we could write the
previous examples like this:
c := sync.NewCond(&sync.Mutex{})
c.L.Lock()
for conditionTrue() == false {
 c.Wait()
}
c.L.Unlock()
Here we instantiate a new Cond. The NewCond function takes in a type that satis‐
fies the sync.Locker interface. This is what allows the Cond type to facilitate
coordination with other goroutines in a concurrent-safe way.
Here we lock the Locker for this condition. This is necessary because the call to
Wait automatically calls Unlock on the Locker when entered.
Here we wait to be notified that the condition has occurred. This is a blocking
call and the goroutine will be suspended.
Here we unlock the Locker for this condition. This is necessary because when the
call to Wait exits, it calls Lock on the Locker for the condition.
This approach is much more efficient. Note that the call to Wait doesn’t just block, it
suspends the current goroutine, allowing other goroutines to run on the OS thread. A
few other things happen when you call Wait: upon entering Wait, Unlock is called on
the Cond variable’s Locker, and upon exiting Wait, Lock is called on the Cond variable’s
Locker. In my opinion, this takes a little getting used to; it’s effectively a hidden side
effect of the method. It looks like we’re holding this lock the entire time while we wait
for the condition to occur, but that’s not actually the case. When you’re scanning
code, you’ll just have to keep an eye out for this pattern.
The sync Package | 53
Let’s expand on this example and show both sides of the equation: a goroutine that is
waiting for a signal, and a goroutine that is sending signals. Say we have a queue of
fixed length 2, and 10 items we want to push onto the queue. We want to enqueue
items as soon as there is room, so we want to be notified as soon as there’s room in
the queue. Let’s try using a Cond to manage this coordination:
c := sync.NewCond(&sync.Mutex{})
queue := make([]interface{}, 0, 10)
removeFromQueue := func(delay time.Duration) {
 time.Sleep(delay)
 c.L.Lock()
 queue = queue[1:]
 fmt.Println("Removed from queue")
 c.L.Unlock()
 c.Signal()
}
for i := 0; i < 10; i++{
 c.L.Lock()
 for len(queue) == 2 {
 c.Wait()
 }
 fmt.Println("Adding to queue")
 queue = append(queue, struct{}{})
 go removeFromQueue(1*time.Second)
 c.L.Unlock()
}
First, we create our condition using a standard sync.Mutex as the Locker.
Next, we create a slice with a length of zero. Since we know we’ll eventually add
10 items, we instantiate it with a capacity of 10.
We enter the critical section for the condition by calling Lock on the condition’s
Locker.
Here we check the length of the queue in a loop. This is important because a sig‐
nal on the condition doesn’t necessarily mean what you’ve been waiting for has
occurred—only that something has occurred.
We call Wait, which will suspend the main goroutine until a signal on the condi‐
tion has been sent.
Here we create a new goroutine that will dequeue an element after one second.
54 | Chapter 3: Go’s Concurrency Building Blocks
Here we exit the condition’s critical section since we’ve successfully enqueued an
item.
We once again enter the critical section for the condition so we can modify data
pertinent to the condition.
Here we simulate dequeuing an item by reassigning the head of the slice to the
second item.
Here we exit the condition’s critical section since we’ve successfully dequeued an
item.
Here we let a goroutine waiting on the condition know that something has
occurred.
This produces:
Adding to queue
Adding to queue
Removed from queue
Adding to queue
Removed from queue
Adding to queue
Removed from queue
Adding to queue
Removed from queue
Adding to queue
Removed from queue
Adding to queue
Removed from queue
Adding to queue
Removed from queue
Adding to queue
Removed from queue
Adding to queue
As you can see, the program successfully adds all 10 items to the queue (and exits
before it has a chance to dequeue the last two items). It also always waits until at least
one item is dequeued before enqueing another.
We also have a new method in this example, Signal. This is one of two methods that
the Cond type provides for notifying goroutines blocked on a Wait call that the condi‐
tion has been triggered. The other is a method called Broadcast. Internally, the run‐
time maintains a FIFO list of goroutines waiting to be signaled; Signal finds the
goroutine that’s been waiting the longest and notifies that, whereas Broadcast sends a
signal to all goroutines that are waiting. Broadcast is arguably the more interesting of
the two methods as it provides a way to communicate with multiple goroutines at
once. We can trivially reproduce Signal with channels (as we’ll see in the section
The sync Package | 55
“Channels” on page 64), but reproducing the behavior of repeated calls to Broadcast
would be more difficult. In addition, the Cond type is much more performant than
utilizing channels.
To get a feel for what it’s like to use Broadcast, let’s imagine we’re creating a GUI
application with a button on it. We want to register an arbitrary number of functions
that will run when that button is clicked. A Cond is perfect for this because we can use
its Broadcast method to notify all registered handlers. Let’s see how that might look:
type Button struct {
 Clicked *sync.Cond
}
button := Button{ Clicked: sync.NewCond(&sync.Mutex{}) }
subscribe := func(c *sync.Cond, fn func()) {
 var goroutineRunning sync.WaitGroup
 goroutineRunning.Add(1)
 go func() {
 goroutineRunning.Done()
 c.L.Lock()
 defer c.L.Unlock()
 c.Wait()
 fn()
 }()
 goroutineRunning.Wait()
}
var clickRegistered sync.WaitGroup
clickRegistered.Add(3)
subscribe(button.Clicked, func() {
 fmt.Println("Maximizing window.")
 clickRegistered.Done()
})
subscribe(button.Clicked, func() {
 fmt.Println("Displaying annoying dialog box!")
 clickRegistered.Done()
})
subscribe(button.Clicked, func() {
 fmt.Println("Mouse clicked.")
 clickRegistered.Done()
})
button.Clicked.Broadcast()
clickRegistered.Wait()
We define a type Button that contains a condition, Clicked.
56 | Chapter 3: Go’s Concurrency Building Blocks
Here we define a convenience function that will allow us to register functions to
handle signals from a condition. Each handler is run on its own goroutine, and
subscribe will not exit until that goroutine is confirmed to be running.
Here we set a handler for when the mouse button is raised. It in turn calls Broad
cast on the Clicked Cond to let all handlers know that the mouse button has
been clicked (a more robust implementation would first check that it had been
depressed).
Here we create a WaitGroup. This is done only to ensure our program doesn’t exit
before our writes to stdout occur.
Here we register a handler that simulates maximizing the button’s window when
the button is clicked.
Here we register a handler that simulates displaying a dialog box when the mouse
is clicked.
Next, we simulate a user raising the mouse button from having clicked the appli‐
cation’s button.
This produces:
Mouse clicked.
Maximizing window.
Displaying annoying dialog box!
You can see that with one call to Broadcast on the Clicked Cond, all three
handlers are run. Were it not for the clickRegistered WaitGroup, we could call
button.Clicked.Broadcast() multiple times, and each time all three handlers
would be invoked. This is something channels can’t do easily and thus is one of the
main reasons to utilize the Cond type.
Like most other things in the sync package, usage of Cond works best when con‐
strained to a tight scope, or exposed to a broader scope through a type that encapsu‐
lates it.
Once
What do you think this code will print out?
var count int
increment := func() {
 count++
}
The sync Package | 57
var once sync.Once
var increments sync.WaitGroup
increments.Add(100)
for i := 0; i < 100; i++ {
 go func() {
 defer increments.Done()
 once.Do(increment)
 }()
}
increments.Wait()
fmt.Printf("Count is %d\n", count)
It’s tempting to say the result will be Count is 100, but I’m sure you’ve noticed the
sync.Once variable, and that we’re somehow wrapping the call to increment within
the Do method of once. In fact, this code will print out the following:
Count is 1
As the name implies, sync.Once is a type that utilizes some sync primitives internally
to ensure that only one call to Do ever calls the function passed in—even on different
goroutines. This is indeed because we wrap the call to increment in a sync.Once Do
method.
It may seem like the ability to call a function exactly once is a strange thing to encap‐
sulate and put into the standard package, but it turns out that the need for this pat‐
tern comes up rather frequently. Just for fun, let’s check Go’s standard library and see
how often Go itself uses this primitive. Here’s a grep command that will perform the
search:
grep -ir sync.Once $(go env GOROOT)/src |wc -l
This produces:
70
There are a few things to note about utilizing sync.Once. Let’s take a look at another
example; what do you think it will print?
var count int
increment := func() { count++ }
decrement := func() { count-- }
var once sync.Once
once.Do(increment)
once.Do(decrement)
fmt.Printf("Count: %d\n", count)
This produces:
Count: 1
58 | Chapter 3: Go’s Concurrency Building Blocks
2 Personally, I recommend O’Reilly’s excellent book, Head First Design Patterns.
Is it surprising that the output displays 1 and not 0? This is because sync.Once only
counts the number of times Do is called, not how many times unique functions passed
into Do are called. In this way, copies of sync.Once are tightly coupled to the func‐
tions they are intended to be called with; once again we see how usage of the types
within the sync package work best within a tight scope. I recommend that you for‐
malize this coupling by wrapping any usage of sync.Once in a small lexical block:
either a small function, or by wrapping both in a type. What about this example?
What do you think will happen?
var onceA, onceB sync.Once
var initB func()
initA := func() { onceB.Do(initB) }
initB = func() { onceA.Do(initA) }
onceA.Do(initA)
This call can’t proceed until the call at returns.
This program will deadlock because the call to Do at won’t proceed until the call to
Do at exits—a classic example of a deadlock. For some, this may be slightly counter‐
intuitive since it appears as though we’re using sync.Once as intended to guard
against multiple initialization, but the only thing sync.Once guarantees is that your
functions are only called once. Sometimes this is done by deadlocking your program
and exposing the flaw in your logic—in this case a circular reference.
Pool
Pool is a concurrent-safe implementation of the object pool pattern. A complete
explanation of the object pool pattern is best left to literature on design patterns2
;
however, since Pool resides in the sync package, we’ll briefly discuss why you might
be interested in utilizing it.
At a high level, a the pool pattern is a way to create and make available a fixed num‐
ber, or pool, of things for use. It’s commonly used to constrain the creation of things
that are expensive (e.g., database connections) so that only a fixed number of them
are ever created, but an indeterminate number of operations can still request access to
these things. In the case of Go’s sync.Pool, this data type can be safely used by multi‐
ple goroutines.
Pool’s primary interface is its Get method. When called, Get will first check whether
there are any available instances within the pool to return to the caller, and if not, call
its New member variable to create a new one. When finished, callers call Put to place
The sync Package | 59
the instance they were working with back in the pool for use by other processes.
Here’s a simple example to demonstrate:
myPool := &sync.Pool{
 New: func() interface{} {
 fmt.Println("Creating new instance.")
 return struct{}{}
 },
}
myPool.Get()
instance := myPool.Get()
myPool.Put(instance)
myPool.Get()
Here we call Get on the pool. These calls will invoke the New function defined on
the pool since instances haven’t yet been instantiated.
Here we put an instance previously retrieved back in the pool. This increases the
available number of instances to one.
When this call is executed, we will reuse the instance previously allocated and put
it back in the pool. The New function will not be invoked.
As we can see, we only see two calls to the New function:
Creating new instance.
Creating new instance.
So why use a pool and not just instantiate objects as you go? Go has a garbage collec‐
tor, so the instantiated objects will be automatically cleaned up. What’s the point?
Consider this example:
var numCalcsCreated int
calcPool := &sync.Pool {
 New: func() interface{} {
 numCalcsCreated += 1
 mem := make([]byte, 1024)
 return &mem
 },
}
// Seed the pool with 4KB
calcPool.Put(calcPool.New())
calcPool.Put(calcPool.New())
calcPool.Put(calcPool.New())
calcPool.Put(calcPool.New())
const numWorkers = 1024*1024
var wg sync.WaitGroup
wg.Add(numWorkers)
60 | Chapter 3: Go’s Concurrency Building Blocks
for i := numWorkers; i > 0; i-- {
 go func() {
 defer wg.Done()
 mem := calcPool.Get().(*[]byte)
 defer calcPool.Put(mem)
 // Assume something interesting, but quick is being done with
 // this memory.
 }()
}
wg.Wait()
fmt.Printf("%d calculators were created.", numCalcsCreated)
Notice that we are storing the address of the slice of bytes.
And here we are asserting the type is a pointer to a slice of bytes.
This produces:
8 calculators were created.
Had I run this example without a sync.Pool, though the results are nondeterministic, in the worst case I could have been attempting to allocate a gigabyte of
memory, but as you see from the output, I’ve only allocated 4 KB.
Another common situation where a Pool is useful is for warming a cache of preallocated objects for operations that must run as quickly as possible. In this case,
instead of trying to guard the host machine’s memory by constraining the number of
objects created, we’re trying to guard consumers’ time by front-loading the time it
takes to get a reference to another object. This is very common when writing highthroughput network servers that attempt to respond to requests as quickly as possi‐
ble. Let’s take a look at such a scenario.
First, let’s create a function that simulates creating a connection to a service. We’ll
make this connection take a long time:
func connectToService() interface{} {
 time.Sleep(1*time.Second)
 return struct{}{}
}
Next, let’s see how performant a network service would be if for every request we
started a new connection to the service. We’ll write a network handler that opens a
connection to another service for every connection the network handler accepts. To
make the benchmarking simple, we’ll only allow one connection at a time:
func startNetworkDaemon() *sync.WaitGroup {
 var wg sync.WaitGroup
 wg.Add(1)
The sync Package | 61
 go func() {
 server, err := net.Listen("tcp", "localhost:8080")
 if err != nil {
 log.Fatalf("cannot listen: %v", err)
 }
 defer server.Close()
 wg.Done()
 for {
 conn, err := server.Accept()
 if err != nil {
 log.Printf("cannot accept connection: %v", err)
 continue
 }
 connectToService()
 fmt.Fprintln(conn, "")
 conn.Close()
 }
 }()
 return &wg
}
Now let’s benchmark this:
func init() {
 daemonStarted := startNetworkDaemon()
 daemonStarted.Wait()
}
func BenchmarkNetworkRequest(b *testing.B) {
 for i := 0; i < b.N; i++ {
 conn, err := net.Dial("tcp", "localhost:8080")
 if err != nil {
 b.Fatalf("cannot dial host: %v", err)
 }
 if _, err := ioutil.ReadAll(conn); err != nil {
 b.Fatalf("cannot read: %v", err)
 }
 conn.Close()
 }
}
cd src/gos-concurrency-building-blocks/the-sync-package/pool/ && \
go test -benchtime=10s -bench=.
This produces:
BenchmarkNetworkRequest-8 10 1000385643 ns/op
PASS
ok command-line-arguments 11.008s
62 | Chapter 3: Go’s Concurrency Building Blocks
Looks like like roughly 1E9 ns/op. This seems reasonable as far as performance goes,
but let’s see if we can improve it by using a sync.Pool to host connections to our fic‐
titious service:
func warmServiceConnCache() *sync.Pool {
 p := &sync.Pool {
 New: connectToService,
 }
 for i := 0; i < 10; i++ {
 p.Put(p.New())
 }
 return p
}
func startNetworkDaemon() *sync.WaitGroup {
 var wg sync.WaitGroup
 wg.Add(1)
 go func() {
 connPool := warmServiceConnCache()
 server, err := net.Listen("tcp", "localhost:8080")
 if err != nil {
 log.Fatalf("cannot listen: %v", err)
 }
 defer server.Close()
 wg.Done()
 for {
 conn, err := server.Accept()
 if err != nil {
 log.Printf("cannot accept connection: %v", err)
 continue
 }
 svcConn := connPool.Get()
 fmt.Fprintln(conn, "")
 connPool.Put(svcConn)
 conn.Close()
 }
 }()
 return &wg
}
And if we benchmark this, like so:
cd src/gos-concurrency-building-blocks/the-sync-package/pool && \
go test -benchtime=10s -bench=.
The sync Package | 63
We get:
BenchmarkNetworkRequest-8 5000 2904307 ns/op
PASS
ok command-line-arguments 32.647s
2.9E6 ns/op: three orders of magnitude faster! You can see how utilizing this pattern
when working with things that are expensive to create can drastically improve
response time.
As we’ve seen, the object pool design pattern is best used either when you have con‐
current processes that require objects, but dispose of them very rapidly after instan‐
tiation, or when construction of these objects could negatively impact memory.
However, there is one thing to be wary of when determining whether or not you
should utilize a Pool: if the code that utilizes the Pool requires things that are not
roughly homogenous, you may spend more time converting what you’ve retrieved
from the Pool than it would have taken to just instantiate it in the first place. For
instance, if your program requires slices of random and variable length, a Pool isn’t
going to help you much. The probability that you’ll receive a slice the length you
require is low.
So when working with a Pool, just remember the following points:
• When instantiating sync.Pool, give it a New member variable that is thread-safe
when called.
• When you receive an instance from Get, make no assumptions regarding the
state of the object you receive back.
• Make sure to call Put when you’re finished with the object you pulled out of the
pool. Otherwise, the Pool is useless. Usually this is done with defer.
• Objects in the pool must be roughly uniform in makeup.


