#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75f

typedef void (*FreeFnCb)(void *);

typedef struct HTEntry {
  char *key;
  void *value;
  struct HTEntry *next;
} HTEntry;

typedef struct HashTable {
  HTEntry **buckets;
  size_t size;
  size_t capacity;
  FreeFnCb free_callback;
} HashTable;

static char *duplicate_string(const char *src) {
  size_t len = strlen(src) + 1;
  char *dst = malloc(len);
  if (dst) {
    memcpy(dst, src, len);
  }
  return dst;
}

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

  for (size_t i = 0; i < ht->capacity; i++) {
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
  HTEntry **new_bucket = calloc(new_capacity, sizeof(HTEntry *));
  if (!new_bucket) {
    return false;
  }
  for (size_t i = 0; i < ht->capacity; i++) {
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

bool ht_insert(HashTable *ht, char *key, void *value) {
  if (!ht) {
    return false;
  }
  if (ht->size > 0 && (float)ht->size / ht->capacity >= LOAD_FACTOR) {
    if (!ht_resize(ht, ht->capacity * 2)) {
      return false;
    }
  }
  uint32_t idx = ht_hash(key) % ht->capacity;
  HTEntry *entry = ht->buckets[idx];

  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      if (ht->free_callback && entry->value != value) {
        ht->free_callback(entry->value);
      }
      entry->value = value;
      return true;
    }
    entry = entry->next;
  }

  HTEntry *new_entry = malloc(sizeof(HTEntry));
  if (!new_entry)
    return false;

  new_entry->key = duplicate_string(key);
  if (!new_entry->key) {
    free(new_entry);
    return false;
  }

  new_entry->value = value;
  new_entry->next = ht->buckets[idx];
  ht->buckets[idx] = new_entry;
  ht->size++;

  return true;
}

void *ht_lookup(HashTable *ht, const char *key) {
  if (!ht || !key)
    return NULL;

  uint32_t idx = ht_hash(key) % ht->capacity;
  HTEntry *entry = ht->buckets[idx];

  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      return entry->value;
    }
    entry = entry->next;
  }
  return NULL;
}

bool ht_remove(HashTable *ht, const char *key) {
  if (!ht || !key)
    return false;

  uint32_t idx = ht_hash(key) % ht->capacity;
  HTEntry *entry = ht->buckets[idx];
  HTEntry *prev = NULL;

  while (entry) {
    if (strcmp(entry->key, key) == 0) {
      if (prev) {
        prev->next = entry->next;
      } else {
        ht->buckets[idx] = entry->next;
      }

      if (ht->free_callback) {
        ht->free_callback(entry->value);
      }
      free(entry->key);
      free(entry);
      ht->size--;
      return true;
    }
    prev = entry;
    entry = entry->next;
  }
  return false;
}

size_t ht_size(HashTable *ht) { return ht ? ht->size : 0; }
