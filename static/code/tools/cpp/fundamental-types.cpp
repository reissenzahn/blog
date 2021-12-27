
// fundamental types corrrespond to the most common basic storage units of a computer and the most common ways of using them to hold data

// many assumptions about implementation-defined features can be checked using static assertions
static_assert(4 <= sizeof(int), "sizeof(int) too small!");

int main() {
  
  // booleans hold either true or false
  bool b1 {true};

  // true has the value 1 when converted to an integer and false has the value 0
  int b2 {false};  // 0

  // nonzero integers implicitly convert to true and 0 converts to false
  bool b3 = 7;  // true

  // a non-null pointer converts to true while pointers with the value nullptr convert to false
  bool b4 = nullptr;  // false


  // characters hold single characters
  char c {'a'};

  // whether char is signed or unsigned is implementation defined
  signed char sc {'a'};
  unsigned char us {'a'};
  
  // every character has an integer value in the character set used by the implementation
  cout << int{'a'} << '\n';
  cout << int{'b'} << '\n';
  cout << int{'c'} << '\n';

  // escape characters
  cout << '\n';  // newline
  cout << '\t';  // tab
  cout << '\b';  // backspace
  cout << '\r';  // carriage return
  cout << '\\';  // backslash
  cout << '\?';  // question mark
  cout << '\'';  // single quote
  cout << '\"';  // double quote


  // integers types hold integers
  int i1 {1};

  // each integer type comes in three forms (int, signed int and unsigned int) and plain ints are always signed
  signed i2 {1};
  unsigned i3 {1};

  // as well as four sizes (short, int, long, long long)
  short i4 {1};
  int i4 {1};
  long i5 {1};
  long long i6 {1};

  // hexadecimal literals can be used
  int i7 = 0123;
  int i8 = 0x3f;

  // the suffix U is used for explicitly unsigned literals while L is used for long literals
  unsigned int = 3U;
  long = 3L;


  // floating-point types hold approximations of real numbers
  float f1 {1.0};        // single-precision
  double f2 {1.0};       // double-precision
  long double f3 {1.0};  // extended-precision

  // a floating-point literal is of type double by default; the suffix F is used for a literal of type float while L is used for a literal of type long double
  float f4 {1.0F};
  long double f5 {1.0L};


  // each fundamental type has a fixed size that determines the range of values that can be stored in it
  
  // the size of a type is implementation-defined though it is guaranteed that a char has at least 8 bits, a short at least 16 bits and a long at least 32 bits

  // the size of an object or type can be obtained using the sizeof operator which expresses in terms of multiples of the size of char (typically 1 byte)
  std::cout << sizeof(bool) << '\n';         // 
  std::cout << sizeof(char) << '\n';         // 
  std::cout << sizeof(short) << '\n';        // 
  std::cout << sizeof(int) << '\n';          // 
  std::cout << sizeof(long) << '\n';         // 
  std::cout << sizeof(long long) << '\n';    //
  std::cout << sizeof(float) << '\n';        // 
  std::cout << sizeof(double) << '\n';       // 
  std::cout << sizeof(long double) << '\n';  // 

  // implementation-defined aspects of fundamental types can found using numeric_limits
  std::cout << std::numeric_limits<int>::lowest() << '\n';
  std::cout << std::numeric_limits<int>::max() << '\n';


  // the fundamental types can be mixed freely in assignments and expressions

  // a conversion is value-preserving if a value can be represented exactly in a variable of a certain type; conversions that are not value-preserving are best avoided


  // size_t is an implementation-defined unsigned integer type that can hold the size in bytes of every object

  return 0;
}