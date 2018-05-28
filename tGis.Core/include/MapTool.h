#pragma once

#ifndef __MAPTOOL_H__
#define __MAPTOOL_H__

#include "Helper.h"

#include "IMapTool.h"

BEGIN_NAME_SPACE(tGis, Core)


class TGISCORE_API MapTool : public IMapTool
{
public:
	MapTool();
	virtual ~MapTool();

	virtual void SetMapWidget(IMapWidget* mapWidget);

	virtual void SetEnabled(bool enabled);
	virtual bool GetEnabled();

	virtual void MouseDown(void*);
	virtual void MouseMove(void*);
	virtual void MouseUp(void*);
	virtual void MouseClicked(void*);
	virtual void MouseDoubleClicked(void*);
	virtual void MouseWheel(void*);

	virtual void KeyDown(void*);
	virtual void KeyUp(void*);
	virtual void KeyPress(void*);

protected:
	IMapWidget* _mapWidget;

private:
	bool _enabled;
};


END_NAME_SPACE(tGis, Core)


#endif

