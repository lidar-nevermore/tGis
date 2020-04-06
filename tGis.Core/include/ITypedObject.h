#pragma once

#ifndef __I_TYPEDOBJECT_H__
#define __I_TYPEDOBJECT_H__

#include "Helper.h"
#include "elr_mpl.h"


class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API ITypedObject
{
	ITypedObject() {};
	virtual ~ITypedObject() {};

	//用于判断数据源、数据集、图层、图层渲染器、可视对象的具体类型
	//不同具体类型上述对象上的操作不同
	//另外在保存工作空间时记录具体类型，然后加载工作空间时可以创建具体对象

	//需要用于区分具体类型的子类型才需要实现这几个接口
	virtual const char* GetType() = 0;
	virtual bool IsTypeOf(const char* type) = 0;
};

END_NAME_SPACE(tGis, Core)


#endif
