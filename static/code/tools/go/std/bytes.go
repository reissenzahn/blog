package main

import (
  "fmt"
  "bytes"
)

func main() {

  // byte is an alias for uint8
  var b byte = 97
  fmt.Printf("%c\n", b)  //

  // 
}


The bytes package contains functions for manipulation of byte slices.


 := []byte("hello")
data2 := []byte("ell")

With Contains we check if data2 slice is a subslice of data1.
bytes.Contains(data1, data2)

bytes.Equal([]byte("falcon"), []byte("owl"))

The Fields function splits a byte slice into subslices removing any space characters, including newlines.
     data3 := []byte{111, 119, 108, 9, 99, 97, 116, 32, 32, 32, 32, 100, 111,
          103, 32, 112, 105, 103, 32, 32, 32, 32, 98, 101, 97, 114}

     fields := bytes.Fields(data3)
     fmt.Println(fields)

     for _, e := range fields {
          fmt.Printf("%s ", string(e))
     }

     fmt.Println()
}



data := [][]byte{[]byte("an"), []byte("old"), []byte("wolf")}
     joined := bytes.Join(data, []byte(" "))

     fmt.Println(data)
     fmt.Println(joined)
     fmt.Println(string(joined))

     fmt.Println("--------------------------")

     data2 := []byte{102, 97, 108, 99, 111, 110, 32}
     data3 := bytes.Repeat(data2, 3)

     fmt.Println(data3)
     fmt.Println(string(data3))

     fmt.Println("--------------------------")

     data4 := []byte{32, 32, 102, 97, 108, 99, 111, 110, 32, 32, 32}
     data5 := bytes.Trim(data4, " ")

     fmt.Println(data5)
     fmt.Println(string(data5))
}
The example joins byte slices with Join, repeats a byte slice with Repeat, and trims byte slices of the specified byte with Trim.


// bytes.Buffer is a variable-sized buffer of bytes with Read and Write methods

var buf bytes.Buffer

buf.Write([]byte("a old"))
buf.WriteByte(32)
buf.WriteString("cactus")
buf.WriteByte(32)
buf.WriteByte(32)
buf.WriteRune('🌵')

fmt.Println(buf.Bytes())
fmt.Println(buf.String())
