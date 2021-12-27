
// a template can be defined to accept an arbitrary number of arguments of arbitrary types

void f() {}

template<typename T>
void g(T x)
{
  cout << x << " ";
}

template<typename T, typename... Tail>
void f(T head, Tail... tail)
{
  g(head);
  f(tail...);
}

// notice when you pass a list of arguments to it that you can separate the first argument from the rest


