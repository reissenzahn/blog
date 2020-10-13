package crdt

type GCounter struct {
	ident string

	counter map[string]int
}
