package main

import (
	"fmt"
	"errors"
	"bufio"
	"strings"
)


type Writer int

func (*Writer) Write(p []byte) (n int, err error) {
	fmt.Println(len(p))
	return len(p), nil
}


type FaultyWriter int

func (*FaultyWriter) Write(p []byte) (n int, err error) {
	fmt.Printf("%q\n", p)
	return 0, errors.New("error!")
}


func main() {

	// bufio.Writer allows us to avoid the overhead of many small write operations by first accumulating data inside a buffer and then performing a write when the buffer is full
	w := new(Writer)
	w.Write([]byte{'a'})  // 1
	w.Write([]byte{'b'})  // 1
	w.Write([]byte{'c'})  // 1
	w.Write([]byte{'d'})  // 1

	// the default bufio.Writer uses a 4096 byte long buffer though the buffer size can be set with the NewWriterSize() function
	bw := bufio.NewWriterSize(w, 3)
	bw.Write([]byte{'e'})
	bw.Write([]byte{'f'})
	bw.Write([]byte{'g'})  // 3
	bw.Write([]byte{'h'})
	
	// data is only written when the buffer is full or when explicitly requested with the Flush() method
	err := bw.Flush()  // 1
	if err != nil {
		panic(err)
	}

	// the Available() method allows us to check how much space is left inside the buffer
	fmt.Println(bw.Available())  // 3
	bw.Write([]byte{'i'})
	bw.Write([]byte{'j'})
	fmt.Println(bw.Available())  // 1
	bw.Write([]byte{'k'})
	fmt.Println(bw.Available())  // 0

	// there are utility methods to write data of common types
	bw.WriteByte('a')
	bw.WriteRune('ł')
	bw.WriteString("hey")
	_ = bw.Flush()

	// writing an amount of bytes larger than the internal buffer calls the Write method directly
	bw.Write([]byte("lmno"))  // 4

	// bufio.Writer implements the io.ReaderFrom interface that has a ReadFrom() method for reading all the data from an io.Reader
	s := strings.NewReader("efgh")
	bw.ReadFrom(s)  // 3
	_ = bw.Flush()  // 1

	// a buffered writer does not write any more data after encountering an error
	fw := bufio.NewWriterSize(new(FaultyWriter), 3)
	fw.Write([]byte{'a'})
	fw.Write([]byte{'b'})
	fw.Write([]byte{'c'})  // abc
	fw.Write([]byte{'d'})
	_ = bw.Flush()
	fmt.Println(err)


	// bufio.Reader allows us to read in batches from an underlying io.Reader into a buffer which reduces the number of read operations required

	// if the buffer is 

}


If buffer isn’t full and holds less than n bytes then it’ll try to read more from underlying io.Reader
If requested amount is bigger than the size of the buffer then bufio.ErrBufferFull will be returned
If n is bigger than the size of stream, EOF will be returned
Let’s see how it works (source code):


Peek
Method Peek allows to see first n bytes of buffered data without actually “eating” it:

s1 := strings.NewReader(strings.Repeat("a", 20))
r := bufio.NewReaderSize(s1, 16)
b, err := r.Peek(3)
if err != nil {
	panic(err)
}
fmt.Printf("%q\n", b)

b, err = r.Peek(17)
if err != nil {
	fmt.Println(err)
}

s2 := strings.NewReader("aaa")
r.Reset(s2)
b, err = r.Peek(10)
if err != nil {
    fmt.Println(err)
}
"aaa"



// the minimum size of the buffer used by bufio.Reader is 16


Returned slice uses the same underlying array as the internal buffer used by bufio.Reader.

Consequently what is inside returned slice becomes invalid after any read operations done by reader under the hood. It’s because it might be overwritten by other buffered data (source code):
s1 := strings.NewReader(strings.Repeat("a", 16) + strings.Repeat("b", 16))
r := bufio.NewReaderSize(s1, 16)
b, _ := r.Peek(3)
fmt.Printf("%q\n", b)
r.Read(make([]byte, 16))
r.Read(make([]byte, 15))
fmt.Printf("%q\n", b)
"aaa"
"bbb"

