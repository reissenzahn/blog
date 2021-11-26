
public class BinaryTree {

  private static class Node {
    private int value;
    private Node left;
    private Node right;

    public Node(int value) {
      this.value = value;
    }
  }

  public static int size(Node root) {
    if (root == null) return 0;

    return size(root.left) + size(root.right) + 1;
  }

  public static int height(Node root) {
    if (root == null) return -1;

    return Math.max(height(root.left), height(root.right)) + 1;
  }

  public static boolean sameTree(Node root1, Node root2) {
    if (root1 == null && root2 == null) return true;

    // one node is null and the other is not
    if (root1 == null || root2 == null) return false;

    return root1.value == root2.value && sameTree(root1.left, root2.left) && sameTree(root1.right, root2.right);
  }

  public static void preOrder(Node node) {
    if (node != null) {
      System.out.println(node.value);
      preOrder(node.left);
      preOrder(node.right);
    }
  }

  public static void inOrder(Node node) {
    if (node != null) {
      inOrder(node.left);
      System.out.println(node.value);
      inOrder(node.right);
    }
  }

  public static void postOrder(Node node) {
    if (node != null) {
      postOrder(node.left);
      postOrder(node.right);
      System.out.println(node.value);
    }
  }

  public static void main(String[] args) {
    
    // size
    // Node root = new Node(10);
    // root.left = new Node(2);
    // root.right = new Node(5);
    // root.left.left = new Node(6);
    // root.right.left = new Node(8);
    // root.right.right = new Node(3);

    // System.out.println(BinaryTree.size(root));  // 6

    // height
    // Node root = new Node(10);
    // root.left = new Node(5);
    // root.right = new Node(6);
    // root.right.left = new Node(8);
    // root.right.right = new Node(7);
    // root.right.right.right = new Node(4);

    // System.out.println(BinaryTree.height(root));  // 3

    // sameTree
    // Node root1 = new Node(10);
    // root1.left = new Node(16);
    // root1.right = new Node(15);
    // root1.right.left = new Node(18);
    
    // Node root2 = new Node(10);
    // root2.left = new Node(16);
    // root2.right = new Node(15);
    // root2.right.left = new Node(18);
    // // root2.right.right = new Node(23);

    // System.out.println(BinaryTree.sameTree(root1, root2));  // true
  
    // traversals
    Node root = new Node(4);
    root.left = new Node(3);
    root.right = new Node(7);
    root.left.left = new Node(8);
    root.left.right = new Node(5);
    root.right.right = new Node(6);
    root.left.left.left = new Node(2);
    root.right.right.left = new Node(9);
    root.right.right.right = new Node(1);

    BinaryTree.preOrder(root);
    BinaryTree.inOrder(root);
    BinaryTree.postOrder(root);
  }
}

