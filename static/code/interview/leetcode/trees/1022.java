class Solution {
  private int total = 0;
  
  public int sumRootToLeaf(TreeNode root) {
    recurse(root, 0);
    return total;
  }
  
  private void recurse(TreeNode curr, int num) {
    if (curr == null) return;
    
    num = (num << 1) | curr.val;
    
    if (curr.left == null && curr.right == null) total += num;
      
    recurse(curr.left, num);
    recurse(curr.right, num);
  }
}