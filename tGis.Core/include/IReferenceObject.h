#pragma once

#ifndef __I_ReferenceObject_H__
#define __I_ReferenceObject_H__

#include <cassert>
#include "Helper.h"
#include "elr_mpl.h"

#include "IInclusionObject.h"

BEGIN_NAME_SPACE(tGis, Core)


//继承IInclusionObject是为了让IReferenceObject在本库的堆里创建
//
//由于无法从语言层面约束一个类的子类都只能new，
//只能尽量从语言层面约束类的正确使用方式
//
//一个返回IReferenceObject的接口，
//如果具有创建性值，那么接口内部需要调用Reference
//具有创建性值也就是说IReferenceObject不可当作属性，
//比如从GradientColorRepository中获取GradientColor
//比如从SimpleSymbolLibrary中获取Symbol
//
//如果接口功能为访问IReferenceObject类型属性
//那么接口内部不可以调用Reference
//
//一个类调用返回IReferenceObject的接口后持有这个指针用于完成功能，
//那么这个类需要成对的调用Reference和Release
//符合这个情况时，IReferenceObject和本类具有相同的生命周期
//
struct TGIS_CORE_API IReferenceObject : public IInclusionObject
{
	IReferenceObject() 
	{
		_refCount = 1;
	};

	virtual ~IReferenceObject() {};

public:
	//Reference 和 Release 不被允许改写（没有多态特性）
	//也就是说要求，必须在构造和析构函数里实现相关需求
	void Reference()
	{
		assert(_is_in_heap);

		_refCount++;
	}

	void Release()
	{
		assert(_is_in_heap);

		if (_refCount > 0)
			_refCount--;
		if (_refCount == 0)
			delete this;
	}

private:
	int _refCount;
};

END_NAME_SPACE(tGis, Core)


#endif
