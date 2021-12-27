
public class Stack<T> {

  private Node<T> top;

  public void push(T value) {
    top = new Node<>(value, top);
  }

  public T pop() {
    if (isEmpty())
      throw new EmptyStackException();

    T value = top.value;
    top = top.next;

    return value;
  }

  public T peek() {
    if (isEmpty())
      throw new EmptyStackException();

    return top.value;
  }

  public boolean isEmpty() {
    return top == null;
  }

  private static class Node<T> {

    private T value;
    private Node<T> next;

    public Node(T value, Node<T> next) {
      this.value = value;
      this.next = next;
    }
  }
}
