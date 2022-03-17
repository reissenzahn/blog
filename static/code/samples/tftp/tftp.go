package main

import (
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"net"
	"bytes"
)


// netascii is a modified form of ASCII that consists of basic ASCII ranging from 0x20 to 0x7F plus 8 control characters

// the control characters are: 0x00 (NUL), 0x10 (LF), 0x13 (CR), 0x07 (BEL), 0x08 (BS), 0x09 (HT), 0x11 (VT) and 0x12 (FF)

// another requirement is that newlines are always CRLF; any CR can only be followed either by an LF or a NUL

// all strings are encoded as a sequence of bytes in netascii terminated by a zero byte

const (
	BlockSize = 512
)

const (
	RRQ   = uint16(1)
	WRQ   = uint16(2)
	DATA  = uint16(3)
	ACK   = uint16(4)
	ERROR = uint16(5)
	OACK   = uint16(6)
)

type RRQPacket struct {
	filename string
	mode string
	options map[string]string
}

func UnpackRRQPacket(buf []byte) (*RRQPacket, error) {
	if len(buf) < 4 {
		return nil, fmt.Errorf("rqq packet too short")
	}

	var packet RRQPacket

	parts := bytes.Split(buf[2:], []byte{0})
	
	packet.filename = string(parts[0])
	packet.mode = string(parts[1])
	packet.options = make(map[string]string)

	for i := 2; i + 1 < len(parts); i += 2 {
		packet.options[string(parts[i])] = string(parts[i])
	}

	return &packet, nil
}

func (self *RRQPacket) String() string {
	return fmt.Sprintf("RRQ{filename=%s, mode=%s}", self.filename, self.mode)
}

type DATAPacket struct {
	blockNumber int
	data []byte
}

func NewDATAPacket(blockNumber int, data []byte) *DATAPacket {
	return &DATAPacket{
		blockNumber: blockNumber,
		data: data,
	}
}

func (self *DATAPacket) Pack() []byte {
	buf := make([]byte, 2 + 2 + len(self.data))

	binary.BigEndian.PutUint16(buf, DATA)
	binary.BigEndian.PutUint16(buf[2:], DATA)

	copy(buf[4:], self.data)

	return buf
}

func (self *DATAPacket) String() string {
	return fmt.Sprintf("DATA{blockNumber=%d, data=%s", self.blockNumber, hex.EncodeToString(self.data))
}


func Listen(address string) error {
	laddr, err := net.ResolveUDPAddr("udp", address)
	if err != nil {
		return err
	}
	
	conn, err := net.ListenUDP("udp", laddr)
	if err != nil {
		return err
	}
	defer conn.Close()

	fmt.Printf("listening at %s...\n", conn.LocalAddr().String())

	for {
		var buf [2048]byte

		n, raddr, err := conn.ReadFromUDP(buf[0:])
		if err != nil {
			fmt.Println(err)
			continue
		}

		if len(buf) < 2 {
			fmt.Println("packet too short")
			continue
		}
	
		opcode := binary.BigEndian.Uint16(buf[:2])

		switch opcode {
		case RRQ:
			packet, err := UnpackRRQPacket(buf[:])
			if err != nil {
				fmt.Println(err)
				continue
			}

			go Handle(raddr, packet)
		}


		for _, b := range buf[0:n] {
			fmt.Printf("%08b ", b)
		}

		fmt.Println(string(buf[0:n]))
	}

	return nil
}

func Handle(raddr net.UDPAddr, packet RRQPacket) {
	fmt.Println(packet.String())

	laddr, err := net.ResolveUDPAddr("udp", address)
	if err != nil {
		return err
	}
	
	conn, err := net.ListenUDP("udp", laddr)
	if err != nil {
		return err
	}
	defer conn.Close()

	fmt.Printf("listening at %s...\n", conn.LocalAddr().String())

	file, err := os.Open(filename)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer file.Close()

	buf := make([]byte, BlockSize)

	blockNumber := 1

	for {
		n, raddr, err := file.ReadFromUDP(buf)
		if err != nil && err != io.EOF {
			fmt.Println(err)
			break
		}

		

		packet := NewDATAPacket(blockNumber, buf)

		conn.WriteToUDP(packet.Pack(), raddr)
	
		blockNumber++
	}
}(packet)


func main() {
	Listen(":6969")
}
