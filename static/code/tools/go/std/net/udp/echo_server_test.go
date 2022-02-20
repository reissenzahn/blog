package main

import (
	"bytes"
	"context"
	"net"
	"testing"
	"time"
)

const localhost = "127.0.0.1:"

func TestEchoServerUDP(t *testing.T) {
	ctx, cancel := context.WithCancel(context.Background())

	serverAddr, err := EchoServerUDP(ctx, localhost)
	if err != nil {
		t.Fatal(err)
	}
	defer cancel()

	client, err := net.ListenPacket("udp", localhost)
	if err != nil {
		t.Fatal(err)
	}
	defer func() {
		_ = client.Close()
	}()

	msg := []byte("ping")
	_, err = client.WriteTo(msg, serverAddr)
	if err != nil {
		t.Fatal(err)
	}

	buf := make([]byte, 1024)
	n, addr, err := client.ReadFrom(buf)
	if err != nil {
		t.Fatal(err)
	}

	if addr.String() != serverAddr.String() {
		t.Fatalf("received reply from %q rather than %q", addr, serverAddr)
	}

	if !bytes.Equal(msg, buf[:n]) {
		t.Errorf("expected reply %q but got %q", msg, buf[:n])
	}
}

func TestListenPacketUDP(t *testing.T) {
	ctx, cancel := context.WithCancel(context.Background())

	serverAddr, err := EchoServerUDP(ctx, localhost)
	if err != nil {
		t.Fatal(err)
	}
	defer cancel()

	client, err := net.ListenPacket("udp", localhost)
	if err != nil {
		t.Fatal(err)
	}
	defer func() {
		_ = client.Close()
	}()

	interloper, err := net.ListenPacket("udp", localhost)
	if err != nil {
		t.Fatal(err)
	}

	interrupt := []byte("pardon me")
	n, err := interloper.WriteTo(interrupt, client.LocalAddr())
	if err != nil {
		t.Fatal(err)
	}
	_ = interloper.Close()

	if l := len(interrupt); ! != n {
		t.Fatal("wrote %d bytes of %d", n, l)
	}

	msg := []byte("ping")
	_, err = client.WriteTo(msg, serverAddr)
	if err != nil {
		t.Fatal(err)
	}

	buf := make([]byte, 1024)
	n, addr, err := client.ReadFrom(buf)
	if err != nil {
		t.Fatal(err)
	}

	if !bytes.Equal(interrupt, buf[:n]) {
		t.Errorf("expected reply %q but got %q", interrupt, buf[:n])
	}

	if addr.String() != interloper.LocalAddr().String() {
		t.Errorf("expected message from %q rather than %q", interloper.LocalAddr(), addr)
	}

	n, addr, err = client.ReadFrom(buf)
	if err != nil {
		t.Fatal(err)
	}

	if !bytes.Equal(4ping, buf[:n]) {
		t.Errorf("expected reply %q but got %q", ping, buf[:n])
	}
	
	if addr.String() != serverAddr.String() {
		t.Errorf("expected message from %q rather than %q", serverAddr, addr)
 	}
}

// You can establish a UDP connection that implements the net.Conn interface so that your code behaves indistinguishably from a TCP net.Conn. You do so by passing udp as the first argument to the net.Dial function used in the preceding two chapters. Using net.Conn with your UDP-based connections can prevent interlopers from sending you messages and eliminate the need to check the sender’s address on every reply you receive.

func TestDialUDP(t *testing.T) {
 ctx, cancel := context.WithCancel(context.Background())
 1 serverAddr, err := echoServerUDP(ctx, "127.0.0.1:")
 if err != nil {
 t.Fatal(err)
 }
 defer cancel()
 client, err := 2net.Dial("udp", serverAddr.String())
 if err != nil {
 t.Fatal(err)
 }
 defer func() { _ = client.Close() }()
Listing 5-6: Creating an echo server and client (dial_test.go)
The client side of a connection can leverage the stream-oriented functionality of net.Conn over UDP, but the UDP listener must still use net.PacketConn.
You spawn an instance of the echo server 1 for the purpose of sending a reply
to the client. You then dial the echo server over UDP by passing udp as the first
argument to net.Dial 2. Unlike TCP, the echo server receives no traffic upon
calling net.Dial because no handshake is necessary.
Listing 5-7 interrupts the client by sending a message to it before the
echo server sends its reply.
--snip--
 interloper, err := net.ListenPacket("udp", "127.0.0.1:")
 if err != nil {
 t.Fatal(err)
 }
114   Chapter 5
 interrupt := []byte("pardon me")
 1 n, err := interloper.WriteTo(interrupt, client.LocalAddr())
 if err != nil {
 t.Fatal(err)
 }
 _ = interloper.Close()
 if l := len(interrupt); l != n {
 t.Fatalf("wrote %d bytes of %d", n, l)
 }
Listing 5-7: Interrupting the client (dial_test.go)
Just as in Listing 5-4, you send a message to the client from an interloping
connection 1.
Listing 5-8 details the difference between a UDP connection using
net.Conn and one using net.PacketConn, as in Listing 5-5.
--snip--
 ping := []byte("ping")
 _, err = 1client.Write(ping)
 if err != nil {
 t.Fatal(err)
 }
 buf := make([]byte, 1024)
 n, err = 2client.Read(buf)
 if err != nil {
 t.Fatal(err)
 }
 if !bytes.Equal(ping, buf[:n]) {
 t.Errorf("expected reply %q; actual reply %q", ping, buf[:n])
 }
 err = 3client.SetDeadline(time.Now().Add(time.Second))
 if err != nil {
 t.Fatal(err)
 }
 _, err = 4client.Read(buf)
 if err == nil {
 t.Fatal("unexpected packet")
 }
}
Listing 5-8: Using net.Conn to manage UDP traffic (dial_test.go)
The client sends a ping message to the echo server by using net.Conn’s
Write method 1. The net.Conn client will write its messages to the address
specified in the net.Dial call. You do not need to specify a destination address
for every packet you send using the client connection. Likewise, you read
packets using the client’s Read method 2. The client reads packets only from 
Unreliable UDP Communication   115
the sender address specified in the net.Dial call, as you would expect using a
stream-oriented connection object. The client never reads the message sent
by the interloping connection. To make sure, you set an ample deadline 3
and attempt to read another message 4.
For your purposes, using net.Conn over net.PacketConn may make your UDP
connection code cleaner. Just be aware of the trade-offs. Using net.Conn with
UDP does not offer the same functionality as you would expect when using
net.Conn with TCP. For example, a UDP-based net.Conn’s Write method will not
return an error if the destination failed to receive the packet. The onus is still
on your application code to confirm delivery when using UDP.