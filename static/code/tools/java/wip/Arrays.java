// an array is a special kind of object that holds fixed-size contiguous collection of elements of the same type that can be accessed using an index

public class Arrays {

  // $ javac Arrays.java && java Arrays
  public static void main(String[] args) {
  
    // an array is created with the new operator like objects
    int[] arr1 = new int[5];

    // array indices must be of type int (or a type that can be widened to an int)
    for (int i = 0; i < 5; i++) {
      arr1[i] = i;
      System.out.println(arr1[i]);  // 0 1 2 3 4
    }


    // an array can be created and initialized with an array initializer
    int[] arr2 = new int[]{0, 1, 2, 3, 4};

    // a trailing comma may be included and the new operator and type can be omitted if the array initializer is part of a variable declaration
    int[] arr3 = {0, 1, 2, 3, 4};


    // all arrays have a length field that indicates the number of elements in the array
    System.out.println(arr1.length);

    // accessing an invalid index will throw an ArrayIndexOutOfBoundsException
    System.out.println(arr1[arr1.length]);  // exception


    // a multi-dimensional array consists of an array of arrays
    int[][] arr4 = new int[5][5];

    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        arr4[i][j] = i * j;
      }
    }

    System.out.println(arr4[2][2]);  // 4

    // initializer lists can also be used with multi-dimensional arrays
    int[][] arr5 = new int[][]{ new int[]{0, 0, 0, 0, 0},
      new int[]{0, 1, 2, 3, 4},
      new int[]{0, 2, 4, 6, 8},
      new int[]{0, 3, 6, 9, 12},
      new int[]{0, 4, 8, 12, 16} };

    // you only have to specify the length of leftmost dimension (or dimensions) and can even create ragged arrays
    int[][] arr6 = new int[5][];
    for (int i = 0; i < 5; i++) {
      arr6[i] = new int[i + 1];
    }

  
    // a shallow copy of an array can be made with the clone() method
    int[] copy = (int[]) arr1.clone();

    // the arraycopy() function copies elements from one array to another
    int[] src = {0, 1, 2, 3, 4};
    int[] dst = {-1, -1, -1, -1, -1};
    System.arraycopy(src, 2, dst, 0, 5);

  
    // an array type can be widened to another array type if the type of the its elements is assignable to the type of the elements of the other array
    String[] arr7 = new String[]{"a", "b", "c"};
    Object[] arr8 = arr7;
  }
}
