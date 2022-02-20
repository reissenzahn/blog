package main

import (
	"crypto/md5"
	"fmt"
	"strings"
	"sync"
	"time"
)

type Generator struct {
	year, month, day, seed, i int
	tld string
	lock *sync.Mutex
}

func New(year, month, day, seed int, tld string) *Generator {
	if !strings.HasPrefix(tld, ".") {
		tld = "." + tld
	}

	return &Generator{
		year: year,
		month: month,
		day: day,
		tld: tld,
		seed: seed,
		lock: new(sync.Mutex),
	}
}

func (g *Generator) Next() string {
	g.lock.Lock();
	defer g.lock.Unlock();

	g.i++;

	sum := md5.Sum([]byte(fmt.Sprintf("%v%v%v%v%v", g.year, g.month, g.day, g.seed, g.i)))

	return fmt.Sprintf("%x%s", sum, g.tld)
}

func main() {
	t := time.Now()

	gen := New(t.Year(), int(t.Month()), t.Day(), 1234, "com")

	for i := 0; i < 5; i++ {
		fmt.Println(gen.Next())
	}
}

// source: https://blog.malwarebytes.com/security-world/2016/12/explained-domain-generating-algorithm/
// source: https://github.com/cs-5/rsdga
