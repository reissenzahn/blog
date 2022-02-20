package main
import "net"
import "fmt"
func main() {
  ServerConn, _ := net.ListenPacket("udp", "127.0.0.1:6001")
  defer ServerConn.Close()
  buf := make([]byte, 1024)
  for {
    n, addr, _ := ServerConn.ReadFrom(buf)
    fmt.Println("Received ", string(buf[0:n]), " from ", addr)
  }
}