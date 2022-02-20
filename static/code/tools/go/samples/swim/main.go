package main

import (
	"time"
	"math/rand"
	"sync"
	"os"
	"net"
	"fmt"
	"errors"
)

const (
	ProtocolPeriod = 3*time.Second
	TimeoutPeriod = 1*time.Second
	NumProbers = 2
)

type MessageAction byte

const (
	PING MessageAction = iota
	ACK
	ACKFOR
	PINGREQ
)

type Message struct {
	action MessageAction
	payload []byte
}

func NewMessage(action MessageAction, payload []byte) *Message {
	return &Message{action: action, payload: payload}
} 

func (self *Message) encode() []byte {
	buf := make([]byte, 1 + len(self.payload))

	buf[0] = byte(self.action)

	copy(buf[1:], self.payload)
	
	return buf
}

func (self *Message) decode(buf []byte) error {
	if (len(buf) < 1) {
		return errors.New("packet too short")
	}

	action := MessageAction(buf[0])

	switch action {
	case PING:
		self.action = PING
		return nil
	case ACK:
		self.action = ACK
		return nil
	case PINGREQ:
		self.action = PINGREQ
		copy(self.payload, buf[1:])
		return nil
	}

	return errors.New("unknown packet")
}


type MemberStatus int

const (
	Alive MemberStatus = iota
	Suspect
	Confirm
	Dead
)

type Member struct {
	addr string
	status MemberStatus
	pending bool
}

func NewMember(addr string) *Member { 
	return &Member{addr: addr, status: Alive, pending: false}
}


type MemberList struct {
	members []*Member
	next    int
	lock    sync.RWMutex
}

func NewMemberList() *MemberList {
	return &MemberList{}
}

func (self *MemberList) Add(member *Member) {
	self.lock.Lock()
	defer self.lock.Unlock()

	if len(self.members) == 0 {
		self.members = append(self.members, member)
	} else {
		index := rand.Intn(len(self.members))
		
		self.members = append(self.members[:index+1], self.members[index:]...)
		self.members[index] = member

		if index < self.next {
			self.next++
		}
	}
}

func (self *MemberList) Remove(member *Member) {
	if len(self.members) == 0 {
		return
	}

	for index, val := range self.members {
		if (val == member) {
			self.members = append(self.members[:index], self.members[index+1:]...)

			if index < self.next && self.next > 0 {
				self.next--
			}

			return
		}
	}
}

func (self *MemberList) Pick() *Member {
	self.lock.Lock()
	defer self.lock.Unlock()

	if len(self.members) == 0 {
		return nil
	}

	member := self.members[self.next]
	self.next = (self.next + 1) % len(self.members)

	return member
}

func (self *MemberList) PickRandom(k int) []*Member {
	self.lock.RLock()
	defer self.lock.RUnlock()

	if (k > len(self.members)) {
		k = len(self.members)
	}
	
	members := make([]*Member, k)

	fmt.Println(rand.Perm(len(self.members))[:k])

	for index, val := range rand.Perm(len(self.members))[:k] {
		members[index] = self.members[val]
	}
	
	return members
}


func sendPacket(addr string, buf []byte) error {
	raddr, err := net.ResolveUDPAddr("udp", addr)
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp", nil, raddr)
	if err != nil {
		return err
	}
	defer conn.Close()

	n, err := conn.Write(buf)
	if err != nil {
		return err
	}

	fmt.Printf("SENT: to=%s bytes=%d data=%v\n", raddr.String(), n, buf)

	return nil
}


func main() {
	if len(os.Args) < 3 {
		fmt.Println("usage: main [port] [peer addresses]")
		return
	}

	rand.Seed(time.Now().UnixNano())

	laddr := ":" + os.Args[1]

	memberList := NewMemberList()

	for _, val := range os.Args[2:] {
		memberList.Add(NewMember(val))
	}

	// server
	go func() {
		for {
			conn, err := net.ListenPacket("udp", laddr)
			if err != nil {
				fmt.Println(err)
				continue
			}
			
			buf := make([]byte, 1024)
			
			n, raddr, err := conn.ReadFrom(buf)
			if err != nil {
				fmt.Println(err)
				conn.Close()
				continue;
			}

			var msg Message
			if err = msg.decode(buf); err != nil {
				fmt.Println(err)
				conn.Close()
				continue;
			}

			switch msg.action {
			case ACK:
				for _, val := range memberList.members {
					fmt.Print("TRACE: ")
					fmt.Println(string(msg.payload))
					if (val.addr == string(msg.payload)) {
						val.pending = false
						break
					}
				}
			}
			
			fmt.Printf("RECV: from=%s bytes=%d\n", raddr.String(), n)

			conn.Close()
		}
	}()

	// client

	ticker := time.NewTicker(ProtocolPeriod)
	defer ticker.Stop()

	for {
		<-ticker.C

		// ping the next member
		member := memberList.Pick()
		
		fmt.Printf("LOG: send PING to %s \n", member.addr)
		sendPacket(member.addr, NewMessage(PING, []byte(laddr)).encode())
		
		member.pending = true
		
		// wait for protocol period and check if ack received
		<-time.NewTimer(TimeoutPeriod).C
		
		if (!member.pending) {
			continue
		}
		
		// send pingreq
		members := memberList.PickRandom(NumProbers)
		
		for _, val := range members {
			fmt.Printf("LOG: send PINGREQ to %s \n", val.addr)
			sendPacket(val.addr, NewMessage(PINGREQ, []byte(member.addr)).encode())
		}
	}	
}
