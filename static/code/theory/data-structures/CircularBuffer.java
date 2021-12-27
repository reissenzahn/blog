import java.util.*;

@SuppressWarnings("unchecked")
public class CircularBuffer<T> {

  private T[] buffer;
  private int capacity;
  private int size;

  private int read;
  private int write;

  public CircularBuffer(int capacity) {
    this.buffer = (T[]) new Object[capacity];
    this.capacity = capacity;
    this.size = 0;
    this.read = 0;
    this.write = 0;
  }

  public void enqueue(T element) {
    this.buffer[this.write] = element;

    // if an element has been overwritten then increment read
    if (this.size != 0 && this.write == this.read) {
      this.read = (this.read + 1) % this.capacity;
    }

    this.write = (this.write + 1) % this.capacity;

    this.size++;
  }

  public T dequeue() {
    if (this.isEmpty()) return null;

    T temp = this.buffer[this.read];

    this.read = (this.read + 1) % this.capacity;
  
    this.size--;

    return temp;
  }

  public T peek() {
    if (this.isEmpty()) return null;

    return this.buffer[this.read];
  }

  public int size() {
    return this.size;
  }

  public boolean isEmpty() {
    return this.size() == 0;
  }

  public void clear() {
    this.buffer = (T[]) new Object[this.capacity];
    this.size = 0;
    this.read = 0;
    this.write = 0;
  }
  
  public String toString() {
    return "[" + Arrays.toString(this.buffer) + "]";
  }

  public static void main(String[] args) {
    CircularBuffer<String> cb = new CircularBuffer<>(3);

    cb.enqueue("A");
    System.out.println(cb);
    cb.enqueue("B");
    System.out.println(cb);
    cb.enqueue("C");
    System.out.println(cb);
    cb.enqueue("D");
    System.out.println(cb);

    System.out.println(cb.dequeue());
    System.out.println(cb);
    System.out.println(cb.dequeue());

    cb.enqueue("E");
    System.out.println(cb);
    cb.enqueue("F");
    System.out.println(cb);
  }
}