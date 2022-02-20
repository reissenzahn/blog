package main

Marshaling is the process of converting a Go structure into a JSON record.

Unmarshaling is the process of converting a JSON record given as a byte slice into a Go structure.

The number one bug when converting JSON records into Go structures and vice versa is not making the required fields of your Go structures exported.

import (
 "encoding/json"
 "fmt"
)

type UseAll struct {
	Name string `json:"username"`
	Surname string `json:"surname"`
	Year int `json:"created"`
}

func main() {
 useall := UseAll{Name: "Mike", Surname: "Tsoukalos", Year: 2021}

 t, err := json.Marshal(&useall)
 if err != nil {
 fmt.Println(err)
 } else {
 fmt.Printf("Value %s\n", t)
 }

 str := `{"username": "M.", "surname": "Ts", "created":2020}`

 jsonRecord := []byte(str)
 temp := UseAll{}
 err = json.Unmarshal(jsonRecord, &temp)
 if err != nil {
 fmt.Println(err)
 } else {
 fmt.Printf("Data type: %T with value %v\n", temp, temp)
 }
}
// Ignoring empty fields in JSON
type NoEmpty struct {
 Name string `json:"username"`
 Surname string `json:"surname"`
 Year int `json:"creationyear,omitempty"`
}

Last, imagine that you have some sensitive data on some of the fields of a Go
structure that you do not want to include in the JSON records. You can do that by
including the "-" special value in the desired json: structure tags. This is shown in
the next code excerpt:
// Removing private fields and ignoring empty fields
type Password struct {
 Name string `json:"username"`
 Surname string `json:"surname,omitempty"`
Telling a UNIX System What to Do
[ 278 ]
 Year int `json:"creationyear,omitempty"`
 Pass string `json:"-"`
}
So, the Pass field is going to be ignored when converting a Password structure into a
JSON record using json.Marshal().
These two techniques are illustrated in tagsJSON.go, which can be found in the ch06
directory of the GitHub repository of this book. Running tagsJSON.go produces the
next output:
$ go run tagsJSON.go
noEmptyVar decoded with value {"username":"Mihalis","surname":""}
password decoded with value {"username":"Mihalis"}
For the first line of output, we have the following: the value of noEmpty, which is
converted into a NoEmpty structure variable named noEmptyVar, is NoEmpty{Name:
"Mihalis"}. The noEmpty structure has the default values for the Surname and Year
fields. However, as they are not specifically defined, json.Marshal() ignores the Year
field because it has the omitempty tag but does not ignore the Surname field, which
has the empty string value.
For the second line of output: the value of the password variable is Password{Name:
"Mihalis", Pass: "myPassword"}. When the password variable is converted into a
JSON record, the Pass field is not included in the output. The remaining two fields of
the Password structure, Surname and Year, are omitted because of the omitempty tag.
So, what is left is the username field.
So far, we have seen working with single JSON records. But what happens when
you have multiple records to process? The next subsection answers this question
and many more!
Reading and writing JSON data as streams
Imagine that you have a slice of Go structures that represent JSON records that you
want to process. Should you process the records one by one? It can be done but does
it look efficient? It does not! The good thing is that Go supports the processing of
multiple JSON records as streams instead of individual records, which is faster and
more efficient. This subsection teaches how to perform that using the JSONstreams.go
utility, which contains the following two functions:
Chapter 6
[ 279 ]
// DeSerialize decodes a serialized slice with JSON records
func DeSerialize(e *json.Decoder, slice interface{}) error {
 return e.Decode(slice)
}
The DeSerialize() function is used for reading input in the form of JSON records,
decoding it, and putting it into a slice. The function writes the slice, which is of
the interface{} data type and is given as a parameter, and gets its input from the
buffer of the *json.Decoder parameter. The *json.Decoder parameter along with its
buffer is defined in the main() function in order to avoid allocating it all the time and
therefore losing the performance gains and efficiency of using this type—the same
applies to the use of *json.Encoder that follows:
// Serialize serializes a slice with JSON records
func Serialize(e *json.Encoder, slice interface{}) error {
 return e.Encode(slice)
}
The Serialize() function accepts two parameters, a *json.Encoder and a slice of any
data type, hence the use of interface{}. The function processes the slice and writes
the output to the buffer of the json.Encoder—this buffer is passed as a parameter to
the encoder at the time of its creation.
The JSONstreams.go utility generates random data. Running JSONstreams.go creates
the next output:
$ go run JSONstreams.go
After Serialize:[{"key":"XVLBZ","value":16},{"key":"BAICM","value":89}]
After DeSerialize:
0 {XVLBZ 16}
1 {BAICM 89}
The input slice of structures, which is generated in main(), is serialized as seen in the
first line of the output. After that it is deserialized into the original slice of structures.
Both the Serialize() and DeSerialize() functions can work
with any type of JSON record due to the use of interface{}.
Telling a UNIX System What to Do
[ 280 ]
Pretty printing JSON records
This subsection illustrates how to pretty print JSON records, which means printing
JSON records in a pleasant and readable format without knowing the format of the
Go structure that holds the JSON records. As there exist two ways to read JSON
records, individually and as a stream, there exist two ways to pretty print JSON data:
as single JSON records and as a stream. Therefore, we are going to implement two
separate functions named prettyPrint() and JSONstream(), respectively.
The implementation of the prettyPrint() function is the following:
func PrettyPrint(v interface{}) (err error) {
 b, err := json.MarshalIndent(v, "", "\t")
 if err == nil {
 fmt.Println(string(b))
 }
 return err
}
All the work is done by json.MarshalIndent(), which applies indent to format the
output.
Although both json.MarshalIndent() and json.Marshal() produce a JSON text
result (byte slice), only json.MarshalIndent() allows applying customizable indent,
whereas json.Marshal() generates a more compact output.
For pretty printing streams of JSON data, you should use the JSONstream() function:
func JSONstream(data interface{}) (string, error) {
 buffer := new(bytes.Buffer)
 encoder := json.NewEncoder(buffer)
 encoder.SetIndent("", "\t")
The json.NewEncoder() function returns a new Encoder that writes to a writer that is
passed as a parameter to json.NewEncoder(). An Encoder writes JSON values to an
output stream. Similarly to json.MarshalIndent(), the SetIndent() method allows
you to apply a customizable indent to a stream.
 err := encoder.Encode(data)
 if err != nil {
 return "", err
 }
 return buffer.String(), nil
}
Chapter 6
[ 281 ]
After we are done configuring the encoder, we are free to process the JSON stream
using Encode().
These two functions are illustrated in prettyPrint.go, which generates JSON records
using random data. Running prettyPrint.go produces the following kind of output:
Last record: {BAICM 89}
{
 "key": "BAICM",
 "value": 89
}
[
 {
 "key": "XVLBZ",
 "value": 16
 },
 {
 "key": "BAICM",
 "value": 89
 }
]
The previous output shows the beautified output of a single JSON record followed
by the beautified output of a slice with two JSON records—all JSON records are
represented as Go structures.

