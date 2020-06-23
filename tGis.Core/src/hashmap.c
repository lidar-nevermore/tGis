#include "hashmap.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define ALIGN_MEM(size, boundary)     (((size) + ((boundary) - 1)) & ~((boundary) - 1)) 
#define HASH_BLOCK_SIZE           2002U
#define OCCUPATION_RATE           0.75
#define EXPECT_ARRAY_SIZE         32



void hashmap_init(HASHMAP * hashmap,
	size_t capacity,
	size_t key_bytes,
	size_t val_bytes,
	void*(*alloc_mem)(size_t size),
	void(*free_mem)(void* mem),
	size_t(*hash_code)(const void * key),
	int(*equal_key)(const void * a, const void * b),
	void(*assign_key)(void * a, const void * b),
	void(*assign_val)(void * a, const void * b)
	)
{
	hashmap->size = 0;
	hashmap->array_size = (capacity + HASH_BLOCK_SIZE - 1) / HASH_BLOCK_SIZE;
	hashmap->capacity = HASH_BLOCK_SIZE*hashmap->array_size;
	hashmap->key_bytes = key_bytes;
	hashmap->key_offset = ALIGN_MEM(key_bytes, sizeof(int));
	hashmap->val_bytes = val_bytes;
	hashmap->node_bytes = ALIGN_MEM(sizeof(HASH_NODE) - 1 + hashmap->key_offset + val_bytes, sizeof(int));
	hashmap->hash_array = (HASH_NODE_PTR**)alloc_mem(hashmap->array_size*sizeof(HASH_NODE_PTR*));

	memset(hashmap->hash_array, 0, hashmap->array_size * sizeof(HASH_NODE_PTR*));

	hashmap->alloc_mem = alloc_mem;
	hashmap->free_mem = free_mem;
	hashmap->hash_code = hash_code;
	hashmap->equal_key = equal_key;
	hashmap->assign_key = assign_key;
	hashmap->assign_val = assign_val;	
}

int hashmap_exists(HASHMAP * hashmap, const void * key)
{
	size_t hcode = hashmap->hash_code(key)%hashmap->capacity;
	size_t aindex = hcode / HASH_BLOCK_SIZE;
	size_t bindex = hcode % HASH_BLOCK_SIZE;
	HASH_NODE_PTR* bnode = NULL;
	HASH_NODE_PTR  vnode = NULL;

	bnode = hashmap->hash_array[aindex];
	if (bnode != NULL)
	{
		vnode = bnode[bindex];
		while (vnode != NULL)
		{
			if (0 < hashmap->equal_key(key, (void*)(vnode->data)))
				return 1;
			vnode = vnode->next;
		}
	}

	return 0;
}

void hashmap_resize(HASHMAP * hashmap)
{
	size_t aindex = 0;
	size_t bindex = 0;
	HASH_NODE_PTR* bnode = NULL;
	HASH_NODE_PTR  vnode1 = NULL;
	HASH_NODE_PTR  vnode2 = NULL;
	size_t hcode = 0;
	size_t aindex_n = 0;
	size_t bindex_n = 0;
	HASH_NODE_PTR* bnode_n = NULL;
	HASH_NODE_PTR  vnode_n = NULL;
	size_t array_size = hashmap->array_size; 
	HASH_NODE_PTR** hash_array = hashmap->hash_array;
	size_t capacity = hashmap->capacity;

	hashmap->array_size = (size_t)ceil(array_size * 2.0 / (1 + exp((double)array_size - EXPECT_ARRAY_SIZE)));
	hashmap->hash_array = (HASH_NODE_PTR**)hashmap->alloc_mem(hashmap->array_size * sizeof(HASH_NODE_PTR*));
	hashmap->capacity = HASH_BLOCK_SIZE*hashmap->array_size;
	memset(hashmap->hash_array, 0, hashmap->array_size * sizeof(HASH_NODE_PTR*));
	memcpy(hashmap->hash_array, hash_array, array_size * sizeof(HASH_NODE_PTR*));
	hashmap->free_mem(hash_array);

	hash_array = hashmap->hash_array;
	capacity = hashmap->capacity;

	for (aindex = 0; aindex < array_size; aindex++)
	{
		bnode = hash_array[aindex];
		if (bnode != NULL)
		{
			for (bindex = 0; bindex < HASH_BLOCK_SIZE; bindex++)
			{
				vnode1 = NULL;
				vnode2 = bnode[bindex];
				while (vnode2 != NULL)
				{
					hcode = hashmap->hash_code((void*)(vnode2->data)) % capacity;
					aindex_n = hcode / HASH_BLOCK_SIZE;
					bindex_n = hcode % HASH_BLOCK_SIZE;

					if (aindex_n == aindex && bindex_n == bindex)
					{
						vnode1 = vnode2;
						vnode2 = vnode2->next;
					}
					else
					{
						bnode_n = hash_array[aindex_n];
						if (bnode_n == NULL)
						{
							bnode_n = (HASH_NODE_PTR*)hashmap->alloc_mem(HASH_BLOCK_SIZE * sizeof(HASH_NODE_PTR));
							memset(bnode_n, 0, HASH_BLOCK_SIZE * sizeof(HASH_NODE_PTR));
							hash_array[aindex_n] = bnode_n;
						}
						vnode_n = vnode2;
						vnode2 = vnode_n->next;
						if (vnode1 != NULL)
							vnode1->next = vnode2;
						else
							bnode[bindex] = vnode2;
						vnode_n->next = bnode_n[bindex_n];
						bnode_n[bindex_n] = vnode_n;
					}
				}
			}
		}
	}
}


