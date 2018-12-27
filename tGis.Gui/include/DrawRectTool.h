#ifndef __DRAWRECTTOOL_H__
#define __DRAWRECTTOOL_H__

#include "tVisualize.h"
#include "tGisGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API DrawRectTool : public MapTool
{
public:
	DrawRectTool();
	virtual ~DrawRectTool();

public:
	virtual void SetMapWidget(IMapWidget* mapWidget);

	virtual void MouseDown(void*);
	virtual void MouseMove(void*);
	virtual void MouseUp(void*);


protected:
	OverlayRect _rect;
};

END_NAME_SPACE(tGis, Gui)


#endif

