#pragma once

#ifndef __MAPTOOL_H__
#define __MAPTOOL_H__

#include "Helper.h"

#include "IMapTool.h"

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_CORE_API MapTool : public IMapTool
{
	friend class MapWidget;
public:
	MapTool();
	virtual ~MapTool();

	virtual IMapWidget* GetMapWidget();

	virtual void SetEnabled(bool enabled);
	virtual bool GetEnabled();

protected:
	IMapWidget* _mapWidget;
	virtual void SetMapWidget(IMapWidget* mapWidget);
	bool _enabled;
};


END_NAME_SPACE(tGis, Core)


#endif

