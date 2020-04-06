#pragma once

#ifndef __I_DATASOURCE_H__
#define __I_DATASOURCE_H__

#include "Helper.h"
#include "ITypedObject.h"
#include "IInclusionObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

//外部扩展时不得直接实现本接口，只允许继承DataSource
struct TGIS_CORE_API IDataSource : public ITypedObject, public IInclusionObject
{
	friend class DataSource; //为了访问_is_in_heap

	//必须保证该接口任何时候都返回同一个指针
	//只是字符串值一样存放地址不一样也是不对的
	//因此DataSource一旦创建其名称不得修改
	virtual const char* GetName() = 0;

	virtual bool IsConnected() = 0;
	virtual void Connect() = 0;
	virtual void Disconnect() = 0;
	virtual IDataSource* GetParent() = 0;

	virtual void Refresh() = 0;

	//被聚合的对象没有顺序
	//通过在遍历接口传入回调遍历被聚合对象
	virtual size_t GetDatasetCount() = 0;
	//virtual void ForEachDataset(void(*pfunc)(IDataset* dt, void* ud)) = 0;

	virtual size_t GetDataSourceCount() = 0;
	//virtual void ForEachDataSource(void(*pfunc)(IDataSource* ds, void* ud)) = 0;

	IDataSource() {};
	virtual ~IDataSource() {};
private:
	IDataSource(const IDataSource &) = delete;
	IDataSource &operator=(const IDataSource &) = delete;
};

typedef IDataSource* IDataSourcePtr;

END_NAME_SPACE(tGis, Core)



#endif



