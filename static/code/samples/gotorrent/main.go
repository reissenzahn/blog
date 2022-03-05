package main

import (
	"encoding/binary"
	"fmt"
	"net"
	"os"
	"strconv"
	"bytes"
	"io"
	"crypto/sha1"

	"github.com/jackpal/bencode-go"
)

// d
//   8:announce
//     41:http://bttracker.debian.org:6969/announce
//   7:comment
//     35:"Debian CD from cdimage.debian.org"
//   13:creation date
//     i1573903810e
//   4:info
//     d
//       6:length
//         i351272960e
//       4:name
//         31:debian-10.2.0-amd64-netinst.iso
//       12:piece length
//         i262144e
//       6:pieces
//         26800:����������...
//     e
// e

type Peer struct {
	IP   net.IP
	Port uint16
}

func (self *Peer) String() string {
	return net.JoinHostPort(self.IP.String(), strconv.Itoa(int(self.Port)))
}

func ParseCompactPeers(buf []byte) ([]Peer, error) {
	if len(buf)%6 != 0 {
		return nil, fmt.Errorf("malformed compact peers list")
	}

	n := len(buf) / 6
	peers := make([]Peer, n)

	for i := 0; i < n; i++ {
		offset := i * 6
		peers[i].IP = net.IP(buf[offset : offset+4])
		peers[i].Port = binary.BigEndian.Uint16([]byte(buf[offset+4 : offset+6]))
	}

	return peers, nil
}

type MetainfoFile struct {
	Announce    string
	InfoHash    [20]byte
	PieceHashes [][20]byte
	PieceLength int
	Length      int
	Name        string
}

func NewMetainfoFile(path string) (*MetainfoFile, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	parsed := struct {
		Announce string `bencode:"announce"`
		Info     struct {
			Pieces      string `bencode:"pieces"`
			PieceLength int    `bencode:"piece length"`
			Length      int    `bencode:"length"`
			Name        string `bencode:"name"`
		} `bencode:"info"`
	}{}

	if err = bencode.Unmarshal(file, &parsed); err != nil {
		return nil, err
	}

	var infoBuf bytes.Buffer
	if err := bencode.Marshal(&infoBuf, parsed.Info); err != nil {
		return nil, err
	}

	piecesBuf := []byte(parsed.Info.Pieces)

	if len(piecesBuf)%20 != 0 {
		return nil, fmt.Errorf("malformed piece hashes")
	}

	pieceHashes := make([][20]byte, len(piecesBuf)/20)

	for i := 0; i < len(piecesBuf)/20; i++ {
		copy(pieceHashes[i][:], piecesBuf[i*20:(i+1)*20])
	}

	metainfoFile := MetainfoFile{
		Announce:    parsed.Announce,
		InfoHash:    sha1.Sum(infoBuf.Bytes()),
		PieceHashes: pieceHashes,
		PieceLength: parsed.Info.PieceLength,
		Length:      parsed.Info.Length,
		Name:        parsed.Info.Name,
	}

	return &metainfoFile, nil
}

const (
	CHOKE        byte = 0
	UNCHOKE      byte = 1
	INTERESTED   byte = 2
	UNINTERESTED byte = 3
	HAVE         byte = 4
	BITFIELD     byte = 5
	REQUEST      byte = 6
	PIECE        byte = 7
	CANCEL       byte = 8
)

type Message struct {
	ID      byte
	Payload []byte
}

func (self *Message) Name() string {
	if self == nil {
		return "KeepAlive"
	}

	switch self.ID {
	case CHOKE:
		return "CHOKE"
	case UNCHOKE:
		return "UNCHOKE"
	case INTERESTED:
		return "INTERESTED"
	case UNINTERESTED:
		return "UNINTERESTED"
	case HAVE:
		return "HAVE"
	case BITFIELD:
		return "BITFIELD"
	case REQUEST:
		return "REQUEST"
	case PIECE:
		return "PIECE"
	case CANCEL:
		return "CANCEL"
	default:
		return fmt.Sprintf("UNKNOWN#%d", self.ID)
	}
}

func (self *Message) String() string {
	if self == nil {
		return self.Name()
	}

	return fmt.Sprintf("%s [%d]", self.Name(), len(self.Payload))
}


type Handshake struct {
	Pstr     string
	InfoHash [20]byte
	PeerID   [20]byte
}

func NewHandshake(infoHash, peerID [20]byte) *Handshake {
	return &Handshake{
		Pstr:     "BitTorrent Protocol",
		InfoHash: infoHash,
		PeerID:   peerID,
	}
}

func (self *Handshake) Serialize() []byte {
	buf := make([]byte, len(self.Pstr)+49)

	buf[0] = byte(len(self.Pstr))

	curr := 1
	curr += copy(buf[curr:], self.Pstr)
	curr += copy(buf[curr:], make([]byte, 8))
	curr += copy(buf[curr:], self.InfoHash[:])
	curr += copy(buf[curr:], self.PeerID[:])

	return buf
}

func ParseHandshake(r io.Reader) (*Handshake, error) {
	buf := make([]byte, 1)

	_, err := io.ReadFull(r, buf)
	if err != nil {
		return nil, err
	}

	pstrlen := int(buf[0])
	if pstrlen == 0 {
		return nil, fmt.Errorf("pstrlen cannot be zero")
	}

	buf = make([]byte, pstrlen+8+20+20)

	_, err = io.ReadFull(r, buf)
	if err != nil {
		return nil, err
	}

	var infoHash, peerID [20]byte

	copy(infoHash[:], buf[pstrlen+8:pstrlen+8+20])
	copy(peerID[:], buf[pstrlen+8+20:])

	h := Handshake{
		Pstr:     string(buf[0:pstrlen]),
		InfoHash: infoHash,
		PeerID:   peerID,
	}

	return &h, nil
}

type Bitfield []byte

func (self Bitfield) HasPiece(index int) bool {
	byteIndex := index / 8
	offset := index % 8

	if byteIndex < 0 || byteIndex >= len(self) {
		return false
	}

	return (self[byteIndex]>>uint(7-offset))&1 != 0
}

func (self Bitfield) SetPiece(index int) {
	byteIndex := index / 8
	offset := index % 8

	if byteIndex < 0 || byteIndex >= len(self) {
		return
	}

	self[byteIndex] |= (1 << uint(7-offset))
}

type Client struct {
	Conn     net.Conn
	Choked   bool
	Bitfield Bitfield
	peer 		 Peer
	infoHash [20]byte
	peerID   [20]byte
}

func completeHandshake(conn net.Conn, infoHash, peerID [20]byte) (*Handshake, error) {
	conn.SetDeadline(time.Now().Add(3 * time.Second))
	defer conn.SetDeadline(time.Time{})

	req := NewHandshake(infoHash, peerID)
	
	if _, err := conn.Write(req.Serialize()); err != nil {
		return nil, err
	}
	
	res, err := ParseHandshake(conn)
	if err != nil {
		return nil, err
	}

	if !bytes.Equal(res.InfoHash[:], infoHash[:]) {
		return nil, fmt.Errorf("expected infohash %x but got %x", res.InfoHash, infoHash)
	}

	return res, nil
}

func recvBitfield(conn net.Conn) (Bitfield, error) {
	conn.SetDeadline(time.Now().Add(5 * time.Second))
	defer conn.SetDeadline(time.Time{})

	msg, err := ReadMessage
}


func main() {

}
