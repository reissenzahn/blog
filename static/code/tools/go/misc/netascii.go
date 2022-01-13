package tftp

const (
	cr = '\r'
	lf = '\n'
	nul = 0
)


// NetasciiReader reads from an io.Reader and decodes the read data from netascii into a format appropriate for the local system
type NetasciiReader struct {
	r *bufio.Reader
}

func NewReader(reader io.Reader) *Reader {
	return &Reader{r: bufio.NewReader(reader)}
}

func (this *NetasciiReader) Read(buf []byte) (int, error) {
	n := 0

	for n < len(buf) {
		curr, err := this.r.ReadByte()
		if err != nil {
			return n, err
		}

		// any cr can only be followed either by an lf or a nul.
		if curr == cr {
			next, err := this.r.ReadByte()
			if err != nil {
				return n, err
			}
			
			// windows uses crlf for a newline while linux uses only lf
			if runtime.GOOS != "windows" && next == lf {
				curr = lf
			} else if next == nul {
				// crnul becomes cr
			} else {
				this.r.UnreadByte()
			}
		}

		buf[n] = curr
		n++
	}

	return n, nil
}


// NetasciiWriter writes encoded netascii to the provided io.Writer
type NetasciiWriter struct {
	w    *bufio.Writer
	last byte
}

func NewWriter(w io.Writer) *Writer {
	return &Writer{w: bufio.NewWriter(w)}
}

func (this *NetasciiWriter) Write(buf []byte) (int, error) {
	n := 0

	for _, curr := range buf {

		// newlines are always crlf
		if e.last != cr && curr == lf {
			err = this.w.WriteByte(cr)
			if err != nil {
				break
			}

			this.last = cr
			n++
		}
		
		// any cr can only be followed either by an lf or a nul
		if this.last == cr && curr != lf && curr != nul {
			err = this.w.WriteByte(nul)
			if err != nil {
				break
			}

			this.last = nul
			n++
		}

		err = this.w.WriteByte(curr)
		if err != nil {
			break
		}

		this.last = curr
		n++
	}

	// we may have written more than buf which is an error
	if n > len(buf) {
		return len(buf), err
	}

	return n, err
}

func (this *NetasciiWriter) Flush() error {
	return this.w.Flush()
}
