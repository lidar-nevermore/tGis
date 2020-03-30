
#include "qevent.h"

#include "DrawRectTool.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

DrawRectTool::DrawRectTool()
{
	_rect.SetVisible(false);
}


DrawRectTool::~DrawRectTool()
{
	if (_mapWidget != nullptr)
		_mapWidget->RemoveMapTool(this);
}

void DrawRectTool::SetMapWidget(IMapWidget * mapWidget)
{
	if (mapWidget == nullptr && _mapWidget != nullptr)
	{
		IOverlayLayer* overlay = _mapWidget->GetOverlayLayer();
		overlay->RemoveOverlayObject(&_rect);
		QMapWidget* widget = (QMapWidget*)_mapWidget;
		widget->MousePressEvent.Remove<DrawRectTool>(this, &DrawRectTool::MouseDown);
		widget->MouseMoveEvent.Remove<DrawRectTool>(this, &DrawRectTool::MouseMove);
		widget->MouseReleaseEvent.Remove<DrawRectTool>(this, &DrawRectTool::MouseUp);
	}
	_mapWidget = mapWidget;
	if(mapWidget != nullptr)
	{
		IOverlayLayer* overlay = mapWidget->GetOverlayLayer();
		overlay->AddOverlayObject(&_rect);
		QMapWidget* widget = (QMapWidget*)_mapWidget;
		widget->MousePressEvent.Add<DrawRectTool>(this, &DrawRectTool::MouseDown);
		widget->MouseMoveEvent.Add<DrawRectTool>(this, &DrawRectTool::MouseMove);
		widget->MouseReleaseEvent.Add<DrawRectTool>(this, &DrawRectTool::MouseUp);
	}
}


void DrawRectTool::MouseDown(QMapWidget* s, QMouseEvent * ev)
{
	if (_enabled == false)
		return;

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

void DrawRectTool::MouseMove(QMapWidget* s, QMouseEvent * ev)
{
	if (_enabled == false)
		return;

	QMouseEvent* e = (QMouseEvent*)ev;
	Qt::MouseButtons buttons = e->buttons();
	if (buttons & Qt::LeftButton)
	{
		_rect._right = e->x();
		_rect._bottom = e->y();
		_mapWidget->PresentMap();
	}
}

void DrawRectTool::MouseUp(QMapWidget* s, QMouseEvent *)
{
	if (_enabled == false)
		return;

	_rect.SetVisible(false);
	_mapWidget->PresentMap();
}

END_NAME_SPACE(tGis, Gui)
