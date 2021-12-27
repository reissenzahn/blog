import java.util.*;

public class HuffmanCoding {

  private Node root;
  private Map<Character, String> mapping;

  public HuffmanCoding(char[] alphabet, int[] freqs) {
    PriorityQueue<Node> queue = new PriorityQueue<Node>(alphabet.length);

    for (int i = 0; i < alphabet.length; i++) {
      queue.add(new Node(alphabet[i], freqs[i]));
    }

    while (queue.size() > 1) {
      Node left = queue.poll();
      Node right = queue.poll();

      Node parent = new Node('-', left.freq + right.freq);

      parent.left = left;
      parent.right = right;

      queue.add(parent);
    }

    this.root = queue.poll();
    this.mapping = new HashMap<>();

    this.generateMapping(root, "");
  }

  private void generateMapping(Node node, String s) {
    if (node.left == null && node.right == null) {
      mapping.put(node.c, s);
      return;
    }

    this.generateMapping(node.left, s + "0");
    this.generateMapping(node.right, s + "1");
  }

  public String compress(String s) {
    StringBuilder sb = new StringBuilder();

    for (int i = 0; i < s.length(); i++) {
      sb.append(this.mapping.get(s.charAt(i)));
    }

    return sb.toString();
  }

  public String decompress(String s) {
    StringBuilder sb = new StringBuilder();

    Node node = root;

    for (int i = 0; i < s.length(); i++) {
      if (s.charAt(i) == '0') {
        node = node.left;
      } else {
        node = node.right;
      }

      if (node.left == null && node.right == null) {
        sb.append(node.c);
        node = root;
      }
    }

    return sb.toString();
  }

  private static class Node implements Comparable<Node> {
    private char c;
    private int freq;
    private Node left;
    private Node right;
    
    public Node(char c, int freq) {
      this.c = c;
      this.freq = freq;
      this.left = null;
      this.right = null;
    }

    @Override
    public int compareTo(Node other) {
      return this.freq - other.freq;
    }
  }

  // $ javac HuffmanCoding.java && java Huffman
  public static void main(String[] args) {
    char[] alphabet = {'a', 'b', 'c', 'd', 'e', 'f'};
    int[] freqs = {5, 9, 12, 13, 16, 45};

    HuffmanCoding huffman = new HuffmanCoding(alphabet, freqs);

    for (Character c : huffman.mapping.keySet()) {
      System.out.println(c + ": " + huffman.mapping.get(c));
    }

    String s = huffman.compress("bfacde");

    System.out.println(s);
    System.out.println(huffman.decompress(s));
  }
}