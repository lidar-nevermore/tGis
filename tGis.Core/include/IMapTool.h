#pragma once

#ifndef __I_MAPTOOL_H__
#define __I_MAPTOOL_H__

#include "Helper.h"
#include "tGis_CoreCfg.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IMapWidget;

struct TGIS_CORE_API IMapTool
{
	friend class MapWidget;

	virtual IMapWidget* GetMapWidget() = 0;

	virtual void SetEnabled(bool enabled) = 0;
	virtual bool GetEnabled() = 0;

	IMapTool() {};
	virtual ~IMapTool() {};

protected:
	virtual void SetMapWidget(IMapWidget* mapWidget) = 0;

private:
	IMapTool(const IMapTool &) = delete;
	IMapTool &operator=(const IMapTool &) = delete;
};

typedef IMapTool* IMapToolPtr;

END_NAME_SPACE(tGis, Core)


#endif
