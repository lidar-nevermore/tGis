#pragma once

#ifndef __I_DATASET_H__
#define __I_DATASET_H__

#include "Helper.h"
#include "ITypedObject.h"
#include "IInclusionObject.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataSource;

//外部扩展时不得直接实现本接口，只允许继承Dataset
struct TGIS_CORE_API IDataset : public ITypedObject, public IInclusionObject
{
	friend class DataSource; //为了访问_is_in_heap

	//必须保证该接口任何时候都返回同一个指针
	//只是字符串值一样存放地址不一样也是不对的
	//因此Dataset一旦创建其名称不得修改
	virtual const char* GetName() = 0;

	virtual bool IsReadOnly() = 0;
	virtual bool IsOpened() = 0;
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual IDataSource* GetParent() = 0;
	virtual const OGREnvelope* GetEnvelope() = 0;
	virtual const OGRSpatialReference* GetSpatialReference() = 0;

	IDataset() {};
	virtual ~IDataset() {};
private:
	IDataset(const IDataset &) = delete;
	IDataset &operator=(const IDataset &) = delete;
};

typedef IDataset* IDatasetPtr;

END_NAME_SPACE(tGis, Core)


#endif



