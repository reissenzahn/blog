#include <stdio.h>

// an array is a fixed-size contiguous collection of elements of the same type that can be accessed using an index

// equivalent to int sum(int *arr, int len) 
int sum(int arr[], int len) {
  int total = 0;

  for (int i = 0; i < len; i++) {
    total += arr[i];
  }

  return total;
}

int main() {
  // array indexes start with 0 and end at one less than their declared size though these bounds are not enforced
  int arr1[5];

  for (int i = 0; i < 5; i++) {
    arr1[i] = i;
  }
  
  printf("%d\n", nums[2]);  // 2

  // the internal representation of an array has no information about the number of elements it contains

  // using the sizeof operator with an array will return the number of bytes allocated to the array
  printf("%zu\n", sizeof(arr1));
  printf("%d\n", sizeof(arr1) / sizeof(int));

  // arrays can be readily initialized using a block type statement
  int arr2[5] = {0, 1, 2, 3, 4};

  for (int i = 0; i < 5; i++) {
    printf("%d ", arr2[i]);  // 0 1 2 3 4
  }
  printf("\n");

  // a two-dimensional array is arranged in memory with the first row followed by the second row and then the third and so on
  int arr3[2][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%d ");
    }
    printf("\n");
  }

  // we can have multi-dimensional arrays of rank greater than 2
  int arr4[3][2][4] = {
    {{1, 2, 3, 4}, {5, 6, 7, 8}},
    {{9, 10, 11, 12}, {13, 14, 15, 16}},
    {{17, 18, 19, 20}, {21, 22, 23, 24}}
  };

  // When an array name is used by itself, the addres of the first element of the array is returned.

  int vector[5] = {1, 2, 3, 4, 5};
  int *pv = vector;

  // We can use either the array name by itself or use the address-of operator with the array’s first element as illustrated below. These are equivalent and will return the address of vector.

  printf("%p\n",vector);
  printf("%p\n",&vector[0]);

  // The expression &vector is sometimes used to obtain the address of an array. It differs from the other notations in that it returns a pointer to the entire array. The other two approaches yield a pointer to an integer. Instead of returning a pointer to an integer, it returns a pointer to an array of integers.

  // We can also use array subscripts with pointers. Effectively, the notation pv[i] is evaluated as *(pv + i).

  // The pointer pv contains the address of a block of memory. The bracket notation will take the address contained in pv and adds the value contained in the index i using pointer arithmetic. This new address is then dereferenced to return its contents.

  pv = vector;
  int value = 3;
  for(int i=0; i<5; i++) {
  *pv++ *= value;
  }


  // The pointer pv is an lvalue. An lvalue denotes the term used on the lefthand side of an assignment operator. An lvalue must be capable of being modified. An array name such as vector is not an lvalue and cannot be modified.

  int vector[5] = {1, 2, 3, 4, 5};
  int *pv = vector;
  pv = vector + 1;
  vector = vector + 1; // Syntax error

  // declaration with initialization
  char arr1[] = {1, 2, 3, 4, 5};

  // if an array is partially initialized then elements that are not initialized receive the zero value of the appropriate type
  int zeros[1024] = {0};  // {0, ..., 0}

  // declaration without initialization


  // iterate over array
  for (size_t i = 0; i < sizeof(nums); i++) {
    nums[i] = nums[i] * 2;
  }
  
  printf("%d\n", nums[0]);  // 0
  printf("%d\n", nums[1]);  // 2
  printf("%d\n", nums[2]);  // 4
  printf("%d\n", nums[3]);  // 6
  printf("%d\n", nums[4]);  // 8

  // when an array name is passed to a function what is passed is a pointer to the initial element
  printf("%d\n", sum(nums, 5));  // 20



}



// Array notation and pointer notation can be used somewhat interchangeably. However, an array name is not a pointer. Although an array name can be treated as a pointer at times, and array notation can be used with pointers, they are distinct and cannot always be used in place of each other.

// In most situations, the array’s size must be passed as there is nothing inherent in an array’s internal representation that determines its length.

// A jagged array is a two-dimensional array where each row may have a different number of columns.





// Using malloc to Create a One-Dimensional Array

int *pv = (int*) malloc(5 * sizeof(int));
for(int i=0; i<5; i++) {
  pv[i] = i+1;
}

for(int i=0; i<5; i++) {
 *(pv+i) = i+1;
}


// Dynamically Allocating a Two-Dimensional Array

// The following illustrates one way of allocating a two-dimensional array where the allocated memory is not guaranteed to be contiguous. 

int rows = 2;
int columns = 5;

int **matrix = (int **) malloc(rows * sizeof(int *));

for (int i = 0; i < rows; i++) {
  matrix[i] = (int *) malloc(columns * sizeof(int));
}


// Allocating Contiguous Memory

// to allocate a two-dimensional array where the allocated memory is contiguous we can start by allocating an array of pointers to integers and then allocate memory for all of the elements of the array
int rows = 2;
int columns = 5;

// the second malloc allocates memory for all of the elements of the array
int **matrix = (int **) malloc(rows * sizeof(int *));
matrix[0] = (int *) malloc(rows * columns * sizeof(int));

// then each element of the first array is assigned a portion of the memory allocated by the second malloc
for (int i = 1; i < rows; i++) {
  matrix[i] = matrix[0] + i * columns;
}

// the memory for the first array may be separate but a contiguous region of memory is allocated for the body

