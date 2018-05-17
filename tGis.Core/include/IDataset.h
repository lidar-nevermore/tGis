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
	virtual const char* GetOpenString() = 0;

	virtual bool IsOpened() = 0;
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual IDataSource* GetDataSource() = 0;
	virtual const OGREnvelope* GetEnvelope() = 0;
	virtual const OGRSpatialReference* GetSpatialReference() = 0;

	virtual ~IDataset() {};
};


END_NAME_SPACE(tGis, Core)


#endif



