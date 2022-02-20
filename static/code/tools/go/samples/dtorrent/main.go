package main

import (
	"encoding/binary"
	"fmt"
	"net"
	"strconv"
	"os"

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
	return net.JoinHostPort(p.IP.String(), strconv.Itoa(int(p.Port)))
}

func ParseCompactPeers(buf []byte) ([]Peer, error) {
	if len(buf) % 6 != 0 {
		return nil, fmt.Errorf("malformed compact peers list")
	}

	n := len(buf) / 6
	res := make([]Peer, n)

	for i := 0; i < n; i++ {
		offset := i * 6
		peers[i].IP = net.IP(buf[offset:offset+4])
		peers[i].Port = binary.BigEndian.Uint16([]byte(buf[offset+4:offset+6]))
	}

	return peer, nil
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
		Announce string      `bencode:"announce"`
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
	if err := bencode.Marshal(&buf, parsed.Info); err != nil {
		return nil, err
	}

	piecesBuf := []byte(parsed.Info.Pieces)

	if len(piecesBuf)%20 != 0 {
		return nil, fmt.Errorf("malformed piece hashes")
	}

	pieceHashes := make([][20]byte, len(buf) / 20)

	for i := 0; i < len(buf)/20; i++ {
		copy(pieceHashes[i][:], buf[i*20:(i+1)*20])
	}

	metainfoFile := MetaInfoFile{
		Announce:    parsed.Announce,
		InfoHash:    sha1.Sum(buf.bytes()),
		PieceHashes: pieceHashes,
		PieceLength: parsed.Info.PieceLength,
		Length:      parsed.Info.Length,
		Name:        parsed.Info.Name, 
	}

	return &metainfoFile, nil
}


const (
	CHOKE byte = 0
	UNCHOKE byte = 1
	INTERESTED byte = 2
	UNINTERESTED byte = 3
	HAVE byte = 4
	BITFIELD byte = 5
	REQUEST byte = 6
	PIECE byte = 7
	CANCEL byte = 8
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
		return self.name()
	}

	return fmt.Sprintf("%s [%d]", self.name(), len(self.Payload))
}



type Handshake struct {
	Pstr     string
	InfoHash [20]byte
	PeerID   [20]byte
}

