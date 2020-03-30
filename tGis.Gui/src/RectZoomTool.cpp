#include "RectZoomTool.h"


BEGIN_NAME_SPACE(tGis, Gui)


RectZoomTool::RectZoomTool()
{
}


RectZoomTool::~RectZoomTool()
{
	if (_mapWidget != nullptr)
		_mapWidget->RemoveMapTool(this);
}

void RectZoomTool::SetMapWidget(IMapWidget * mapWidget)
{
	if (mapWidget == nullptr && _mapWidget != nullptr)
	{
		IOverlayLayer* overlay = _mapWidget->GetOverlayLayer();
		overlay->RemoveOverlayObject(&_rect);
		QMapWidget* widget = (QMapWidget*)_mapWidget;
		widget->MousePressEvent.Remove<RectZoomTool>(this, &RectZoomTool::MouseDown);
		widget->MouseMoveEvent.Remove<RectZoomTool>(this, &RectZoomTool::MouseMove);
		widget->MouseReleaseEvent.Remove<>(this, &RectZoomTool::MouseUp);
	}
	_mapWidget = mapWidget;
	if (mapWidget != nullptr)
	{
		IOverlayLayer* overlay = mapWidget->GetOverlayLayer();
		overlay->AddOverlayObject(&_rect);
		QMapWidget* widget = (QMapWidget*)_mapWidget;
		widget->MousePressEvent.Add<RectZoomTool>(this, &RectZoomTool::MouseDown);
		widget->MouseMoveEvent.Add<RectZoomTool>(this, &RectZoomTool::MouseMove);
		widget->MouseReleaseEvent.Add<>(this, &RectZoomTool::MouseUp);
	}
}


void RectZoomTool::MouseUp(QMapWidget* s, QMouseEvent * ev)
{
	if (_enabled == false)
		return;

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

END_NAME_SPACE(tGis, Gui)

