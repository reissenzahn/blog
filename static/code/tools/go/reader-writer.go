package main

import (
	"fmt"
	"io"
)


// io.Reader and io.Writer are basic interfaces that provide Read() and Write()

// the io.Reader interface represents an entity from which you can read a stream of bytes
type ExampleReader struct {
}

// Read() reads up to len(buf) bytes into buf and returns the number of bytes read or an io.EOF error when the stream ends
func (e *ExampleReader) Read(buf byte[]) (n int, err error) {
}


// the io.Writer interface
type ExampleWriter struct {

}

func (e *ExampleWriter) Write(p byte[]) (n int, err error) {
}


func main() {

}
