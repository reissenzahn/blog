package main

import (
	"fmt"
	"os"
	"path/filepath"
)

func main() {
	args := os.Args
	if len(args) == 1 {
		fmt.Println("usage: which [name]")
		return
	}

	file := args[1]

	dirs := filepath.SplitList(os.Getenv("PATH"))

	for _, dir := range dirs {
		path := filepath.Join(dir, file)

		info, err := os.Stat(path)
		if err != nil {
			continue
		}

		mode := info.Mode()

		if mode.IsRegular() && mode & 0111 != 0 {
			fmt.Println(path)
			return
		}
	}
}

// $ go run which.go which
