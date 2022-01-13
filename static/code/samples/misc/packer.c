#include <unistd.h>
#include <stdio.h>


// pack unsigned integers

void pack_uint16(uint8_t *buf, uint16_t val) {
  *buf++ = val >> 8;
  *buf++ = val; 
}

void pack_uint32(uint8_t *buf, uint32_t val) {
  *buf++ = val >> 24;
  *buf++ = val >> 16;
  *buf++ = val >> 8;
  *buf++ = val;
}

void pack_uint64(uint8_t *buf, uint32_t val) {
  *buf++ = val >> 56;
  *buf++ = val >> 48;
  *buf++ = val >> 40;
  *buf++ = val >> 32;
  *buf++ = val >> 24;
  *buf++ = val >> 16;
  *buf++ = val >> 8;
  *buf++ = val;
}


// unpack unsigned integers

uint16_t unpack_uint16(uint8_t *buf) {
  return ((uint16_t) buf[0] << 8) | buf[1];
}

uint32_t unpack_uint32(uint8_t *buf) {
  return ((uint32_t) buf[0] << 24) |
    ((uint32_t) buf[1] << 16) |
    ((uint32_t) buf[2] << 8) |
    buf[3];
}

uint64_t unpack_uint64(uint8_t *buf) {
  return ((uint64_t) buf[0] << 56) |
    ((uint64_t) buf[1] << 48) |
    ((uint64_t) buf[2] << 40) |
    ((uint64_t) buf[3] << 32) |
    ((uint64_t) buf[4] << 24) |
    ((uint64_t) buf[5] << 16) |
    ((uint64_t) buf[6] << 8) |
    buf[7];
}


// unpack bytes

uint8_t *unpack_bytes(uint8_t **buf, size_t len) {
  uint8_t *ret = malloc(len + 1);
  if (ret == NULL) {
    fprintf(stderr, "[%s:%d] Out of memory!\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }

  memcpy(ret, *buf, len);

  *buf += len;

  return ret;
}


// https://github.com/codepr/sol/blob/tutorial/src/pack.c




#include <stdio.h>
#include <stdint.h>

void print_binary(unsigned char *buf, size_t size) {
  for (size_t i = 0; i < size; i++) {
    for (int j = 7; j >= 0; j--) {
      printf("%c", (buf[i] & (1 << j)) ? '1' : '0');
    }
    printf(" ");
  }
  printf("\n");
}

void pack_uint16(uint8_t *buf, uint16_t val) {
  *buf = val >> 8;
  buf++;

  *buf = val; 
  buf++;
}

uint16_t unpack_u16(uint8_t *buf) {
  return ((uint16_t) buf[0] << 8) |
    ((uint16_t) buf[1] << 0);
}

int main() {
  char buf[6];

  char *ptr1 = buf;

  print_binary(buf, sizeof(buf));

  pack_uint16(ptr1, 0x1234);
  ptr1 += 2;
  pack_uint16(ptr1, 0x5678);
  ptr1 += 2;
  pack_uint16(ptr1, 0x9abc);
  ptr1 += 2;

  print_binary(buf, sizeof(buf));

  char *ptr2 = buf;

  printf("%#x\n", unpack_u16(ptr2));
  ptr2 += 2;
  printf("%#x\n", unpack_u16(ptr2));
  ptr2 += 2;
  printf("%#x\n", unpack_u16(ptr2));
  ptr2 += 2;

  return 0;
}
