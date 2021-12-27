


char v[6];


// a pointer variable can hold the address of an object of the appropriate type
char* p = &v[3];  // point to fourth element of v
char x = *p;      // *p is the object that p points to

// & means address of and * means contents of


// we try to ensure that a pointer always points to an object so that dereferencing it is valid

// when we don't have an object to point to or if we need to represent the notion of "no object available" then we give the pointer the value nullptr

// there is only one nullptr shared by all pointer types
double* pd = nullptr;
Link<Record>* lst = nullptr;
int x = nullptr;  // error


// we can move pointer to point to the next element of an array using ++

// p is assume to point to a zero-terminated array of char 
int cout_x(char* p, char x)
{
  if (p == nullptr) return 0;

  int count = 0;
  for (; *p != 0; ++p)
    if (*p == x)
      ++count;
  
  return count;
}


