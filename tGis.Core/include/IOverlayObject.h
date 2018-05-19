#pragma once

#ifndef __I_OVERLAYOBJECT_H__
#define __I_OVERLAYOBJECT_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IGeoSurface;

struct TGISCORE_API IOverlayObject
{
	virtual void Paint(IGeoSurface*) = 0;

	virtual ~IOverlayObject() {};
};


END_NAME_SPACE(tGis, Core)


#endif
