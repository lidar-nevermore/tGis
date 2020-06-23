#include "linked_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



#define ALIGN_MEM(size, boundary)     (((size) + ((boundary) - 1)) & ~((boundary) - 1)) 

void linked_array_init(LINKED_ARRAY *arr,
	size_t data_bytes,
	size_t node_capacity,
	void*(*alloc_mem)(size_t size),
	void(*free_mem)(void* mem),
	void(*assign)(void * a, const void * b)
	)
{
	arr->data_bytes = data_bytes;
	arr->node_capacity = node_capacity;
	arr->node_data_bytes = data_bytes*node_capacity;
	arr->node_bytes = ALIGN_MEM(arr->node_data_bytes + sizeof(LA_NODE) - 1, sizeof(int));
	arr->node_data_bytes -= data_bytes;
	arr->node_count = 0;
	arr->front_idx = 0;
	arr->end_idx = 0;
	arr->front_ptr = NULL;
	arr->end_ptr = NULL;
	arr->end_node = NULL;
	arr->capacity = 0;
	arr->size = 0;
	
	arr->tail = NULL;
	arr->head = NULL;
	arr->alloc_mem = alloc_mem;
	arr->free_mem = free_mem;
	arr->assign = assign;
}

/*和linked_array_release一样的，对外的语义不一样*/
/*clear表示清除容器种的元素，release表示释放容器*/
void linked_array_clear(LINKED_ARRAY * arr)
{
	LA_NODE *p1, *p2;

	p1 = p2 = arr->head;
	while (p1 != NULL)
	{
		p2 = p1->next;
		arr->free_mem(p1);
		p1 = p2;
	}
	arr->node_count = 0;
	arr->front_idx = 0;
	arr->end_idx = 0;
	arr->front_ptr = NULL;
	arr->end_ptr = NULL;
	arr->end_node = NULL;
	arr->capacity = 0;
	arr->size = 0;

	arr->tail = NULL;
	arr->head = NULL;
}

void linked_array_release(LINKED_ARRAY * arr)
{
	LA_NODE *p1, *p2;

	p1 = p2 = arr->head;
	while (p1 != NULL)
	{
		p2 = p1->next;
		arr->free_mem(p1);
		p1 = p2;
	}
	arr->node_count = 0;
	arr->front_idx = 0;
	arr->end_idx = 0;
	arr->front_ptr = NULL;
	arr->end_ptr = NULL;
	arr->end_node = NULL;
	arr->capacity = 0;
	arr->size = 0;

	arr->tail = NULL;
	arr->head = NULL;
}

inline void __linked_array_exn(LINKED_ARRAY * arr, size_t node_add)
{
	size_t i;
	LA_NODE* p;

	for (i = 0; i < node_add; i++)
	{
		p = (LA_NODE*)arr->alloc_mem(arr->node_bytes);
		p->next = NULL;
		if (arr->tail == NULL)
		{
			p->prev = NULL;
			arr->head = p;
			arr->tail = p;
		}
		else
		{
			arr->tail->next = p;
			p->prev = arr->tail;
			arr->tail = p;
		}
	}

	arr->node_count += node_add;
	arr->capacity += node_add*arr->node_capacity;

	if (arr->front_ptr == NULL)
	{
		arr->front_ptr = arr->head->data;
		arr->front_idx = 0;
	}

	if (arr->end_ptr == NULL)
	{
		arr->end_node = arr->tail;
		arr->end_ptr = arr->tail->data;
		arr->end_idx = 0;
	}
}

void linked_array_expand(LINKED_ARRAY * arr, size_t capacity)
{
	size_t node_count;
	size_t node_add;
	
	if (arr->capacity < capacity)
	{
		node_count = (capacity + arr->node_capacity - 1) / arr->node_capacity;
		node_add = node_count - arr->node_count;
		__linked_array_exn(arr, node_add);
	}
}

void * linked_array_set(LINKED_ARRAY * arr, size_t idx, const void * data)
{
	size_t rest_idx = idx;
	LA_NODE* p = arr->head;
	void* data_ptr = NULL;

	if (idx < arr->capacity)
	{
		while (rest_idx > arr->node_capacity)
		{
			p = p->next;
			rest_idx -= arr->node_capacity;
		}
		data_ptr = p->data + rest_idx*arr->data_bytes;
		if (data != NULL)
		{
			if (arr->assign != NULL)
				arr->assign(data_ptr, data);
			else
				memcpy(data_ptr, data, arr->data_bytes);
		}

		return data_ptr;
	}

	return NULL;
}

