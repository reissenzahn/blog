
public class DeleteMiddleNode {

  private static class Node {
    private int value;
    private Node next;

    public Node(int value, Node next) {
      this.value = value;
      this.next = next;
    }
  }

  public static void deleteMiddleNode(Node node) {
    node.value = node.next.value;
    node.next = node.next.next;
  }
}
