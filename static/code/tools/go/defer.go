

defer delays the invocation of a function until the surrounding function exits



func main() {
 f, err := os.Open("test.txt")
 if err != nil {
 	log.Fatal(err)
 }

 defer f.Close()
 
 data := make([]byte, 2048)
 for {
 	count, err := f.Read(data)
 	os.Stdout.Write(data[:count])
 	if err != nil {
 		if err != io.EOF {
 			log.Fatal(err)
 		}
 		break
 	}
 }
}


// you can defer multiple closures in a Go function. They run in last-in-first-out order; the last defer registered runs first.

The code within defer closures runs after the return statement. 

any variables passed into a deferred closure aren’t evaluated until the closure runs



A common pattern in Go is for a function that allocates a resource to also return a
closure that cleans up the resource.

func getFile(name string) (*os.File, func(), error) {
 file, err := os.Open(name)
 if err != nil {
 return nil, nil, err
 }
 return file, func() {
 file.Close()
 }, nil
}


Our helper function returns a file, a function, and an error. That * means that a file
reference in Go is a pointer. We’ll talk more about that in the next chapter.
Now in main, we use our getFile function:

f, closer, err := getFile(os.Args[1])
if err != nil {
 log.Fatal(err)
}
defer closer()


