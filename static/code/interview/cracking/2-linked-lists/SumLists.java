
public class SumLists {

  public static Node sumLists(Node head1, Node head2) {
    Node dummy = new Node(-1, null);
    Node tail = dummy;

    boolean carry = false;

    while (head1 != null || head2 != null) {
      int sum = 0;

      if (carry) {
        sum += 1;
        carry = false;
      }

      if (head1 != null) {
        sum += head1.value;
        head1 = head1.next;
      }

      if (head2 != null) {
        sum += head2.value;
        head2 = head2.next;
      }

      if (sum > 9) {
        sum -= 10;
        carry = true;
      }

      tail.next = new Node(sum, null);
      tail = tail.next;
    }

    return dummy.next;
  }

  private static class Node {
    private int value;
    private Node next;

    public Node(int value, Node next) {
      this.value = value;
      this.next = next;
    }
  }

  // $ javac SumLists.java && java SumLists
  public static void main(String[] args) {
    Node head1 = new Node(7, new Node(1, new Node(6, null)));
    Node head2 = new Node(5, new Node(9, new Node(2, null)));

    Node head = sumLists(head1, head2);

    while (head != null) {
      System.out.print(head.value + " ");
      head = head.next;
    }

    System.out.println();
  }
}