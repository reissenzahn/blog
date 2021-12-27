
// the unary suffix & in a declaration means reference to

// a reference is similar to a pointer except that you don't need to use a prefix * to access the value referred to by the reference

// a reference cannot be made to refer to a different object after its initialization

T a[n];  // array of n Ts
T* p;    // pointer to T
T& r;    // reference to T



// here we pass v by non-const reference so we can modify the vector passed to it
void vector_init(Vector& v, int s) {
  v.elem = new double[s];
  v.sz = s;
}
