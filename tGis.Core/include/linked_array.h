#ifndef __LINKED_ARRAY_H__
#define __LINKED_ARRAY_H__

#include <stdlib.h>
#include <string.h>

#include "tGis_CoreCfg.h"


typedef  struct TGIS_CORE_API _LA_NODE
{
    struct _LA_NODE *next;
    struct _LA_NODE *prev;
	unsigned char    data[1];
}LA_NODE;


typedef  struct TGIS_CORE_API _LINKED_ARRAY
{
	LA_NODE *head;
	LA_NODE *tail;
	/*当前容量，能够存储元素的数量*/
	size_t capacity;
	/*子数组的数量*/
	size_t node_count;
	/*子数组的容量*/
	size_t node_capacity;
	/*元素占据的字节数*/
	size_t data_bytes;
	/*子数组节点的字节数*/
	size_t node_bytes;
	/*子数组节点中最后一个数据位置的字节数*/
	size_t node_data_bytes;

	/********************************************/
	/*     以下字段仅在作为栈和队列时有效果     */
	/********************************************/
	/*当前存储元素的数量*/
	size_t size;
	/*尾部子数组节点*/
	LA_NODE* end_node;
	/*头部子数组中起始数据索引*/
	/*作为队列使用时的队头索引*/
	size_t front_idx;
	/*尾部子数组中末尾数据索引*/
	/*作为队列使用时的队尾索引，作为栈使用时的栈顶*/
	size_t end_idx;
	/*头部子数组中起始数据指针*/
	char* front_ptr;
	/*尾部子数组中末尾数据指针*/
	char* end_ptr;

	void*(*alloc_mem)(size_t size);
	void(*free_mem)(void* mem);
	void(*assign)(void * a, const void * b);	
}LINKED_ARRAY;


TGIS_CORE_API void linked_array_init(LINKED_ARRAY *arr,
	size_t data_bytes,
	size_t node_capacity,
	void*(*alloc_mem)(size_t size),
	void(*free_mem)(void* mem),
	void(*assign)(void * a, const void * b)
	);


TGIS_CORE_API void linked_array_release(LINKED_ARRAY *arr);


TGIS_CORE_API void linked_array_expand(LINKED_ARRAY *arr, size_t capacity);


TGIS_CORE_API void* linked_array_set(LINKED_ARRAY *arr, size_t idx, const void * data);


TGIS_CORE_API void* linked_array_get(LINKED_ARRAY *arr, size_t idx, void * data);


TGIS_CORE_API void* linked_array_at(LINKED_ARRAY *arr, size_t idx);


TGIS_CORE_API void* linked_array_enqueue(LINKED_ARRAY *arr, const void * data);


TGIS_CORE_API void* linked_array_dequeue(LINKED_ARRAY *arr, void * data);


TGIS_CORE_API void* linked_array_push(LINKED_ARRAY *arr, const void * data);


TGIS_CORE_API void* linked_array_pop(LINKED_ARRAY *arr, void * data);


TGIS_CORE_API void* linked_array_peek(LINKED_ARRAY *arr, void * data);

#endif
