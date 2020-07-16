#ifndef __DrawPolygonTool_H__
#define __DrawPolygonTool_H__

#include "tGis_Visualize.h"
#include "tGis_wxGuiCfg.h"

#include "wxGLMapWidget.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class MapPanTool;

class TGIS_GUI_API DrawPolygonTool : public MapTool
{
public:
	DrawPolygonTool();
	virtual ~DrawPolygonTool();

public:
	virtual void SetEnabled(bool enabled);
	void AttachPanTool(MapPanTool* mapPanTool);

protected:
	virtual void SetMapWidget(IMapWidget* mapWidget);


	virtual void MouseAll(wxGLMapWidget*, wxMouseEvent*);

	virtual void MouseMove(wxGLMapWidget*, wxMouseEvent*);
	virtual void MouseLeftUp(wxGLMapWidget*, wxMouseEvent*);
	virtual void MouseRightUp(wxGLMapWidget*, wxMouseEvent*);

	virtual void KeyUp(wxGLMapWidget*, wxKeyEvent*);

	virtual void EndDraw(wxGLMapWidget*);

protected:
	bool _needRestore;
	MapPanTool* _mapPanTool;
	bool _isDrawing;
	OverlayPolygon _polygon;
};

END_NAME_SPACE(tGis, Gui)


#endif

