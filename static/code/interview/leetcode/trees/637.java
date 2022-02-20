class Solution {
  public List<Double> averageOfLevels(TreeNode root) {
    List<Double> avgs = new ArrayList<>();
    
    if (root == null) return null;
    
    Queue<TreeNode> queue = new LinkedList<>();
    queue.offer(root);
    
    while (!queue.isEmpty()) {
      int n = queue.size();
      long sum = 0;
      
      for (int i = 0; i < n; i++) {
        TreeNode curr = queue.poll();
        
        if (curr.left != null) queue.offer(curr.left);
        if (curr.right != null) queue.offer(curr.right);
        
        sum += curr.val;
      }
      
      avgs.add((double) sum / n);
    }
    
    return avgs;
  }
}