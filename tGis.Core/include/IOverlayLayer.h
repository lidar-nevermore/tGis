#pragma once

#ifndef __I_OVERLAYLAYER_H__
#define __I_OVERLAYLAYER_H__

#include "Helper.h"



BEGIN_NAME_SPACE(tGis, Core)

struct IOverlayObject;
struct IGeoSurface;

struct TGISCORE_API IOverlayLayer
{
	virtual bool GetVisible() = 0;
	virtual void SetVisible(bool) = 0;

	virtual void AddOverlayObject(IOverlayObject*) = 0;
	virtual void RemoveOverlayObject(IOverlayObject*) = 0;

	virtual void Paint(IGeoSurface*) = 0;

	virtual ~IOverlayLayer() {};
private:
	IOverlayLayer(const IOverlayLayer &) = delete;
	IOverlayLayer &operator=(const IOverlayLayer &) = delete;
};


END_NAME_SPACE(tGis, Core)


#endif
