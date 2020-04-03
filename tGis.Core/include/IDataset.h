#pragma once

#ifndef __I_DATASET_H__
#define __I_DATASET_H__

#include "Helper.h"
#include "ITGisObject.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataSource;

struct TGIS_CORE_API IDataset : public ITGisObject
{
	friend class DataSource;

	virtual const char* GetName() = 0;

	virtual bool IsReadOnly() = 0;
	virtual bool IsOpened() = 0;
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual IDataSource* GetDataSource() = 0;
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



