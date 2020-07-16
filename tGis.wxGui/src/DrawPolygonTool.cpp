
#include "DrawPolygonTool.h"
#include "MessageFlag.inl"
#include "MapPanTool.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

DrawPolygonTool::DrawPolygonTool()
{
	_needRestore = false;
	_mapPanTool = nullptr;
	_isDrawing = false;
	_polygon.SetVisible(false);
	_msgFlag = MF_LBUTTON | MF_MOUSEMOVE | MF_RBUTTON | MF_VK_ESCAPE;
}


DrawPolygonTool::~DrawPolygonTool()
{
	if (_mapWidget != nullptr)
		_mapWidget->RemoveMapTool(this);
}

void DrawPolygonTool::SetMapWidget(IMapWidget * mapWidget)
{
	if (mapWidget == nullptr && _mapWidget != nullptr)
	{
		IOverlayLayer* overlay = _mapWidget->GetOverlayLayer();
		overlay->RemoveOverlayObject(&_polygon);
		wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
		widget->MouseEvent.Remove<DrawPolygonTool>(this, &DrawPolygonTool::MouseAll);
		widget->KeyUpEvent.Remove<>(this, &DrawPolygonTool::KeyUp);
	}
	_mapWidget = mapWidget;
	if(mapWidget != nullptr)
	{
		IOverlayLayer* overlay = mapWidget->GetOverlayLayer();
		overlay->AddOverlayObject(&_polygon);
		wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
		widget->MouseEvent.Add<DrawPolygonTool>(this, &DrawPolygonTool::MouseAll);
		widget->KeyUpEvent.Add<>(this, &DrawPolygonTool::KeyUp);
	}
}

void DrawPolygonTool::SetEnabled(bool enabled)
{
	if (enabled == false)
		_polygon.SetVisible(false);
	else
		_polygon.SetVisible(true);

	MapTool::SetEnabled(enabled);
}

void DrawPolygonTool::AttachPanTool(MapPanTool * mapPanTool)
{
	_mapPanTool = mapPanTool;
}

void DrawPolygonTool::MouseAll(wxGLMapWidget * s, wxMouseEvent * e)
{
	if (_needRestore && e->ShiftDown() == false)
	{
		_needRestore = false;
		SetEnabled(true);
	}

	if (_enabled == false)
		return;

	if (e->ShiftDown() && _mapPanTool != nullptr)
	{
		_needRestore = true;
		_mapPanTool->SetEnabled(true);
		_polygon.SetVisible(true);
		return;
	}

	if (e->Moving())
		MouseMove(s, e);
	else if (e->LeftUp())
		MouseLeftUp(s, e);
	else if (e->RightUp())
		MouseRightUp(s, e);
}

void DrawPolygonTool::MouseMove(wxGLMapWidget* s, wxMouseEvent * ev)
{
	if (false == _isDrawing)
		return;

	size_t c = _polygon.GetVertexCount();

	double x, y;
	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	viewPort->Surface2Spatial(ev->GetX(), ev->GetY(), &x, &y);
	_polygon.SetVertex(c - 1, x, y);
	_mapWidget->PresentMap();
}

void DrawPolygonTool::MouseLeftUp(wxGLMapWidget * s, wxMouseEvent * ev)
{
	if (false == _isDrawing)
	{
		_polygon.ClearVertex();
		_isDrawing = true;
	}

	double x, y;
	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	viewPort->Surface2Spatial(ev->GetX(), ev->GetY(), &x, &y);

	_polygon.AddVertex(x, y);
	size_t c = _polygon.GetVertexCount();
	if (c == 1)
		_polygon.AddVertex(x, y);
	else
		_polygon.SetVertex(c - 1, x, y);

	_mapWidget->PresentMap();	
}

void DrawPolygonTool::MouseRightUp(wxGLMapWidget * s, wxMouseEvent * ev)
{
	if (_isDrawing)
	{
		_isDrawing = false;
		_polygon.RemoveVertex(_polygon.GetVertexCount() - 1);
		_mapWidget->PresentMap();

		EndDraw(s);
	}
}

void DrawPolygonTool::KeyUp(wxGLMapWidget *s, wxKeyEvent *ev)
{
	if (_enabled == false)
		return;

	if (_isDrawing && ev->GetKeyCode() == WXK_ESCAPE)
	{
		size_t c = _polygon.GetVertexCount();
		if (c > 1)
		{
			_polygon.RemoveVertex(c - 1);
			_mapWidget->PresentMap();
		}
	}
}

void DrawPolygonTool::EndDraw(wxGLMapWidget *)
{
}

END_NAME_SPACE(tGis, Gui)