void * linked_array_get(LINKED_ARRAY * arr, size_t idx, void * data)
{
	size_t rest_idx = idx;
	LA_NODE* p = arr->head;
	void* data_ptr = NULL;

	if (idx < arr->capacity)
	{
		while (rest_idx > arr->node_capacity)
		{
			p = p->next;
			rest_idx -= arr->node_capacity;
		}
		data_ptr = p->data + rest_idx*arr->data_bytes;
		if (data != NULL)
		{
			if (arr->assign != NULL)
				arr->assign(data, data_ptr);
			else
				memcpy(data, data_ptr, arr->data_bytes);
		}			

		return data_ptr;
	}

	return NULL;
}

void * linked_array_at(LINKED_ARRAY * arr, size_t idx)
{
	size_t rest_idx = idx;
	LA_NODE* p = arr->head;
	void* data_ptr = NULL;

	if (idx < arr->capacity)
	{
		while (rest_idx > arr->node_capacity)
		{
			p = p->next;
			rest_idx -= arr->node_capacity;
		}
		data_ptr = p->data + rest_idx*arr->data_bytes;

		return data_ptr;
	}

	return NULL;
}

inline void * __linked_array_append(LINKED_ARRAY * arr, const void * data)
{
	void* data_ptr;

	if (arr->end_ptr == NULL)
		__linked_array_exn(arr, 1);

	data_ptr = arr->end_ptr;
	if(data != NULL)
	{
		if (arr->assign != NULL)
			arr->assign(data_ptr, data);
		else
			memcpy(data_ptr, data, arr->data_bytes);
	}		

	arr->size++;
	arr->end_idx++;
	if (arr->end_idx < arr->node_capacity)
		arr->end_ptr += arr->data_bytes;
	else
	{
		arr->end_node = arr->end_node->next;
		if (arr->end_node == NULL)
			arr->end_ptr = NULL;
		else
		{
			arr->end_ptr = arr->end_node->data;
			arr->end_idx = 0;
		}
	}

	return data_ptr;
}

void * linked_array_enqueue(LINKED_ARRAY * arr, const void * data)
{
	return __linked_array_append(arr, data);
}

void * linked_array_dequeue(LINKED_ARRAY * arr, void * data)
{
	LA_NODE* p;
	void* data_ptr;
	if (arr->size > 0)
	{
		arr->size--;
		data_ptr = arr->front_ptr;
		if (data != NULL)
		{
			if (arr->assign != NULL)
				arr->assign(data, data_ptr);
			else
				memcpy(data, data_ptr, arr->data_bytes);
		}
			
		arr->front_idx++;
		if (arr->front_idx < arr->node_capacity)
			arr->front_ptr += arr->data_bytes;
		else
		{
			p = arr->head->next;
			if (p == NULL)
			{
				arr->front_ptr = arr->head->data;
				arr->front_idx = 0;

				arr->end_ptr = arr->tail->data;
				arr->end_idx = 0;
			}
			else
			{
				p = arr->head;
				arr->head = arr->head->next;
				arr->front_ptr = arr->head->data;
				arr->front_idx = 0;
				p->next = NULL;
				p->prev = arr->tail;
				arr->tail->next = p;
				arr->tail = p;
			}
		}

		return data_ptr;
	}

	return NULL;
}

void * linked_array_push(LINKED_ARRAY * arr, const void * data)
{
	return __linked_array_append(arr, data);
}

void * linked_array_pop(LINKED_ARRAY * arr, void * data)
{
	void* data_ptr;
	if (arr->size > 0)
	{
		if (arr->end_idx > 0)
		{
			arr->end_idx--;
			arr->end_ptr -= arr->data_bytes;			
		}
		else
		{
			arr->end_node = arr->end_node->prev;
			arr->end_idx = arr->node_capacity - 1;
			arr->end_ptr = arr->end_node->data + arr->node_data_bytes;
		}

		data_ptr = arr->end_ptr;
		if (data != NULL)
		{
			if (arr->assign != NULL)
				arr->assign(data, data_ptr);
			else
				memcpy(data, data_ptr, arr->data_bytes);
		}

		return data_ptr;
	}
	return NULL;
}

void * linked_array_peek(LINKED_ARRAY * arr, void * data)
{
	void* data_ptr;
	if (arr->size > 0)
	{
		if (arr->end_idx > 0)
			data_ptr = arr->end_ptr - arr->data_bytes;
		else
			data_ptr = arr->end_node->prev->data + arr->node_data_bytes;

		if (data != NULL)
		{
			if (arr->assign != NULL)
				arr->assign(data, data_ptr);
			else
				memcpy(data, data_ptr, arr->data_bytes);
		}

		return data_ptr;
	}
	return NULL;
}


