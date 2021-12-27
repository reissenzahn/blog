import java.util.*;

public class RemoveDups {

  private static class Node {
    private int value;
    private Node next;

    public Node(int value, Node next) {
      this.value = value;
      this.next = next;
    }
  }

  // O(n)
  public static void removeDups(Node head) {
    Node prev = null;
    Node curr = head;

    Set<Integer> seen = new HashSet<>();

    while (curr != null) {
      if (seen.contains(curr.value)) {
        prev.next = curr.next;
      } else {
        seen.add(curr.value);
        prev = curr;
      }

      curr = curr.next;
    }
  }

  public static void removeDups2(Node head) {
    Node curr = head;

    while (curr != null) {
      // remove all future nodes that have the same value
      Node runner = curr;

      while (runner.next != null) {
        if (runner.next.value == curr.value) {
          runner.next = runner.next.next;
        } else {
          runner = runner.next;
        }
      }

      curr = curr.next;
    }
  }

  // $ javac RemoveDups.java && java RemoveDups
  public static void main(String[] args) {
    Node head = new Node(1, new Node(2, new Node(2, new Node(3, new Node(4, new Node(4, null))))));
    
    // RemoveDups.removeDups(head);
    RemoveDups.removeDups2(head);

    Node curr = head;
    while (curr != null) {
      System.out.println(curr.value + " ");
      curr = curr.next;
    }
  }
}