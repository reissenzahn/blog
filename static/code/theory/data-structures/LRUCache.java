import java.util.*;

public class LRUCache<K, V> {

  private static class Node<T, U> {
    
    T key;
    U value;
    Node<T, U> prev;
    Node<T, U> next;

    public Node(T key, U value) {
      this.key = key;
      this.value = value;
    }
  }

  private int capacity;
  private int size;
  private Map<K, Node<K, V>> cache;
  private Node<K, V> sentinel;  // sentinel node

  public LRUCache(int capacity) {
    this.capacity = capacity;
    this.size = 0;

    Node<K, V> newNode = new Node<>(null, null);
    newNode.next = newNode;
    newNode.prev = newNode;
    this.sentinel = newNode;
    
    this.cache = new HashMap<K, Node<K, V>>();
  }

  public V get(K key) {
    Node<K, V> node = this.cache.get(key);

    if (node == null) {
      return null;
    }

    this.makeMRU(node);
    
    return node.value;
  }

  public void put(K key, V value) {
    Node<K, V> node = this.cache.get(key);

    if (node == null) {
      Node<K, V> newNode = new Node<>(key, value);

      this.cache.put(key, newNode);
      this.prepend(newNode);

      if (this.cache.size() > capacity) {
        Node<K, V> tail = this.evictLRU();
        this.cache.remove(tail.key);
      }
    } else {
      node.value = value;
      this.makeMRU(node);
    }
  }

  // insert a new head node
  private void prepend(Node<K, V> node) {
    node.next = this.sentinel.next;
    node.prev = this.sentinel;
    
    this.sentinel.next.prev = node;
    this.sentinel.next = node;
  }

  // remove a node
  private void remove(Node<K, V> node) {
    if (node == this.sentinel) return;

    node.next.prev = node.prev;
    node.prev.next = node.next;
  }

  // move a node to the head
  private void makeMRU(Node<K, V> node) {
    this.remove(node);
    this.prepend(node);
  }

  // remove and return tail
  private Node<K, V> evictLRU() {
    Node<K, V> node = this.sentinel.prev;
    this.remove(node);
    return node;
  }


  public static void main(String[] args) {
    LRUCache<Integer, Integer> cache = new LRUCache<>(2);

    cache.put(1, 1);
    cache.put(2, 2);
    cache.get(1);
    cache.put(3, 3);
    cache.get(2);
    cache.put(4, 4);
    cache.get(1);
    cache.get(3);
    cache.get(4);
  }
}
