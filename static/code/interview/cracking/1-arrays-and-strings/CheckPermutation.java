import java.util.*;

public class CheckPermutation {

  public static boolean checkPermutation1(String s1, String s2) {
    if (s1.length() != s2.length()) {
      return false;
    }

    char[] c1 = s1.toCharArray();
    Arrays.sort(c1);

    char[] c2 = s2.toCharArray();
    Arrays.sort(c2);

    for (int i = 0; i < s1.length(); i++) {
      if (c1[i] != c2[i]) {
        return false;
      }
    }

    return true;
  }

  public static boolean checkPermutation2(String s1, String s2) {
    if (s1.length() != s2.length()) {
      return false;
    }

    int[] counts = new int[128];

    for (char c : s1.toCharArray()) {
      counts[c]++;
    }

    for (char c : s2.toCharArray()) {
      counts[c]--;
      if (counts[c] < 0) {
        return false;
      }
    }

    return true;
  }

  // $ javac CheckPermutation.java && java CheckPermutation
  public static void main(String[] args) {
    System.out.println(CheckPermutation.checkPermutation1("abcde", "cbead"));  // true
    System.out.println(CheckPermutation.checkPermutation2("abcde", "cbead"));  // true
  }
}