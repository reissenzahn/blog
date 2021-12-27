
public class StringCompression {

  public static String compress(String s) {
    StringBuilder sb = new StringBuilder();

    int numConsecutive = 0;

    for (int i = 0; i < s.length(); i++) {
      numConsecutive++;

      if (i + 1 >= s.length() || s.charAt(i) != s.charAt(i + 1)) {
        sb.append(s.charAt(i));
        sb.append(numConsecutive);
        numConsecutive = 0;
      }
    }

    return sb.length() < s.length() ? sb.toString() : s;
  }

  // $ javac StringCompression.java && java StringCompression
  public static void main(String[] args) {
    System.out.println(StringCompression.compress("aabcccccaaa"));
  }
}