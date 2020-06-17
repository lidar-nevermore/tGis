#pragma once

#ifndef __I_INCLUSIONOBJECT_H__
#define __I_INCLUSIONOBJECT_H__

#include "Helper.h"
#include "elr_mpl.h"


class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

//内含对象接口
//表示该类的对象被聚合到容器类对象中发挥作用的

//这里重载new和delete有两个原因
//在框架里有很多聚合关系，容器对象保留的是被聚合对象的指针，
//为了实现容器对象释放时自动释放被聚合的对象，
//第一，需要知道被聚合的对象是在堆里还是在栈里，
//在堆里的对象是需要容器对象编码释放的，在栈里的对象则不需要；
//第二，容器类编码释放被聚合对象的代码编译到了本库中，
//而被聚合对象的创建在客户库中，客户库和本库可能使用了不同的堆，
//在本库中delete客户库堆中的对象是不对的，要确保所有继承自IInclusionObject
//的对象都是在本库的堆里创建的。
struct TGIS_CORE_API IInclusionObject
{
	IInclusionObject();
	virtual ~IInclusionObject() {};

	//没有重载new[]和delete[]，
	//也就是说如果以数组方式申请的内存，本库内部是不负责释放的
	static void* operator new(size_t size);
	static void* operator new(size_t size, std::nothrow_t& nothrow_value) throw();
	static void operator delete(void *p);
	static void operator delete(void *p, std::nothrow_t& nothrow_value) throw();

protected:
	bool _is_in_heap;
};

END_NAME_SPACE(tGis, Core)


#endif
