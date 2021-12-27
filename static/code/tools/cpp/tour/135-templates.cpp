
// a template is a class or a function that we parameterize with a set of types or values

// temlates are a compile-time mechanism and so their use incurs no run-time overhead compared to hand-crafted code

template<typename T>
class Vector {
private:
  T* elem;
  int sz;

public:
  Vector(int s);
  ~Vector() { delete[] elem };

  // copy and move operations

  T& operator[](int i);
  const T& operator[](int i) const;
  int size() const { return sz; }
};

template<typename T>
Vector<T>::Vector(int s)
{
  if (s < 0) throw Negative_size{};

  elem = new T[s];

  sz = s;
}

template<typename T>
const T& Vector<T>::operator[](int i) const;
{
  if (i < 0 || size() <= i)
    throw out_of_range{"Vector::operator[]"};
  
  return elem[i];
}

Vector<char> vc(200);
Vector<string> vs(17);
Vector<list<int>> vli(45);

void write(const Vector<string>& vs)
{
  for (int i = 0; i != vs.size(); ++i)
    cout << vs[i] << '\n';
}


// to support the range-for loop for our Vector we must define suitable begin() and end() functions

template<typename T>
T* begin(Vector<T>& x)
{
  return x.size() ? &x[0] : nullptr;  // pointer to first element or nullptr
}

template<typename T>
T* end(Vector<T>& x)
{
  return begin(x) + x.size();  // pointer to one-past-last element
}

for (auto& s : vs)
  cout << s << '\n';



