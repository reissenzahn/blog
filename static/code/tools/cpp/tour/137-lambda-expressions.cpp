
// a lambda expression generates a function object

// the [&] is a capture list specifying that local names used (such as x) will be accessed through references

// [ ] - capture nothing
// [&] - capture all local names used by reference
// [=] - capture all local names used by value
// [&x] - capture only x by reference
// [=x] - capture only x by value

[&](int a) { return a < x; }

[&](const string& a) { return a < s};


template<typename C, typename oper>
void for_all(C& c, Oper op)
{
  for (auto& x : c)
    op(*x);
}

vector<unique_ptr<Shape>> v;

for_all(c, [](Shape& s) { s.draw() })