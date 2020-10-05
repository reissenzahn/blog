
public class SinglyLinkedList<T> {

  private static class Node<T> {

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

  private Node<T> head = null;
  private Node<T> tail = null;
  private int size = 0;

  public SinglyLinkedList() {
  }

  // addFirst adds a new element to the front of the list
  public void addFirst(T element) {
    head = new Node<>(element, head);
    if (isEmpty())
      tail = head;
    size++;
  }

  // addLast adds a new element to the end of the list
  public void addLast(T element) {
    Node<T> node = new Node<>(element, null);
    if (isEmpty())
      head = node;
    else
      tail.setNext(node);
    tail = node;
    size++;
  }

  // removeFirst removes and returns the first element of the list
  public T removeFirst() {
    if (isEmpty()) return null;
    T element = head.getElement();
    head = head.getNext();
    size--;
    if (size == 0)
      tail = null;
    return element;
  }

  // first returns the first element in the list
  public T first() {
    if (isEmpty()) return null;
    return head.getElement();
  }

  // last returns the last element in the list
  public T last() {
    if (isEmpty()) return null;
    return tail.getElment();
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
