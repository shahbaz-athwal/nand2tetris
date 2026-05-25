#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*FreeFnCb)(void *);

typedef struct {
  char *key;
  void *value;
  struct HMEntry *next;
} HTEntry;

typedef struct {
  HTEntry **buckets;
  size_t size;
  size_t capacity;
} HashTable;

static uint32_t ht_hash(const char *key) {
  uint32_t hash = 2166136261u;
  while (*key) {
    hash ^= (uint8_t)*key++;
    hash *= 16777619u;
  }
  return hash;
}
