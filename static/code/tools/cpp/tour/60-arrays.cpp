
// the size of an array must be a constant expression
char v[6];

// we don't have to specify an array bound when we initialize it with a list
int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};


void copy_fct()
{
  int v1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int v2[10];

  for (auto i = 0; i != 10; ++i)
    v2[i] = v1[i];
}


// the range-for statement traverses a sequence

void print()
{
  int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (auto x : v) {
    cout << x << '\n';
  }

  for (auto x : {10, 21, 32, 43, 54, 65})
    cout << x << '\n';
}





