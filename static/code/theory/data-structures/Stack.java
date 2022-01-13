
public class Stack<T> {

  private Node<T> top;

  public void push(T value) {
    Node<T> node = new Node<>(value);
    node.next = top;
    top = node;
  }

  public T pop() {
    if (isEmpty()) {
      return null;
    }

    T value = top.value;
    top = top.next;

    return value;
  }

  public T peek() {
    if (isEmpty()) {
      return null;
    }

    return top.value;
  }

  public boolean isEmpty() {
    return top == null;
  }

  private static class Node<T> {

    private T value;
    private Node<T> next;

    public Node(T value) {
      this.value = value;
    }
  }

  // javac Stack.java && java Stack
  public static void main(String[] args) {
    Stack<String> stack = new Stack<>();

    stack.push("C");
    stack.push("B");
    stack.push("A");

    System.out.println(stack.peek());  // A
    System.out.println(stack.peek());  // A

    System.out.println(stack.pop());  // A
    System.out.println(stack.pop());  // B
    System.out.println(stack.pop());  // C
  }
}
