
class LinkedListNode<T> {

  public T value;
  public LinkedListNode<T> next;

  public LinkedListNode(T value, LinkedListNode<T> next) {
    this.value = value;
    this.next = next;
  }
}

class LinkedList<T> {

  public LinkedListNode<T> head;
  public LinkedListNode<T> tail;

  public LinkedList() {
  }

  public void prepend(T value) {
    LinkedListNode<T> node = new LinkedListNode<>(value, this.head);

    this.head = node;

    if (this.tail == null) {
      this.tail = node;
    }

    return;
  }
  
  public void append(T value) {
    LinkedListNode<T> newNode = new LinkedListNode<>(value, null);

    if (this.head == null) {
      this.head = newNode;
      this.tail = newNode;

      return;
    }

    this.tail.next = newNode;
    this.tail = newNode;

    return;
  }

  public T delete(T value) {
    if (this.head == null) {
      return null;
    }

    LinkedListNode<T> deletedNode = null;

    while (this.head )
  }

  public static void main(String[] args) {

  }
}
