#include <stdio.h>

int main() {

  // if both operands of a binary operator are integers then an integer operation is performed regardless of the type of the rest of the expression
  
  // the result of integer division is truncated towards zero
  printf("%d\n", 5 / 2);   // 2
  printf("%d\n", -5 / 2);  // -2

  // this can be addressed by casting one of the operands to a floating-point type or by using a floating-point constant
  printf("%f\n", (double) 5 / 2);  // 2.5
  printf("%f\n", -5 / 2.0);        // -2.5


  // i++ increments i and returns the un-incremented value while ++i increments i and returns the incrmented value
  int i = 1;
  printf("%d\n", i++);  // 1

  int j = 1;
  printf("%d\n", ++j);  // 2


  // bitwise operators
  unsigned char x = 5;  // 00000101
  unsigned char y = 9;  // 00001001

  printf("%d\n", x & y);   // 00000001
  printf("%d\n", x | y);   // 00001101
  printf("%d\n", x ^ y);   // 00001100
  printf("%d\n", ~x);      // 11111010
  printf("%d\n", y << 2);  // 00100100
  printf("%d\n", y >> 2);  // 00000010
}


Q: Why doesn't the code

int a = 1000, b = 1000;
long int c = a * b;
work?
A: Under C's integral promotion rules, the multiplication is carried out using int arithmetic, and the result may overflow or be truncated before being promoted and assigned to the long int left-hand side. Use an explicit cast on at least one of the operands to force long arithmetic:

	long int c = (long int)a * b;
or perhaps
	long int c = (long int)a * (long int)b;
(both forms are equivalent).
Notice that the expression (long int)(a * b) would not have the desired effect. An explicit cast of this form (i.e. applied to the result of the multiplication) is equivalent to the implicit conversion which would occur anyway when the value is assigned to the long int left-hand side, and like the implicit conversion, it happens too late, after the damage has been done.
