#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 16;
#define LOAD_FACTOR 0.75f;

typedef void (*FreeFnCb)(void *);

typedef struct HTEntry {
  char *key;
  void *value;
  struct HTEntry *next;
} HTEntry;

typedef struct {
  HTEntry **buckets;
  size_t size;
  size_t capacity;
  FreeFnCb free_callback;
} HashTable;

static uint32_t ht_hash(const char *key) {
  uint32_t hash = 2166136261u;
  while (*key) {
    hash ^= (uint8_t)*key++;
    hash *= 16777619u;
  }
  return hash;
}

HashTable *ht_create(FreeFnCb free_callback) {
  HashTable *ht = malloc(sizeof(HashTable));
  if (!ht)
    return NULL;
  ht->capacity = INITIAL_CAPACITY;
  ht->free_callback = free_callback;
  ht->size = 0;
  ht->buckets = calloc(ht->capacity, sizeof(HTEntry *));
  if (!ht->buckets) {
    free(ht);
    return NULL;
  }

  return ht;
}

void ht_destroy(HashTable *ht) {
  if (!ht)
    return;

  for (int i = 0; i < ht->capacity; i++) {
    HTEntry *node = ht->buckets[i];
    while (node) {
      HTEntry *next = node->next;
      if (ht->free_callback) {
        ht->free_callback(node->value);
      }
      free(node->key);
      free(node);
      node = next;
    }
  }
  free(ht->buckets);
  free(ht);
}

static bool ht_resize(HashTable *ht, size_t new_capacity) {
  HTEntry **new_bucket = calloc(new_capacity, sizeof(HTEntry));
  if (!new_bucket) {
    return false;
  }
  for (int i = 0; i < ht->capacity; i++) {
    HTEntry *node = ht->buckets[i];
    while (node) {
      HTEntry *next = node->next;
      uint32_t idx = ht_hash(node->key) % new_capacity;
      node->next = new_bucket[idx];
      new_bucket[idx] = node;

      node = next;
    }
  }
  free(ht->buckets);
  ht->buckets = new_bucket;
  ht->capacity = new_capacity;
  return true;
}
