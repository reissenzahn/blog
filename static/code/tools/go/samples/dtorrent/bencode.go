package main

// // strings are prefixed with their length followed by a colon
// 5:hello

// // integers are represented as strings in base 10 prefixed with "i" and postfixed with "e"
// i10e


// // lists consist of an arbitrary number of bencoded elements prefixed with "l" and postfixed with "e"
// l
//   i10e
//   5:hello
// e

// // dictionaries consist of an arbitrary number of key/value pairs prefixed with "d" and postfixed with "e" where all keys are bencoded strings and must be sorted
// d
//   5:helloi10e
//   3:byei20e
// e

type decoder struct {
	bufio.Reader
}

func (decoder *decoder) readIntUntil(until byte) (interface{}, error) {
	res, err := decoder.ReadSlice(until)
	if err != nil {
		return nil, err
	}

	str := string(res[:len(res)-1])

	if value, err := strconv.ParseInt(str, 10, 64); err == nil {
		return value, nil
	} else if value, err := strconv.ParseUint(str, 10, 64); err == nil {
		return value, nil
	}
	return nil, err
}

func (decoder *decoder) readInt() (interface{}, error) {
	return decoder.readIntUntil('e')
}

func (decoder *decoder) readInterfaceType(identifier byte) (item interface{}, err error) {
	switch identifier {
	case 'i':
		item, err = decoder.readInt()
	case 'l':
		item, err = decoder.readList()
	case 'd':
		item, err = decoder.readDictionary()
	default:
		if err := decoder.UnreadByte(); err != nil {
			return nil, err
		}
		item, err = decoder.readString()
	}

	return item, err
}

func (decoder *decoder) readList() ([]interface{}, error) {
	var list []interface{}
	for {
		ch, err := decoder.ReadByte()
		if err != nil {
			return nil, err
		}

		if ch == 'e' {
			break
		}

		item, err := decoder.readInterfaceType(ch)
		if err != nil {
			return nil, err
		}
		list = append(list, item)
	}
	return list, nil
}

func (decoder *decoder) readString() (string, error) {
	len, err := decoder.readIntUntil(':')
	if err != nil {
		return "", err
	}

	var stringLength int64
	var ok bool
	if stringLength, ok = len.(int64); !ok {
		return "", errors.New("string length may not exceed the size of int64")
	}

	if stringLength < 0 {
		return "", errors.New("string length can not be a negative number")
	}

	buffer := make([]byte, stringLength)
	_, err = io.ReadFull(decoder, buffer)
	return string(buffer), err
}

func (decoder *decoder) readDictionary() (map[string]interface{}, error) {
	dict := make(map[string]interface{})
	for {
		key, err := decoder.readString()
		if err != nil {
			return nil, err
		}

		ch, err := decoder.ReadByte()
		if err != nil {
			return nil, err
		}

		item, err := decoder.readInterfaceType(ch)
		if err != nil {
			return nil, err
		}

		dict[key] = item

		nextByte, err := decoder.ReadByte()
		if err != nil {
			return nil, err
		}

		if nextByte == 'e' {
			break
		} else if err := decoder.UnreadByte(); err != nil {
			return nil, err
		}
	}
	return dict, nil
}

func DecodeBencode(r io.Reader) (map[string]interface{}, error) {
	decoder := decoder{*bufio.NewReader(r)}

	if b, err := decoder.ReadByte(); err != nil {
		return make(map[string]interface{}), nil
	}
	
	if b != 'd' {
		return nil, errors.New("bencode data must begin with a dictionary")
	}

	return decoder.readDictionary()
}
