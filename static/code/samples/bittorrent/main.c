#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

// DEBUG
void print_bytes(uint8_t *buf, size_t size) {
  printf("[ ");
  for (size_t i = 0; i < size; i++) {
    printf("%*u ", 3, buf[i]);
  }
  printf("]\n");
}

void print_bits(uint8_t *buf, size_t size) {
  printf("[ ");
  for (size_t i = 0; i < size; i++) {
    for (int j = 7; 0 <= j; j--) {
      printf("%c", (buf[i] & (1 << j)) ? '1' : '0');
    }
    printf(" ");
  }
  printf("]\n");
}


// PACK
int pack_uint32(uint8_t *buf, uint32_t val) {
  *buf++ = val >> 24;
  *buf++ = val >> 16;
  *buf++ = val >> 8;
  *buf++ = val;

  return 4;
}

int pack_uint16(uint8_t *buf, uint16_t val) {
  *buf++ = val >> 8;
  *buf++ = val;

  return 2;
}

int pack_bytes(uint8_t *buf, void *data, size_t n) {
  memcpy(buf, data, n);
  
  return n;
}


// STRING
struct string {
  char *data;
  size_t size;
};

int string_init(struct string *self, char *data, size_t size) {
  char *data = strndup(data, size);
  if (data == NULL) {
    return -1;
  }

  self->data = data;
  self->size = size;
}

void string_free(struct string *self) {
  free(self->data);
}


// LIST

struct list_node {
  struct list_node *next;
  size_t size;

  // TODO: why is this not a void *?
  unsigned char *data;
};

struct list {
  struct list_node *head;
  unsigned int size;
};

void list_init(struct list *self) {
  self->head = NULL;
  self->size = 0;
}

int list_add(struct list *self, unsigned char *data, size_t size) {
  struct list_node *node = malloc(sizeof(struct list_node));
  if (node == NULL) {
    return -1;
  }

  if (list->head == NULL) {
    list->head = node;
  } else {
    struct list_node *curr = head;

    while (curr->next != NULL) {
      curr = curr->next;
    }

    curr->next = node;
  }

  list->size++;

  return 0;
}

void list_free(struct list *self) {
  struct list_node *curr = self->head;

  while (curr != NULL) {
    struct list_node *temp = curr->next;
    free(curr);
    curr = temp;
  }
}


// DICTIONARY



// BENCODE
struct bencode {
  enum {
    BENCODE_INTEGER,
    BENCODE_STRING,
    BENCODE_LIST,
    BENCODE_DICTIONARY
  } type;

  union {
    int64_t integer;
    struct string *string;
    struct list *list;
    struct dictionary *dictionary;
  };
};


void bencode_print(struct bencode *self) {
  switch (self->type) {
  case BENCODE_INTEGER:
    printf("int: %ld\n", self->integer);
    break;
  case BENCODE_STRING:
    break;
  }
}

static struct bencode *bencode_parse(const char *data) {
  if (isdigit(data[0])) {
    return bencode_parse_string(data);
  } else if (data[0] == 'i') {
    return bencode_parse_integer(data);
  } else if (data[0] == 'l') {
    return bencode_parse_list(data);
  } else if (data[0] == 'd') {
    return bencode_parse_dict(data);
  }

  return NULL;
}

static struct bencode *bencode_parse_integer(char *data) {
  struct bencode *ret = (struct bencode *) malloc(sizeof(struct bencode));
  assert(ret != NULL);

  assert(*data == 'i')
  data++;

  ret->type = BENCODE_INTEGER;
  ret->integer = strtol(data, &data, 10);
  assert(*data == 'e');

  return ret;
}

static struct bencode *bencode_parse_string(char *data) {
  struct bencode *ret = (struct bencode *) malloc(sizeof(struct bencode));
  assert(ret != NULL);

  size_t size = strtol(data, &data, 10);
  assert(*data == ':');

  struct string *str = malloc(sizeof(struct string));
  string_init(str, data, size);

  ret->type = BENCODE_STRING;
  ret->string = str;

  return ret;
}

static struct bencode *bencode_parse_list(char *data) {
  struct bencode *ret = (struct bencode *) malloc(sizeof(struct bencode));
  assert(ret != NULL);

  assert(*data == 'l');
  data++;

  struct list *list = malloc(sizeof(struct list));
  assert(list != NULL);

  list_init(list);

  while (*data != 'e') {
    // TODO: this is all fucked because it needs the endptr thing
    struct bencode *item = bencode_parse(data);
    assert(item);
    list_add(list, (unsigned char *) &item, sizeof(item));
  }

