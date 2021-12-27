
// enumerations are used to represent small sets of integer values

enum class Color { red, blue, green };

enum class Traffic_light { green, yellow, red };

Color col = Color::red;
Traffic_light light = Traffic_light::red;

// enumerators are in the scope of their enum class
Color::red

Color z = Traffic_light::red;  // error

// we cannot implicitly mix Color and integer values
int i = Color::red;  // error
Color c = 2;  // error


// by default an enum class has only assignment, initialization and comparisons defined


Traffic_light& operator++(Traffic_light& t) {
  switch (t) {
  case Traffic_light::green: return t = Traffic_light::yellow;
  case Traffic_light::yellow: return t = Traffic_light::red;
  case Traffic_light::red: return t = Traffic_light::green;
  }
}

Traffic_light next = ++light;


