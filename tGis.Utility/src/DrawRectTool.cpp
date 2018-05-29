#include "DrawRectTool.h"

#include "qevent.h"
#include "tVisualize.h"

BEGIN_NAME_SPACE(tGis, Utility)

DrawRectTool::DrawRectTool()
{
	_rect.SetVisible(false);
}


DrawRectTool::~DrawRectTool()
{

}

void DrawRectTool::SetMapWidget(IMapWidget * mapWidget)
{
	if (mapWidget == nullptr && _mapWidget != nullptr)
	{
		IOverlayLayer* overlay = _mapWidget->GetOverlayLayer();
		overlay->RemoveOverlayObject(&_rect);
	}
	MapTool::SetMapWidget(mapWidget);
	if(mapWidget != nullptr)
	{
		IOverlayLayer* overlay = mapWidget->GetOverlayLayer();
		overlay->AddOverlayObject(&_rect);
	}
}


void DrawRectTool::MouseDown(void * ev)
{
	QMouseEvent* e = (QMouseEvent*)ev;
	Qt::MouseButtons buttons = e->buttons();
	if (buttons & Qt::LeftButton)
	{
		IGeoSurface* surface = _mapWidget->GetGeoSurface();
		_rect.SetVisible(true);
		_rect._left = e->x();
		_rect._top = e->y();
	}
}

void DrawRectTool::MouseMove(void * ev)
{
	QMouseEvent* e = (QMouseEvent*)ev;
	Qt::MouseButtons buttons = e->buttons();
	if (buttons & Qt::LeftButton)
	{
		_rect._right = e->x();
		_rect._bottom = e->y();
		_mapWidget->PresentSurface();
	}
}

void DrawRectTool::MouseUp(void *)
{
	_rect.SetVisible(false);
	_mapWidget->PresentSurface();
}

END_NAME_SPACE(tGis, Utility)
