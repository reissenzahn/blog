package main

import (
	"encoding/hex"
	"fmt"
	"net"
	"net/url"
)

func main() {
	addr, err := net.ResolveUDPAddr("udp", ":8080")
	if err != nil {
		panic(err)
	}

	fmt.Printf("listening at %s...\n", addr.String())

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		panic(err)
	}

	for {
		var buf [2048]byte

		n, err := conn.Read(buf[0:])
		if err != nil {
			panic(err)
		}

		fmt.Println(url.QueryEscape(string(buf[0:n])))
		fmt.Println(hex.EncodeToString(buf[0:n]))
	}
}


// $ nc -u localhost 8080

