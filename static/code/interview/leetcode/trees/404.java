class Solution {
  public int sumOfLeftLeaves(TreeNode root) {
    return recurse(root, false);
  }
  
  public int recurse(TreeNode curr, boolean isLeft) {
    if (curr == null) {
      return 0;
    }
    
    if (curr.left == null && curr.right == null && isLeft) {
      return curr.val;
    }
    
    return recurse(curr.left, true) + recurse(curr.right, false);
  }
}