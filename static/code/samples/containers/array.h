#ifndef ARRAY_H
#define ARRAY_H

#define DEFAULT_CAPACITY 10

struct array {
  void *data;
  int capacity;
  int size;
};

int array_init(struct array *this, int capacity) {
  if (capacity <= 0) {
    return -1;
  }
  
  void *data = malloc(sizeof(void *) * capacity);
  if (data == NULL) {
    return -1;
  }

  this->data = data;
  this->capacity = capacity;
  this->size = 0;

  return 0;
}

int array_get(struct array *this, int index, void **value) {
  if (index >= this->length || index < 0) {
    return -1;
  }

  *value = this->data[index];

  return 0;
}

int array_add(struct array *this, void *value) {
  if (this->length + 1 >= this->capacity) {
    if (this->capacity == 0) {
      this->capacity = 1;
    } else {
      this->capacity *= 2;
    }

    void *data = realloc(this->data, sizeof(void *) * this->capacity);
    if (data == NULL) {
      return -1;
    }

    this->data = data;
  }

  this->data[this->length] = value;
  this->length++;
}

int array_set(struct array *this, int index, void *value) {
  if (index >= this->length || index < 0) {
    return -1;
  }

  // TODO: free the old value at this->data[index]

  this->data[index] = value;
}

int array_clear(struct array *this) {
  for (int i = 0; i < this->length; i++) {
    // TODO: free the old value at this->data[index]

    this->data[i] = NULL;
  }

  this->length = 0;
}

#endif


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

