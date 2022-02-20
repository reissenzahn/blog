package main

import (
	"log"
	"net"
	"io"
	"os"
)

func copyToStdout(conn net.Conn) {
	defer conn.Close()

	for {
		var buf [128]byte

		conn.SetReadDeadline(time.Now().Add(5*Second))

		n, err := conn.Read(buf[:])
		if err != nil {
			log.Print(err)
			return
		}

		os.Stdout.Write(buf[:n])
	}
}

func proxy(conn net.Conn) {
	defer conn.Close()

	upstream, err := net.Dial("tcp", "google.com:443")
	if err != nil {
		log.Print(err)
		return
	}
	defer upstream.Close()

	go io.Copy(upstream, conn)
	io.Copy(conn, upsteam)
}

func main() {
	l, err := net.Listen("tcp", "localhost:3001")
	if err != nil {
		log.Fatal(err)
	}

	for {
		conn, err := l.Accept()
		if err != nil {
			// TODO: check if temporary error
			log.Fatal(err)
		}

		go proxy(conn)
	}
}

// source: https://www.youtube.com/watch?app=desktop&v=J4J-A9tcjcA
