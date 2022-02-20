class Solution {
  private int xDepth, yDepth, xParent, yParent;
  
  public boolean isCousins(TreeNode root, int x, int y) {
    recurse(root.left, x, y, 1, root.val);
    recurse(root.right, x, y, 1, root.val);
    
    return (xParent != yParent) && (xDepth == yDepth);
  }
  
  private void recurse(TreeNode curr, int x, int y, int depth, int parent) {
    if (curr == null) return;
    
    if (curr.val == x) {
      xDepth = depth;
      xParent = parent;
      return;
    }
    
    if (curr.val == y) {
      yDepth = depth;
      yParent = parent;
      return;
    }
    
    recurse(curr.left, x, y, depth + 1, curr.val);
    recurse(curr.right, x, y, depth + 1, curr.val);
  }
}