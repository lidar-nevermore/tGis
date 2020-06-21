#include "IInclusionObject.h"

extern "C"
{
#include "bi_list.h"
}

BEGIN_NAME_SPACE(tGis, Core)

class HeapPtrsCtor
{
public:
	BI_LIST _heap_ptrs;

	HeapPtrsCtor()
	{
		bi_list_init(&_heap_ptrs,
			2*sizeof(void*),
			malloc,
			free,
			nullptr,
			nullptr);
	}

	~HeapPtrsCtor()
	{
		bi_list_release(&_heap_ptrs);
	}
};

HeapPtrsCtor _g_heap_ptrs_ctor;

//对象的this指针并不与所申请的内存对齐，
//而是处在所申请内存的范围内
//因此采用了判断范围的方式
IInclusionObject::IInclusionObject()
{
	void* p = (void*)this;
	_is_in_heap = false;
	void* pos = bi_list_head_pos(&_g_heap_ptrs_ctor._heap_ptrs);
	while (pos != nullptr)
	{
		void** p_range = (void**)bi_list_at_pos(&_g_heap_ptrs_ctor._heap_ptrs, pos, nullptr);
		if ((p >= p_range[0] && p <= p_range[1])
			|| (p >= p_range[1] && p <= p_range[0]))
		{
			_is_in_heap = true;
			bi_list_delete_at(&_g_heap_ptrs_ctor._heap_ptrs, pos);
			break;
		}

		pos = bi_list_next_pos(&_g_heap_ptrs_ctor._heap_ptrs, pos);
	}
}

void * IInclusionObject::operator new(size_t size)
{
	void* p = ::operator new(size);
	void* p_range[2] = { p,(char*)p + size };
	bi_list_insert_head(&_g_heap_ptrs_ctor._heap_ptrs, p_range);
	return p;
}

void * IInclusionObject::operator new(size_t size, std::nothrow_t & nothrow_value) throw()
{
	void* p = ::operator new(size, nothrow_value);
	void* p_range[2] = { p,(char*)p + size };
	bi_list_insert_head(&_g_heap_ptrs_ctor._heap_ptrs, p_range);
	return p;
}

void IInclusionObject::operator delete(void *p)
{
	::operator delete(p);
}

void IInclusionObject::operator delete(void * p, std::nothrow_t & nothrow_value) throw()
{
	::operator delete(p, nothrow_value);
}

END_NAME_SPACE(tGis, Core)
