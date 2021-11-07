import java.util.Collections;
import java.util.Arrays;
import java.util.List;

@SuppressWarnings("unchecked")
class BinaryHeap<T extends Comparable<T>> {

  private static final int DEFAULT_CAPACITY = 10;

  private T[] array;
  private int capacity;
  private int size = 0;

  public BinaryHeap() {
    this(DEFAULT_CAPACITY);
  }

  public BinaryHeap(int capacity) {
    this.capacity = capacity;
    array = (T[]) new Comparable[capacity];
  }

  public void insert(T value) {
    resize();

    // insert in leftmost position of last level
    array[size] = value;
    size++;

    // percolate up
    int index = size - 1;

    // while it has a parent and is less than its parent
    while (hasParent(index) && array[index].compareTo(parent(index)) < 0) {

      // swap with its parent
      swap(getParentIndex(index), index);
      
      // consider the new parent
      index = getParentIndex(index);
    }
  }

  public T extract() {
    if (size == 0)
      throw new IllegalStateException();
    
    // extract root and replace it with the last node
    T value = array[0];
    array[0] = array[size - 1];
    size--;
    
    // percolate down from root
    int index = 0;

    // the node cannot have a right child without a left child
    while (hasLeftChild(index)) {
      
      // find the smaller index
      int smallerChildIndex = getLeftChildIndex(index);
      if (hasRightChild(index) && rightChild(index).compareTo(leftChild(index)) < 0) {
        smallerChildIndex = getRightChildIndex(index);
      }

      // stop once neither child is smaller
      if (array[index].compareTo(array[smallerChildIndex]) < 0) {
        break;
      }

      // swap the node with the smaller child
      swap(index, smallerChildIndex);
      
      // consider new child
      index = smallerChildIndex;
    }
    
    return value;
  }

  private int getLeftChildIndex(int index) {
    return 2 * index + 1;
  }

  private int getRightChildIndex(int index) {
    return 2 * index + 2;
  }

  private int getParentIndex(int index) {
    // integer division always truncates
    return (index - 1) / 2;
  }

  private boolean hasLeftChild(int index) {
    return getLeftChildIndex(index) < size;
  }

  private boolean hasRightChild(int index) {
    return getRightChildIndex(index) < size;
  }

  private boolean hasParent(int index) {
    return getParentIndex(index) >= 0;
  }

  private T leftChild(int index) {
    return array[getLeftChildIndex(index)];
  }

  private T rightChild(int index) {
    return array[getRightChildIndex(index)];
  }

  private T parent(int index) {
    return array[getParentIndex(index)];
  }

  private void swap(int indexOne, int indexTwo) {
    T temp = array[indexOne];
    array[indexOne] = array[indexTwo];
    array[indexTwo] = temp;
  }

  private void resize() {
    if (size == capacity) {
      array = Arrays.copyOf(array, capacity * 2);
      capacity *= 2;
    }
  }
}

public class Example {

  public static void main(String[] args) {
    BinaryHeap<Integer> heap = new BinaryHeap<Integer>();

    List<Integer> keys = Arrays.asList(2, 3, 7, 4, 6, 9, 13, 14, 17, 10, 11, 16);
    
    Collections.shuffle(keys);

    for (Integer key : keys)
      heap.insert(key);

    for (Integer key : keys)
      System.out.println(heap.extract());
  }
}
