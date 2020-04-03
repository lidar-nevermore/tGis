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
*     Ϊ�˿���չʹ�ã�����Ľ��ṹ����voidָ���������ͣ�ʹ��ʱ
* �밴����Ҫ�������ͣ������ṩ�ĸ�ԭ�����µĺ�����
*
*    void  assign(void * a,const void * b);
*    int   is_equal(const void * a,const void * b);
*    void* alloc_node();
*    void  free_node(void* mem);
*
*     ��һ������Ϊvoid���͵����ݸ�ֵ���ڶ����ж��Ƿ���ȣ�������
*����ΪBI_NODE�����ڴ棬���ĸ������ͷ������BI_NODE��
*
*
*
*     �ڱ��ļ��У�������DEMO_POINT_T,�Ѿ��ṩ�������ĸ�������ʹ��
* ����Ϊbi_list_init�Ĳ����������ڳ�ʼ����list�д洢DEMO_POINT_T��
* ����Ҫ�������д洢�������ͣ��ṩ��Ӧ����5��������ʵ�֡�
*
*
*
*     ������Ϊ˫�����������������У���ջ��һЩ�ǳ����õ����ݽṹ��
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

/*�ṩ����ĸ�ֵ����*/
void demo_assign(void * a, const void * b);

/*�ṩ������ж���Ⱥ���*/
int demo_is_equal(const void * a, const void * b);

void* demo_alloc_mem(size_t size);

void demo_free_mem(void* mem);

#endif // !BI_LIST_DATA_DEMO



/**
 *��ʼ����������һ�����������ʼ����
 *���棡һ������ֻ�������һ��
 **/
void bi_list_init(BI_LIST *list,
	size_t data_bytes,
	void*(*alloc_mem)(size_t size),
	void(*free_mem)(void* mem),
	int(*equal)(const void * a, const void * b),
	void(*assign)(void * a, const void * b)
	);

/*�ͷ�������ռ�õ���Դ��������ʹ�ý�����������*/
void bi_list_release(BI_LIST *list);

/*������ͷ���Ԫ��*/
void* bi_list_insert_head(BI_LIST *list, const void * data);

/*������β���Ԫ��*/
void* bi_list_insert_tail(BI_LIST *list, const void * data);

/*�ڵ�����֮ǰ����Ԫ��*/
void bi_list_insert_before(BI_LIST *list, void* pos, const void *data);

/*�ڵ�����֮�����Ԫ��*/
void bi_list_insert_after(BI_LIST *list, void* pos, const void *data);

/*ɾ������ͷ��Ԫ�ص�data��,���ɾ����Ԫ�ز�����Ҫ����NULL����*/
void bi_list_delete_head(BI_LIST *list, void * data);

/*ɾ������β��Ԫ�ص�data�У����ɾ����Ԫ�ز�����Ҫ����NULL����*/
void bi_list_delete_tail(BI_LIST *list, void * data);

/*ɾ������������λ�ã���������һ��Ԫ�صĵ�����*/
void* bi_list_delete_at(BI_LIST * list, void * pos);

/*��ȡ����ͷ����Ԫ�أ���������ͷ��Ԫ�ص�ָ��*/
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

/*��ȡ��βͷ����Ԫ�أ���������β��Ԫ�ص�ָ��*/
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

/*��ȡ����ͷ���ĵ�����*/
inline void * bi_list_head_pos(BI_LIST * list)
{
	return list->head;
}

/*��ȡ����β���ĵ�����*/
inline void * bi_list_tail_pos(BI_LIST * list)
{
	return list->tail;
}

/*��ȡ����������Ԫ�أ����ص�������Ԫ�ص�ָ��*/
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

/*����������β���ƶ�һ��*/
inline void * bi_list_next_pos(BI_LIST * list, void * pos)
{
	return ((BI_NODE*)pos)->next;
}

/*����������ͷ���ƶ�һ��*/
inline void * bi_list_prev_pos(BI_LIST * list, void * pos)
{
	return ((BI_NODE*)pos)->prev;
}

/*����ĳԪ���Ƿ��������У�����ڣ��򷵻���ָ�룻���򷵻�NULL*/
BI_NODE* bi_list_find(BI_LIST *list, const void *data);

/*���������е�index��λ�ô���Ԫ�ص�ָ�룬*indexֵ��1��ʼ��index<= list->size*/
void *  bi_list_at_index(BI_LIST *list, unsigned index);

#endif
