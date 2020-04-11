
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
		wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
		widget->MouseEvent.Remove<DrawRectTool>(this, &DrawRectTool::MouseAll);
	}
	_mapWidget = mapWidget;
	if(mapWidget != nullptr)
	{
		IOverlayLayer* overlay = mapWidget->GetOverlayLayer();
		overlay->AddOverlayObject(&_rect);
		wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
		widget->MouseEvent.Add<DrawRectTool>(this, &DrawRectTool::MouseAll);
	}
}


void DrawRectTool::MouseAll(wxGLMapWidget * s, wxMouseEvent * e)
{
	if (_enabled == false)
		return;

	if (e->LeftDown())
		MouseDown(s, e);
	else if (e->Dragging())
		MouseMove(s, e);
	else if (e->LeftUp())
		MouseUp(s, e);
}

void DrawRectTool::MouseDown(wxGLMapWidget* s, wxMouseEvent * e)
{
	_rect.SetVisible(true);
	_rect._left = e->GetX();
	_rect._top = e->GetY();
	_rect._right = e->GetX();
	_rect._bottom = e->GetY();
}

void DrawRectTool::MouseMove(wxGLMapWidget* s, wxMouseEvent * ev)
{
	wxMouseEvent* e = (wxMouseEvent*)ev;
	_rect._right = e->GetX();
	_rect._bottom = e->GetY();
	_mapWidget->PresentMap();
}

void DrawRectTool::MouseUp(wxGLMapWidget* s, wxMouseEvent *)
{
	_rect.SetVisible(false);
	_mapWidget->PresentMap();
}

END_NAME_SPACE(tGis, Gui)
