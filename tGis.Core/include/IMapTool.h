#pragma once

#ifndef __I_MAPTOOL_H__
#define __I_MAPTOOL_H__

#include "Helper.h"


BEGIN_NAME_SPACE(tGis, Core)

struct IMapWidget;

struct TGISCORE_API IMapTool
{
	virtual void SetMapWidget(IMapWidget* mapWidget) = 0;

	virtual void SetEnabled(bool enabled) = 0;
	virtual bool GetEnabled() = 0;

	virtual void MouseDown(void*) = 0;
	virtual void MouseMove(void*) = 0;
	virtual void MouseUp(void*) = 0;
	virtual void MouseClicked(void*) = 0;
	virtual void MouseDoubleClicked(void*) = 0;
	virtual void MouseWheel(void*) = 0;

	virtual void KeyDown(void*) = 0;
	virtual void KeyUp(void*) = 0;
	virtual void KeyPress(void*) = 0;

	IMapTool() {};
	virtual ~IMapTool() {};
private:
	IMapTool(const IMapTool &) = delete;
	IMapTool &operator=(const IMapTool &) = delete;
};

typedef IMapTool* IMapToolPtr;

END_NAME_SPACE(tGis, Core)


#endif
