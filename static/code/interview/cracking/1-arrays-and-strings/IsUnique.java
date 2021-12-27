public class IsUnique {

  // O(n) time and O(1) space
  public static boolean isUniqueASCII(String s) {
    if (s.length() > 128) {
      return false;
    }

    boolean[] seen = new boolean[128];

    for (int i = 0; i < s.length(); i++) {
      int c = s.charAt(i);

      if (seen[c]) {
        return false;
      }

      seen[c] = true;
    }

    return true;
  }

  // optimize space
  public static boolean isUniqueAZ(String s) {
    int seen = 0;

    for (int i = 0; i < s.length(); i++) {
      int c = s.charAt(i) - 'a';

      if ((seen & 1 << c) > 0) {
        return false;
      }

      seen |= (1 << c);
    }

    return true;
  }

  // $ javac IsUnique.java && java IsUnique
  public static void main(String[] args) {
    System.out.println(IsUnique.isUniqueASCII("unique"));  // false
    System.out.println(IsUnique.isUniqueASCII("same"));    // true
    
    System.out.println(IsUnique.isUniqueAZ("unique"));  // false
    System.out.println(IsUnique.isUniqueAZ("same"));    // true
  }
}