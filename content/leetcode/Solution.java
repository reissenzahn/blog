import java.util.HashMap;
import java.util.Map;
import java.util.Arrays;

public class Solution {

  public int[] twoSum(int[] nums, int target) {   
    // two pass solution
    Map<Integer, Integer> map = new HashMap<>();
    for (int i = 0; i < nums.length; i++) {
      map.put(nums[i], i);
    }

    for (int i = 0; i < nums.length; i++) {
      int need = target - nums[i];
      if (map.containsKey(need) && map.get(need) != i) {
        return new int[] {i, map.get(need)};
      }
    }
    
    throw new IllegalArgumentException("No solution!");
    
    // one pass solution
    Map<Integer, Integer> map = new HashMap<Integer, Integer>();
    
    for (int i = 0; i < nums.length; i++) {
      int need = target - nums[i];
      if (map.containsKey(need)) {
        return new int[] {i, map.get(need)};
      }
      map.put(nums[i], i);
    }

    throw new IllegalArgumentException("No solution!");
  }

  public static void main(String[] args) {
    Solution s = new Solution();

    int[] nums = {2, 7, 11, 15};
    int target = 9;

    System.out.println(Arrays.toString(s.twoSum(nums, target)));
  }
}
