
#include "qevent.h"
#include "DrawRectTool.h"


BEGIN_NAME_SPACE(tGis, Gui)

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
		_rect.SetVisible(true);
		_rect._left = e->x();
		_rect._top = e->y();
		_rect._right = e->x();
		_rect._bottom = e->y();
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
		_mapWidget->PresentMap();
	}
}

void DrawRectTool::MouseUp(void *)
{
	_rect.SetVisible(false);
	_mapWidget->PresentMap();
}

END_NAME_SPACE(tGis, Gui)