  ret->type = BENCODE_LIST;
  ret->list = list;
  assert(*data == 'e');
  
  return ret;
}

static struct bencode *bencode_parse_dictionary(char *data) {
  struct bencode *ret = malloc(sizeof(struct bencode));
  assert(ret != NULL);

  data++;

  while (*data != NULL) {
    
  }

  ret->type = BENCODE_DICTIONARY;
  ret->dictionary = dictionary;
  
  return ret;
}

int main() {

}


// BITFIELD
struct bitfield {
  size_t bits;
  uint8_t *data;
  size_t size;
};

int bitfield_init(struct bitfield *self, size_t bits) {
  size_t size = (bits / 8) + ((bits % 8) != 0);

  uint8_t *data = (uint8_t *) malloc(sizeof(uint8_t) * size);
  if (data == NULL) {
    return -1;
  }

  self->bits = bits;
  self->data = data;
  self->size = size;

  return 0;
}

int bitfield_get(struct bitfield *self, size_t index) {
  return ((self->data[index / 8] >> (7 - (index % 8))) & 1) != 0;
}

void bitfield_set(struct bitfield *self, size_t index) {
  self->data[index / 8] |= (1 << (7 - (index % 8)));
}

void bitfield_clear(struct bitfield *self, size_t index) {
  self->data[index / 8] &= ~(1 << (7 - (index % 8)));
}


// MESSAGE
struct message {
  enum {
    CHOKE = 0,
    UNCHOKE = 1,
    INTERESTED = 2,
    UNINTERESTED = 3,
    HAVE = 4,
    BITFIELD = 5,
    REQUEST = 6,
    PIECE = 7,
    CANCEL = 8,
  } type;

  union {
    struct {
      uint32_t piece_index;
    } have;

    struct bitfield bitfield;

    struct {
      uint32_t piece_index;
      uint32_t block_offset;
      uint32_t block_length;
    } request, cancel;

    struct {
      uint32_t piece_index;
      uint32_t block_offset;
      uint8_t *block_data;
      uint8_t size;
    } piece;
  };
};

ssize_t message_serialize(struct message *self, uint8_t **buf) {
  uint8_t *cur;

  switch (self->type) {
  case CHOKE:
  case UNCHOKE:
  case INTERESTED:
  case UNINTERESTED:
    *buf = cur = (uint8_t *) malloc(4 + 1);
    cur += pack_uint32(*buf, 1);
    *cur++ = self->type;
    break;
  case HAVE:
    *buf = cur = (uint8_t *) malloc(4 + 1 + 4);
    cur += pack_uint32(cur, 5);
    *cur++ = self->type;
    cur += pack_uint32(cur, self->have.piece_index);
    break;
  case BITFIELD:
    *buf = cur = (uint8_t *) malloc(4 + 1 + self->bitfield.size);
    cur += pack_uint32(cur, 1 + self->bitfield.size);
    *cur++ = self->type;
    cur += pack_bytes(cur, self->bitfield.data, self->bitfield.size);
    break;
  case REQUEST:
    *buf = cur = (uint8_t *) malloc(4 + 1 + 4 + 4 + 4);
    cur += pack_uint32(*buf, 13);
    *cur++ = self->type;
    cur += pack_uint32(cur, self->cancel.piece_index);
    cur += pack_uint32(cur, self->cancel.block_offset);
    cur += pack_uint32(cur, self->cancel.block_length);
    break;
  case PIECE:
    *buf = cur = (uint8_t *) malloc(4 + 1 + 4 + 4 + self->piece.size);
    cur += pack_uint32(cur, 9 + self->piece.size);
    *cur++ = self->type;
    cur += pack_uint32(cur, self->piece.piece_index);
    cur += pack_uint32(cur, self->piece.block_offset);
    cur += pack_bytes(cur, self->piece.block_data, self->piece.size);
    break;
  case CANCEL:
    *buf = cur = (uint8_t *) malloc(4 + 1 + 4 + 4 + 4);
    cur += pack_uint32(cur, 13);
    *cur++ = self->type;
    cur += pack_uint32(cur, self->cancel.piece_index);
    cur += pack_uint32(cur, self->cancel.block_offset);
    cur += pack_uint32(cur, self->cancel.block_length);
    break;
  default:
    return -1;
  }

  return cur - *buf;
}

