public class Queue<T> {

  private Node<T> front;
  private Node<T> back;
  private int size;

  private static class Node<T> {

    private T value;
    private Node<T> next;

    public Node(T value, Node<T> next) {
      this.value = value;
      this.next = next;
    }

    public String toString() {
      return value.toString();
    }
  }

  public Queue() {
    this.front = null;
    this.back = null;
    this.size = 0;
  }

  public void enqueue(T value) {
    Node<T> node = new Node<T>(value, null);

    if (this.back != null) {
      this.back.next = node;
    }
    
    this.back = node;

    if (this.front == null) {
      this.front = this.back;
    }

    this.size++;
  }

  public T dequeue() {
    if (isEmpty()) return null;

    T value = this.front.value;
    this.front = this.front.next;
    
    if (isEmpty()) this.back = null;

    this.size--;

    return value;
  }

  public T peek() {
    if (isEmpty()) return null;

    return this.front.value;
  }

  public int size() {
    return this.front == null;
  }

  public boolean isEmpty() {
    return this.size() == 0;
  }

  public String toString() {
    StringBuilder sb = new StringBuilder();

    sb.append("[");

    Node<T> curr = this.front;
    while (curr != null) {
      sb.append(curr);
      if (curr.next != null) sb.append(", ");
      curr = curr.next;
    }

    sb.append("]");

    return sb.toString();
  }

  public static void main(String[] args) {
    Queue<String> queue = new Queue<>();

    queue.enqueue("A");
    queue.enqueue("B");
    queue.enqueue("C");

    System.out.println(queue);
    System.out.println(queue.size());

    System.out.println(queue.peek());
    System.out.println(queue.dequeue());
    System.out.println(queue.peek());
    System.out.println(queue.dequeue());
    System.out.println(queue.peek());
    System.out.println(queue.dequeue());

    System.out.println(queue);
    System.out.println(queue.size());
  }
}
