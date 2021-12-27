
// an abstract type is a type that insulates a user from implementation details by decoupling the interface from the representation

// since we don't know anything about the representation of an abstract type (not even its size), we must allocate objects on the heap and access them through references or pointers

class Container {
public:
  virtual double& operator[](int) = 0;
  virtual int size() const = 0;
  virtual ~Container() {}
};

// the virtual keywork means: may be redefined later in a class derived from this one; a function declared virtual is called a virtual function

// a class derived from Container provides an implemtation for the Container interface

// the =0 syntax says the function is pure virtual; that is, some class derived from Container must define the function

// it is not possible to define an object that is just a Container

// a class with a pure virtual function is called an abstract class


// Vector_container is said to be derived from class Container and Container is said to be a base of Vector_container (alternatively, Vector_container is a subclass and Container is a superclass); the derived class is said to inherit members from its base class

// the members operator[]() and size() are said to override the corresponding members in the base class Container as does ~Vector_container() override ~Container()

// notice that ~Vector() is implicitly invokes by Vector_container()
class Vector_container : public Container {
  Vector v;

public:
  Vector_container(int s) : v(s) {}
  ~Vector_container() {}

  double& operator[](int i) { return v[i]; }
  int size() const { return v.size(); }
}



// we can use the Container interface in complete ignorance of implementation details; since use() only knows about the Container interface, it will work just as well for another implementation of Container
void use(Container& c)
{
  const int sz = c.size();

  for (int i = 0; i != sz; ++i)
    cout << c[i] << '\n';
}

Vector_container vc(10);
use(vc);