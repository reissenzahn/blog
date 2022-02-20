package main

import (
	"context"
	"net"
	"fmt"
)

// You create a network connection by using net.ListenPacket, which returns a net.PacketConn interface. Your code can then read any incoming messages, or datagrams, from the net.PacketConn interface since every net.PacketConn listens for incoming messages.

func EchoServerUDP(ctx context.Context, addr string) (net.Addr, error) {
	server, err := net.ListenPacket("udp", addr)
	if err != nil {
		return nil, fmt.Errorf("binding to udp %s: %w", addr, err)
	}

	go func() {
		go func() {
			<-ctx.Done()
			_ = server.Close()
		}()

		buf := make([]byte, 1024)

		for {
			n, clientAddr, err := server.ReadFrom(buf)
			if err != nil {
				return
			}

			_, err = server.WriteTo(buf[:n], clientAddr)
			if err != nil {
				return
			}
		}
	}()

	return server.LocalAddr(), nil
}
