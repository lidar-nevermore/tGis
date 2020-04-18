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
	//如果子类的Open要调用基类的Open，请在最后调用
	virtual void Open();
	virtual bool IsOpened();
	//如果子类的Close要调用基类的Close，请在最先调用
	virtual void Close();
	virtual IDataSource* GetParent();

protected:
	bool _opened;
	DataSource* _parent;
};


END_NAME_SPACE(tGis, Core)

#endif