void * hashmap_set(HASHMAP * hashmap, const void * key, const void * val)
{
	size_t hcode = 0U;
	size_t aindex = 0U;
	size_t bindex = 0U;
	HASH_NODE_PTR* bnode = NULL;
	HASH_NODE_PTR  vnode = NULL;
	void* val_ptr = NULL;

	//if (((double)hashmap->size) / hashmap->capacity > OCCUPATION_RATE)
	if (hashmap->size >= hashmap->capacity)
	{
		hashmap_resize(hashmap);
	}

	hcode = hashmap->hash_code(key) % hashmap->capacity;
	aindex = hcode / HASH_BLOCK_SIZE;
	bindex = hcode % HASH_BLOCK_SIZE;

	bnode = hashmap->hash_array[aindex];
	if (bnode == NULL)
	{
		bnode = (HASH_NODE_PTR*)hashmap->alloc_mem(HASH_BLOCK_SIZE*sizeof(HASH_NODE_PTR));
		memset(bnode, 0, HASH_BLOCK_SIZE * sizeof(HASH_NODE_PTR));
		hashmap->hash_array[aindex] = bnode;
	}

	vnode = bnode[bindex];
	while (vnode != NULL)
	{
		if (0 < hashmap->equal_key(key, (void*)(vnode->data)))
			break;
		vnode = vnode->next;
	}

	if (vnode == NULL)
	{
		hashmap->size += 1;
		vnode = (HASH_NODE_PTR)hashmap->alloc_mem(hashmap->node_bytes);
		vnode->next = bnode[bindex];
		bnode[bindex] = vnode;
	}

	if (hashmap->assign_key != NULL)
		hashmap->assign_key((void*)(vnode->data), key);
	else
		memcpy((void*)(vnode->data), key, hashmap->key_bytes);

	val_ptr = (void*)(vnode->data + hashmap->key_offset);
	if (val != NULL)
	{
		if (hashmap->assign_val != NULL)
			hashmap->assign_val(val_ptr, val);
		else
			memcpy(val_ptr, val, hashmap->val_bytes);
	}
	
	return val_ptr;
}

void * hashmap_get(HASHMAP * hashmap, const void * key, void* val)
{
	size_t hcode = hashmap->hash_code(key) % hashmap->capacity;
	size_t aindex = hcode / HASH_BLOCK_SIZE;
	size_t bindex = hcode % HASH_BLOCK_SIZE;
	HASH_NODE_PTR* bnode = NULL;
	HASH_NODE_PTR  vnode = NULL;
	void* val_ptr = NULL;

	bnode = hashmap->hash_array[aindex];
	if (bnode != NULL)
	{
		vnode = bnode[bindex];
		while (vnode != NULL)
		{
			if (0 < hashmap->equal_key(key, (void*)(vnode->data)))
			{
				val_ptr = (void*)(vnode->data + hashmap->key_offset);
				if (val != NULL)
				{
					if (hashmap->assign_val != NULL)
						hashmap->assign_val(val, val_ptr);
					else
						memcpy(val, val_ptr, hashmap->val_bytes);
				}
				return val_ptr;
			}
			vnode = vnode->next;
		}
	}

	return NULL;
}

