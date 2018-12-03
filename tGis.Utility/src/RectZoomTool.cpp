#include "RectZoomTool.h"


BEGIN_NAME_SPACE(tGis, Utility)


RectZoomTool::RectZoomTool()
{
}


RectZoomTool::~RectZoomTool()
{
}

void RectZoomTool::MouseUp(void * ev)
{
	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	double left;
	double top;
	double right;
	double bottom;
	viewPort->Surface2Spatial(_rect._left, _rect._top, &left, &top);
	viewPort->Surface2Spatial(_rect._right, _rect._bottom, &right, &bottom);
	viewPort->IncludeEnvelope(left, top, right, bottom);
	_rect.SetVisible(false);
	_mapWidget->RepaintMap();
}

END_NAME_SPACE(tGis, Utility)

