#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "tGis_CoreCfg.h"


typedef struct TGIS_CORE_API _HASHMAP_ENTRY
{
	void* key;
	void* val;
}HASHMAP_ENTRY;

typedef struct TGIS_CORE_API _HASH_NODE
{
	struct _HASH_NODE* next;
	unsigned char    data[1];
}HASH_NODE;

typedef HASH_NODE* HASH_NODE_PTR;

typedef struct TGIS_CORE_API _HASHMAP_ITER
{
	size_t array_index;
	size_t block_index;
	HASH_NODE_PTR  val_node;
}HASHMAP_ITER;

typedef struct TGIS_CORE_API _HASHMAP
{
	size_t size;
	size_t capacity;
	size_t key_bytes;
	size_t key_offset;
	size_t val_bytes;
	size_t node_bytes;
	size_t array_size;
	HASH_NODE_PTR**  hash_array;

	void*(*alloc_mem)(size_t size);
	void(*free_mem)(void* mem);
	size_t(*hash_code)(const void * key);
	int(*equal_key)(const void * a, const void * b);
	void(*assign_key)(void * a, const void * b);
	void(*assign_val)(void * a, const void * b);	
}HASHMAP;



TGIS_CORE_API void hashmap_init(HASHMAP* hashmap,
	size_t capacity,
	size_t key_bytes,
	size_t val_bytes,
	void*(*alloc_mem)(size_t size),
	void(*free_mem)(void* mem),
	size_t(*hash_code)(const void * key),
	int(*equal_key)(const void * a, const void * b),
	void(*assign_key)(void * a, const void * b),
	void(*assign_val)(void * a, const void * b)	
	);

TGIS_CORE_API int hashmap_exists(HASHMAP* hashmap, const void* key);

TGIS_CORE_API void* hashmap_set(HASHMAP* hashmap, const void* key, const void* val);

TGIS_CORE_API void* hashmap_get(HASHMAP* hashmap, const void* key, void* val);

TGIS_CORE_API void  hashmap_del(HASHMAP* hashmap, const void* key);

TGIS_CORE_API void  hashmap_first(HASHMAP* hashmap, HASHMAP_ITER* iter);

TGIS_CORE_API HASHMAP_ENTRY hashmap_at(HASHMAP* hashmap, HASHMAP_ITER* iter);

TGIS_CORE_API void  hashmap_next(HASHMAP* hashmap, HASHMAP_ITER* iter);

TGIS_CORE_API void hashmap_release(HASHMAP* hashmap);


// Equal functions

inline int _8bit_equals(const void *v1, const void *v2) {
	unsigned char a = *(const unsigned char *)v1;
	unsigned char b = *(const unsigned char *)v2;
	return a == b ? 1 : 0;
}

inline int _16bit_equals(const void *v1, const void *v2) {
	unsigned short a = *(const unsigned short *)v1;
	unsigned short b = *(const unsigned short *)v2;
	return a == b ? 1 : 0;
}

inline int _32bit_equals(const void *v1, const void *v2) {
	unsigned int a = *(const unsigned int *)v1;
	unsigned int b = *(const unsigned int *)v2;
	return a == b ? 1 : 0;
}

inline int _64bit_equals(const void *v1, const void *v2) {
	unsigned long long a = *(const unsigned long long *)v1;
	unsigned long long b = *(const unsigned long long *)v2;
	return a == b ? 1 : 0;
}

inline int _str_equals(const void *a, const void *b) {
	const char *str1 = *(const char * const *)a;
	const char *str2 = *(const char * const *)b;

	return strcmp(str1, str2) == 0 ? 1 : 0;
}

inline int _ptr_equals(const void *v1, const void *v2) {
	const void *a = *(const void * const *)v1;
	const void *b = *(const void * const *)v2;
	return a == b ? 1 : 0;
}

// Hash functions 

inline size_t _uint32_hash(const void* key_) {
	unsigned int key = *(const unsigned int *)key_;
	key += ~(key << 16);
	key ^= (key >> 5);
	key += (key << 3);
	key ^= (key >> 13);
	key += ~(key << 9);
	key ^= (key >> 17);
	return key;
}

inline size_t _uint64_hash(const void* key_) {
	unsigned long long key = *(const unsigned long long*)key_;
	key += ~(key << 34);
	key ^= (key >> 29);
	key += ~(key << 11);
	key ^= (key >> 14);
	key += ~(key << 7);
	key ^= (key >> 28);
	key += ~(key << 26);
	return (size_t)key;
}

inline size_t _str_hash(const void *key_) {
	const char *key = *(const char * const *)key_;
	size_t hash = 0;
	char ch;
	while ((ch = *key++) != 0) {
		hash += (size_t)ch;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

#if UINTPTR_MAX == UINT32_MAX
#define _ptr_hash _uint32_hash
#elif UINTPTR_MAX == UINT64_MAX
#define _ptr_hash _uint64_hash
#else
#define _ptr_hash 0
#endif

inline size_t _hash_combine(size_t hash_a, size_t hash_b) {
	return hash_a ^ (hash_b + 0x9e3779b9 + (hash_a << 6) + (hash_a >> 2));
}


#endif
