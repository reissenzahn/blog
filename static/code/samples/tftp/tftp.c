
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#define MAX_FILENAME_SIZE  1024
#define MAX_ERROR_MSG_SIZE 1024
#define MAX_MODE_SIZE      8
#define BLOCK_SIZE         512


/* types */

enum opcode {
  RRQ   = 1,
  WRQ   = 2,
  DATA  = 3,
  ACK   = 4,
  ERROR = 5
};

enum errorcode {
  UNDEFINED = 0,
  FILE_NOT_FOUND,
  ACCESS_VIOLATION,
  DISK_FULL,
  ILLEGAL_OPERATION,
  UNKNOWN_TRANSFER_ID,
  FILE_ALREADY_EXISTS,
  NO_SUCH_USER
};

struct packet {
  uint16_t opcode;

  union {
    struct {
      uint8_t filename[MAX_FILENAME_SIZE];
      uint8_t mode[MAX_MODE_SIZE];
    } rrq, wrq;

    struct {
      uint16_t block_num;
      size_t size;
      uint8_t block[BLOCK_SIZE];
    } data;

    struct {
      uint16_t block_num;
    } ack;

    struct {
      uint16_t error_code;
      uint8_t error_msg[MAX_ERROR_MSG_SIZE];
    } error;
  };
};

const char *error_messages[] = {
	"Undefined error",
	"File not found",
	"Access violation",
	"Disk full or allocation error",
	"Illegal TFTP operation",
	"Unknown transfer ID",
	"File already exists",
	"No such user"
};


/* packets */

int pack_uint16(uint8_t *buf, uint16_t val) {
  *buf++ = val >> 8;
  *buf++ = val;

  return 2;
}

int pack_bytes(uint8_t *buf, void *data, size_t n) {
  memcpy(buf, data, n);
  
  return n;
}

ssize_t serialize_packet(uint8_t *buf, struct packet *pkt) {
  if (buf == NULL || pkt == NULL) {
    return -1;
  }

  uint8_t *start = buf;

  buf += pack_uint16(buf, pkt->opcode);

  switch (pkt->opcode) {
  case RRQ:
    buf += pack_bytes(buf, pkt->rrq.filename, strlen(pkt->rrq.filename));
    *buf++ = 0;
    buf += pack_bytes(buf, pkt->rrq.mode, strlen(pkt->rrq.mode));
    *buf++ = 0;
    break;

  case WRQ:
    buf += pack_bytes(buf, pkt->wrq.filename, strlen(pkt->wrq.filename));
    *buf++ = 0;
    buf += pack_bytes(buf, pkt->wrq.mode, strlen(pkt->wrq.mode));
    *buf++ = 0;
    break;
      
  case DATA:
    buf += pack_uint16(buf, pkt->data.block_num);
    buf += pack_bytes(buf, pkt->data.block, pkt->data.size);
    break;
  
  case ACK:
    buf += pack_uint16(buf, pkt->data.block_num);
    break;
  
  case ERROR:
    buf += pack_uint16(buf, pkt->error.error_code);
    buf += pack_bytes(buf, pkt->error.error_msg, strlen(pkt->error.error_msg));
    *buf++ = 0;
    break;
  default:
    return -1;
  }

  return buf - start; 
}


uint16_t unpack_uint16(uint8_t **buf) {
  uint16_t ret = ((uint16_t) (*buf)[0] << 8) | (*buf)[1];
  *buf += 2;
  return ret;
}

void unpack_str(char *dest, uint8_t **src, size_t n) {
  size_t i = 0;

  while (i < n && (*src)[i] != 0) {
    dest[i] = (*src)[i];
    i++;
  }

  *src += i + 1;

  while (i < n) {
    dest[i] = '\0';
    i++;
  }
}

void unpack_bytes(uint8_t *dest, uint8_t **src, size_t n) {
  memcpy(dest, *src, n);
  *src += n;
}

int deserialize_packet(struct packet *pkt, uint8_t *buf, size_t size) {
  if (pkt == NULL || buf == NULL || size < 4) {
    return -1;
  }

  memset(pkt, 0, sizeof(struct packet));

  pkt->opcode = unpack_uint16(&buf);

  switch (pkt->opcode) {
  case RRQ:
    unpack_str(pkt->rrq.filename, &buf, MAX_FILENAME_SIZE);
    unpack_str(pkt->rrq.mode, &buf, MAX_MODE_SIZE);
    break;
  
  case WRQ:
    unpack_str(pkt->wrq.filename, &buf, MAX_FILENAME_SIZE);
    unpack_str(pkt->wrq.mode, &buf, MAX_MODE_SIZE);
    break;

  case DATA:
    pkt->data.block_num = unpack_uint16(&buf);
    pkt->data.size = (size - (2 * sizeof(uint16_t)));
    unpack_bytes(pkt->data.block, &buf, pkt->data.size);
    break;

  case ACK:
    pkt->ack.block_num = unpack_uint16(&buf);
    break;
  
  case ERROR:
    pkt->error.error_code = unpack_uint16(&buf);
    unpack_str(pkt->error.error_msg, &buf, MAX_ERROR_MSG_SIZE);
    break;
  
  default:
    return -1;
  }

  return 0;
}


void print_bytes(uint8_t *buf, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (i != 0 && i % 8 == 0) {
      printf("\n");
    }

    printf("%*u ", 3, buf[i]);
  }
  printf("\n");
}

