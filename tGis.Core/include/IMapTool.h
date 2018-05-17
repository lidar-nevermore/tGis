#pragma once

#ifndef __I_MAPTOOL_H__
#define __I_MAPTOOL_H__

#include "Helper.h"


BEGIN_NAME_SPACE(tGis, Core)

struct IMapWidget;

struct TGISCORE_API IMapTool
{
	virtual void SetMapWidget(IMapWidget* mapWidget) = 0;
	//响应相同消息的两个工具不兼容
	virtual bool IsCompatible(IMapTool*) = 0;

	virtual void CleanUp() = 0;
	virtual void MouseDown(void*) = 0;
	virtual void MouseMove(void*) = 0;
	virtual void MouseUp(void*) = 0;
	virtual void MouseClicked(void*) = 0;
	virtual void MouseDoubleClicked(void*) = 0;
	virtual void MouseWheel(void*) = 0;

	virtual void KeyDown(void*) = 0;
	virtual void KeyUp(void*) = 0;
	virtual void KeyPress(void*) = 0;

	virtual ~IMapTool() {};
};


END_NAME_SPACE(tGis, Core)


#endif
