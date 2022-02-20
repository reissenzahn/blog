package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
)

func main() {
	args := os.Args
	if len(args) == 1 {
		fmt.Println("usage: client <host>:<port>")
		return
	}

	conn, err := net.Dial("tcp", args[1])
	if err != nil {
		fmt.Println(err)
		return
	}

	for {
		fmt.Print(">> ")
		request, _ := bufio.NewReader(os.Stdin).ReadString('\n')
		fmt.Fprintf(conn, request + "\n")

		reply, _ := bufio.NewReader(conn).ReadString('\n')
		fmt.Println("<< " + reply)

		if strings.TrimSpace(string(request)) == "STOP" {
			fmt.Println("exiting...")
			return
		}
	}
}

