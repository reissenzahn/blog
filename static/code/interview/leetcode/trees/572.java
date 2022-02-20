class Solution {
  public boolean isSubtree(TreeNode root, TreeNode subRoot) {
    if (root == null) return false;
    
    if (isEqual(root, subRoot)) return true;
    
    return isSubtree(root.left, subRoot) || isSubtree(root.right, subRoot);
  }
  
  private boolean isEqual(TreeNode r1, TreeNode r2) {
    if (r1 == null && r2 == null) return true;
    
    if (r1 == null || r2 == null) return false;
    
    return r1.val == r2.val && isEqual(r1.left, r2.left) && isEqual(r1.right, r2.right);
  }
}