void print_packet(struct packet *pkt) {
  if (pkt == NULL) {
    printf("NULL packet\n");
    return;
  }

  printf("-------[Packet]-------\n");

  switch (pkt->opcode) {
  case RRQ:
    printf("opcode: %d (RRQ)\n", pkt->opcode);
    printf("filename: %s\n", pkt->rrq.filename);
    printf("mode: %s\n", pkt->rrq.mode);
    break;
  
  case WRQ:
    printf("opcode: %d (WRQ)\n", pkt->opcode);
    printf("filename: %s\n", pkt->wrq.filename);
    printf("mode: %s\n", pkt->wrq.mode);
    break;

  case DATA:
    printf("opcode: %d (DATA)\n", pkt->opcode);
    printf("block #: %u\n", pkt->data.block_num);
    printf("size: %u\n", pkt->data.size);
    printf("data:\n");
    print_bytes(pkt->data.block, pkt->data.size);
    break;
  
  case ACK:
    printf("opcode: %d (ACK)\n", pkt->opcode);
    printf("block #: %u\n", pkt->data.block_num);
    break;
  
  case ERROR:
    printf("opcode: %d (ERROR)\n", pkt->opcode);
    printf("error code: %u\n", pkt->error.error_code);
    printf("error msg: %s\n", pkt->error.error_msg);
    break;
  
  default:
    printf("opcode: %d (UNKNOWN)\n", pkt->opcode);
    break;
  }

  printf("----------------------\n");
}





int main(int argc, char *argv[]) {

  // TEST SERDER

  struct packet test_rrq;
  test_rrq.opcode = RRQ;
  memcpy(test_rrq.rrq.filename, "test.txt", strlen("test.txt") + 1);
  memcpy(test_rrq.rrq.mode, "octet", strlen("octet") + 1);
  
  uint8_t b1[1024];
  ssize_t test_rrq_len = serialize_packet(b1, &test_rrq);
  assert(test_rrq_len == 2 + strlen("test.txt") + 1 + strlen("octet") + 1);

  struct packet test_rrq_out;
  deserialize_packet(&test_rrq_out, b1, 1024);

  assert(test_rrq_out.opcode == RRQ);
  assert(memcmp(test_rrq_out.rrq.filename, "test.txt", 9) == 0);
  assert(memcmp(test_rrq_out.rrq.mode, "octet", 6) == 0);


  struct packet test_wrq;
  test_wrq.opcode = WRQ;
  memcpy(test_wrq.rrq.filename, "test.txt", strlen("test.txt") + 1);
  memcpy(test_wrq.rrq.mode, "octet", strlen("octet") + 1);
  
  uint8_t b2[1024];
  ssize_t test_wrq_len = serialize_packet(b2, &test_wrq);
  assert(test_wrq_len == 2 + strlen("test.txt") + 1 + strlen("octet") + 1);

  struct packet test_wrq_out;
  deserialize_packet(&test_wrq_out, b2, 1024);

  assert(test_wrq_out.opcode == WRQ);
  assert(memcmp(test_wrq_out.wrq.filename, "test.txt", 9) == 0);
  assert(memcmp(test_wrq_out.wrq.mode, "octet", 6) == 0);


  struct packet test_data;
  test_data.opcode = DATA;
  test_data.data.block_num = 123;
  test_data.data.size = 5;
  uint8_t arr[] = {101, 102, 103, 104, 105};
  memcpy(test_data.data.block, arr, 5);

  uint8_t b3[1024];
  ssize_t test_data_len = serialize_packet(b3, &test_data);

  assert(test_data_len == 2 + 2 + 5);

  struct packet test_data_out;
  deserialize_packet(&test_data_out, b3, test_data_len);
  
  assert(test_data_out.data.block_num == 123);
  assert(test_data_out.data.size == 5);
  assert(memcmp(test_data_out.data.block, arr, test_data_out.data.size) == 0);


  struct packet test_ack;
  test_ack.opcode = ACK;
  test_ack.ack.block_num = 123;

  uint8_t b4[1024];
  ssize_t test_ack_len = serialize_packet(b4, &test_ack);

  assert(test_ack_len == 2 + 2);

  struct packet test_ack_out;
  deserialize_packet(&test_ack_out, b4, test_ack_len);

  assert(test_ack_out.opcode == ACK);
  assert(test_ack_out.ack.block_num == 123);


  struct packet test_error;
  test_error.opcode = ERROR;
  test_error.error.error_code = FILE_NOT_FOUND;
  memcpy(test_error.error.error_msg, error_messages[FILE_NOT_FOUND], strlen(error_messages[FILE_NOT_FOUND]) + 1);

  uint8_t b5[1024];
  ssize_t test_error_len = serialize_packet(b5, &test_error);

  assert(test_error_len == 2 + 2 + strlen(error_messages[FILE_NOT_FOUND]) + 1);

  struct packet test_error_out;
  deserialize_packet(&test_error_out, b5, test_error_len);

  assert(test_error_out.opcode == ERROR);
  assert(test_error_out.error.error_code == FILE_NOT_FOUND);
  assert(memcmp(test_error.error.error_msg, error_messages[FILE_NOT_FOUND], strlen(error_messages[FILE_NOT_FOUND]) + 1) == 0);  

  exit(EXIT_SUCCESS);
}
