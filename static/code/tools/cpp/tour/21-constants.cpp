
// there are two notions of immutability:

// const: a promise not to change the value enforced by the compiler.

// constexpr: indicates that the expression should be evaluated at compile time.

const int dmv = 17;  // named constant
int var = 17;        // not a constant

constexpr double max1 = 1.4 * square(dmv);  // ok provided square(17) is a constant expression
constexpr double max2 = 1.4 * square(var);  // error: var is not a constant expression
const double max3 = 14 * square(var);       // ok may be evaluated at run time

double sum(cont vector<double>&);  // sum will not modify its argument
vector<double> v {1.2, 3.4, 4.5};
const double s1 = sum(v);  // ok; evaluated at run time
constexpr double s2 = sum(v);  // error: sum(v) is not a constant expression

// a function must be defined as constexpr to be used in a constant expression that is evaluted by the compiler; it must be rather simple: just a return-statement computing a value
constexpr double square(double x)
{
  return x * x;
}

// a constexpr function can be used for non-constant arguments but the result is then not a constant expression

// a constant cannot be left uninitialized 

