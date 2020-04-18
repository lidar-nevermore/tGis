
#include "MapPanTool.h"

BEGIN_NAME_SPACE(tGis, Gui)

MapPanTool::MapPanTool()
{
	_mouseDownX = -1;
	_mouseDownY = -1;
}


MapPanTool::~MapPanTool()
{
	if (_mapWidget != nullptr)
		_mapWidget->RemoveMapTool(this);
}

void MapPanTool::SetMapWidget(IMapWidget * mapWidget)
{
	if (mapWidget == nullptr && _mapWidget != nullptr)
	{
		wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
		widget->MouseEvent.Remove<MapPanTool>(this, &MapPanTool::MouseAll);
	}
	_mapWidget = mapWidget;
	if (mapWidget != nullptr)
	{
		wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
		widget->MouseEvent.Add<MapPanTool>(this, &MapPanTool::MouseAll);
	}
}

void MapPanTool::MouseAll(wxGLMapWidget * s, wxMouseEvent * e)
{
	if (_enabled == false)
		return;

	if (e->LeftDown())
		MouseDown(s, e);
	else if (e->Dragging())
		MouseMove(s, e);
	else if (e->LeftUp() || e->Leaving())
		MouseUp(s, e);
}

void MapPanTool::MouseDown(wxGLMapWidget*, wxMouseEvent * e)
{
	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	_mouseDownX = e->GetX();
	_mouseDownY = e->GetY();
	viewPort->Surface2Spatial(_mouseDownX, _mouseDownY, &_mouseDownSpatialX, &_mouseDownSpatialY);
}

void MapPanTool::MouseMove(wxGLMapWidget*, wxMouseEvent *e)
{
	int mx = e->GetX();
	int my = e->GetY();
	if ((_mouseDownX != -1 || _mouseDownY != -1) && mx != _mouseDownX && my != _mouseDownY )
	{
		GeoViewPort* viewPort = _mapWidget->GetViewPort();
		double mouseX;
		double mouseY;
		viewPort->Surface2Spatial(mx, my, &mouseX, &mouseY);

		double spatialCenterX;
		double spatialCenterY;
		viewPort->GetSpatialCenter(&spatialCenterX, &spatialCenterY);
		viewPort->SetSpatialCenter(spatialCenterX + _mouseDownSpatialX - mouseX, spatialCenterY + _mouseDownSpatialY - mouseY);
		_mapWidget->PresentMap();
	}
}

void MapPanTool::MouseUp(wxGLMapWidget*, wxMouseEvent* ev)
{
	_mapWidget->RepaintMap();
}

END_NAME_SPACE(tGis, Gui)

