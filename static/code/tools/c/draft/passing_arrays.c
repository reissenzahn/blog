
int main() {

  // when an array is passed to a function, the array's address is passed by value; this 
}


// Passing a One-Dimensional Array

// When a one-dimensional array is passed to a function, the array’s address is passed by value.

// In most situations, the array’s size must be passed as there is nothing inherent in an array’s internal representation that determines its length.

// We can declare the array in the function declaration using one of two notations: array notation or pointer notation.

void displayArray(int arr[], int size) {
 for (int i = 0; i < size; i++) {
 printf("%d\n", arr[i]);
 }
}
int vector[5] = {1, 2, 3, 4, 5};
displayArray(vector, 5);

void displayArray(int* arr, int size) {
 for (int i = 0; i < size; i++) {
 printf("%d\n", *(arr+i));
 }
}

// or some combination of the two
void displayArray(int arr[], int size) {
 for (int i = 0; i < size; i++) {
 printf("%d\n", *(arr+i));
 }
}


// Passing a Multidimensional Array

Passing a multidimensional array to a function can be confusing, especially when point‐
er notation is used. When passing a multidimensional array, we need to determine
whether to use array notation or pointer notation in the function’s signature. Another
consideration is how to convey the array’s shape. By shape, we are referring to the num‐
ber and size of its dimensions. If we want to use array notation within the function, it
is imperative to specify the array’s shape. Otherwise, the compiler is unable to use
subscripts.
To pass the matrix array, use either:
void display2DArray(int arr[][5], int rows) {
or:
void display2DArray(int (*arr)[5], int rows) {
In both versions the number of columns is specified. This is needed because the compiler
needs to know the number of elements in each row. If this information is not passed,
then it is unable to evaluate expressions such as arr[0][3] as explained in the section
“Pointers and Multidimensional Arrays” on page 94.
In the first version, the expression arr[] is an implicit declaration of a pointer to an
array. In the second version, the expression (*arr) is an explicit declaration of the
pointer.
The following declaration will not work correctly:
void display2DArray(int *arr[5], int rows) {
While it will not generate a syntax error, the array passed is assumed to
be a five-element array of pointers to integers. “Using a OneDimensional Array of Pointers” on page 92 discusses arrays of pointers.


A simple implementation of this function and invocation follows:
void display2DArray(int arr[][5], int rows) {
 for (int i = 0; i<rows; i++) {
 for (int j = 0; j<5; j++) {
 printf("%d", arr[i][j]);
 }
 printf("\n");
 }
}
void main() {
 int matrix[2][5] = {
 {1, 2, 3, 4, 5},
 {6, 7, 8, 9, 10}
 };
 display2DArray(matrix, 2);
}
The function does not allocate memory for the array. Only the address is passed. The
program stack’s state for this call is shown in Figure 4-13.
Figure 4-13. Passing multidimensional array
You may encounter a function declared as follows. It is passed a single pointer and the
number of rows and columns:
void display2DArrayUnknownSize(int *arr, int rows, int cols) {
 for(int i=0; i<rows; i++) {
 for(int j=0; j<cols; j++) {
 printf("%d ", *(arr + (i*cols) + j));
 }
 printf("\n");
 }
}

The printf statement calculates the address of each element by adding to arr the num‐
ber of elements in the previous row(s), (i*cols), and then adding j to specify the
column. To invoke the function, we can use the following:
 display2DArrayUnknownSize(&matrix[0][0], 2, 5);
Within the function, we cannot use array subscripts as shown below:
 printf("%d ", arr[i][j]);
This is not possible because the pointer is not declared as a two-dimensional array.
However, it is possible to use array notation as shown below. We can use a single sub‐
script since it will be interpreted simply as an offset within the array, whereas two sub‐
scripts cannot be used because the compiler doesn’t know the size of the dimensions:
 printf("%d ", (arr+i)[j]);
The first element’s address is passed using &matrix[0][0] instead of matrix. While
using matrix will execute correctly, a warning will be generated, indicating incompatible
pointer types. The expression &matrix[0][0] is a pointer to an integer, whereas matrix
is a pointer to an array of integers.
When passing an array with more than two dimensions, all but the size of the first
dimension need to be specified. The following demonstrates a function written to dis‐
play a three-dimensional array. The last two dimensions are specified in the declaration:
void display3DArray(int (*arr)[2][4], int rows) {
 for(int i=0; i<rows; i++) {
 for(int j=0; j<2; j++) {
 printf("{");
 for(int k=0; k<4; k++) {
 printf("%d ", arr[i][j][k]);
 }
 printf("}");
 }
 printf("\n");
 }
}
The following code shows the function’s invocation:
 int arr3d[3][2][4] = {
 {{1, 2, 3, 4}, {5, 6, 7, 8}},
 {{9, 10, 11, 12}, {13, 14, 15, 16}},
 {{17, 18, 19, 20}, {21, 22, 23, 24}}
 };
 display3DArray(arr3d,3);
The output follows:

{1 2 3 4 }{5 6 7 8 }
{9 10 11 12 }{13 14 15 16 }
{17 18 19 20 }{21 22 23 24 }
Allocation of the array’s memory is depicted in Figure 4-14.
Figure 4-14. Three-dimensional array
The expression arr3d[1] refers to the array’s second row and is a pointer to a twodimensional array with two rows and four columns. The expression arr3d[1][0] refers
to the second row, first column of the array and is a pointer to a one-dimensional array
of size 5.
