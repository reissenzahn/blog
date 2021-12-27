
// every standard-library facility is provided through some standard header

#include <string>
#include <list>

// the standard library is defined in a namespace called std

string s {"hello"};
list<string> l {"one", "two", "three"};


// strings

string compose(const string& name, const string& domain) {

  // string has a move constructor
  return name + '@' + domain;
}

auto addr = compose("dmr", "bell-labs.com");

addr += '\n';


// iostream

cout << 10;

string name{"joe"};

cout << "hello, " << name;

cerr << "error!";

int i; 
cin >> i;

// a whitespace character terminates the read
string str;
cin >> str;

string str;
getline(cin, str);


struct Entry {
  string name;
  int number;
};

ostream& operator<<(ostream& os, const Entry& e) {
  return os << "{\"" << e.name << "\", " << e.number << "}";
}


// TODO: 4.5.2 Iterator Types (105)

