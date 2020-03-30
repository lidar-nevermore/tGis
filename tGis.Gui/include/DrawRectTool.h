#ifndef __DRAWRECTTOOL_H__
#define __DRAWRECTTOOL_H__

#include "tVisualize.h"
#include "tGisGuiCfg.h"

#include "QMapWidget.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API DrawRectTool : public MapTool
{
public:
	DrawRectTool();
	virtual ~DrawRectTool();

protected:
	virtual void SetMapWidget(IMapWidget* mapWidget);

	virtual void MouseDown(QMapWidget*, QMouseEvent*);
	virtual void MouseMove(QMapWidget*, QMouseEvent*);
	virtual void MouseUp(QMapWidget*, QMouseEvent*);


protected:
	OverlayRect _rect;
};

END_NAME_SPACE(tGis, Gui)


#endif

