
// 

struct Vector {
  int sz;
  double* elem;
};

void vector_init(Vector& v, int s) {
  v.elem = new double[s];
  v.sz = s;
}


// a variable of type Vector can be defined like this
Vector v;


double read_and_sum(int s)
{
  Vector v;
  vector_init(v, s);
  
  for (int i = 0; i != s; ++i)
    cin >> v.elem[i];
  
  double sum = 0;
  for (int i = 0; i != s; ++i)
    sum += v.elem[i];
  
  return sum;
}


// we use the dot operator to access struct members through a name (and through a reference) and -> to access struct members through a pointer
void f(Vector v, Vector& rv, Vector* pv)
{
  int i = v.sz;
  int i2 = rv.sz;
  int i4 = pv->sz;
}


