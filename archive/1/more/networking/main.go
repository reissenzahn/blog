package main

func main() {

	// // 1. IP Addresses and Masks

	// // parse a dotted IPv4 or colon IPv6 address
	// addr := net.ParseIP("127.0.0.1")

	// fmt.Println(addr.String())

	// // get default mask 255.255.0.0
	// mask := addr.DefaultMask()

	// // apply mask to get network address
	// network := addr.Mask(mask)

	// fmt.Println(network)

	// // 2. DNS lookup

	// addr, _ := net.ResolveIPAddr("ip", "www.google.com")

	// fmt.Println(addr)

	// 3. Lookup Port

	// port, _ := net.LookupPort("tcp", "telnet")

	// fmt.Println(port)

	// 4. Daytime Server

	// addr, _ := net.ResolveTCPAddr("tcp", "localhost:6000")

	// listener, _ := net.ListenTCP("tcp", addr)

	// for {
	// 	conn, _ := listener.Accept()

	// 	now := time.Now().String()

	// 	conn.Write([]byte(now))

	// 	conn.Close()
	// }

	// 5. Echo Server
}
