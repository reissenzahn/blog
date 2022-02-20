package main

import (
	"fmt"
	"net/http"
	"os"
	"time"
)

func main() {
	port := ":8000"

	args := os.Args
	if len(args) != 1 {
		port = ":" + args[1]
	}

	http.HandleFunc("/time", func (w http.ResponseWriter, r *http.Request) {
		t := time.Now().Format(time.RFC1123)
		
		fmt.Fprintf(w, "<h1>%s</h1>", t)
		
		fmt.Printf("%s\n", r.URL.Path)
	})

	http.HandleFunc("/", func (w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "<h1>404</h1>")

		fmt.Printf("%s\n", r.URL.Path)
	})
	
	fmt.Printf("listening on %s...\n", port)

	err := http.ListenAndServe(port, nil)
	if err != nil {
		fmt.Println(err)
		return
	}
}

// go run time.go
// curl localhost:8000/time
