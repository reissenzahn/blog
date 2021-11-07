import java.util.*;

public class Trie {

  private char rootCharacter = '\0';
  private Node root = new Node(rootCharacter);

  private static class Node {
    char c;
    boolean isWordEnding = false;
    Map<Character, Node> children = new HashMap<>();

    public Node(char c) {
      this.c = c;
    }

    public void getChild(char c) {
      return children.get(c);
    }

    public void addChild(char c, Node node) {
      this.children.put(c, node);
    }
  }

  public void insert(String key) {
    Node curr = root;

    for (int i = 0; i < key.length(); i++) {
      char c = key.charAt(i);
      
      Node next = curr.getChild(c);

      if (next == null) {
        next = new Node(c);
        curr.addChild(c, next);
      }

      curr = next;
    }

    if (node != root) {
      node.isWordEnding = true;
    }
  }

  public void delete(String key) {
    Node curr = root;

    for (int i = 0; i < key.length(); i++) {
      char c = key.charAt(i);

      Node next = curr.getChild(c);

      
    }
  }
}
