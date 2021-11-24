// Status: WIP
// Source: https://www.youtube.com/watch?v=14g1YBvx8Co&list=PLSVu1-lON6LwnTOLZxw3zSn3wPdjO_e_R&index=2

public class DoublyLinkedList<T> {

  private int size;          // number of elements in list
  private Node<T> sentinel;  // sentinel node

  public DoublyLinkedList() {
    Node<T> newNode = new Node<>(null);
    newNode.next = newNode;
    newNode.prev = newNode;

    this.sentinel = newNode;
    this.size = 0;
  }

  // insert value into list after node
  public void insert(Node<T> node, T value) {
    Node<T> newNode = new Node(value);
    newNode.next = node.next;
    newNode.prev = node;
    
    node.next.prev = newNode;
    node.next = newNode;

    this.size++;
  }

  public void prepend(T value) {
    this.insert(this.sentinel, value);
  }

  public void append(T value) {
    this.insert(this.sentinel.prev, value);
  }

  // delete node
  public void delete(Node<T> node) {
    if (node == this.sentinel) return;

    node.next.prev = node.prev;
    node.prev.next = node.next;
    
    this.size--;
  }

  public void concat(DoublyLinkedList<T> other) {
    this.sentinel.prev.next = other.sentinel.next;
    other.sentinel.next.prev = this.sentinel.prev;
    other.sentinel.prev.next = this.sentinel;
    this.sentinel.prev = other.sentinel.prev;
    this.size += other.size;
    other.clear();
  }

  public int size() {
    return this.size;
  }

  public boolean isEmpty() {
    return this.size() == 0;
  }

  public void clear() {
    Node<T> newNode = new Node<>(null);
    newNode.next = newNode;
    newNode.prev = newNode;

    this.sentinel = newNode;
    this.size = 0;
  }

  public String toString() {
    StringBuilder builder = new StringBuilder();
    builder.append("[ ");
    
    Node<T> curr = this.sentinel.next;
    while (curr != this.sentinel) {
      builder.append(curr.toString());
      builder.append(" ");
      curr = curr.next;
    }
    
    builder.append("]");

    return builder.toString();
  }

  private static class Node<T> {

    private T value;
    private Node<T> prev;  // previous node in sequence
    private Node<T> next;  // next node in sequence

    public Node(T value) {
      this.value = value;
    }

    public String toString() {
      return value.toString();
    }
  }

  public static void main(String[] args) {
    DoublyLinkedList<String> l = new DoublyLinkedList<>();
    l.append("B");
    l.prepend("A");
    l.append("C");
    System.out.println(l);  // [ A B C ]

    DoublyLinkedList<String> m = new DoublyLinkedList<>();
    m.append("E");
    m.prepend("D");
    m.append("F");
    System.out.println(m);  // [ D E F ]

    l.concat(m);
    System.out.println(l);  // [ A B C D E F ]
  }
}

