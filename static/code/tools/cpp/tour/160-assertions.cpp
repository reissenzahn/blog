
// exceptions report errors found at run time

// if an error can be found at compile time it is usually prefereable to do so

// we can perform simple checks on other properties that are known at compile time and report failures as compiler error messages
static_assert(4 <= sizeof(int), "Integers are too small!");

// the static_assert mechanism can be used for anything that can be expressed in terms of constant expressions
constexpr double C = 299792.458;

void f(double speed)
{
  const double local_max = 160.0 / (60 * 60);

  static_assert(speed < C, "can't go that fast");  // error
  static_assert(local_max < C, "can't go that fast");  // ok
}
