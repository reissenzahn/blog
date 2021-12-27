
public class Test {

  public static void main(String[] args) {

    int or = 0b00001100 | 0b00011001;   // ... 00011101
    int and = 0b00001100 & 0b00011001;  // ... 00001000
    int xor = 0b00001100 ^ 0b00011001;  // ... 00010101
    int complement = ~ 0b00001100;      // ... 11110011
    int leftShift = 0b00000011 << 2;    // ... 00001100
    int rightShift = 0b00001100 >> 2;   // ... 00000011

    System.out.println(Integer.toBinaryString(rightShift));
  }

  public boolean getBit(int n, int i) {
    return (n & (1 << i)) != 0;
  }

  public int setBit(int n, int i) {
    return num | (1 << i);
  }

  public int clearBit(int n, int i) {
    return n & ~(1 << i)
  }
}