

public class Operators {

  // $ javac Operators.java && java Operators
  public static void main(String[] args) {

    // an expression is evaluated according to the parentheses in the expression, the precedence of the operators and the associativity of the operators

    // precedence specifies the order in which operations are performed
    System.out.println(1 + 2 * 3);  // 1 + (2 * 3) = 7

    // associativity defines how to evaluate expressions involving several operators that have the same precedence
    System.out.println(1 + 2 - 3);  // (1 + 2) - 3 = 0

    // operands are always evaluated in order from left to right
    int n = 2;
    System.out.println(++n + ++n * ++n);  // 3 + 4 * 5 = 23


    // arithmetic operators
    System.out.println(7 + 3);  // 10
    System.out.println(7 - 3);  // 4
    System.out.println(7 * 3);  // 21
    System.out.println(7 / 3);  // 
    System.out.println(7 % 3);  //
    System.out.println(-7);     //

    //  if either of the operands of an arithmetic operator is a floating-point number then floating-point arithmetic is used; otherwise, integer arithmetic is used

    // if either operand of the divide operator is a floating-point then the result is a floating-point value; if both operands are integers then the result is an integer and the remainder is truncated
    System.out.println(7 / 3);     // 
    System.out.println(7 / 3.0f);  // 

    // floating-point division by zero results in positive infinity or NaN while integer division by zero throws an ArithmeticException
    // System.out.println(7 / 0);      // exception
    System.out.println(7 / 0.0);    // +Infinity
    System.out.println(0.0 / 0.0);  // NaN


    // string concatenation
    System.out.println("Hello, " + "Java!");  // Hello, Java!

    // an object is converted to a string by invoking its toString() method
    System.out.println(new int[]{1, 2, 3});

    // if either operand of + is a string then the other one is converted to a string as well
    System.out.println("total: " + 7 + 3);  // total: 73


    // increment and decrement
    int i = 1;
    System.out.println(i++);  // 1
    System.out.println(i);    // 2
    System.out.println(i--);  // 2
    System.out.println(i);    // 1

    int j = 1;
    System.out.println(++j);  // 2
    System.out.println(j);    // 2
    System.out.println(--j);  // 2
    System.out.println(j);    // 2


    // comparison operators
    System.out.println(1 == 2);  // 
    System.out.println(1 != 2);  // 
    System.out.println(1 > 2);   // 
    System.out.println(1 >= 2);  // 
    System.out.println(1 < 2);   // 
    System.out.println(1 <= 2);  // 

    // the equals operator checks whether two primitive values are identical or whether two reference types refer to the same object
    System.out.println(1 == 1);      // true
    System.out.println("1" == "1");  // false


    // boolean operators
    System.out.println(true && false);  // 
    System.out.println(true & false);   // 
    System.out.println(true || false);  // 
    System.out.println(true | false);   // 
    System.out.println(true ^ false);   // 
    System.out.println(!true);          // 

    // && does not evaluate its second operator if its first operand is false while & always evaluates both operands
    System.out.println(false && 1/0 == 0);  // 
    // System.out.println(false & 1/0 == 0);   // exception

    // || does not evaluate its second operand if its first operand is true while | always evaluates both operands
    System.out.println(true || 1/0 == 0);  // 
    // System.out.println(true | 1/0 == 0);   // exception


    // bitwise operators
    int i1 = 0b00001010;
    int i2 = 0b00000111;
    int i3 = 0b11100111;
    int i4 = 0b00011011;
    int i5 = 0b10000000000000000000000011001110;

    System.out.println(Integer.toBinaryString(~i1));      // 11111111111111111111111111110101
    System.out.println(Integer.toBinaryString(i1 & i2));  // 10
    System.out.println(Integer.toBinaryString(i1 | i2));  // 1111
    System.out.println(Integer.toBinaryString(i1 ^ i2));  // 1101

    // << shifts bits left (high-order bits are lost and zero bits are shifted in from the right)
    printBinaryInt(i3 << 3);  // 11100111000

    // >> shifts bits right (low-order bits are lost and the high-order bits shifted in are the same as the original high-order bit)
    System.out.println(Integer.toBinaryString(i4 >> 3));  // 11
    System.out.println(Integer.toBinaryString(i5 >> 2));  // 11100000000000000000000000110011

    // // >>> is like >> except that it always shifts zeros into the high-order bits
    System.out.println(Integer.toBinaryString(i5 >>> 2));  // 100000000000000000000000110011


    // conditional operator
    int x = 1;
    int y = 2;
    System.out.println((x > y) ? x : y);  // 2


    // instance of checks if an object is an instance of a specified type
    System.out.println("" instanceof String);  // true

    // null is never an instance of anything
    System.out.println(null instanceof Object);  // false

    // if an instanceof expression evaluates to true then it safe to cast and the object to a variable of the given type
    Object obj = new int[] {1, 2, 3};

    if (obj instanceof int[]) {
    int[] arr = (int[]) obj;
    System.out.println(arr[1]);  // 2
    }
  }
}