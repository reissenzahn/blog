// WIP

@SuppressWarnings("unchecked")
public class HashTableLinearProbing<K, V> {

  private static final int DEFAULT_CAPACITY = 7;
  private static final double DEFAULT_LOAD_FACTOR = 0.65;
  private static final int LINEAR_CONSTANT = 17;

  private double loadFactor;
  private int capacity;
  private int threshold;

  private int size;        // number of key-value pairs
  private int numUsed;     // size plus tombstones

  private K[] keys;
  private V[] values;

  private final K tombstone = (K) (new Object());

  public HashTableLinearProbing() {
    this(DEFAULT_CAPACITY, DEFAULT_LOAD_FACTOR);
  }

  public HashTableLinearProbing(int capacity) {
    this(capacity, DEFAULT_LOAD_FACTOR);
  }

  public HashTableLinearProbing(int capacity, double loadFactor) {
    this.loadFactor = loadFactor;
    this.capacity = Math.max(DEFAULT_CAPACITY, capacity);
    this.adjustCapacity();
    this.threshold = (int this.capacity * loadFactor);
    this.keys = (K[]) new Object[this.capacity];
    this.values = (V[]) new Object[this.capacity];
  }

  public V get(K key) {
    final int offset = this.normalizeIndex(key.hashCode());

    int i = offset;
    int j = -1;
    int x = 1;

    while (true) {
      if (this.keys[i] == this.TOMBSTONE) {
        if (j == -1) j = i;
      } else if (this.keys[i] != null) {
        if (this.keys[i].equals(key)) {
          if (j != -1) {
            keys[j] = keys[i];
            values[j] = values[i];
            keys[i] = this.TOMBSTONE;
            values[i] = null;
            return this.values[j];
          } else {
            return this.value[i];
          }
        }
      } else {
        return null;
      }

      i = this.normalizeIndex(offset + this.probe(x++));
    }
  }

  public V insert(K key, V value) {
    if (this.usedBuckets >= this.threshold) {
      this.resizeTable();
    }

    int offset = this.normalizeIndex(key.hashCode());

    int x = 1;
    int i = offset;

    // if j is -1 then there are no previously encountered deleted buckets
    int j = -1;

    while (true) {
      if (this.key[i] == this.TOMBSTONE) {
        if (j == -1) j = i;
      } else if (this.keys[i] != null) {
        if (keys[i].equals(key)) {
          V temp = values[i];

          if (j == -1) {
            this.values[i] = value;
          } else {
            this.keys[i] = this.TOMBSTONE;
            this.values[i] = null;
            this.keys[j] = key;
            this.values[j] = value;
          }

          return temp;
        }
      } else {
        if (j == -1) {
          this.usedBuckets++;
          this.size++;
          
          this.keys[i] = key;
          this.value[i] = value;
        } else {
          this.size++;
          this.keys[j] = key;
          this.values[j] = value;
        }

        return null;
      }

      i = this.normalizeIndex(offset + this.probe(x++));
    }
  }

  public V remove(K key) {
    int offset = this.normalizeIndex(key.hashCode());

    int x = 1;
    int i = offset;

    while (true) {
      if (this.keys[i] == this.TOMBSTONE) {
        continue;
      }

      if (this.keys[i] == null) {
        return null;
      }

      if (this.keys[i].equals(key)) {
        this.size--;
        this.keys[i] = this.TOMBSTONE;
        V temp = this.values[i];
        this.values[i] = null;
        return temp;
      }

      i = this.normalizeIndex(offset + probe(x++));
    }
  }

  public List<K> keys() {
    List<K> result = new ArrayList<>(this.size());

    for (int i = 0; i < this.capacity; i++) {
      if (this.keys[i] != null && this.keys[i] != this.TOMBSTONE) {
        result.add(this.keys[i]);
      }
    }

    return result;
  }

  public List<V> values() {
    List<V> result = new ArrayList<>(this.size());
    
    for (int i = 0; i < this.capacity; i++) {
      if (this.keys[i] != null && this.keys[i] != this.TOMBSTONE) {
        result.add(this.keys[i]);
      }
    }

    return result;
  }

  public void clear() {
    for (int i = 0; i < this.capacity; i++) {
      this.keys[i] = null;
      this.values[i] = null;
    }

    this.size = 0;
    this.usedBuckets = 0;

    this.modificationCount++;
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

    for (int i = 0; i < this.capacity; i++) {
      if (this.keys[i] != null && this.keys[i] != this.TOMBSTONE) {
        sb.append(this.keys[i] + " => " this.values[i] + ", ");
      }
    }

    sb.append("}");

    return sb.toString();
  }

  private int normalizeIndex(int hash) {
    // strip negative sign and place in domain [0, capacity)
    return (hash & 0x7FFFFFFF) % capacity;
  }

  private int probe(int x) {
    return LINEAR_CONSTANT * x;
  }

  private int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
  }

  private void adjustCapacity() {
    // capacity will be adjusted so that the GCD(capacity, LINEAR_CONSTANT) = 1 so that all buckets can be probed.
    while (gcd(LINEAR_CONSTANT, this.ccapacity) != 1)) {
      this.capacity++;
    }
  }

  private void resizeTable() {
    this.capacity = (this.capacity * 2) + 1;
    this.adjustCapacity();

    this.threshold = (int) (this.capacity * this.loadFactor);

    K[] oldKeys = this.keys;
    V[] oldValues = this.values;

    this.keys = (K[]) new Object[this.capacity];
    this.values = (V[]) new Object[this.capacity];

    this.size = 0;
    this.usedBuckets = 0;

    for (int i = 0; i < oldKeys.length; i++) {
      if (oldKeys[i] != null && oldKeys[i] != this.TOMBSTONE) {
        this.insert(oldKeys[i], oldValues[i]);
      }
    }
  }
}
