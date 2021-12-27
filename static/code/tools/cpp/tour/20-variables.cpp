
// every name and every expression has a type that determines the operations that may be performed on it

// a declaration is a statement that introduces a name into the program and specifies its type

// a type defines a set of possible values and a set of operations for an object

// an object is some memory that holds a value of some type

// a value is a set of bits interpreted according to a type

// a variable is a named object



// initialization can be performed using the = operator or using initializer lists
double d1 = 2.3;
double d2 {2.3};

complex<double> z = 1;
complex<double> z2 {d1, d2};

vector<int> v {1, 2, 3, 4, 5};

// the initializer list form prevents narrowing conversions that lose information
int i1 = 7.2;  // i1 becomes 7
int i2 {7.2};  // error


// the auto keyword allows the type of a variable to be deduced from the initializer
auto b = true;  // bool
auto ch = 'x';  // char
auto i = 123;   // int
auto d = 1.2;   // double

// we use the = syntax with auto as there is no type conversion involved anyway




