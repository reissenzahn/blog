package main

import (
 "encoding/xml"
 "fmt"
)

type Employee struct {
 XMLName xml.Name `xml:"employee"`
 ID int `xml:"id,attr"`
 FirstName string `xml:"name>first"`
 LastName string `xml:"name>last"`
 Height float32 `xml:"height,omitempty"`
 Address
 Comment string `xml:",comment"`
}
This is where the structure for the XML data is defined. However, there is additional
information regarding the name and the type of each XML element. The XMLName
field provides the name of the XML record, which in this case will be employee.
A field with the tag ",comment" is a comment and it is formatted as such in the
output. A field with the tag attr appears as an attribute to the provided field name
(which is id in this case) in the output. The name>first notation tells Go to embed
the first tag inside a tag called name.
Lastly, a field with the omitempty option is omitted from the output if it is empty. An
empty value is any of 0, false, a nil pointer or interface, and any array, slice, map,
or string with a length of zero.
type Address struct {
 City, Country string
}
func main() {
 r := Employee{ID: 7, FirstName: "Mihalis", LastName: "Tsoukalos"}
 r.Comment = "Technical Writer + DevOps"
 r.Address = Address{"SomeWhere 12", "12312, Greece"}
 output, err := xml.MarshalIndent(&r, " ", " ")
As is the case with JSON, xml.MarshalIndent() is for beautifying the output.
 if err != nil {
 fmt.Println("Error:", err)
 }
 output = []byte(xml.Header + string(output))
 fmt.Printf("%s\n", output)
}
Chapter 6
[ 283 ]
The output of xml.go is the following:
<?xml version="1.0" encoding="UTF-8"?>
 <employee id="7">
 <name>
 <first>Mihalis</first>
 <last>Tsoukalos</last>
 </name>
 <City>SomeWhere 12</City>
 <Country>12312, Greece</Country>
 <!--Technical Writer + DevOps-->
 </employee>
The previous output shows the XML version of the Go structure given as input to the
program.
In the next section we develop a utility that converts JSON records to XML records
and vice versa.
Converting JSON to XML and vice versa
As promised, we are going to produce a utility that converts records between the
JSON and XML formats. The input is given as a command-line argument. The
utility tries to guess the format of the input starting from XML. If xml.Unmarshal()
fails, then the utility tries using json.Unmarshal(). If there is not a match, then the
user is informed about the error condition. On the other hand, if xml.Unmarshal()
is successful, the data is stored into an XMLrec variable and is then converted into a
JSONrec variable. The same happens with json.Unmarshal() in the case where the
xml.Unmarshal() call is unsuccessful.
The logic of the utility can be found in the Go structures:
type XMLrec struct {
 Name string `xml:"username"`
 Surname string `xml:"surname,omitempty"`
 Year int `xml:"creationyear,omitempty"`
}
type JSONrec struct {
 Name string `json:"username"`
 Surname string `json:"surname,omitempty"`
 Year int `json:"creationyear,omitempty"`
}
Telling a UNIX System What to Do
[ 284 ]
Both structures store the same data. However, the former (XMLrec) is for storing XML
data whereas the latter (JSONrec) is for storing JSON data.
Running JSON2XML.go produces the next kind of output:
$ go run JSON2XML.go '<XMLrec><username>Mihalis</username></XMLrec>'
<XMLrec><username>Mihalis</username></XMLrec>
{"username":"Mihalis"}
So, we give an XML record as input, which is converted into a JSON record.
The next output illustrates the reverse process:
$ go run JSON2XML.go '{"username": "Mihalis"}'
{"username": "Mihalis"}
<XMLrec><username>Mihalis</username></XMLrec>
In the previous output the input is a JSON record, and the output is an XML record.
The next section discusses working with YAML files in Go.
Working with YAML
In this section, we briefly discuss how to work with YAML files in Go. The Go
standard library does not include support for YAML files, which means that you
should look at external packages for YAML support. There exist three main packages
that allow you to work with YAML from Go:
• https://github.com/kylelemons/go-gypsy
• https://github.com/go-yaml/yaml
• https://github.com/goccy/go-yaml
Choosing one is a matter of personal preference. We are going to work with go-yaml
in this section using the code found in yaml.go. Due to the use of Go modules,
yaml.go is developed in ~/go/src/github.com/mactsouk/yaml—you can also find
it in the GitHub repository of this book. The most important part of it is the next:
var yamlfile = `
image: Golang
matrix:
 docker: python
 version: [2.7, 3.9]
`
Chapter 6
[ 285 ]
The yamlfile variable contains the YAML data; you usually read the data from a
file—we are just using that to save some space.
type Mat struct {
 DockerImage string `yaml:"docker"`
 Version []float32 `yaml:",flow"`
}
The Mat structure defines two fields and their associations with the YAML file. The
Version field is a slice of float32 values. As there is no name for the Version field,
the name is going to be version. The flow keyword says that the marshal is using a
flow style, which is useful for structs, sequences, and maps.
type YAML struct {
 Image string
 Matrix Mat
}
The YAML structure embeds a Mat structure and contains a field named Image, which
is associated with image in the YAML file. The main() function contains the expected
yaml.Unmarshal() and yaml.Marshal() calls.
Once you have the source file at the desired place, run the next commands—if you
need to run any extra commands, the go binary is nice enough to help you:
$ go mod init
$ go mod tidy
The go mod init command initializes and writes a new go.mod file in the current
directory whereas the go mod tidy command synchronizes go.mod with the source
code.
Running yaml.go produces the next output:
$ go run yaml.go
After Unmarshal (Structure):
{Golang {python [2.7 3.9]}}
If you want to play it safe and you are using packages that do not
belong to the standard library, then developing inside ~/go/src,
committing to a GitHub repository, and using Go modules for all
dependencies might be the best option. However, this does not
mean that you must develop your own packages in the form of Go
modules.
Telling a UNIX System What to Do
[ 286 ]
After Marshal (YAML code):
image: Golang
matrix:
 docker: python
 version: [2.7, 3.9]
The previous output shows how the {Golang {python [2.7 3.9]}} text is converted
into a YAML file and vice versa. Now that we know about working with JSON,
XML, and YAML data in Go, we are ready to learn about the viper package.