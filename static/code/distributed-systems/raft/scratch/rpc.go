package main
 
import (
	"errors"
	"net/rpc"
	"net"
	"fmt"
)

type Args struct {
	Val int
}

type Reply struct {
	Val int
}

type Counter struct {
	Count int
}

func (self *Counter) Add(args *Args, reply *Reply) error {
	if args.Val < 0 {
		return errors.New("amount must be positive")
	}
	
	self.Count += args.Val

	reply.Val = self.Count

	return nil
}

func main() {

	if false {
		counter := &Counter{Count: 0}

		server := rpc.NewServer()

		server.Register(counter)

		listener, err := net.Listen("tcp", ":8888")
		if err != nil {
			panic(err)
		}

		for {
			conn, err := listener.Accept()
			if err != nil {
				panic(err)
			}

			go func() {
				server.ServeConn(conn)
			}()
		}
	} else {
		client, err := rpc.Dial("tcp", ":8888")

		if err != nil {
			panic(err)
		}
	
		args := Args{Val: 2 }
	
		var reply Reply
	
		client.Call("Counter.Add", args, &reply)
	
		fmt.Printf("%d\n", reply.Val)
	}
}