// alternatively we can allocate all of the memory for the array at once
int *matrix = (int *) malloc(rows * columns * sizeof(int));

// however when the we cannot use array subscripts later as we have lost the shape information needed by the compiler to permit subscripts
for (int i = 0; i < rows; i++) {
  for (int j = 0; j < columns; j++) {
    *(matrix + (i*columns) + j) = i*j;
  }
}














// TODO:

// When an array name is used by itself, it returns the address of an array, which is also the address of the first element of the array:


// variable length arrays allow the declaraton and creation of an array within a function whose size is based on a variable

// void compute(int size) {
//   char* buffer[size];
//   ...
// }

// This means the allocation of memory is done at runtime and memory is allocated as part of the stack frame. Also, when the sizeof operator is used with the array, it will be executed at runtime rather than compile time. When the function exits, the memory is effectively deallocated.








// TODO:

// there is a strong relationship between pointers and arrays
int arr[5];

for (int i = 0; i < 5; i++) {
  arr[i] = i;
}
  
// any operation that can be achieved by array subscripting can also be done with pointers  
int *ptr;

// set ptr to point to the first element of arr
ptr = &arr[0];

// the name of an array is a synonym for the location of the initial element
ptr = arr;

// ptr + i points to the i-th object beyond ptr
printf("%d\n", *(ptr + 0));  // 0
printf("%d\n", *(ptr + 1));  // 1
printf("%d\n", *(ptr + 2));  // 2
printf("%d\n", *(ptr + 3));  // 3
printf("%d\n", *(ptr + 4));  // 4{{% /code %}}


// pointer arithmetic and array subscripting
int arr[] = {0, 1, 2, 3, 4, 5};
int n = 2;

int *ptr = arr;

printf("addr = %p, val = %d\n", (void *) (ptr + n), *(ptr + n));
printf("addr = %p, val = %d\n", (void *) &ptr[n], ptr[n]);
printf("addr = %p, val = %d\n", (void *) &n[ptr], n[ptr]);

// array subscripting and pointer arithmetic work on both arrays any pointers
int arr[] = {0, 1, 2, 3, 4, 5};
int *ptr = arr;

printf("%d\n", arr[2]);  // 2
printf("%d\n", ptr[2]);  // 2

printf("%d\n", *(arr + 2));  // 2
printf("%d\n", *(ptr + 2));  // 2

// we can use a char * to point to the bytes of an int
int x = 0x04030201;
char *ptr = &x;

printf("byte 1: addr = %p, val = %d\n", (void *) ptr, *ptr);
printf("byte 2: addr = %p, val = %d\n", (void *) (ptr + 1), *(ptr + 1));
printf("byte 3: addr = %p, val = %d\n", (void *) (ptr + 2), *(ptr + 2));
printf("byte 4: addr = %p, val = %d\n", (void *) (ptr + 3), *(ptr + 3));






// Using a One-Dimensional Array of Pointers

// The following sequence declares an array of integer pointers, allocates memory for each element, and initializes this memory to the array’s index:

int* arr[5];
for(int i = 0; i < 5; i++) {
  arr[i] = (int *) malloc(sizeof(int));
  *arr[i] = i;
}

// equivalent body
*(arr+i) = (int*)malloc(sizeof(int));
**(arr+i) = i;

*arr[0] 0
**arr 0
**(arr+1) 1
arr[0][0] 0
arr[3][0] 3



// Pointers and Multidimensional Arrays

Parts of multidimensional arrays can be treated as subarrays. For example, each row of
a two-dimensional array can be treated as a one-dimensional array. 

int matrix[2][5] = {{1,2,3,4,5},{6,7,8,9,10}};

for(int i=0; i<2; i++) {
  for(int j=0; j<5; j++) {
    printf("matrix[%d][%d] Address: %p Value: %d\n", i, j, &matrix[i][j], matrix[i][j]);
  }
}

int (*pmatrix)[5] = matrix;

The expression, (*pmatrix), declares a pointer to an array. Combined with the rest of the declaration, pmatrix is defined as a pointer to a two-dimensional array of integers with five elements per column. If we had left the parentheses off, we would have declared
a five-element array of pointers to integers. The size of the first dimension is 2 since we
know the dimensions of the matrix. If a different size is used to access the array, then
the results are unpredictable.
If we want to access the second element, 2, using pointer notation, it might seem rea‐
sonable to use the following:
 printf("%p\n", matrix);
 printf("%p\n", matrix + 1);
The output follows:
100
120
The address returned by matrix+1 is not offset by 4 from the beginning of the array.
Instead, it is offset by the first row’s size, 20 bytes. Using matrix by itself returns the
address of the array’s first element. Since a two-dimensional array is an array of arrays,
we get the address of a five-element integer array. Its size is 20. We can verify this with
the following statement, which will display 20:
 printf("%d\n",sizeof(matrix[0])); // Displays 20
To access the array’s second element, we need to add 1 to the first row of the array as
follows: *(matrix[0] + 1). The expression, matrix[0], returns the address of the first
element of the first row of the array. This address is the address of an array of integers.
Thus, when we add one to it, the size of a single integer is added to it, giving us the
second element. The output will be 104 and 2.
 printf("%p %d\n", matrix[0] + 1, *(matrix[0] + 1));










// a jagged array is a two-dimensional array possessing a different number of columns for each row






