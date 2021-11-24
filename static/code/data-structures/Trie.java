import java.util.*;

public class Trie {

  private static String ROOT_CHAR = '\0';

  private Node root = new Node(ROOT_CHAR);

  private static class Node {
    char c;
    int count = 0;
    boolean isWordEnding = false;
    Map<Character, Node> children = new HashMap<>();

    public Node(char c) {
      this.c = c;
    }

    public Node getChild(char c) {
      return children.get(c);
    }

    public void addChild(char c, Node node) {
      children.put(c, node);
    }
  }

  public void insert(String key) {
    Node currNode = root;

    for (int i = 0; i < key.length(); i++) {
      char c = key.charAt(i);

      Node nextNode = currNode.children.get(c);

      if (nextNode == null) {
        nextNode = new Node(c);
        currNode.addChild(c, nextNode);
      }

      currNode = nextNode;
      currNode.count += 1;
    }

    if (currNode != root) {
      currNode.isWordEnding = true;
    }
  }

  public void delete(String key) {
    if (!this.contains(key)) {
      return;
    }

    Node currNode = root;

    for (int i = 0; i < key.length(); i++) {
      char c = key.charAt(i);

      Node nextNode = currNode.children.get(c);

      nextNode.count -= 1;

      if (nextNode.count <= 0) {
        currNode.children.remove(c);
        return;
      }

      currNode = nextNode;
    }
  }
  
  public int count(String key) {
    Node currNode = root;

    for (int i = 0; i < key.length(); i++) {
      if (currNode == null) {
        return 0;
      }
      
      char c = key.charAt(i);

      currNode = currNode.children.get(c);
    }

    if (currNode != null) {
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
  
  public static void main(String[] args) {
    Trie trie = new Trie();
  }
}