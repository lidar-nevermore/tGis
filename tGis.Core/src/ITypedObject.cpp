#include "ITypedObject.h"

extern "C"
{
#include "hashmap.h"
}

#include "ogr_spatialref.h"

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

void* ITypedObject::_heap_ptr_map = (void*)&(_g_heap_ptr_map_ctor._heap_ptr_map);

ITypedObject::ITypedObject()
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

void* ITypedObject::operator new(size_t size)
{
	void* p = ::operator new(size);
	hashmap_set((HASHMAP*)_heap_ptr_map, &p, nullptr);
	return p;
}

void ITypedObject::operator delete(void *p)
{
	return ::operator delete(p);
}

bool ITypedObject::CanTransform(const OGRSpatialReference * from, const OGRSpatialReference * to)
{
	if (from == to)
		return true;

	if (from != nullptr)
	{
		if (to == nullptr)
			return false;
		if (from->IsSame(to))
			return true;

		OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation(const_cast<OGRSpatialReference*>(from), const_cast<OGRSpatialReference*>(to));
		if (poCT == nullptr)
			return false;

		double x = 76.3;
		double y = 57.0;
		int ret = poCT->Transform(1, &x, &y);
		OGRCoordinateTransformation::DestroyCT(poCT);

		return ret == TRUE;
	}

	return false;
}



END_NAME_SPACE(tGis, Core)
