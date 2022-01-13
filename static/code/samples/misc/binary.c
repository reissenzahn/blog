#include <stdio.h>

void print_bits(unsigned char *buf, size_t size) {
  for (size_t i = 0; i < size; i++) {
    for (int j = 7; j >= 0; j--) {
      printf("%c", (buf[i] & (1 << j)) ? '1' : '0');
    }
    printf(" ");
  }
  printf("\n");
}

void print_bytes(unsigned char *buf, size_t size) {
  for (size_t i = 0; i < size; i++) {
    printf("%*u ", 3, buf[i]);
  }
  printf("\n");
}


int main() {
  char buf[6] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc};

  print_bits(buf, sizeof(buf));
  print_bytes(buf, sizeof(buf));
}


// https://jameshfisher.com/2017/02/23/printing-bits/
// https://jameshfisher.com/2016/12/22/printing-bytes/
