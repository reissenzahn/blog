
// namespaces are a mechanism for expression that some declarations belong together and that their names should not clash with other names

namespace My_code {
  class complex { ... };
  complex sqrt(complex);
  // ...
  int main();
}

int My_code::main()
{
  complex z {1, 2};
  auto z2 = sqrt(z);
  std::cout << '{' << z2.real() << ',' << z2.imag() << "}\n";
  // ...
}

int main() {
  return My_code::main();
}

// the simplest way to access a name in another namespace is to qualify it with the namespace name
std::cout
My_code::main

// to gain access to names in the standard-library namespace, we can use a using directive
using namespace std;

