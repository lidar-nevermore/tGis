#pragma once

#ifndef __I_OVERLAYOBJECT_H__
#define __I_OVERLAYOBJECT_H__

#include "Helper.h"
#include "tGisCoreCfg.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IGeoSurface;

struct TGIS_CORE_API IOverlayObject
{
	virtual bool GetVisible() = 0;
	virtual void SetVisible(bool) = 0;

	virtual void Paint(IGeoSurface*) = 0;

	virtual ~IOverlayObject() {};
};

typedef IOverlayObject* IOverlayObjectPtr;

END_NAME_SPACE(tGis, Core)


#endif
