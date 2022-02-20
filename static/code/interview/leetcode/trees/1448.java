class Solution {
  private int count = 0;
  
  public int goodNodes(TreeNode root) {
    recurse(root, root.val);
    return count;
  }
  
  private void recurse(TreeNode curr, int maxSoFar) {
    if (curr == null) return;
    
    if (curr.val >= maxSoFar) {
      maxSoFar = curr.val;
      count++;
    }
    
    recurse(curr.left, maxSoFar);
    recurse(curr.right, maxSoFar);
  }
}