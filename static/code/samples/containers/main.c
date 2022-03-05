#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_CAPACITY        3
#define DEFAULT_MAX_LOAD_FACTOR 0.75

struct dictionary_entry {
  void *key;
  void *value;
  struct dictionary_entry *next;
};

struct dictionary {
  double max_load_factor;
  int threshold;
  int capacity;
  int size;
  int (*hash)(void *key);
  int (*compare)(void *key1, void *key2);
  struct dictionary_entry **table;
};

int hash_pjw(void *key) {
  int ret = 0;
  char *ptr = key;

  while (*ptr != '\0') {
    ret = (ret << 4) + (*ptr);
    
    int tmp = ret & 0xf0000000;

    if (tmp) {
      ret = ret ^ (tmp >> 24);
      ret = ret ^ tmp;
    }

    ptr++;
  }

  return ret;  // TODO: take mod now or later?
}

int compare_strcmp(void *key1, void *key2) {
  return strcmp((char *) key1, (char *) key2);
}

static void dictionary_resize_table(struct dictionary *this) {
  int old_capacity = this->capacity;

  this->capacity *= 2;
  this->threshold = (int) (this->capacity * this->max_load_factor);

  struct dictionary_entry **new_table = malloc(sizeof(struct dictionary_entry *) * this->capacity);

  for (int i = 0; i < old_capacity; i++) {
    if (this->table[i] != NULL) {
      struct dictionary_entry *entry = this->table[i];

      while (entry != NULL) {
        int index = this->hash(entry->key) % this->capacity;

        if (new_table[index] != NULL) {
          entry->next = new_table[index];
          new_table[index] = entry;
        } else {
          new_table[index] = entry;
        }

        entry = entry->next;
      }
    }
  }

  free(this->table);

  this->table = new_table;
}

int dictionary_init(struct dictionary *this, int capacity, int max_load_factor, int (*hash)(void *key), int (*compare)(void *key1, void *key2)) {
  capacity = capacity > DEFAULT_CAPACITY ? capacity : DEFAULT_CAPACITY;

  struct dictionary_entry **table = malloc(sizeof(struct dictionary_entry *) * this->capacity);
  if (table == NULL) {
    return -1;
  }

  this->capacity = capacity;
  this->max_load_factor = max_load_factor;
  this->threshold = (int) (this->capacity * max_load_factor);
  this->table = table;
  this->size = 0;
  this->hash = hash;
  this->compare = compare;

  return 0;
}

int dictionary_init_default(struct dictionary *this) {
  return dictionary_init(this, DEFAULT_CAPACITY, DEFAULT_MAX_LOAD_FACTOR, hash_pjw, compare_strcmp);
}

int dictionary_put(struct dictionary *this, void *key, void *value) {
  int index = this->hash(key) % this->capacity;

  if (this->table[index] == NULL) {
    struct dictionary_entry *new_entry = malloc(sizeof(struct dictionary_entry));
    if (new_entry == NULL) {
      return -1;
    }
    
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = NULL;

    this->table[index] = new_entry;
  } else {
    struct dictionary_entry *entry = this->table[index];

    while (entry != NULL) {
      if (this->compare(entry->key, key) == 0) {
        entry->value = value;
        return 0;
      }

      entry = entry->next;
    }

    struct dictionary_entry *new_entry = malloc(sizeof(struct dictionary_entry));
    if (new_entry == NULL) {
      return -1;
    }
      
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = this->table[index];

    this->table[index] = new_entry;
  }

  this->size++;

  if (this->size > this->threshold) {
    dictionary_resize_table(this);
  }

  return 0;
}

int dictionary_get(struct dictionary *this, void *key, void **value) {
  int index = this->hash(key) % this->capacity;

  struct dictionary_entry *entry = this->table[index];

  while (entry != NULL) {
    if (this->compare(key, entry->key) == 0) {
      *value = entry->value;
      return 0;
    }
    
    entry = entry->next;
  }

  return -1;
}

// int dictionary_remove(struct dictionary *this, void *key) {
//   int index = this->hash(key) % this->capacity;

//   struct dictionary_entry *prev = NULL;
//   struct dictionary_entry *entry = this->table[index];

//   while (entry != NULL) {
//     if (this->compare(key, entry->key) == 0) {
//       if (entry == this->table[index]) {
//         this->table[index] = NULL;
//       } else {
//         prev->next = entry->next;
//       }

//       this->size--;

//       return 0;
//     }

//     prev = entry;
//     entry = entry->next;
//   }



//   return -1;
// }

// void dictionary_print(struct dictionary *this) {
//   printf("{ ");

//   for (int i = 0; i < this->capacity; i++) {
//     struct dictionary_entry *entry = this->table[i];
    
//     if (entry != NULL) {
//       while (entry != NULL) {
//         printf("%s => ", (char *) entry->key);  // TODO: abstract printing string
//         printf("%s ", (char *) entry->value);   // TODO: abstract printing string
//       }
//     }
//   }

//   printf("}\n");
// }

int main(void) {
  struct dictionary dict;

  dictionary_init_default(&dict);

  dictionary_put(&dict, "key1", "value1");
  dictionary_put(&dict, "key2", "value2");
  dictionary_put(&dict, "key3", "value3");
  dictionary_put(&dict, "key4", "value4");
  dictionary_put(&dict, "key5", "value5");
  dictionary_put(&dict, "key6", "value6");
  dictionary_put(&dict, "key7", "value7");

  return 0;
}

