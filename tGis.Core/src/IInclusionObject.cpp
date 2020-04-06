#include "IInclusionObject.h"

extern "C"
{
#include "hashmap.h"
}

BEGIN_NAME_SPACE(tGis, Core)

class HeapPtrMapCtor
{
public:
	HASHMAP _heap_ptr_map;

	HeapPtrMapCtor()
	{
		hashmap_init(&_heap_ptr_map,
			1000,
			sizeof(void*),
			0,
			malloc,
			free,
			_ptr_hash,
			_ptr_equals,
			nullptr,
			nullptr);
	}

	~HeapPtrMapCtor()
	{
		hashmap_release(&_heap_ptr_map);
	}
};

HeapPtrMapCtor _g_heap_ptr_map_ctor;

void* IInclusionObject::_heap_ptr_map = (void*)&(_g_heap_ptr_map_ctor._heap_ptr_map);

//TODO:
//这里有bug
//对象的this指针并不与所申请的内存对齐，
//而是处在所申请内存的范围内
//因此判断方式需要修改
IInclusionObject::IInclusionObject()
{
	void* p = (void*)this;
	if (hashmap_exists((HASHMAP*)_heap_ptr_map, &p))
	{
		//判断完毕就没必要在_heap_ptr_map中存储指针了
		//可以节省内存
		hashmap_del((HASHMAP*)_heap_ptr_map, &p);
		_is_in_heap = true;
	}
	else
		_is_in_heap = false;
}

void * IInclusionObject::operator new(size_t size)
{
	void* p = ::operator new(size);
	hashmap_set((HASHMAP*)_heap_ptr_map, &p, nullptr);
	return p;
}

void * IInclusionObject::operator new(size_t size, std::nothrow_t & nothrow_value) throw()
{
	void* p = ::operator new(size, nothrow_value);
	hashmap_set((HASHMAP*)_heap_ptr_map, &p, nullptr);
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
