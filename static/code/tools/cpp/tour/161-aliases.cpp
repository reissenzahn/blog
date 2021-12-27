
// it is often useful to introduce a synonym for a type or a template

using size_t = unsigned int;

// it is common for a parameterized type to provide an alias for types related to their template arguments

template<typename T>
class Vector {
public:
  using value_type = T;
};

// every standard-library container provides value_type a the name of its value type

template<typename C>
using Value_type = typename C::value_type;

template<typename Container>
void algo(Container& c) {
  Vector<Value_type<Container>> vec;
}


// the aliasing mechanism can be used to define a new template by binding some or all template arguments
template<typename Key, typename Value>
class Map {

};

template<typename Value>
using String_map = Map<string, Value>;

String_map<int> m;
