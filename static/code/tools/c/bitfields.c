#include <stdio.h>
#include <stdbool.h>

// adjacent bit field members may be packed to share and straddle the individual bytes

// bit fields can have only one of four types: unsigned int, signed int, int and bool
struct bitfield {
  unsigned int i : 3;  // 0..7
  signed int j : 3;    // -4..3
  int k : 3;           // 0..7 or -4..3
};

struct flags {
  unsigned int i : 1;  // 0..1
  bool j : 1;          // 0..1
};

struct packed {
  unsigned i : 5;  // i
  unsigned : 11;   // 11 unused bits
  unsigned j : 6;  // j
  unsigned k : 2;  // k
};

// the special unnamed bit field of width zero specifies that the next bit field begins at the beginning of the next allocation unit
struct unnamed {
  unsigned i : 5;   // i
  unsigned : 0;     // 27 unused bits
  unsigned j : 6;   // j
  unsigned k : 15;  // k
};

// bitfields can be useful for extracting or setting bits in a byte
union converter {
  unsigned char byte;
  struct {
    unsigned i : 1;
    unsigned j : 4;
    unsigned k : 3;
  } bits;
};

// or even for extracting parts of float
union inspector {
  float f;
  struct {
    unsigned fraction : 23;
    unsigned exponent : 8;
    unsigned sign : 1;
  };
};

int main() {
  printf("%zu\n", sizeof(struct bitfield));  // 4
  printf("%zu\n", sizeof(struct flags));     // 4
  printf("%zu\n", sizeof(struct packed));    // 4
  printf("%zu\n", sizeof(struct unnamed));   // 8
  
  struct bitfield b;
  b.i = 7;
  b.i++;  // unsigned overflow
  printf("%d\n", b.i);

  union converter c;
  c.byte = 0xB3;             // 10110011
  printf("%d\n", c.bits.i);  // 1 (1)
  printf("%d\n", c.bits.j);  // 9 (1001)
  printf("%d\n", c.bits.k);  // 5 (101)

  union inspector i;
  i.f = (float) 65.65625;
  printf("%d\n", i.sign);
  printf("%d\n", i.exponent);
  printf("%d\n", i.fraction);
}