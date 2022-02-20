class Solution {
  public boolean isUnivalTree(TreeNode root) {
    return recurse(root, root.val);
  }
  
  private boolean recurse(TreeNode curr, int val) {
    if (curr == null) return true;
    
    return curr.val == val && recurse(curr.left, val) && recurse(curr.right, val);
  }
}