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

//�ⲿ��չʱ����ֱ��ʵ�ֱ��ӿڣ�ֻ����̳�Dataset
struct TGIS_CORE_API IDataset : public ITypedObject, public IInclusionObject
{
	friend class DataSource; //Ϊ�˷���_is_in_heap

	//���뱣֤�ýӿ��κ�ʱ�򶼷���ͬһ��ָ��
	//ֻ���ַ���ֵһ����ŵ�ַ��һ��Ҳ�ǲ��Ե�
	//���Datasetһ�����������Ʋ����޸�
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



