

public class ReferenceTypes {

  public static void main(String[] args) {



    // an object reference can be converted to the type of its superclass (or any ancestor class)
    Object obj1 = "string";

    // an object reference can be converted to the type of a subclass with a cast in which case a check will be performed at runtime to make sure the cast is valid
    String str = (String) obj1;
    System.out.println(str);


    // an array can be converted to an Object and then back to an arrow with a cast
    int[] arr1 = new int[]{ 1, 2, 3 };
    Object obj2 = arr1
    arr1 = (int[]) obj2;
    System.out.println(Arrays.toString(arr1));

    // we can also convert an array to another type of array if the base types of the two arrays are reference types that can themselves be converted
    String[] arr2 = new String[]{ "a", "b", "c" };
    CharSequence[] arr3 = arr2;
    arr2 = (String[]) arr3;
    System.out.println(Arrays.toString(arr2));

    // an array of a primitive type cannot be converted to any other array type even if the primitive base types can be converted:
    // double[] data = new int[] {1,2,3};

  }
}

