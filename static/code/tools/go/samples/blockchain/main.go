

// blockchain is a public distributed database of records

// all uses hold a full (or partial) copy of the database and a new record can be added only with a consent of other users


// a block holds some data, its hash, the hash of the previous block and various other metadata

type Block struct {
	Timestamp     int64
	Data          []byte
	PrevBlockHash []byte
	Hash          []byte
}


// calculating hashes is computationally expensive  which makes adding new blocks difficult thus preventing their modification after they have been added

func (b *Block) SetHash() {
	timestamp := []byte(strconv.FormatInt(b.Timestamp, 10))
	headers := bytes.Join([][]byte{b.PrevBlockHash, b.Data, timestamp}, []byte{})
	hash := sha256.Sum256(headers)

	b.Hash = hash[:]
}
Next, following a Golang convention, we’ll implement a function that’ll simplify the creation of a block:

func NewBlock(data string, prevBlockHash []byte) *Block {
	block := &Block{time.Now().Unix(), []byte(data), prevBlockHash, []byte{}}
	block.SetHash()
	return block
}
And that’s it for the block!

Blockchain
Now let’s implement a blockchain. In its essence blockchain is just a database with certain structure: it’s an ordered, back-linked list. Which means that blocks are stored in the insertion order and that each block is linked to the previous one. This structure allows to quickly get the latest block in a chain and to (efficiently) get a block by its hash.

In Golang this structure can be implemented by using an array and a map: the array would keep ordered hashes (arrays are ordered in Go), and the map would keep hash → block pairs (maps are unordered). But for our blockchain prototype we’ll just use an array, because we don’t need to get blocks by their hash for now.

type Blockchain struct {
	blocks []*Block
}
This is our first blockchain! I’ve never thought it would be so easy 😉

Now let’s make it possible to add blocks to it:

func (bc *Blockchain) AddBlock(data string) {
	prevBlock := bc.blocks[len(bc.blocks)-1]
	newBlock := NewBlock(data, prevBlock.Hash)
	bc.blocks = append(bc.blocks, newBlock)
}
That’s it! Or not?..

To add a new block we need an existing block, but there’re not blocks in our blockchain! So, in any blockchain, there must be at least one block, and such block, the first in the chain, is called genesis block. Let’s implement a method that creates such a block:

func NewGenesisBlock() *Block {
	return NewBlock("Genesis Block", []byte{})
}
Now, we can implement a function that creates a blockchain with the genesis block:

func NewBlockchain() *Blockchain {
	return &Blockchain{[]*Block{NewGenesisBlock()}}
}
Let’s check that the blockchain works correctly:

func main() {
	bc := NewBlockchain()

	bc.AddBlock("Send 1 BTC to Ivan")
	bc.AddBlock("Send 2 more BTC to Ivan")

	for _, block := range bc.blocks {
		fmt.Printf("Prev. hash: %x\n", block.PrevBlockHash)
		fmt.Printf("Data: %s\n", block.Data)
		fmt.Printf("Hash: %x\n", block.Hash)
		fmt.Println()
	}
}
Output:

Prev. hash:
Data: Genesis Block
Hash: aff955a50dc6cd2abfe81b8849eab15f99ed1dc333d38487024223b5fe0f1168

Prev. hash: aff955a50dc6cd2abfe81b8849eab15f99ed1dc333d38487024223b5fe0f1168
Data: Send 1 BTC to Ivan
Hash: d75ce22a840abb9b4e8fc3b60767c4ba3f46a0432d3ea15b71aef9fde6a314e1

Prev. hash: d75ce22a840abb9b4e8fc3b60767c4ba3f46a0432d3ea15b71aef9fde6a314e1
Data: Send 2 more BTC to Ivan
Hash: 561237522bb7fcfbccbc6fe0e98bbbde7427ffe01c6fb223f7562288ca2295d1
That’s it!

Conclusion
We built a very simple blockchain prototype: it’s just an array of blocks, with each block having a connection to the previous one. The actual blockchain is much more complex though. In our blockchain adding new blocks is easy and fast, but in real blockchain adding new blocks requires some work: one has to perform some heavy computations before getting a permission to add block (this mechanism is called Proof-of-Work). Also, blockchain is a distributed database that has no single decision maker. Thus, a new block must be confirmed and approved by other participants of the network (this mechanism is called consensus). And there’re no transactions in our blockchain yet!

In future articles we’ll cover each of these features.

// source: https://jeiwan.net/posts/building-blockchain-in-go-part-2/
