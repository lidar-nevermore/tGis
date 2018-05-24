#pragma once

#ifndef __I_DATASET_H__
#define __I_DATASET_H__

#include "Helper.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataSource;

struct TGISCORE_API IDataset
{
	virtual const char* GetType() = 0;
	virtual const char* GetName() = 0;
	virtual const char* GetCreationString() = 0;

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


END_NAME_SPACE(tGis, Core)


#endif



