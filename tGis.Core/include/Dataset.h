#pragma once

#ifndef __DATASET_H__
#define __DATASET_H__

#include "IDataset.h"

#include "Helper.h"


BEGIN_NAME_SPACE(tGis, Core)

class DataSource;

class TGIS_CORE_API Dataset : public IDataset
{
public:
	virtual ~Dataset();

protected:
	Dataset(DataSource* ds);

public:
	//��������OpenҪ���û����Open������������
	virtual void Open();
	virtual bool IsOpened();
	//��������CloseҪ���û����Close���������ȵ���
	virtual void Close();
	virtual IDataSource* GetParent();

protected:
	bool _opened;
	DataSource* _parent;
};


END_NAME_SPACE(tGis, Core)

#endif


