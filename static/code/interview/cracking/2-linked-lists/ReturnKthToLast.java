
// a -> b -> c -> d -> e -> f

public class ReturnKthToLast {

  // O(n)
  public static int returnKthToLast(Node head, int k) {
    Node curr = head;
    Node trail = head;

    for (int i = 0; i < k; i++) {
      if (curr == null) {
        return -1;
      }

      curr = curr.next;
    }

    while (curr != null) {
      curr = curr.next;
      trail = trail.next;
    }

    return trail.value;
  }

  private static class Node {
    private int value;
    private Node next;

    public Node(int value, Node next) {
      this.value = value;
      this.next = next;
    }
  }

  // $ javac ReturnKthToLast.java && java ReturnKthToLast
  public static void main(String[] args) {
    // 1 -> 2 -> 2 -> 3 -> 4 -> 4

    Node head = new Node(1, new Node(2, new Node(2, new Node(3, new Node(4, new Node(4, null))))));

    System.out.println(ReturnKthToLast.returnKthToLast(head, 3));
  }
}