Discard
This method throws away n bytes without even returning it. If bufio.Reader buffered so far more than or equal to n then it doesn’t have to read anything from io.Reader — it simply drops first n bytes from the buffer (source code):
type R struct{}
func (r *R) Read(p []byte) (n int, err error) {
    fmt.Println("Read")
    copy(p, "abcdefghijklmnop")
    return 16, nil
}
func main() {
    r := new(R)
    br := bufio.NewReaderSize(r, 16)
    buf := make([]byte, 4)
    br.Read(buf)
    fmt.Printf("%q\n", buf)
    br.Discard(4)
    br.Read(buf)
    fmt.Printf("%q\n", buf)
}
Read
"abcd"
"ijkl"
Call to Discard didn’t required reading more data from reader r. If on the other hand buffer has less than n bytes then bufio.Reader will read required amount of data making sure no less than n bytes will be discarded (source code):
type R struct{}
func (r *R) Read(p []byte) (n int, err error) {
    fmt.Println("Read")
    copy(p, "abcdefghijklmnop")
    return 16, nil
}
func main() {
    r := new(R)
    br := bufio.NewReaderSize(r, 16)
    buf := make([]byte, 4)
    br.Read(buf)
    fmt.Printf("%q\n", buf)
    br.Discard(13)
    fmt.Println("Discard")
    br.Read(buf)
    fmt.Printf("%q\n", buf)
}
Read
"abcd"
Read
Discard
"bcde"
Note that 2nd read call has been made because of call to Discard.
Read
At the core of our bufio.Reader sits Read method. It has the same signature as the only method of io.Reader interface so bufio.Reader implements this omnipresent interface:
type Reader interface {
        Read(p []byte) (n int, err error)
}
Read method from bufio.Reader does maximum one read from the underlying io.Reader:
If internal buffer holds at least one byte then no matter what is the size of the input slice (len(p)) method Read will get data only from the internal buffer without reading from the underlying reader (source code):
func (r *R) Read(p []byte) (n int, err error) {
    fmt.Println("Read")
    copy(p, "abcd")
    return 4, nil
}
func main() {
    r := new(R)
    br := bufio.NewReader(r)
    buf := make([]byte, 2)
    n, err := br.Read(buf)
    if err != nil {
        panic(err)
    }
    buf = make([]byte, 4)
    n, err = br.Read(buf)
    if err != nil {
        panic(err)
    }
    fmt.Printf("read = %q, n = %d\n", buf[:n], n)
}



Read
read = "cd", n = 2
Our instance of io.Reader returns “abcd” indefinitely (never gives io.EOF). 2nd call to Read uses slice of length 4 but since internal buffer already holds “cd” after the first read from io.Reader then bufio.Reader returns everything from the buffer without event talking to underlying reader.
2. If internal buffer is empty then one reading from underlying io.Reader will be executed. It’s visible in the previous example where we started with empty buffer and call:
n, err := br.Read(buf)
triggered reading to fill the buffer.
3. If internal buffer is empty but passed slice is bigger than buffer then bufio.Reader will skip buffering and will read directly into passed slice of bytes (source code):
type R struct{}
func (r *R) Read(p []byte) (n int, err error) {
    fmt.Println("Read")
    copy(p, strings.Repeat("a", len(p)))
    return len(p), nil
}
func main() {
    r := new(R)
    br := bufio.NewReaderSize(r, 16)
    buf := make([]byte, 17)
    n, err := br.Read(buf)
    if err != nil {
        panic(err)
    }
    fmt.Printf("read = %q, n = %d\n", buf[:n], n)
    fmt.Printf("buffered = %d\n", br.Buffered())
}
Read
read = "aaaaaaaaaaaaaaaaa", n = 17
buffered = 0
Internal buffer doesn’t have any data (buffered = 0) after reading from bufio.Reader.




{Read, Unread}Byte
These methods have been implemented too either read single byte from the buffer or return last read byte back to the buffer (source code):

// the ReadByte() and UnreadByte() methods read a single byte from the buffer or return the last read byte back to the buffer
br := bufio.NewReader(strings.NewReader("abcd"))

b, _ := br.ReadByte()
fmt.Println(b)
fmt.Println(br.Buffered())

_ = br.UnreadByte()
fmt.Println(br.Buffered())


{Read, Unread}Rune
These two work as previous methods but handling Unicode characters (UTF-8 encoded) instead.



