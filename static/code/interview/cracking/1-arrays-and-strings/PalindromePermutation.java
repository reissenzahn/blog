
public class PalindromePermutation {
  
  // O(n) time
  public static boolean isPermutationOfPalindrome(String s) {
    int[] counts = new int[Character.getNumericValue('z') - Character.getNumericValue('a') + 1];

    for (char c : s.toCharArray()) {
      counts[Character.getNumericValue(c) - Character.getNumericValue('a')]++;
    }

    boolean foundOdd = false;

    for (int count : counts) {
      if (count % 2 == 1) {
        if (foundOdd) {
          return false;
        }

        foundOdd = true;
      }
    }
  
    return true;
  }

  // optimise space
  public static boolean isPermutationOfPalindrome2(String s) {
    int counts = 0;

    for (char c : s.toCharArray()) {
      int mask = 1 << Character.getNumericValue(c);
      
      if ((counts & mask) == 0) {
        counts |= mask;
      } else {
        counts &= ~mask;
      }
    }

    // all even
    if (counts == 0) {
      return true;
    }

    // exactly one bit set
    return (counts & (counts - 1)) == 0;
  }

  // $ javac PalindromePermutation.java && java PalindromePermutation
  public static void main(String[] args) {
    System.out.println(PalindromePermutation.isPermutationOfPalindrome("tactcoapapa"));
  }
}