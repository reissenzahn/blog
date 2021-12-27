
// a move constructor allows an object to move simply and cheaply from one scope to another

Vector operator+(const Vector& a, const Vector& b)
{
  if (a.size() != b.size())
    throw Vector_size_mismatch{};
  
  Vector res(a.size());
  for (int i = 0; i != a.size(); ++i)
    res[i] = a[i] + b[i];
  
  return res;
}

void f(const Vector& x, const Vector& y, const Vector& z)
{
  Vector r;

  // res is operator+() is never used again after the copy; we wanted to move a Vector rather than copy it
  r = x + y + z;
}

// a move operation is applied when an rvalue reference is used as an initializer or as the right-hand side of an assignment; after a move, a moved-from object should be in a state that allows a destructor to be run


class Vector {
  // ...

  Vector(const Vector& a);
  Vector& operator=(const Vector& a);

  Vector(Vector&& a);             // move constructor
  Vector& operator=(Vector&& a);  // move assignment
};

// a move constructor is supposed to remove the value from its argument
Vector::Vector(Vector&& a) : elem{a.elem}, sz{a.sz} {
  a.elem = nullptr;
  a.sz = 0;
}

// given that definition the compiler will choose the move constructor to implement the transfer of the return value out of the function; so r = x + y + z will involve no copying of Vectors 

// && means rvalue reference and is a reference to which we can bind an rvalue; an rvalue is a value you can't assign to (e.g. an integer returned from by a function call); so an rvalue reference is a reference to something that nobody else can assign to (so we can safely steal its value)

// when the programmer knows that a value will not be used again but the compiler can't be expected to figure that out then the programmer can be specific
Vector x(1000);
Vector y(1000);
Vector z(1000);

// std::move() returns an rvalue reference to its argument
z = x;  // we get a copy
y = std::move(x)  // we get a move

return z;  // we get a move


// more uses
std::vector<thread> my_threads;

Vector init(int n)
{
  thread t {heartbeat};

  my_threads.push_back(move(t));

  // ...

  Vector vec(n);
  for (int i = 0; i < vec.size(); ++i) vec[i] = 777;
  return vec;  // move vec out of init()
}

auto v = init(10000);


// we can delete the default copy or move for a class
class Shape {
public:
  Shape(const Shape&) = delete;
  Shape& operator=(const Shape&) = delete;

  Shape(Shape&&) = delete;
  Shape& operator=(Shape&&) = delete;

  ~Shape();

};


// a move operation is not implicitly generated fro a class where the user has explicitly declared a destructor

