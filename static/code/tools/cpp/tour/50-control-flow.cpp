
bool accept()
{
  cout << "Do you want to proceed (y or n)?\n";
  
  char answer = 0;
  cin >> answer;

  if (answer == 'y') return true;

  return false;
}

// a switch statement tests a value against a set of constants; the case constants must be distinct and if the value tested does not match any of them then the default is chosen

bool accept2()
{
  cout << "Do you want to proceed (y or n)?\n";
  
  char answer = 0;
  cin >> answer;

  switch (answer) {
  case 'y':
    return true;
  case 'n':
    return false;
  default:
    cout << "I'll take that for a no.\n";
    return false;
  }
}

// the while statement executes until its condition becomes false

bool accept3()
{
  int tries = 1;

  while (tries < 4) {
    cout << "Do you want to proceed (y or n)?\n";

    char answer = 0;
    cin >> answer;

    switch (answer) {
    case 'y':
      return true;
    case 'n':
      return false;
    default:
      cout << "Sorry, I don't understand that.\n";
      ++tries;
    }
  }

  cout << "I'll take that for a no.\n";

  return false;
}



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

