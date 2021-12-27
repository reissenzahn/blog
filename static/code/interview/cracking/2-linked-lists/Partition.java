
public class Partition {

  public static Node partition(Node node, int x) {
    Node head = node;
    Node tail = node;

    while (node != null) {
      Node next = node.next;

      if (node.value < x) {
        node.next = head;
        head = node;
      } else {
        tail.next = node;
        tail = node;
      }

      node = next;
    }

    tail.next = null;

    return head;
  }

  private static class Node {
    private int value;
    private Node next;

    public Node(int value, Node next) {
      this.value = value;
      this.next = next;
    }
  }

  public static void main(String[] args) {

  }
}