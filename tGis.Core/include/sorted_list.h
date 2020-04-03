#ifndef __SORTED_LIST_H__
#define __SORTED_LIST_H__

#include <stdlib.h>
#include <string.h>

#define SK_MAX_LEVEL 10

typedef struct _SK_LINK
{
	struct _SK_NODE* next;
	struct _SK_NODE* prev;
}SK_LINK;

typedef struct _SK_NODE
{
	char* data;
	/*����level��������ֵΪ���level��ȥ1*/
	int level;
	SK_LINK  link[1];
}SK_NODE;


typedef  struct _ST_LIST
{
	/*ռλ�ģ�ʼ��ΪNULL��ֻ��Ϊ����_ST_LIST����ǿת��_SK_NODE*/
	char* data;
	/*����level��������ֵΪ���level��ȥ1*/
	int level;
	SK_LINK  head[SK_MAX_LEVEL];
	size_t size;
	size_t key_bytes;
	size_t key_offset;
	size_t val_bytes;

	void*(*alloc_mem)(size_t size);
	void(*free_mem)(void* mem);
	int(*compare_key)(const void * a, const void * b);
	void(*assign_key)(void * a, const void * b);
	void(*assign_val)(void * a, const void * b);
}ST_LIST;


void sorted_list_init(ST_LIST* list,
	size_t key_bytes,
	size_t val_bytes,
	void*(*alloc_mem)(size_t size),
	void(*free_mem)(void* mem),
	int(*compare_key)(const void * a, const void * b),
	void(*assign_key)(void * a, const void * b),
    void(*assign_val)(void * a, const void * b)
);

void* sorted_list_add(ST_LIST* list, const void* key, const void* val);

void* sorted_list_get(ST_LIST* list, const void* key, void* val);

void* sorted_list_del(ST_LIST* list, void* iter);

inline void * sorted_list_first(ST_LIST * list)
{
	return list->head[0].next;
}

/*��ȡ����������Ԫ�أ����ص�������Ԫ�ص�ָ��*/
inline void * sorted_list_at(ST_LIST * list, void * iter, void* val)
{
	SK_NODE* node = (SK_NODE*)iter;
	void* val_ptr = (void*)(node->data + list->key_offset);

	if (val != NULL)
	{
		if (list->assign_val != NULL)
			list->assign_val(val, val_ptr);
		else
			memcpy(val, val_ptr, list->val_bytes);
	}
	return val_ptr;
}

inline void * sorted_list_next(ST_LIST * list, void * iter)
{
	SK_NODE* node = (SK_NODE*)iter;
	return node->link[0].next;
}

void  sorted_list_release(ST_LIST* list);

#endif
