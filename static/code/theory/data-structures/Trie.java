import java.util.*;

public class Trie {

  private Node root;

  public Trie() {
    this.root = new Node('\0');
  }

  public void insert(String key) {
    Node curr = this.root;

    // step over each character in key
    for (int i = 0; i < key.length(); i++) {
      char ch = key.charAt(i);
      Node next = curr.children.get(ch);

      // character does not exist so add it
      if (next == null) {
        next = new Node(ch);
        curr.children.put(ch, next);
      }

      next.count += 1;
      curr = next;
    }

    // once we get to the final character we mark it as a word ending
    if (curr != root) {
      curr.isWordEnding = true;
    }
  }

  public void delete(String key) {
    Node curr = root;

    for (int i = 0; i < key.length(); i++) {
      char ch = key.charAt(i);
      Node next = curr.children.get(ch);
      next.count -= 1;

      if (next.count <= 0) {
        curr.children.remove(ch);
        return;
      }

      curr = next;
    }
  }

  public int count(String key) {
    Node curr = root;

    for (int i = 0; i < key.length(); i++) {
      char ch = key.charAt(i);

      if (curr == null) {
        return 0;
      }

      curr = curr.children.get(ch);
    }

    if (curr != null) {
      return node.count;
    }

    return 0;
  }

  public boolean contains(String key) {
    return this.count(key) != 0;
  }

  public void clear() {
    this.root = new Node('\0');
  }

  private static class Node {

    private char ch;
    private int count;
    private boolean isWordEnding;
    private Map<Character, Node> children;

    public Node(char ch) {
      this.ch = ch;
      this.count = 0;
      this.isWordEnding = false;
      this.children = new HashMap<>();
    }
  }

  public static void main(String[] args) {

  }
}