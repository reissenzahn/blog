// WIP (https://github.com/williamfiset/Algorithms/blob/master/src/main/java/com/williamfiset/algorithms/datastructures/hashtable/HashTableSeparateChaining.java)

class Entry<K, V> {
  K key;
  V value;
  int hash;

  public Entry(K key, V value) {
    this.key = key;
    this.value = value;
    this.hash = key.hashCode();
  }

  public boolean equals(Entry<K, V> other) {
    if (this.hash != other.hash) return false;

    return this.key.equals(other.key);
  }

  public String toString() {
    return this.key + " => " + this.value;
  }
}

public class HashTableSeparateChaining<K, V> {

  private static final int DEFAULT_CAPACITY = 3;
  private static final double DEFAULT_LOAD_FACTOR = 0.75;

  private double maxLoadFactor;
  private int capacity;
  private int threshold;
  private int size;
  private LinkedList<Entry<K, V>>[] table;

  public HashTableSeparateChaining() {
    this(DEFAULT_CAPACITY, DEFAULT_LOAD_FACTOR);
  }

  public HashTableSeparateChaining(int capacity) {
    this(capacity, DEFAULT_LOAD_FACTOR);
  }

  public HashTableSeparateChaining(int capacity, int maxLoadFactor) {
    this.maxLoadFactor = maxLoadFactor;
    this.capacity = Math.max(DEFAULT_CAPACITY, capacity);
    this.threshold = (int) (this.capacity * maxLoadFactor);
    this.table = new LinkedList<>[this.capacity];
    this.size = 0;
  }

  public V put(K key, V value) {
    Entry<K, V> entry = new Entry<>(key, value);
    int index = this.normalizeIndex(entry.hash);
    return bucketInsertEntry(index, entry);
  }

  public V get(K key) {
    int index = this.normalizeIndex(key.hashCode());
    Entry<K, V> entry = bucketSeekEntry(index, key);

    if (entry != null) return entry.value;

    return null;
  }

  public V remove(K key) {
    int index = this.normalizeIndex(key.hashCode());
    return this.bucketRemoveEntry(index, key);
  }

  public boolean containsKey(K key) {
    int index = this.normalizeIndex(key.hashCode());
    return bucketSeekEntry(index, key) != null;
  }

  public List<K> keys() {
    List<K> keys = new ArrayList<>(this.size());
    for (LinkedList<Entry<K, V>> chain : table)
      if (chain != null)
        for (Entry<K, V> entry : chain)
          keys.add(entry.key);

    return keys;
  }

  public List<V> values() {
    List<V> values = new ArrayList<>(this.size());
    
    for (LinkedList<Entry<K, V>> chain : table) {
      if (chain != null) {
        for (Entry<K, V> entry : chain) {
          values.add(entry.value);
        }
      }
    }

    return values;
  }

  public void clear() {
    this.table = new LinkedList<>[this.capacity];
    this.size = 0;
  }

  public int size() {
    return this.size;
  }

  public boolean isEmpty() {
    return this.size() == 0;
  }

  public String toString() {
    StringBuilder sb = new StringBuilder();
    
    sb.append("{");
    
    for (int i = 0; i < capacity; i++) {
      if (table[i] == null) continue;
      for (Entry<K, V> entry : table[i]) {
        sb.append(entry + ", ");
      }
    }

    sb.append("}");

    return sb.toString();
  }

  private V bucketSeekEntry(int index, K key) {
    if (key == null) return null;

    LinkedList<Entry<K, V>> chain = this.table[index];

    if (chain == null) return null;

    for (Entry<K, V> entry : chain)
      if (entry.key.equals(key)) return entry;

    return null;
  }

  private V bucketInsertEntry(int index, Entry<K, V> entry) {
    LinkedList<Entry<K, V>> chain = this.table[index];
    
    if (chain == null) {
      this.table[index] = new LinkedList<>();
      chain = this.table[index];
    }

    Entry<K, V> existing = this.bucketSeekEntry(index, entry.key);

    if (existing == null) {
      chain.add(entry);

      this.size++;

      if (this.size > threshold) this.resizeTable();

      return null;
    }

    V value = existing.value;
    existing.value = entry.value;

    return value;
  }

  private V bucketRemoveEntry(int index, K key) {
    Entry<K, V> entry = this.bucketSeekEntry(index, key);

    if (entry != null) {
      LinkedList<Entry<K, V>> chain = this.table[index];
      chain.remove(entry);
      this.size--;
      return entry.value;
    }

    return null;
  }

  private int normalizeIndex(int hash) {
    // hash values are integers so we need to strip the negative sign before taking the modulus
    return (hash & 0x7FFFFFFF) % this.capacity;
  }

  private void resizeTable() {
    this.capacity *= 2;
    this.threshold = (int) (this.capacity * this.maxLoadFactor);

    LinkedList<Entry<K, V>>[] newTable = new LinkedList<>[this.capacity];
    
    for (int i = 0; i < table.length; i++) {
      if (table[i] != null) {
        for (Entry<K, V> entry : table[i]) {
          int index = this.normalizeIndex(entry.hash);
          LinkedList<Entry<K, V>> chain = newTable[index];

          if (chain == null) {
            newTable[index] = new LinkedList<>();
            chain = newTable[index];
          }

          chain.add(entry);
        }

        table[i].clear();
      }
    }

    this.table = newTable;
  }
}

