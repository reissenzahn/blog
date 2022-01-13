package main

import (
	"encoding/binary"
)


the encoding/binary package is built for working with binary protocols

To read and write binary data from byte slices we’ll use ByteOrder:

type ByteOrder interface {
	Uint16([]byte) uint16
	Uint32([]byte) uint32
	Uint64([]byte) uint64
	PutUint16([]byte, uint16)
	PutUint32([]byte, uint32)
	PutUint64([]byte, uint64)
	String() string
}

This interface has two implementations: BigEndian and LittleEndian. To write a fixed-length number to a byte slice we’ll choose an endianness and simply call the appropriate Put method:

v := uint32(500)
buf := make([]byte, 4)

binary.BigEndian.PutUint32(buf, v)

You can then read your value from the buffer by using the associated getter method:

x := binary.BigEndian.Uint32(buf)

The binary package provides two built-in functions for reading and writing fixed-length values to streams. They are appropriately named: Read() and Write().

The Read() function works by inspecting the type of data and reading and decoding the appropriate number of bytes using the endianness specified by the order argument:

This function supports all fixed-length integer, float, and complex number types by using a fast type switch internally. For composite types, like structs and slices, it falls back to a slower reflection-based decoder.

On the encoding side, the Write() function works in the opposite way by inspecting the type of data and then encoding using the endianness specified by order and then writing that data to w:
func Write(w io.Writer, order ByteOrder, data interface{}) error
The same rules apply to this function as the Read() function.


---
// Package encoding/binary offers a convenient way of encoding data structures with numeric values into fixed-size binary representation.

// binary packet with  four fields including SensorID (16 bit), LocationID (16 bit), Timestamp (32 bit), and Temperature (16 bit)


// The binary package supports several ways of encoding and decoding fixed-size numerical values. They are called fixed because the underlying binary representation always utilize a fix number of bytes for a given type (i.e. 32 bits for all 32-bit size values). Some functions in package binary can also encode and decode arrays, slices, and structs containing only fixed-size values


the binary package provides interface binary.ByteOrder

type ByteOrder interface {
    Uint16([]byte) uint16
    Uint32([]byte) uint32
    Uint64([]byte) uint64
    PutUint16([]byte, uint16)
    PutUint32([]byte, uint32)
    PutUint64([]byte, uint64)
    String() string
}

This interface is implemented by types binary.LittleEndian and binary.BigEndian. These types can be used to automatically encode/decode values with the proper endianness.


Let us use the BigEndian type to encode a single packet of the sensor data. Because each field in the packet is fixed-length, it is easy to explicitly construct the packet by populating each field individually, as shown in the following snippet, into a buffer (variable buf):

func main() {
    buf := make([]byte, 10)
    ts := uint32(time.Now().Unix())
    binary.BigEndian.PutUint16(buf[0:], 0xa20c) // sensorID
    binary.BigEndian.PutUint16(buf[2:], 0x04af) // locationID
    binary.BigEndian.PutUint32(buf[4:], ts)     // timestamp
    binary.BigEndian.PutUint16(buf[8:], 479)    // temp 
    fmt.Printf("% x\n", buf)
}

func main() {
    buf := <contains encoded bytes>
    sensorID := binary.BigEndian.Uint16(buf[0:])
    locID := binary.BigEndian.Uint16(buf[2:])
    tstamp := binary.BigEndian.Uint32(buf[4:])
    temp := binary.BigEndian.Uint16(buf[8:])
    fmt.Printf("sid: %0#x, locID %0#x ts: %0#x, temp:%d\n", 
        sensorID, locID, tstamp, temp)
}

Fortunately, the binary package supports Go’s streaming IO interfaces io.Reader and io.Writer when encoding and decoding data.

binary.Write
Function binary.Write(w io.Writer, o ByteOrder, data interface{}) encodes parameter data with the provided io.Writer with the specified ByteOrder. Parameter data is provided as empty interface{} (instead of []bytes). This is because the Write function can do some helpful magic to automatically encode the fixed-size values when they are passed in as:
Numeric data types (i.e. bool, int8, uint8, int16, float32, etc.)
Arrays, slices, or structs containing only fixed-size values
Pointers to the types in the previous bullets
For instance, the following example introduces struct type packet to encapsulate the previously defined fields from the data packet. Notice that the struct only contains numeric fields that appear in the expected order. The code uses function Write to automatically encode values stored in packet value dataIn:
type packet struct {
    Sensid uint32
    Locid  uint16
    Tstamp uint32
    Temp   int16
}
func main() {
    dataIn := packet{
        Sensid: 1, Locid: 1233, Tstamp: 123452123, Temp: 12,
    }
    buf := new(bytes.Buffer)
    err := binary.Write(buf, binary.BigEndian, dataIn)
    if err != nil {
        fmt.Println(err)
        return
    }
}

In the previous snippet, after function Write is executed, writer buf will write the 10 bytes encoded using BigEndian. This time however, notice there is no explicit calls to any ByteOrder methodPutXXX as before. Function Write handles the internal buffer bookkeeping and automatically figures out data sizes from the struct field types.
Decoding with binary.Read
Conversely, function binary.Read(r io.Reader, o ByteOrder, data interface{}) can read encoded data from an io.Reader and decode it automatically into parameter data which must be one of the followings:
A pointer to a numeric value (i.e. bool, int8, uint8, int16, float32, etc.)
A pointer to a struct or array of numeric values
A slice of numbers or structs/arrays containing only numbers
For instance in the following code snippet, bytes from io.Reader buf are decoded with function Read and the decoded values are stored in the provided struct variable dataOut:
type packet struct {
    Sensid uint32
    Locid  uint16
    Tstamp uint32
    Temp   int16
}
func main() {
    buf := <reader with encoded binary data>
    var dataOut packet
    err := binary.Read(buf, binary.BigEndian, &dataOut)
    if err != nil {
        fmt.Println("failed to Read:", err)
        return 
    }
}
Source code github.com/vladimirvivien/go-binary/encode1.go
When the previous code is executed, function Read uses io.Reader buf to read and decode binary values using the BigEndian byte order. Notice there is no explicit calls to ByteOrder methods UintXXX as before. Function Read automatically handles buffer bookkeeping and figures out the proper data sizes to correctly decode the data into the respective fields of variable dataOut.
Encoding multiple packets
As mentioned above, functions binary.Read and binary.Write can also encode and decode data stored in arrays and slices. For instance the following snippet encodes and end decodes multiple data packets stored in a slice of packet:
type packet struct {
    Sensid uint32
    Locid  uint16
    Tstamp uint32
    Temp   int16
}
func main() {
    dataOut := []packet{
        {Sensid: 1, Locid: 1233, Tstamp: 123452123, Temp: 12},
        {Sensid: 2, Locid: 4567, Tstamp: 133452124, Temp: 32},
        {Sensid: 7, Locid: 8910, Tstamp: 143452125, Temp: -12},
    }
    // encode a slice of packet
    buf := new(bytes.Buffer)
    err := binary.Write(buf, binary.LittleEndian, dataOut)
    if err != nil {
        fmt.Println(err)
        return
    }
    // decode all items from slice
    dataIn := make([]packet, 3)
    err := binary.Read(buf, binary.LittleEndian, dataIn)
    if err != nil {
        fmt.Println("failed to Read:", err)
        return
    }
    fmt.Printf("%v", dataIn)
}
Source code github.com/vladimirvivien/go-binary/encode2.go
In the snippet above, slice dataOut is encoded using function Write and stored in byte buffer buf. The reverse is done with function Read which decode the data from buf and reconstructs the slice of packet which is assigned to variable dataIn.


