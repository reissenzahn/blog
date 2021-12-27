import java.util.*;

public class BinaryHeap<T extends Comparable<T>> {

  private ArrayList<T> items;

  public BinaryHeap() {
    this.items = new ArrayList<>();
  }

  public void insert(T item) {
    this.items.add(item);
    this.heapifyUp();
  }

  public T findMax() {
    if (this.items.size() == 0) return null;

    return this.items.get(0);
  }

  public T deleteMax() {
    if (this.items.size() == 0) return null;
    if (this.items.size() == 1) return this.items.remove(0);

    T item = this.items.get(0);
    this.items.set(0, items.remove(items.size() - 1));
    this.heapifyDown(0);

    return item;
  }

  public void buildHeap(T[] array) {
    this.items = new ArrayList<>();

    for (int i = 0; i < array.length; i++) {
      items.add(array[i]);
    }

    for (int k = items.size()/2; k >= 0; k--) {
      this.heapifyDown(k);
    }
  }

  private void heapifyUp() {
    int k = this.items.size() - 1;

    // continue until we reach the top of the heap;
    while (k > 0) {
      int parentIndex = this.getParentIndex(k);

      T item = this.items.get(k);
      T parent = this.items.get(parentIndex);

      // if item is less than or equal to parent then stop
      if (item.compareTo(parent) <= 0) {
        break;
      }

      this.swap(k, parentIndex);

      k = parentIndex;
    }
  }

  private void heapifyDown(int start) {
    int k = start;
    int leftChildIndex = this.getLeftChildIndex(k);

    // continue until we run out of left children (and thus also right children)
    while (leftChildIndex < this.items.size()) {
      int rightChildIndex = this.getRightChildIndex(k);
      int maxChildIndex = leftChildIndex;

      // if there is a right child check if it is greater
      if (rightChildIndex < this.items.size()) {
        T leftChild = this.items.get(leftChildIndex);
        T rightChild = this.items.get(rightChildIndex);

        if (rightChild.compareTo(leftChild) > 0) {
          maxChildIndex = rightChildIndex;
        }
      }

      T item = items.get(k);
      T maxChild = items.get(maxChildIndex);

      // if item is greater than or equal to larger child then stop
      if (item.compareTo(maxChild) >= 0) {
        break;
      }

      this.swap(k, maxChildIndex);

      k = maxChildIndex;
      leftChildIndex = this.getLeftChildIndex(k);
    }
  }

  private void swap(int i, int j) {
    T temp = this.items.get(i);
    this.items.set(i, this.items.get(j));
    this.items.set(j, temp);
  }

  private int getLeftChildIndex(int i) {
    return 2 * i + 1;
  }

  private int getRightChildIndex(int i) {
    return 2 * i + 2;
  }

  private int getParentIndex(int i) {
    return (i - 1) / 2;
  }

  public int size() {
    return this.items.size();
  }

  public boolean isEmpty() {
    return this.items.isEmpty();
  }

  public String toString() {
    return this.items.toString();
  }

  public static void main(String[] args) {
    BinaryHeap<Integer> heap = new BinaryHeap<>();
    heap.insert(15);
    heap.insert(75);
    heap.insert(18);
    heap.insert(36);
    heap.insert(89);
    heap.insert(63);
    heap.insert(90);
    heap.insert(65);
    heap.insert(21);
    heap.insert(70);
    System.out.println(heap);  // [90, 75, 89, 65, 70, 18, 63, 15, 21, 36]
  
    System.out.println(heap.deleteMax());  // 90
    System.out.println(heap.deleteMax());  // 89
    System.out.println(heap.deleteMax());  // 75
    System.out.println(heap.deleteMax());  // 70
    System.out.println(heap.deleteMax());  // 65
    System.out.println(heap.deleteMax());  // 63
    System.out.println(heap.deleteMax());  // 36
    System.out.println(heap.deleteMax());  // 21
    System.out.println(heap.deleteMax());  // 18
    System.out.println(heap.deleteMax());  // 15

    heap.buildHeap(new Integer[]{15, 75, 18, 36, 89, 63, 90, 65, 21, 70});
    System.out.println(heap);  // [90, 89, 63, 65, 75, 15, 18, 36, 21, 70]
  }
}