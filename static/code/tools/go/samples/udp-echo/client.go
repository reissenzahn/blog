package main

func client(ctx context.Context, address string, reader io.Reader) error {
	// resolve the address into an ip/port if a hostname is provided
	raddr, err := net.ResolveUDPAddr("udp", address)
	if err != nil {
		return err
	}
	
	// create a connection for the remote address
	conn, err := net.DialUDP("udp", nil, raddr)
	if err != nil {
		return err
	}
	defer conn.Close()

	done := make(chan error)

	go func() {
		// it is possible that this action blocks
		m, err := io.Copy(conn, reader)
		if err != nil {
			done <- err
			return
		}

		log.Printf("packet-written: bytes=%d", m)

		// set a deadline for the read operation
		err = conn.SetReadDeadline(time.Now().Add(*timeout))
		if err != nil {
			done <- err
			return
		}
		
		buf := make([]byte, maxBufferSize)
		
		n, addr, err := conn.ReadFrom(buf)
		if err != nil {
			done <- err
			return
		}

		log.Printf("packet-received: bytes=%d from=%s", n, addr.String())

		close(done)
	}()

	select {
	case <-ctx.Done():
		log.Println("cancelled")
		return ctx.Err()
	case err = <-don:
		return err
	}
}