ReadSlice
Function returns bytes till first occurrence of passed byte:
func (b *Reader) ReadSlice(delim byte) (line []byte, err error)
Example (source code):
s := strings.NewReader("abcdef|ghij")
r := bufio.NewReader(s)
token, err := r.ReadSlice('|')
if err != nil {
    panic(err)
}
fmt.Printf("Token: %q\n", token)
Token: "abcdef|"
Important to keep in mind that returned slice points to the internal buffer so it can be overwritten during next read operation.
If delimiter cannot be found and EOF has been reached then returned error will be io.EOF. To test it let’s change one line in program above (source code):
s := strings.NewReader("abcdefghij")
which ends up with crash: panic: EOF. Discussed method will return io.ErrBufferFull when delimiter cannot be found and no more data can fit into internal buffer (source code):
s := strings.NewReader(strings.Repeat("a", 16) + "|")
r := bufio.NewReaderSize(s, 16)
token, err := r.ReadSlice('|')
if err != nil {
    panic(err)
}
fmt.Printf("Token: %q\n", token)
This piece of code leads error: panic: bufio: buffer full.
ReadBytes
func (b *Reader) ReadBytes(delim byte) ([]byte, error)
Returns slice of bytes until the first occurrence of delimiter. It has the same signature as ReadSlice which is low-level function and is actually used underneath by ReadBytes (code). What is the difference then? ReadBytes can call ReadSlice multiple times if separator hasn’t been found and can accumulate returned data. It means that ReadBytes isn’t restricted by the buffer’s size (source code):
s := strings.NewReader(strings.Repeat("a", 40) + "|")
r := bufio.NewReaderSize(s, 16)
token, err := r.ReadBytes('|')
if err != nil {
    panic(err)
}
fmt.Printf("Token: %q\n", token)
Token: "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa|"
Additionally it returns new slice of bytes so no risk that data will be overwritten by future read operations.
ReadString
It’s a simple wrapper over ReadBytes discussed above (code):
func (b *Reader) ReadString(delim byte) (string, error) {
    bytes, err := b.ReadBytes(delim)
    return string(bytes), err
}
ReadLine
ReadLine() (line []byte, isPrefix bool, err error)
Uses ReadSlice underneath (ReadSlice('\n')) but also takes care of removing new-line characters (\n or \r\n) from returned slice. Signature is different than ReadBytes or ReadSlice since it contains isPrefix flag which is true when delimiter hasn’t been found because internal buffer couldn’t hold more data (source code):
s := strings.NewReader(strings.Repeat("a", 20) + "\n" + "b")
r := bufio.NewReaderSize(s, 16)
token, isPrefix, err := r.ReadLine()
if err != nil {
    panic(err)
}
fmt.Printf("Token: %q, prefix: %t\n", token, isPrefix)
token, isPrefix, err = r.ReadLine()
if err != nil {
    panic(err)
}
fmt.Printf("Token: %q, prefix: %t\n", token, isPrefix)
token, isPrefix, err = r.ReadLine()
if err != nil {
    panic(err)
}
fmt.Printf("Token: %q, prefix: %t\n", token, isPrefix)
token, isPrefix, err = r.ReadLine()
if err != nil {
    panic(err)
}
Token: "aaaaaaaaaaaaaaaa", prefix: true
Token: "aaaa", prefix: false
Token: "b", prefix: false
panic: EOF
This method doesn’t give any information if the last returned slice ends with new line character (source code):
s := strings.NewReader("abc")
r := bufio.NewReaderSize(s, 16)
token, isPrefix, err := r.ReadLine()
if err != nil {
    panic(err)
}
fmt.Printf("Token: %q, prefix: %t\n", token, isPrefix)
s = strings.NewReader("abc\n")
r.Reset(s)
token, isPrefix, err = r.ReadLine()
if err != nil {
    panic(err)
}
fmt.Printf("Token: %q, prefix: %t\n", token, isPrefix)
Token: "abc", prefix: false
Token: "abc", prefix: false
WriteTo
bufio.Reader implements io.WriterTo interface:
type WriterTo interface {
        WriteTo(w Writer) (n int64, err error)
}
It allows to pass consumer implementing io.Writer and all data will be read from the producer and send further to passed consumer . Let’s see how it works in practise (source code):
type R struct {
    n int
}
func (r *R) Read(p []byte) (n int, err error) {
    fmt.Printf("Read #%d\n", r.n)
    if r.n >= 10 {
         return 0, io.EOF
    }
    copy(p, "abcd")
    r.n += 1
    return 4, nil
}
func main() {
    r := bufio.NewReaderSize(new(R), 16)
    n, err := r.WriteTo(ioutil.Discard)
    if err != nil {
        panic(err)
    }
    fmt.Printf("Written bytes: %d\n", n)
}
Read #0
Read #1
Read #2
Read #3
Read #4
Read #5
Read #6
Read #7
Read #8
Read #9
Read #10
Written bytes: 40