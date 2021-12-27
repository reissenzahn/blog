
// take 1

// a class is defined to have a set of members which can be data, functions or type members

// the interface is defined by the public members of a class and private members are accessible only through that interface

// a constructor is a function used to construct objects of a class; a constructor is guaranteed to be used to initialize objects of its class and so eliminates the problem of uninitialized variables for the class

// the constructor initializes the members using a member initializer list

class Vector {
public:
  Vector(int s) : elem{new double[s]}. sz{s} {}
  double& operator[](int i) { return elem[i] };
  int size() { return sz; }

private:
  double* elem;
  int sz;
};

Vector v(6);

double read_and_sum(int s) {
  Vector v(s);
  for (int i = 0; i != v.size(); ++i)
    cin >> v[i];
  
  double sum = 0;
  for (int i = 0; i != v.size(); ++i)
    sum += v[i];
  
  return sum;
}


// take 2

// a class is a user-defined type provided to represent a concept in the code of a program

// a constructor that can be invoked without an argument is called a default constructor; defining a default constructor allows you to eliminate the possibility of uninitialized variables of that type

// the const specifiers on the functions returning the real and imaginary parts indicate that these functions do not modify the object for which they are called

class complex {
  double re, im;

public:
  complex(double r, double i) : re{r}, im{i} {}
  complex(double r) : re{r}, im{0} {}
  complex() : re{0}, im{0} {}

  double real() const { return re; }
  void real(double d) { re = d; }
  double imag() const { return im; }
  void imag(double d) { im = d; }

  complex& operator+=(complex z) { re += z.re; im += z.im; return *this; }
  complex& operator-=(complex z) { re -= z.re; im -= z.im; return *this; }

  // defined somewhere else
  complex& operator*=(complex);
  complex& operator/=(complex);
}


// here we use the fact that an argument passed by value is copied and so we can modify it would effecting the caller's copy
complex operator+(complex a, complex b) { return a += b; }
complex operator/(complex a, complex b) { return a /= b; }

bool operator==(complex a, complex b)
{
  return a.real() == b.real() && a.imag() == b.imag();
}


complex a {2.3};
complex b {1/a};


// destructors are a mechanism to ensure that the memory allocated by the constructor is deallocated

class Vector {
private:
  double* elem;
  int sz;

public:
  Vector(int s) : elem{new double[s]}, sz{s}
  {
    for (int i = 0; i != s; ++i) elem[i] = 0;
  }

  ~Vector() { delete[] elem; }

  double& operator[](int i);
  int size() const;
}


// the technique of acquiring resources in a constructor and releasing them in a destructor is known as Resource Acquisition is Initialization (RAII)


// the std::initializer_list is used to define a initializer-list constructor 

class Vector {
public:
  Vector(std::initializer_list<double>);

  void push_back(double);
}

// when we use a {}-list like {1,2,3,4}, the compiler will create an object of type initializer_list to give to the program so we can write
Vector v1 = {1,2,3,4,5};
Vector v2 = {1.23, 3.45, 6.7,8};

Vector::Vector(std::initializer_list<double> lst) : elem{new double[lst.size()]}, sz{static_cast<int>(lst.size())}
{
  copy(lst.begin(), lst.end(), elem);
}


Vector read(istream& is) {
  Vector v;

  // loop is terminated by an end-of-file or a formatting error
  for (double d; is >> d;)
    v.push_back(d);

  return v;  // Later: move constructor
}
