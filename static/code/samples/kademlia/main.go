package main

import (
	"crypto/sha1"
	"fmt"
	"net"
	"encoding/hex"
)

type nodeId struct {
	buf [20]byte
}

func NewNodeId(str string) [20]byte {
	h := sha1.New()
	h.Write([]byte(str))
	sum := h.Sum(nil)

	res := [20]byte{}
	copy(res[:], sum)

	return nodeId{buf: res}
}

func (self *nodeId) Distance(other nodeId) {
	dist := 0

	for i := 0; i < 20; i++ {
		dist = (dist << 8) + (self.buf[i] ^ other.buf[i])
	}

	return dist;
}

func (self *nodeId) Equals(other nodeId) {
	for i := 0; i < 20; i++ {
		if self.buf[i] != other.buf[i] {
			return false
		}
	}

	return true
}

func (self *nodeId) String() {
	return hex.EncodeToString(self.buf)
}


type contact struct {
	id      nodeId
	port    uint16
	addr    net.IP
}

func newContact(id nodeId, port uint16, addr net.IP) *contact {
	return &contact{
		nodeId: nodeId,
		port: port,
		addr: addr,
	}
}

func (self *contact) String() {
	return "contact{id=" + self.id.String() + ", port=" + strconv.Itoa(self.port) + ", addr=" + self.addr.String() + "}"
}


type routingTableNode struct {
	contacts     []*contact
	isSplittable bool
	isInner      bool
	left         *routingTableNode
	right        *routingTableNode
}

func newRoutingTableNode() *routingTableNode {
	node := routingTableNode{
		contacts: []contact{},
		isSplittable: true,
		isInner: false,
		left: nil,
		right: nil,
	}

	return &node
}


type routingTable struct {
	localNodeId        [20]byte
	numNodesPerKBucket int
	numNodesToPing     int
	root               *routingTableNode
}

func NewRoutingTable(localNodeId [20]byte, numNodesPerKBucket, numNodesToPing int) *routingTable {
	table := routingTable{
		localNodeId: localNodeId,
		numNodesPerKBucket: numNodesPerKBucket,
		numNodesToPing: numNodesToPing,
		root: newRoutingTableNode(),
	}

	return &table
}

func (self *routingTable) Add(id nodeId, port uint16, addr net.IP) {
	bitIndex := 0
	curr := self.root

	for (curr.isInner) {
		curr = self.pickChild(curr, id, bitIndex++)
	}

	// check if contact already exists
	for _, contant := range curr.contacts {
		if contact.id.Equals(id) {
			contact.port = port
			contact.addr = addr
			return
		}
	}

	// check if there is space to add the contact
	if (len(curr.contacts) < self.numNodesPerKBucket) {
		curr.contacts = append(curr.contacts, newContact(id, port, addr))
	}

	// check if the node is not splittable
	if (!curr.isSplittable) {
		// TODO: how to prevent coupling with sending pings?

		fmt.Println("bucket is full so we need to ping!")

		for _, contact := curr.contacts[:self.numNodesToPing] {
			fmt.Println(contact.id.String())
		}

		return
	}

	// split and redstribute contacts
	curr.left = newRoutingTableNode()
	curr.right = newRoutingTableNode()

	// redistribute existing contacts amongst children
	for _, contact := range curr.contacts {
		child := self.pickChild(curr, contact.id, bitIndex)
		child.contacts = append(child.contacts, contact)
	}

	// node is now an inner node
	curr.isInner = true

	// prevent splitting "far away" nodes by marking the new child where the local node would not end up as not splittable
	close := self.pickChild(curr, self.localId, bitIndex)
	far := curr.left

	if (far == close) {
		far = curr.right
	}

	far.isSplittable = false

	self.Add(id, port, addr)

	return
}

func (self *routingTable) Get(id nodeId) *contact {
	bitIndex := 0
	curr := self.root

	for (self.isInner) {
		curr = self.pickChild(curr, id, bitIndex++)
	}

	for _, contant := range curr.contacts {
		if contact.id.Equals(id) {
			return contact
		}
	}

	return nil
}

func (self *routingTable) Remove(id nodeId) bool {
	bitIndex := 0
	curr := self.root

	for (self.isInner) {
		curr = self.pickChild(curr, id, bitIndex++)
	}

	for index, contant := range curr.contacts {
		if contact.id.Equals(id) {
			curr.contacts = append(curr.contacts[:index], slice[index+1:]...)
			return true
		}
	}

	return false
}

func (self *routingTable) pickChild(node *routingTableNode, nodeId [20]byte, bitIndex int) {
	if (nodeId[bitIndex/8] & (1 << (7 - (bitIndex%8)))) != 0 {
		return node.right
	}

	return node.left
}

func main() {
	fmt.Println("start")
}
