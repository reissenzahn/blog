
@SuppressWarnings("unchecked")
public class DynamicArray<T> {

  private static int DEFAULT_CAPACITY = 10;

  private T[] array;
  private int length = 0;
  private int capacity = 0;

  public DynamicArray() {
    this(DEFAULT_CAPACITY);
  }

  public DynamicArray(int capacity) {
    if (capacity <= 0)
      throw new IllegalArgumentException();
    
    this.capacity = capacity;
    array = (T[]) new Object[capacity];
  }

  public T get(int index) {
    if (index >= length || index < 0)
      throw new IndexOutOfBoundsException();
    
    return array[index];
  }

  public int indexOf(T value) {
    for (int i = 0; i < length; i++) {
      if (value == null) {
        if (array[i] == null)
          return i;
      } else {
        if (value.equals(array[i]))
          return i;
      }
    }

    return -1;
  }

  public boolean contains(T value) {
    return indexOf(value) != -1;
  }

  public void set(int index, T value) {
    if (index >= length || index < 0)
      throw new IndexOutOfBoundsException();
    
    array[index] = value;
  }

  public void add(T value) {
    if (length + 1 >= capacity) {
      if (capacity == 0)
        capacity = 1;
      else
        capacity *= 2;
      
      T[] newArray = (T[]) new Object[capacity];

      for (int i = 0; i < length; i++)
        newArray[i] = array[i];
      
      array = newArray;
    }

    array[length] = value;
    length++;
  }

  public T removeAt(int index) {
    if (index >= length || index < 0)
      throw new IndexOutOfBoundsException();
    
    T value = array[index];
    
    T[] newArray = (T[]) new Object[length - 1];

    for (int i = 0, j = 0; i < length; i++, j++)
      if (i == index)
        j--;
      else
        newArray[j] = array[i];
  
    array = newArray;
    capacity = --length;

    return value;
  }

  public boolean remove(T value) {
    int index = indexOf(value);

    if (index == -1)
      return false;

    removeAt(index);

    return true;
  }

  public void clear() {
    for (int i = 0; i < length; i++)
      array[i] = null;
    
    length = 0;
  }

  public int size() {
    return length;
  }

  public boolean isEmpty() {
    return size() == 0;
  }
  
  public String toString() {
    if (length == 0)
      return "[]";
    
    StringBuilder builder = new StringBuilder();
    builder.append("[");
    
    for (int i = 0; i < length - 1; i++)
      builder.append(array[i] + ", ");
    
    builder.append(array[length - 1] + "]");
    
    return builder.toString();
  }
}