void  hashmap_del(HASHMAP * hashmap, const void * key)
{
	size_t hcode = hashmap->hash_code(key) % hashmap->capacity;
	size_t aindex = hcode / HASH_BLOCK_SIZE;
	size_t bindex = hcode % HASH_BLOCK_SIZE;
	HASH_NODE_PTR* bnode = NULL;
	HASH_NODE_PTR  vnode1 = NULL;
	HASH_NODE_PTR  vnode2 = NULL;

	bnode = hashmap->hash_array[aindex];
	if (bnode != NULL)
	{
		vnode2 = bnode[bindex];
		while (vnode2 != NULL)
		{
			if (0 < hashmap->equal_key(key, (void*)(vnode2->data)))
				break;
			vnode1 = vnode2;
			vnode2 = vnode2->next;
		}

		if (vnode2 != NULL)
		{
			if (vnode1 != NULL)
				vnode1->next = vnode2->next;
			else
				bnode[bindex] = vnode2->next;

			hashmap->free_mem(vnode2);
			hashmap->size -= 1;
		}
	}
}

void hashmap_first(HASHMAP * hashmap, HASHMAP_ITER * iter)
{
	size_t aindex = 0;
	size_t bindex = 0;
	HASH_NODE_PTR* bnode = NULL;
	HASH_NODE_PTR  vnode = NULL;

	iter->val_node = NULL;

	for (aindex = 0; aindex < hashmap->array_size; aindex++)
	{
		bnode = hashmap->hash_array[aindex];
		if (bnode != NULL)
		{
			for (bindex = 0; bindex < HASH_BLOCK_SIZE; bindex++)
			{
				vnode = bnode[bindex];
				if (vnode != NULL)
				{
					iter->array_index = aindex;
					iter->block_index = bindex;
					iter->val_node = vnode;
					return;
				}
			}
		}
	}
}

HASHMAP_ENTRY hashmap_at(HASHMAP * hashmap, HASHMAP_ITER * iter)
{
	HASHMAP_ENTRY entry = { NULL, NULL };

	entry.key = (void*)(iter->val_node->data);
	entry.val = (void*)(iter->val_node->data + hashmap->key_offset);

	return entry;
}


void hashmap_next(HASHMAP * hashmap, HASHMAP_ITER * iter)
{
	HASH_NODE_PTR* bnode = NULL;
	HASH_NODE_PTR  vnode = NULL;

	if (iter->val_node == NULL)
		return;

	iter->val_node = iter->val_node->next;

	if (iter->val_node == NULL)
	{
		while(1)
		{
			iter->block_index++;
			if (iter->block_index >= HASH_BLOCK_SIZE)
			{
				iter->block_index = 0;
				iter->array_index++;
			}

			if (iter->array_index >= hashmap->array_size)
				return;

			bnode = hashmap->hash_array[iter->array_index];
			if (bnode == NULL)
			{
				iter->block_index = 0;
				iter->array_index++;
			}
			else
			{
				vnode = bnode[iter->block_index];
				if (vnode != NULL)
				{
					iter->val_node = vnode;
					return;
				}
			}
		}
	}
}

void hashmap_clear(HASHMAP * hashmap)
{
	size_t aindex = 0;
	size_t bindex = 0;
	HASH_NODE_PTR* bnode = NULL;
	HASH_NODE_PTR  vnode = NULL;
	HASH_NODE_PTR  tnode = NULL;

	for (aindex = 0; aindex < hashmap->array_size; aindex++)
	{
		bnode = hashmap->hash_array[aindex];
		if (bnode != NULL)
		{
			for (bindex = 0; bindex < HASH_BLOCK_SIZE; bindex++)
			{
				vnode = bnode[bindex];
				while (vnode != NULL)
				{
					tnode = vnode->next;
					hashmap->free_mem(vnode);
					vnode = tnode;
				}
			}

			hashmap->free_mem(bnode);
		}
	}

	hashmap->size = 0;
	memset(hashmap->hash_array, 0, hashmap->array_size * sizeof(HASH_NODE_PTR*));
}

void hashmap_release(HASHMAP * hashmap)
{
	size_t aindex = 0;
	size_t bindex = 0;
	HASH_NODE_PTR* bnode = NULL;
	HASH_NODE_PTR  vnode = NULL;
	HASH_NODE_PTR  tnode = NULL;

	for (aindex = 0; aindex < hashmap->array_size; aindex++)
	{
		bnode = hashmap->hash_array[aindex];
		if (bnode != NULL)
		{
			for (bindex = 0; bindex < HASH_BLOCK_SIZE; bindex++)
			{
				vnode = bnode[bindex];
				while (vnode != NULL)
				{
					tnode = vnode->next;
					hashmap->free_mem(vnode);
					vnode = tnode;
				}
			}

			hashmap->free_mem(bnode);
		}
	}

	hashmap->free_mem(hashmap->hash_array);
}


