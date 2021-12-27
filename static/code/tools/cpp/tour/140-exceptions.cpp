
// the throw transfers control to a handler for exceptions of type out_of_range in some function that directly or indirectly called Vector::operator[]

Vector::Vector(int s)
{
  if (s < 0) throw length_error{};
  elem = new double[s];
  sz = s;
}

try {
  Vector v(-27);
} catch (std::length_error) {

} catch (std::bad_alloc) {

}

double& Vector::operator[](int i)
{
  if (i < 0 || size() <= i) throw out_of_range{"Vector::operator[]"};
  return elem[i];
}


try {
  v[v.size()] = 7;
} catch(out_of_range) {
  
}

// you can define your own classes to be used as exceptions and have them carry arbitrary information from a point where an error is detected to a point where it can be handled

// to throw (rethrow) the exception caught in an exception handler we use
throw;
