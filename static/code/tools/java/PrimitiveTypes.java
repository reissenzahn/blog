public class PrimitiveTypes {

  // $ javac PrimitiveTypes.java && java PrimitiveTypes
  public static void main(String[] args) {

    // there are eight primitive types
    boolean t = true;  // boolean
    char c = 'A';      // unicode character (16-bit unsigned integer)

    byte b = 1;   // 8-bit signed integer
    short s = 2;  // 16-bit signed integer
    int i = 3;    // 32-bit signed integer
    long l = 4L;  // 64-bit signed integer
    
    float f = 2.71828f;  // 32-bit floating point
    double d = 3.1415;   // 64-bit floating point
    

    // each primitive type has a corresponding immutable wrapper class (Boolean Byte, Short, Integer, Long, Float, Double)
    Boolean wrapper = t;


    // underscores are ignored in numerical literals and are used to help readability
    long billion = 1000_000_000_000L; 

    // integer literals can be expressed in hexadecimal, octal or binary notation
    int hex = 0xff;
    int oct = 0377;
    int bin = 0b0010_1111;

    // floating-point literals an use scientific notation
    double avogadro = 6.02e23;  // 6.02 * 10^23


    // integer arithmetic wraps around rather than producing an overflow or an underflow
    System.out.println((byte) (127 + 1));   // -128
    System.out.println((byte) (-128 - 1));  // 127

    // the integer wrappers provide constants for maximum and minimum values
    System.out.println(Byte.MIN_VALUE);  // -128
    System.out.println(Byte.MAX_VALUE);  // 127


    // floating-point types can represent positive/negative infinity, positive/negative zero and NaN
    double inf = 1.0/0.0;
    double negInf = -1.0/0.0;
    double negZero = -1.0/inf;
    double NaN = 0.0/0.0;

    // the float wrappers provide constants for representing these values
    System.out.println(Double.NEGATIVE_INFINITY);  // -Infinity
    System.out.println(Double.isNaN(Double.NaN));  // true


    // widening conversions are performed automatically
    double wide = i;

    // narrowing conversions typically require an explicit cast
    byte narrow = (byte) i;

    // the fractional part is truncated when you cast a floating-point to an integer
    int truncated = (int) f;
    System.out.println(truncated);  // 2

    // the static methods Math.round(), Math.floor() and Math.ceil() perform other types of rounding
    int rounded = Math.round(f);
    System.out.println(rounded);  // 3

    // boolean is the only primitive type that cannot be converted to or from another primitive type
    boolean flag = (i == 1);


    // a char can be used anywhere an int or long value is required though char is unsigned so it behaves differently than short
    System.out.println((int) (short) 0xffff);   // -1
    System.out.println((int) (char) '\uffff');  // 65535
  }
}