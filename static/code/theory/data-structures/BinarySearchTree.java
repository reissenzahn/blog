// STATUS: WIP

public class BinarySearchTree<T extends Comparable<T>> {

  private Node root;
  private int size = 0;

  private static class Node {
    T value;
    Node left;
    Node right;

    public Node(T value, Node left, Node right) {
      this.value = value;
      this.left = left;
      this.right = right;
    }
  }

  public boolean insert(T value) {
    if (this.contains(value)) {
      return;
    }

    root = insert(root, value);

  }

  public boolean validate() {
    return this.validateRange(this.root, Integer.MIN_VALUE, Integer.MAX_VALUE);
  }

  private boolean validateRange(Node node, int min, int max) {
    if (node == null) return true;

    // 
    if (node.value < min || node.value >= max) return false;

    return this.validateRange(root.left, min, node.value) && this.validateRange(root.right, node.value, max);
  }

  public int size() {
    return size;
  }

  public boolean isEmpty() {
    return this.size() == 0;
  }
}
