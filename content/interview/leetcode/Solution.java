import java.util.*;


class Solution {
  public String decodeString(String s) {
    Stack<String> stack = new Stack<>();
    
    char[] arr = s.toCharArray();
    
    for (int i = 0; i < arr.length; i++) {
      if (arr[i] != ']') {
        if (Character.isDigit(arr[i])) {
          String v = "";
          
          while (Character.isDigit(arr[i])) {
            v = v + arr[i];
            i++;
          }

          i--;
          
          // System.out.println("b: " + v);
          stack.push(v);
        } else {
          // System.out.println("a: " + arr[i]);
          stack.push(Character.toString(arr[i])); 
        }
      } else {
        String t = stack.pop();
        // System.out.println("t: " + t);
        String u = "";
        
        while (!t.equals("[")) {
          u = t + u;
          t = stack.pop();

          // System.out.println("t: " + t);
        }

        
        String v = "";
        int d = Integer.parseInt(stack.pop());
        for (int j = 0; j < d; j++) {
          v = v + u;
        }
        
        stack.push(v);
      }
    }

    String ret = "";

    while (!stack.isEmpty()) {
      ret = stack.pop() + ret;
    }
    
    return ret;
  }

  // public static void main(String[] args) {
  //   Solution s = new Solution();

  //   System.out.println(s.decodeString("3[a]2[bc]"));
  // }
}