void message_print(struct message *self) {
  print("-------[Message]-------\n");

  switch (self->type) {
  case CHOKE:
    printf("type: CHOKE (%d)\n", self->type);
    break;  
  case UNCHOKE:
    printf("type: UNCHOKE (%d)\n", self->type);
    break;  
  case INTERESTED:
    printf("type: INTERESTED (%d)\n", self->type);
    break;  
  case UNINTERESTED:
    printf("type: UNINTERESTED (%d)\n", self->type);
    break;  
  case HAVE:
    printf("type: HAVE (%d)\n", self->type);
    printf("piece_index: %d\n", self->have.piece_index);
    break;
  case BITFIELD:
    printf("type: BITFIELD (%d)\n", self->type);
    printf("bitfield: ");
    print_bits(self->bitfield.data);
    printf("\n");
    break;
  default:
    printf("type: UNKNOWN (%d)", self->type);
    break;
  }

  print("-----------------------\n");
}


BITFIELD = 5,
REQUEST = 6,
PIECE = 7,
CANCEL = 8,

// int main(int argc, char *argv[]) {

//   struct message msg;
//   uint8_t *buf;

//   // CHOKE, UNCHOKE, INTERESTED, UNINTERESTED
//   // msg.type = CHOKE;

//   // HAVE
//   // msg.type = HAVE;
//   // msg.have.piece_index = 1234;

//   // BITFIELD
//   // struct bitfield bitfield;
//   // bitfield_init(&bitfield, 25);
//   // bitfield_set(&bitfield, 3);
//   // bitfield_set(&bitfield, 8);
//   // bitfield_set(&bitfield, 15);
//   // bitfield_set(&bitfield, 21);

//   // msg.type = BITFIELD;
//   // msg.bitfield = bitfield;

//   // 

//   ssize_t size = message_serialize(&msg, &buf);

//   print_bytes(buf, size);
//   print_bits(buf, size);
//   printf("%zu\n", size);

//   return 0;
// }


// ssize_t serialize_packet(uint8_t *buf, struct packet *pkt) {
//   uint8_t *start = buf;

//   buf += pack_uint16(buf, pkt->opcode);

//   switch (pkt->opcode) {
//   case RRQ:
//     buf += pack_bytes(buf, pkt->rrq.filename, strlen(pkt->rrq.filename));
//     *buf++ = 0;
//     buf += pack_bytes(buf, pkt->rrq.mode, strlen(pkt->rrq.mode));
//     *buf++ = 0;
//     break;

//   case WRQ:
//     buf += pack_bytes(buf, pkt->wrq.filename, strlen(pkt->wrq.filename));
//     *buf++ = 0;
//     buf += pack_bytes(buf, pkt->wrq.mode, strlen(pkt->wrq.mode));
//     *buf++ = 0;
//     break;
      
//   case DATA:
//     buf += pack_uint16(buf, pkt->data.block_num);
//     buf += pack_bytes(buf, pkt->data.block, pkt->data.size);
//     break;
  
//   case ACK:
//     buf += pack_uint16(buf, pkt->data.block_num);
//     break;
  
//   case ERROR:
//     buf += pack_uint16(buf, pkt->error.error_code);
//     buf += pack_bytes(buf, pkt->error.error_msg, strlen(pkt->error.error_msg));
//     *buf++ = 0;
//     break;
//   default:
//     return -1;
//   }

//   return buf - start; 
// }




// uint16_t unpack_uint16(uint8_t **buf) {
//   uint16_t ret = ((uint16_t) (*buf)[0] << 8) | (*buf)[1];
//   *buf += 2;
//   return ret;
// }

// void unpack_str(char *dest, uint8_t **src, size_t n) {
//   size_t i = 0;

//   while (i < n && (*src)[i] != 0) {
//     dest[i] = (*src)[i];
//     i++;
//   }

//   *src += i + 1;

//   while (i < n) {
//     dest[i] = '\0';
//     i++;
//   }
// }

// void unpack_bytes(uint8_t *dest, uint8_t **src, size_t n) {
//   memcpy(dest, *src, n);
//   *src += n;
// }

// int deserialize_packet(struct packet *pkt, uint8_t *buf, size_t size) {
//   if (pkt == NULL || buf == NULL || size < 4) {
//     return -1;
//   }

//   memset(pkt, 0, sizeof(struct packet));

//   pkt->opcode = unpack_uint16(&buf);

//   switch (pkt->opcode) {
//   case RRQ:
//     unpack_str(pkt->rrq.filename, &buf, MAX_FILENAME_SIZE);
//     unpack_str(pkt->rrq.mode, &buf, MAX_MODE_SIZE);
//     break;
  
//   case WRQ:
//     unpack_str(pkt->wrq.filename, &buf, MAX_FILENAME_SIZE);
//     unpack_str(pkt->wrq.mode, &buf, MAX_MODE_SIZE);
//     break;

