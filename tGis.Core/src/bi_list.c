#include "bi_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef BI_LIST_DATA_DEMO

/*提供所需的赋值函数*/
void demo_assign(void * a_, const void * b_)
{
	DEMO_POINT_T* a = (DEMO_POINT_T*)a_;
	DEMO_POINT_T* b = (DEMO_POINT_T*)b_;
	a->x = b->x;
	a->y = b->y;
}

/*提供所需的判断相等函数*/
int demo_is_equal(const void * a_, const void * b_)
{
	DEMO_POINT_T* a = (DEMO_POINT_T*)a_;
	DEMO_POINT_T* b = (DEMO_POINT_T*)b_;
	if ((a->x == b->x) && \
		(a->y == b->y))
		return 1;
	else return 0;
}

void* demo_alloc_mem(size_t size)
{
	return malloc(size);
}

void demo_free_mem(void* mem)
{
	free(mem);
}

#endif // BI_LIST_DATA_DEMO



int _default_data_is_equal(const void * a_, const void * b_)
{
	return 0;
}


#define ALIGN_MEM(size, boundary)     (((size) + ((boundary) - 1)) & ~((boundary) - 1)) 

void bi_list_init(BI_LIST *list,
	size_t data_bytes,
	void*(*alloc_mem)(size_t size),
	void(*free_mem)(void* mem),
	int(*equal)(const void * a, const void * b),
	void(*assign)(void * a, const void * b)
	)
{
	list->data_bytes = data_bytes;
	list->node_bytes = ALIGN_MEM(data_bytes + sizeof(BI_NODE) - 1, sizeof(int));
	list->alloc_mem = alloc_mem;
	list->free_mem = free_mem;	
	if(equal == NULL)
	    list->equal = _default_data_is_equal;
	else
		list->equal = equal;
	list->assign = assign;
		
    list->tail = NULL;
    list->head = NULL;
    list->size = 0;
}


void bi_list_release(BI_LIST *list)
{
    BI_NODE *p1,*p2;

    p1 = p2 = list->head;
    while (p1 != NULL)
    {
        p2 = p1->next;
		list->free_mem(p1);
        p1 = p2;
    }

	list->tail = NULL;
	list->head = NULL;
	list->size = 0;
}


void* bi_list_insert_head(BI_LIST *list,const void * data)
{
    BI_NODE *p;

    p = (BI_NODE*)list->alloc_mem(list->node_bytes);
	if (data != NULL)
	{
		if (list->assign != NULL)
			list->assign((void*)(p->data), data);
		else
			memcpy((void*)(p->data), data, list->data_bytes);
	}
	p->next = list->head;
	p->prev = NULL;
	list->size++;
	if (list->head != NULL)
	{
		list->head->prev = p;
		list->head = p;
	}
	else
	{
		list->head = p;
		list->tail = p;
	}

	return (void*)(p->data);
}


void* bi_list_insert_tail(BI_LIST *list,const void * data)
{
    BI_NODE *p;

    p = (BI_NODE*)list->alloc_mem(list->node_bytes);
	if (data != NULL)
	{
		if (list->assign != NULL)
			list->assign((void*)(p->data), data);
		else
			memcpy((void*)(p->data), data, list->data_bytes);
	}
    p->prev = list->tail;
	p->next = NULL;	
	list->size++;
	if (list->tail != NULL)
	{
		list->tail->next = p;
		list->tail = p;
	}
	else
	{
		list->head = p;
		list->tail = p;
	}

	return (void*)(p->data);
}


void bi_list_insert_before(BI_LIST * list, void * pos, const void * data)
{
	BI_NODE *npos = (BI_NODE*)pos;
	BI_NODE *p = (BI_NODE*)list->alloc_mem(list->node_bytes);

	if (data != NULL)
	{
		if (list->assign != NULL)
			list->assign((void*)(p->data), data);
		else
			memcpy((void*)(p->data), data, list->data_bytes);
	}
	p->next = npos;
	p->prev = npos->prev;
	list->size++;
	if (npos->prev != NULL)
		npos->prev->next = p;
	else
		list->head = p;
	npos->prev = p;
}


void bi_list_insert_after(BI_LIST * list, void * pos, const void * data)
{
	BI_NODE *npos = (BI_NODE*)pos;
	BI_NODE *p = (BI_NODE*)list->alloc_mem(list->node_bytes);

	if (data != NULL)
	{
		if (list->assign != NULL)
			list->assign((void*)(p->data), data);
		else
			memcpy((void*)(p->data), data, list->data_bytes);
	}
	p->next = npos->next;
	p->prev = npos;
	list->size++;
	if (npos->next != NULL)
		npos->next->prev = p;
	else
		list->tail = p;
	npos->next = p;
}


void bi_list_delete_head(BI_LIST *list,void * data)
{
    BI_NODE *p = list->head->next;

	if (p != NULL)
		p->prev = NULL;
	else
		list->tail = NULL;

	if (data != NULL)
	{
		if (list->assign != NULL)
			list->assign(data, (void*)(list->head->data));
		else
			memcpy(data, (void*)(list->head->data), list->data_bytes);
	}
	list->free_mem(list->head);
    list->head = p;
    list->size--;
}


void bi_list_delete_tail(BI_LIST *list,void * data)
{
    BI_NODE *p = list->tail->prev;
	if (p != NULL)
		p->next = NULL;
	else
		list->head = NULL;

	if (data != NULL)
	{
		if (list->assign != NULL)
			list->assign(data, (void*)(list->tail->data));
		else
			memcpy(data, (void*)(list->tail->data), list->data_bytes);
	}
	list->free_mem(list->tail);
    list->tail = p;
    list->size--;
}


void * bi_list_delete_at(BI_LIST * list, void * pos)
{
	BI_NODE *npos = (BI_NODE*)pos;
	BI_NODE *rpos = npos->next;

	if (npos->prev != NULL)
		npos->prev->next = npos->next;
	else
		list->head = npos->next;
	if (npos->next != NULL)
		npos->next->prev = npos->prev;
	else
		list->tail = npos->prev;

	list->size--;
	list->free_mem(npos);
	return rpos;
}


BI_NODE * bi_list_find(BI_LIST *list,const void *data)
{
    int flag = 1;
    int i = 0,s = (int)ceil(list->size/2.0);
    BI_NODE *p1 = list->head;
    BI_NODE *p2 = list->tail;
    while ((flag==1)&&(i<s))
    {
        if (list->equal((void*)(p1->data),data))
        {
            flag = 2;
        }
        else if (list->equal((void*)(p2->data),data))
        {
            flag = 3;
        }
        else
        {
            p1 = p1->next;
            p2 = p2->prev;
        }
        i++;
    }

	if (flag == 2)
		return p1;
	else if (flag == 3)
		return p2;
	else
		return NULL;
}


void *  bi_list_at_index(BI_LIST *list,unsigned index)
{
    BI_NODE *p;
	size_t i,s = list->size/2;

    if (index<s)
    {
        i = 1;
        p = list->head;
        while (i < index)
        {
            p = p->next;
            i++;
        }
    }
    else
    {
        i = list->size;
        p = list->tail;
        while (i > index)
        {
            p = p->prev;
            i--;
        }
    }

    return (void*)(p->data);
}

