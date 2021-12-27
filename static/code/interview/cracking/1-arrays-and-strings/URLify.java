import java.util.*;

public class URLify {

  public static void solve(char[] array, int trueLength) {
    int numSpaces = 0;

    for (int i = 0; i < trueLength; i++) {
      if (array[i] == ' ') {
        numSpaces++;
      }
    }

    // we start from the end and work backwards
    int j = trueLength - 1 + numSpaces * 2;

    for (int i = trueLength - 1; i >= 0; i--) {
      if (array[i] == ' ') {
        array[j] = '0';
        array[j - 1] = '2';
        array[j - 2] = '%';
        j -= 3;
      } else {
        array[j] = array[i];
        j--;
      }
    }
  }

  // $ javac URLify.java && java URLify
  public static void main(String[] args) {
    char[] array = "Mr John Smith    ".toCharArray();
    URLify.solve(array, 13);
    System.out.println(Arrays.toString(array));
  }
}