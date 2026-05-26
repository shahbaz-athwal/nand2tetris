#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Callback used to free stored values (may be NULL) */
typedef void (*FreeFnCb)(void *);

/* Opaque hash table type */
typedef struct HashTable HashTable;

/* Create a new hash table. Pass a free callback to automatically free
   stored values when entries are removed or the table is destroyed.
   Returns NULL on allocation failure. */
HashTable *ht_create(FreeFnCb free_callback);

/* Destroy the hash table and free all resources. The free_callback
   provided at creation will be used to free stored values. */
void ht_destroy(HashTable *ht);

/* Insert a key/value pair. The key parameter is a C-string; the table
   makes an internal copy of the key. Returns true on success. */
bool ht_insert(HashTable *ht, char *key, void *value);

/* Lookup a key; returns the associated value or NULL if not found. */
void *ht_lookup(HashTable *ht, const char *key);

/* Remove a key from the table. Returns true if an entry was removed. */
bool ht_remove(HashTable *ht, const char *key);

/* Number of entries in the table (0 if ht is NULL). */
size_t ht_size(HashTable *ht);

#ifdef __cplusplus
}
#endif

#endif /* HASH_TABLE_H */
