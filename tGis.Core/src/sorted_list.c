#include "sorted_list.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define ALIGN_MEM(size, boundary)     (((size) + ((boundary) - 1)) & ~((boundary) - 1)) 



void sorted_list_init(ST_LIST* list,
	size_t key_bytes,
	size_t val_bytes,
	void*(*alloc_mem)(size_t size),
	void(*free_mem)(void* mem),
	int(*compare_key)(const void * a, const void * b),
	void(*assign_key)(void * a, const void * b),
	void(*assign_val)(void * a, const void * b)
)
{
	list->data = NULL;
	list->level = 0;
	memset(list->head, 0, SK_MAX_LEVEL * sizeof(SK_LINK));
	list->size = 0;
	list->key_bytes = key_bytes;
	list->key_offset = ALIGN_MEM(key_bytes, sizeof(int));
	list->val_bytes = val_bytes;

	list->alloc_mem = alloc_mem;
	list->free_mem = free_mem;
	list->compare_key = compare_key;
	list->assign_key = assign_key;
	list->assign_val = assign_val;	

	srand((unsigned int)time(NULL));
}


static inline void __find(ST_LIST * list, const void* skey, SK_NODE* link[SK_MAX_LEVEL])
{
	SK_NODE* sknode = (SK_NODE*)list;
	int level = sknode->level;
	void* key = NULL;

	while (1)
	{
		if (sknode->link[level].next == NULL)
		{
			link[level] = sknode;
			if (level > 0)
				level--;
			else
				break;
		}
		else
		{
			key = sknode->link[level].next->data;
			if (list->compare_key(skey, key) >= 0)
			{
				sknode = sknode->link[level].next;
			}
			else
			{
				link[level] = sknode;
				if (level > 0)
					level--;
				else
					break;
			}
		}
	}
}


void * sorted_list_add(ST_LIST * list, const void* key, const void * val)
{
	SK_NODE* link[SK_MAX_LEVEL] = { NULL };
	SK_NODE* node;
	size_t offset = 0;
	/*level为0时表示有1个级别*/
	int level = (unsigned int)(((size_t)SK_MAX_LEVEL*rand())/ RAND_MAX);
	int i = 0;

	if (level >= SK_MAX_LEVEL)
		level = SK_MAX_LEVEL - 1;
	offset = ALIGN_MEM(sizeof(SK_NODE) + level * sizeof(SK_LINK), sizeof(int));

	node = (SK_NODE*)list->alloc_mem(offset + list->key_offset + list->val_bytes);
	node->data = ((char*)node) + offset;
	node->level = level;

	if (list->assign_key != NULL)
		list->assign_key(node->data, key);
	else
		memcpy(node->data, key, list->key_bytes);

	if (val != NULL)
	{
		if (list->assign_val != NULL)
			list->assign_val(node->data + list->key_offset, val);
		else
			memcpy(node->data + list->key_offset, val, list->val_bytes);
	}

	__find(list, key, link);

	for (i = level; i >= 0; i--)
	{
		if (i > list->level)
		{
			list->head[i].next = node;
			node->link[i].prev = (SK_NODE*)list;
			node->link[i].next = NULL;
		}
		else
		{
			node->link[i].next = link[i]->link[i].next;
			node->link[i].prev = link[i];
			if (link[i]->link[i].next != NULL)
				link[i]->link[i].next->link[i].prev = node;
			link[i]->link[i].next = node;
		}
	}
	
	list->level = max((int)level, list->level);
	list->size++;

	return node->data;
}


void* sorted_list_get(ST_LIST * list, const void * key, void* val)
{
	SK_NODE* link[SK_MAX_LEVEL] = { NULL };
	SK_NODE* node;
	void* val_ptr = NULL;

	__find(list, key, link);
	node = link[0];

	while (node != NULL
		&& node->data != NULL
	    && list->compare_key(node->data, key) != 0)
	{
		node = node->link[0].next;
	}

	if (node == NULL)
		return NULL;

	val_ptr = (void*)(node->data + list->key_offset);

	if (val != NULL)
	{
		if (list->assign_val != NULL)
			list->assign_val(val, val_ptr);
		else
			memcpy(val, val_ptr, list->val_bytes);
	}

	return val_ptr;
}


void * sorted_list_del(ST_LIST * list, void * iter)
{
	SK_NODE* node = (SK_NODE*)iter;
	SK_NODE *rpos = node->link[0].next;
	int i = 0;

	for (i = 0; i <= node->level; i++)
	{
		if (node->link[i].prev != NULL)
			node->link[i].prev->link[i].next = node->link[i].next;
		if (node->link[i].next != NULL)
			node->link[i].next->link[i].prev = node->link[i].prev;
	}

	list->size--;
	list->free_mem(node);

	return rpos;
}



void sorted_list_release(ST_LIST * list)
{
	SK_NODE* node = list->head[0].next;
	while (node != NULL)
	{
		list->head[0].next = node->link[0].next;
		list->free_mem(node);
		node = list->head[0].next;
	}

	list->size = 0;
}
