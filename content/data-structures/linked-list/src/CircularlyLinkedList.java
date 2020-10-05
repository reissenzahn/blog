
public class CircularlyLinkedList<T> {

  public static class Node<T> {

    private T element;
    private Node<T> next;

    public Node(T element, Node<T> next) {
      this.element = element;
      this.next = next;
    }

    public T getElement() {
      return element;
    }

    public void setElement(T element) {
      this.element = element;
    }

    public Node<T> getNext() {
      return next;
    }

    public void setNext(Node<T> next) {
      this.next = next;
    }
  }

  private Node<T> tail = null;
  private int size = 0;

  public CircularlyLinkedList() {
  }

  // first returns the first element in the list
  public T first() {
    if (isEmpty()) return null;
    return tail.getNext().getElement();
  }

  // last returns the last element in the list
  public T last() {
    if (isEmpty()) return null;
    return tail.getElement();
  }

  // rotate moves the first element to the end of the list.
  public void rotate() {
    if (tail != null)
      tail = tail.getNext()
  }


  // size returns the number of elements in the list
  public int size() {
    return size;
  }

  // isEmpty returns true if the list is empty and false otherwise
  public boolean isEmpty() {
    return size == 0;
  }

  public static void main(String[] args) {

  }
}