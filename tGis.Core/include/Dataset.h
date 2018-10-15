#pragma once

#ifndef __DATASET_H__
#define __DATASET_H__

#include "IDataset.h"

#include "Helper.h"


BEGIN_NAME_SPACE(tGis, Core)

struct IDataSource;

class TGIS_API Dataset : public IDataset
{
public:
	virtual ~Dataset();

protected:
	Dataset(IDataSource* ds);

public:
	virtual void Open();
	virtual void Close();

protected:
	IDataSource* _dataSource;
};


END_NAME_SPACE(tGis, Core)

#endif


