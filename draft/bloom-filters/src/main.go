package main

import (
	"hash"
	"hash/fnv"
	"math"
)

type BloomFilter struct {

	// bitfield holds the bit array
	bitfield []bool

	// m is the size of the bit array
	m int

	// k is the number of hash functions
	k int

	// n is the number of elements in the set
	n int

	// hash is the hash function
	hash hash.Hash64
}

// New returns a new BloomFilter
func New(k, size int) *BloomFilter {
	return &BloomFilter{
		bitfield: make([]bool, size),
		m:        size,
		k:        k,
		n:        0,
		hash:     fnv.New64(),
	}
}

// Insert adds an element to the set
func (bf *BloomFilter) Add(x []byte) {
	h1, h2 := bf.getHash(x)
	for i := 0; i < bf.k; i++ {
		idx := (h1 + uint32(i)*h2) % uint32(bf.m)
		bf.bitfield[idx] = true
	}
	bf.n++
}

// Query checks if a element is in the set
func (bf *BloomFilter) Query(x []byte) bool {
	h1, h2 := bf.getHash(x)
	result := true
	for i := 0; i < bf.k; i++ {
		idx := (h1 + uint32(i)*h2) % uint32(bf.m)
		result = result && bf.bitfield[idx]
	}
	return result
}

// FalsePositiveRate returns the current False Positive Rate
func (bf *BloomFilter) FalsePositiveRate() float64 {
	return math.Pow((1 - math.Exp(-float64(bf.k*bf.n)/
		float64(bf.m))), float64(bf.k))
}

func (bf *BloomFilter) getHash(b []byte) (uint32, uint32) {
	bf.hash.Reset()
	bf.hash.Write(b)
	h := bf.hash.Sum64()
	h1 := uint32(h & ((1 << 32) - 1))
	h2 := uint32(h >> 32)
	return h1, h2
}

func main() {

}
