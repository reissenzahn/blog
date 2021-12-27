
public class OneAway {

  // O(n)
  public static boolean isOneAway(String s1, String s2) {

    if (s1.length() == s2.length()) {
      return oneEditReplace(s1, s2);
    } else if (s1.length() + 1 == s2.length()) {
      return oneEditInsert(s1, s2);
    } else if (s1.length() - 1 == s2.length()) {
      return oneEditInsert(s2, s1);
    }
    
    return false;
  }

  private static boolean oneEditReplace(String s1, String s2) {
    boolean foundDifference = false;

    for (int i = 0; i < s1.length(); i++) {
      if (s1.charAt(i) != s2.charAt(i)) {
        if (foundDifference) {
          return false;
        }

        foundDifference = true;
      }
    }
  
    return true;
  }

  private static boolean oneEditInsert(String s1, String s2) {
    int i = 0;
    int j = 0;

    while (i < s1.length() && j < s2.length()) {
      if (s1.charAt(i) != s2.charAt(j)) {
        if (i != j) {
          return false;
        }
        j++;
      } else {
        i++;
        j++;
      }
    }

    return true;
  }

  // $ javac OneAway.java && java OneAway
  public static void main(String[] args) {
    System.out.println(isOneAway("pale", "ple"));
  }
}
