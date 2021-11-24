// STATUS: WIP

public class Queue<T> {

  private Node<T> front;
  private Node<T> back;

  private static class Node<T> {

    private T value;
    private Node<T> next;

    public Node(T value, Node<T> next) {
      this.value = value;
      this.next = next;
    }
  }

  public void enqueue(T value) {
    Node<T> node = new Node<T>(value, null);

    if (back != null) {
      back.next = node;
    }
    
    back = node;

    if (front == null) {
      front = back;
    }
  }

  public T dequeue() {
    if (isEmpty())
      throw new NoSuchElementException();

    T value = front.value;
    front = front.next;
    
    if (isEmpty())
      back = null;

    return value;
  }

  public T peek() {
    if (isEmpty())
      throw new NoSuchElementException();

    return front.value;
  }

  public boolean isEmpty() {
    return front == null;
  }

  public static void main(String[] args) {

  }
}
