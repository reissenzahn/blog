#include <stdio.h>

// all function arguments are passed by value so an argument is passed to a function by coping the value of the argument into the corresponding parameter
int max(int x, int y) {
  if (x > y) {
    return x;
  }

  return y;
}

// it is possible for a function to modify the argument used to call it by passing a pointer
void inc(int *x) {
  *x += 1;
}

// arrays passed to functions are converted to pointers and so there is no copying of array elements
float avg(float arr[], int n) {
  float total = 0;
  
  for (int i = 0; i < n; i++) {
    total += arr[i];
  }

  return total / n;
}

// const parameters cannot be modified
int sum(const int to) {
  int total = 0;

  for (int i = 0; i < to; i++) {
    total += i;
  }

  return total;
}


// multiple return can be implemented using
void 
Q: How can I return multiple values from a function?

A: There are several ways of doing this. (These examples show hypothetical polar-to-rectangular coordinate conversion functions, which must return both an x and a y coordinate.)

Pass pointers to several locations which the function can fill in:
#include <math.h>

polar_to_rectangular(double rho, double theta,
		double *xp, double *yp)
{
	*xp = rho * cos(theta);
	*yp = rho * sin(theta);
}

...

	double x, y;
	polar_to_rectangular(1., 3.14, &x, &y);
Have the function return a structure containing the desired values:
struct xycoord { double x, y; };

struct xycoord
polar_to_rectangular(double rho, double theta)
{
	struct xycoord ret;
	ret.x = rho * cos(theta);
	ret.y = rho * sin(theta);
	return ret;
}

...

	struct xycoord c = polar_to_rectangular(1., 3.14);
Use a hybrid: have the function accept a pointer to a structure, which it fills in:
polar_to_rectangular(double rho, double theta,
		struct xycoord *cp)
{
	cp->x = rho * cos(theta);
	cp->y = rho * sin(theta);
}

...

	struct xycoord c;
	polar_to_rectangular(1., 3.14, &c);
(Another example of this technique is the Unix system call stat.)
In a pinch, you could theoretically use global variables (though this is rarely a good idea).


int main() {
  printf("%d\n", max(3, 4));  // 4

  int i = 0;
  inc(&i);
  inc(&i);
  inc(&i);
  printf("%d\n", i);  // 3

  float arr[] = {1.0, 2.0, 3.0};
  printf("%f\n", avg(arr, 3));  // 2.0
  
  printf("%d\n", sum(5));  // 10
}