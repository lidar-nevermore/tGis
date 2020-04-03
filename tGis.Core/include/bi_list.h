/*
* The earliest release time: 2009-12-26
* Last modification time: 2018-11-28
*
* This file and the accompanying files of this project may be freely used provided the following
* conditions are met:
*        * This copyright statement is not removed or modified.
*        * The code is not sold in uncompiled form.  (Release as a compiled binary which is part
*          of an application is fine)
*        * The design, code, or compiled binaries are not "Re-branded".
*        
* Optional:
*        * Redistributions in binary form must reproduce the above copyright notice.
*        * I receive a fully licensed copy of the product (regardless of whether the product is
*          is free, shrinkwrap, or commercial).  This is optional, though if you release products
*          which use code I've contributed to, I would appreciate a fully licensed copy.
*
* In addition, you may not:
*        * Publicly release modified versions of the code or publicly release works derived from
*          the code without express written authorization.
*
* NO WARRANTY:
*        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
*        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
*        IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
*        INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
*        NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
*        OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
*        WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
*        ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
*        OF SUCH DAMAGE.
*
*
* Usage:
*
*     为了可扩展使用，下面的结点结构中用void指定数据类型，使用时
* 请按照需要定义类型，并且提供四个原型如下的函数：
*
*    void  assign(void * a,const void * b);
*    int   is_equal(const void * a,const void * b);
*    void* alloc_node();
*    void  free_node(void* mem);
*
*     第一个用来为void类型的数据赋值，第二个判断是否相等，第三个
*用于为BI_NODE申请内存，第四个用于释放申请的BI_NODE。
*
*
*
*     在本文件中，定义了DEMO_POINT_T,已经提供了上述四个函数，使用
* 其作为bi_list_init的参数，即可在初始化的list中存储DEMO_POINT_T。
* 如需要在链表中存储其它类型，提供相应的这5个函数的实现。
*
*
*
*     本链表为双向链表，可以用作队列，堆栈等一些非常有用的数据结构。
*
*
*/

#ifndef __BI_LIST_H__
#define __BI_LIST_H__

#include <stdlib.h>
#include <string.h>

typedef  struct _BI_NODE
{
    struct _BI_NODE *next;
    struct _BI_NODE *prev;
	unsigned char    data[1];
}BI_NODE;


typedef  struct _BI_LIST
{
	BI_NODE *head;
	BI_NODE *tail;
    size_t size;
	size_t data_bytes;
	size_t node_bytes;
	void*(*alloc_mem)(size_t size);
	void(*free_mem)(void* mem);
	int(*equal)(const void * a, const void * b);
	void(*assign)(void * a, const void * b);	
}BI_LIST;


#ifndef BI_LIST_DATA_DEMO
#define BI_LIST_DATA_DEMO  1

typedef struct _DEMO_POINT_T
{
	int x;
	int y;
}DEMO_POINT_T;

/*提供所需的赋值函数*/
void demo_assign(void * a, const void * b);

/*提供所需的判断相等函数*/
int demo_is_equal(const void * a, const void * b);

void* demo_alloc_mem(size_t size);

void demo_free_mem(void* mem);

#endif // !BI_LIST_DATA_DEMO



/**
 *初始化链表，定义一个链表后必须初始化，
 *警告！一个链表只允许调用一次
 **/
void bi_list_init(BI_LIST *list,
	size_t data_bytes,
	void*(*alloc_mem)(size_t size),
	void(*free_mem)(void* mem),
	int(*equal)(const void * a, const void * b),
	void(*assign)(void * a, const void * b)
	);

/*释放链表所占用的资源，在链表使用结束后必须调用*/
void bi_list_release(BI_LIST *list);

/*在链表头添加元素*/
void* bi_list_insert_head(BI_LIST *list, const void * data);

/*在链表尾添加元素*/
void* bi_list_insert_tail(BI_LIST *list, const void * data);

/*在迭代器之前插入元素*/
void bi_list_insert_before(BI_LIST *list, void* pos, const void *data);

/*在迭代器之后插入元素*/
void bi_list_insert_after(BI_LIST *list, void* pos, const void *data);

/*删除链表头部元素到data中,如果删除的元素不再需要传入NULL即可*/
void bi_list_delete_head(BI_LIST *list, void * data);

/*删除链表尾部元素到data中，如果删除的元素不再需要传入NULL即可*/
void bi_list_delete_tail(BI_LIST *list, void * data);

/*删除迭代器处的位置，并返回下一个元素的迭代器*/
void* bi_list_delete_at(BI_LIST * list, void * pos);

/*获取链表头部的元素，返回链表头部元素的指针*/
inline void*  bi_list_get_head(BI_LIST *list, void * data)
{
	void* data_ptr = NULL;
	if (list->size>0)
	{
		data_ptr = (void*)(list->head->data);

		if (data != NULL)
		{
			if (list->assign != NULL)
				list->assign(data, data_ptr);
			else
				memcpy(data, data_ptr, list->data_bytes);
		}
	}

	return data_ptr;
}

/*获取链尾头部的元素，返回链表尾部元素的指针*/
inline void*  bi_list_get_tail(BI_LIST *list, void * data)
{
	void* data_ptr = NULL;
	if (list->size>0)
	{
		data_ptr = (void*)(list->tail->data);

		if (data != NULL)
		{
			if (list->assign != NULL)
				list->assign(data, data_ptr);
			else
				memcpy(data, data_ptr, list->data_bytes);
		}
	}
	
	return data_ptr;
}

/*获取链表头部的迭代器*/
inline void * bi_list_head_pos(BI_LIST * list)
{
	return list->head;
}

/*获取链表尾部的迭代器*/
inline void * bi_list_tail_pos(BI_LIST * list)
{
	return list->tail;
}

/*获取迭代器处的元素，返回迭代器处元素的指针*/
inline void*  bi_list_at_pos(BI_LIST * list, void * pos, void * data)
{
	void* data_ptr = (void*)(((BI_NODE*)pos)->data);

	if (data != NULL)
	{
		if (list->assign != NULL)
			list->assign(data, data_ptr);
		else
			memcpy(data, data_ptr, list->data_bytes);
	}

	return data_ptr;
}

/*将迭代器向尾部移动一步*/
inline void * bi_list_next_pos(BI_LIST * list, void * pos)
{
	return ((BI_NODE*)pos)->next;
}

/*将迭代器向头部移动一步*/
inline void * bi_list_prev_pos(BI_LIST * list, void * pos)
{
	return ((BI_NODE*)pos)->prev;
}

/*查找某元素是否在链表中，如果在，则返回其指针；否则返回NULL*/
BI_NODE* bi_list_find(BI_LIST *list, const void *data);

/*返回链表中第index个位置处的元素的指针，*index值从1开始，index<= list->size*/
void *  bi_list_at_index(BI_LIST *list, unsigned index);

#endif
