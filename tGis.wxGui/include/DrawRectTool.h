#ifndef __DRAWRECTTOOL_H__
#define __DRAWRECTTOOL_H__

#include "tGis_Visualize.h"
#include "tGis_wxGuiCfg.h"

#include "wxGLMapWidget.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API DrawRectTool : public MapTool
{
public:
	DrawRectTool();
	virtual ~DrawRectTool();

protected:
	virtual void SetMapWidget(IMapWidget* mapWidget);

	virtual void MouseAll(wxGLMapWidget*, wxMouseEvent*);
	virtual void MouseDown(wxGLMapWidget*, wxMouseEvent*);
	virtual void MouseMove(wxGLMapWidget*, wxMouseEvent*);
	virtual void MouseUp(wxGLMapWidget*, wxMouseEvent*);

protected:
	OverlayRect _rect;
};

END_NAME_SPACE(tGis, Gui)


#endif