//   case DATA:
//     pkt->data.block_num = unpack_uint16(&buf);
//     pkt->data.size = (size - (2 * sizeof(uint16_t)));
//     unpack_bytes(pkt->data.block, &buf, pkt->data.size);
//     break;

//   case ACK:
//     pkt->ack.block_num = unpack_uint16(&buf);
//     break;
  
//   case ERROR:
//     pkt->error.error_code = unpack_uint16(&buf);
//     unpack_str(pkt->error.error_msg, &buf, MAX_ERROR_MSG_SIZE);
//     break;
  
//   default:
//     return -1;
//   }

//   return 0;
// }







// int main(int argc, char *argv[]) {

//   // TEST SERDER

//   struct packet test_rrq;
//   test_rrq.opcode = RRQ;
//   memcpy(test_rrq.rrq.filename, "test.txt", strlen("test.txt") + 1);
//   memcpy(test_rrq.rrq.mode, "octet", strlen("octet") + 1);
  
//   uint8_t b1[1024];
//   ssize_t test_rrq_len = serialize_packet(b1, &test_rrq);
//   assert(test_rrq_len == 2 + strlen("test.txt") + 1 + strlen("octet") + 1);

//   struct packet test_rrq_out;
//   deserialize_packet(&test_rrq_out, b1, 1024);

//   assert(test_rrq_out.opcode == RRQ);
//   assert(memcmp(test_rrq_out.rrq.filename, "test.txt", 9) == 0);
//   assert(memcmp(test_rrq_out.rrq.mode, "octet", 6) == 0);


//   struct packet test_wrq;
//   test_wrq.opcode = WRQ;
//   memcpy(test_wrq.rrq.filename, "test.txt", strlen("test.txt") + 1);
//   memcpy(test_wrq.rrq.mode, "octet", strlen("octet") + 1);
  
//   uint8_t b2[1024];
//   ssize_t test_wrq_len = serialize_packet(b2, &test_wrq);
//   assert(test_wrq_len == 2 + strlen("test.txt") + 1 + strlen("octet") + 1);

//   struct packet test_wrq_out;
//   deserialize_packet(&test_wrq_out, b2, 1024);

//   assert(test_wrq_out.opcode == WRQ);
//   assert(memcmp(test_wrq_out.wrq.filename, "test.txt", 9) == 0);
//   assert(memcmp(test_wrq_out.wrq.mode, "octet", 6) == 0);


//   struct packet test_data;
//   test_data.opcode = DATA;
//   test_data.data.block_num = 123;
//   test_data.data.size = 5;
//   uint8_t arr[] = {101, 102, 103, 104, 105};
//   memcpy(test_data.data.block, arr, 5);

//   uint8_t b3[1024];
//   ssize_t test_data_len = serialize_packet(b3, &test_data);

//   assert(test_data_len == 2 + 2 + 5);

//   struct packet test_data_out;
//   deserialize_packet(&test_data_out, b3, test_data_len);
  
//   assert(test_data_out.data.block_num == 123);
//   assert(test_data_out.data.size == 5);
//   assert(memcmp(test_data_out.data.block, arr, test_data_out.data.size) == 0);


//   struct packet test_ack;
//   test_ack.opcode = ACK;
//   test_ack.ack.block_num = 123;

//   uint8_t b4[1024];
//   ssize_t test_ack_len = serialize_packet(b4, &test_ack);

//   assert(test_ack_len == 2 + 2);

//   struct packet test_ack_out;
//   deserialize_packet(&test_ack_out, b4, test_ack_len);

//   assert(test_ack_out.opcode == ACK);
//   assert(test_ack_out.ack.block_num == 123);


//   struct packet test_error;
//   test_error.opcode = ERROR;
//   test_error.error.error_code = FILE_NOT_FOUND;
//   memcpy(test_error.error.error_msg, error_messages[FILE_NOT_FOUND], strlen(error_messages[FILE_NOT_FOUND]) + 1);

//   uint8_t b5[1024];
//   ssize_t test_error_len = serialize_packet(b5, &test_error);

//   assert(test_error_len == 2 + 2 + strlen(error_messages[FILE_NOT_FOUND]) + 1);

//   struct packet test_error_out;
//   deserialize_packet(&test_error_out, b5, test_error_len);

//   assert(test_error_out.opcode == ERROR);
//   assert(test_error_out.error.error_code == FILE_NOT_FOUND);
//   assert(memcmp(test_error.error.error_msg, error_messages[FILE_NOT_FOUND], strlen(error_messages[FILE_NOT_FOUND]) + 1) == 0);  

//   exit(EXIT_SUCCESS);
// }
