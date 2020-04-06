#pragma once

#ifndef __I_DATASOURCE_H__
#define __I_DATASOURCE_H__

#include "Helper.h"
#include "ITypedObject.h"
#include "IInclusionObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

//�ⲿ��չʱ����ֱ��ʵ�ֱ��ӿڣ�ֻ����̳�DataSource
struct TGIS_CORE_API IDataSource : public ITypedObject, public IInclusionObject
{
	friend class DataSource; //Ϊ�˷���_is_in_heap

	//���뱣֤�ýӿ��κ�ʱ�򶼷���ͬһ��ָ��
	//ֻ���ַ���ֵһ����ŵ�ַ��һ��Ҳ�ǲ��Ե�
	//���DataSourceһ�����������Ʋ����޸�
	virtual const char* GetName() = 0;

	virtual bool IsConnected() = 0;
	virtual void Connect() = 0;
	virtual void Disconnect() = 0;
	virtual IDataSource* GetParent() = 0;

	virtual void Refresh() = 0;

	//���ۺϵĶ���û��˳��
	//ͨ���ڱ����ӿڴ���ص��������ۺ϶���
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



