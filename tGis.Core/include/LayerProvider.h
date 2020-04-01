#pragma once

#ifndef __LAYERPROVIDER_H__
#define __LAYERPROVIDER_H__


#include "Helper.h"

#include "ILayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_CORE_API LayerProvider : public ILayerProvider
{
protected:
	LayerProvider();
	LayerProvider(const LayerProvider &) = delete;
	LayerProvider &operator=(const LayerProvider &) = delete;

public:
	virtual ~LayerProvider();

public:

	virtual ILayer* CreateLayer(IDataset* dataset, const char* creationString);
	virtual void ReleaseLayer(ILayer*);

	virtual void Release();
};


END_NAME_SPACE(tGis, Core)


#endif


