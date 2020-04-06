#pragma once

#ifndef __OVERLAYLAYER_H__
#define __OVERLAYLAYER_H__

#include "Helper.h"

#include "IOverlayLayer.h"
#include "IOverlayObject.h"

#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class OverlayLayerImpl;

class TGIS_CORE_API OverlayLayer : public IOverlayLayer
{
public:
	OverlayLayer();
	~OverlayLayer();

public:
	bool GetVisible();
	void SetVisible(bool);

	void AddOverlayObject(IOverlayObject*);
	void RemoveOverlayObject(IOverlayObject*);

	void Paint(IGeoSurface*);

private:
	OverlayLayerImpl* _impl_;
};


END_NAME_SPACE(tGis, Core)


#endif
