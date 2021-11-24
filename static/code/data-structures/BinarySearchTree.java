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

  public int size() {
    return size;
  }

  public boolean isEmpty() {
    return this.size() == 0;
  }
}
