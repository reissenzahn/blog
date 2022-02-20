class Solution {
  public TreeNode insertIntoBST(TreeNode root, int val) {
    TreeNode node = new TreeNode(val);
    
    if (root == null) return node;
    
    TreeNode curr = root;
    TreeNode prev = null;
    
    while (curr != null) {
      prev = curr;
      
      if (curr.val > val) {
        curr = curr.left;
      } else {
        curr = curr.right;
      }
    }
    
    if (prev.val > val) {
      prev.left = node;
    } else {
      prev.right = node;
    }
    
    return root;
  }
}