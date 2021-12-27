
// by default objects can be copied; this is true for user-defined types as well as built-in types

// the default meaning is a memberwise copy: copy each member; this is not always appropriate

complex z2{z1};  // copy initialization
complex z3;
z3 = z1;         // copy assignment


// when a class is a resource handle (i.e. responsible for an object accessed through a pointer) then the default memberwise copy is typically a disaster; for instance, it would leave a copy of a Vector referring to the same elements as the original

void bad_copy(Vector v1)
{
  Vector v2 = v1;
  v1[0] = 2;  // v2[0] is now also 2
  v2[1] = 3;  // v1[1] is now also 3
}


// copying of an object is defined by two members: a copy constructor and a copy assignment

class Vector {
private:
  double* elem;
  int sz;

public:
  Vector(int s);
  ~Vector() { delete[] elem; }

  Vector(const Vector& a);             // copy constructor
  Vector& operator=(const Vector& a);  // copy assignment

  double& operator[](int i);
  const double& operator[](int i) const;

  int size() const;
};

Vector::Vector(const Vector& a) : elem{new double[a.sz]}, sz[a.sz]
{
  for (int i = 0; i != sz; ++i)
    elem[i] = a.elem[i];
}

Vector& Vector::operator=(const Vector& a)
{
  double* p = new double[a.sz];
  for (int i = 0; i != a.sz; ++i)
    p[i] = a.elem[i];
  delete[] elem;
  elem = p;
  sz = a.sz;
  return *this;
}

