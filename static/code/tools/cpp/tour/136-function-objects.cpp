
// a function object (or functor) is a kind of template that can be used to define objects that can be called like functions

template<typename T>
class Less_than {
  const T val;

public:
  Less_than(const T& v) : val(v) {}
  bool operator()(const T& x) const { return x < val };
};

Less_than<int> lti {42};
Less_than<string> lts {"Backus"};

lti(45);
lts("str");


template<typename C, typename P>
int count(const C& c, P pred)
{
  int cnt = 0;

  for (const auto& x : c)
    if (pred(x))
      ++cnt;
    
  return cnt;
}


// a predicate is something that we can invoke to return true or false

void f(const Vector<int>& vec, const list<string>& lst, int x, const string& s)
{
  cout << count(vec, Less_than<int>{x});

  cout << count(lst, Less_than<string>{s});
}
