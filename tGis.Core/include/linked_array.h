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
	/*��ǰ�������ܹ��洢Ԫ�ص�����*/
	size_t capacity;
	/*�����������*/
	size_t node_count;
	/*�����������*/
	size_t node_capacity;
	/*Ԫ��ռ�ݵ��ֽ���*/
	size_t data_bytes;
	/*������ڵ���ֽ���*/
	size_t node_bytes;
	/*������ڵ������һ������λ�õ��ֽ���*/
	size_t node_data_bytes;

	/********************************************/
	/*     �����ֶν�����Ϊջ�Ͷ���ʱ��Ч��     */
	/********************************************/
	/*��ǰ�洢Ԫ�ص�����*/
	size_t size;
	/*β��������ڵ�*/
	LA_NODE* end_node;
	/*ͷ������������ʼ��������*/
	/*��Ϊ����ʹ��ʱ�Ķ�ͷ����*/
	size_t front_idx;
	/*β����������ĩβ��������*/
	/*��Ϊ����ʹ��ʱ�Ķ�β��������Ϊջʹ��ʱ��ջ��*/
	size_t end_idx;
	/*ͷ������������ʼ����ָ��*/
	char* front_ptr;
	/*β����������ĩβ����ָ��*/
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
