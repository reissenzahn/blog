
// an object of a derived class can be use wherever an object of a base class is required (interface inheritance) and a base class provides functions or data that simplifies the implementation of derived classes

class Shape {
public:
  virtual Point center() const = 0;
  virtual void move(Point to) = 0;
  
  virtual draw() const = 0;
  virtual void rotate(int angle) = 0;

  // a virtual destructor is essential for an abstract class because an object of a derived class is usually manipulated through the interface provided by its abstract base class; in particular, it may be deleted through a pointer to a base class; in that case, the virtual function call mechanism ensures that the proper destructor is called; that destructor then implicitly invokes the destructors of its bases and members
  virtual ~Shape() {}

  // ...
};

void rotate_all(vector<Shape*>& v, int angle)
{
  for (auto p : v)
    p->rotate(angle);
}

class Circle : public Shape {
public:
  Circle(Point p, int rr);

  Point center() const { return x; };
  void move(Point to) { x = to; };

  void draw() const;
  void rotate(int) {}

private:
  Point x;
  int r;
};

class Smiley : public Circle {
public:
  Smiley(Point p, int r) : Circle{p,r}, mouth{nullptr} {}

  ~Smiley()
  {
    delete mouth;
    for (auto p : eyes) delete p;
  }

  void move(Point to);

  void draw() const;
  void rotate(int) {}

  void add_eye(Shape* s) { eyes.push_back(s); }
  void set_mouth(Shape* s);
  virtual void wink(int i);

  // ...

private:
  vector<Shape*> eyes;
  Shape* mouth;
};

// We can now define Smiley::draw() using calls to Smiley's base and member draw()s

void Smiley::draw()
{
  Circle::draw();
  for (auto p : eyes)
    p->draw();
  mouth->draw();
}



enum class Kind { circle, triangle, smiley };

Shape* read_shape(istream& is)
{
  // read shape header from is and find its Kind k

  switch(k) {
  case Kind::circle:
    return new Circle{p,r};
  case Kind::triangle:
    return new Triangle{p1,p2,p3};
  case Kind::smiley:
    Smiley* ps = new Smiley{p,r};
    ps->add_eye(1);
    ps->add_eye(2);
    ps->set_mouth(m);
    return ps;
  }
}

void user()
{
  std::vector<Shape*> v;
  while (cin)
    v.push_back(read_shape(cin));
  draw_all(v);
  rotate_all(v,45);
  for (auto p : v) delete p;
}

