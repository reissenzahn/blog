#include <stdio.h>

// passing an array as an argument to a function results in the address of the array being passed by value
void display1(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }

  printf("\n");
}

// we can declare an array argument using either array notation or pointer notation
void display2(int *arr, int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }

  printf("\n");
}

// to use a multidimensional array as an argument we need to specify the sizes of all the dimensions except the first in order to be able to use array subscripting
void display3(int arr[][3], int rows) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%d ", arr[i][j]);
    }
  }

  printf("\n");
}

// we can also use pointer notation (though notice we do not want an array of pointers such as int *arr[5])
void display4(int (*arr)[3], int rows) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%d ", arr[i][j]);
    }
  }

  printf("\n");
}

void display6(int (*arr)[3][2], int rows) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 2; k++) {
        printf("%d ", arr[i][j][k]);
      }
    }
  }
  
  printf("\n");
}

// if the dimensions are not specified then we cannot use array subscripting as 
void display5(int *arr, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%d ", *(arr + (i*cols) + j));
    }
  }

  printf("\n");
}

int main() {
  int vector[5] = {1, 2, 3, 4, 5};

  display1(vector, 5);  // 1 2 3 4 5 
  display2(vector, 5);  // 1 2 3 4 5 

  int matrix[3][3] = {
    {1, 2, 3},
    {4, 5, 6}
    {7, 8, 9}
  };

  display3(matrix, 3);  // 1 2 3 4 5 6 7 8 9
  display4(matrix, 3);  // 1 2 3 4 5 6 7 8 9
  display5(&matrix[0][0], 3, 3);  // 1 2 3 4 5 6 7 8 9

  int matrix2[2][3][2] = {
    {{1, 2}, {3, 4}, {5, 6}},
    {{7, 8}, {9, 10}, {11, 12}},
  };

  display4(matrix2, 2);

  return 0;
}