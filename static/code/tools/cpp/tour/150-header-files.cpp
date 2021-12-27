
// with separate compilation, user code sees only declarations of the types and functions used

// the definitions of those types and functions are in separate source files and are compiled separately

// typically we place the declarations that specify the interface to a module in a header file and users will include that header file to access that interface

// Vector.h
class Vector {
public:
  // ...
};

// user.cpp
#include "Vector.h"
#include <cmath>

using namespace std;

double sqrt_sum(Vector& v)
{
  double sum = 0;

  for (int i = 0; i != v.size(); ++i)
    sum += sqrt(v[i]);
  
  return sum;
}

// the implementation of Vector will also include the .h file providing its interface

#include "Vector.h"

Vector::Vector(int s) : elem.... 


int Vector::size()
{
  return sz;
}


// while the code in user.cpp and Vector.cpp shares the Vector interface information presented in Vector.h, the two files are otherwise independent and can be separately compiled
