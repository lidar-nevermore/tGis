#include "MapZoomTool.h"


BEGIN_NAME_SPACE(tGis, Gui)


MapZoomTool::MapZoomTool()
{
	_delayRepaint = false;
}


MapZoomTool::~MapZoomTool()
{
	if (_mapWidget != nullptr)
		_mapWidget->RemoveMapTool(this);
}

void MapZoomTool::SetMapWidget(IMapWidget * mapWidget)
{
	if (mapWidget == nullptr && _mapWidget != nullptr)
	{
		wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
		widget->MouseEvent.Remove<>(this, &MapZoomTool::MouseWheel);
		widget->MouseEvent.Remove<>(this, &MapZoomTool::MouseRightUp);
	}
	_mapWidget = mapWidget;
	if (mapWidget != nullptr)
	{
		wxGLMapWidget* widget = (wxGLMapWidget*)_mapWidget;
		widget->MouseEvent.Add<>(this, &MapZoomTool::MouseWheel);
		widget->MouseEvent.Add<>(this, &MapZoomTool::MouseRightUp);
	}
}


void MapZoomTool::MouseWheel(wxGLMapWidget*, wxMouseEvent *e)
{
	if (_enabled == false)
		return;

	int degree = e->GetWheelDelta();

	if (degree == 0)
		return;

	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	double scale;
	viewPort->GetViewScale(&scale);

	int mx = e->GetX();
	int my = e->GetY();
	double mouseX1;
	double mouseY1;
	viewPort->Surface2Spatial(mx, my, &mouseX1, &mouseY1);

	int dir =  e->GetWheelRotation();
	if (dir < 0)
		degree = -degree;

	int step = (int)(abs(degree / 120.0));
	step = step == 0 ? 1 : step;

	if (e->ShiftDown())
		step = step * 2;

	double newScale = scale;

	for (int i = 0; i < step; i++)
	{
		if (degree > 0)
		{
			newScale *= 0.96; //360/375
		}
		else
		{
			newScale *= 1.042; //375/360
		}
	}

	viewPort->SetViewScale(newScale);

	double mouseX2;
	double mouseY2;
	viewPort->Surface2Spatial(mx, my, &mouseX2, &mouseY2);
	double spatialCenterX;
	double spatialCenterY;
	viewPort->GetSpatialCenter(&spatialCenterX, &spatialCenterY);
	viewPort->SetSpatialCenter(spatialCenterX + mouseX1 - mouseX2, spatialCenterY + mouseY1 - mouseY2);
	if (newScale > scale)
	{
		_mapWidget->RepaintMap();
	}
	else
	{
		_mapWidget->PresentMap();
		_delayRepaint = true;
	}
}

void MapZoomTool::MouseRightUp(wxGLMapWidget *, wxMouseEvent *e)
{
	if (_delayRepaint && e->RightUp())
	{
		_mapWidget->RepaintMap();
		_delayRepaint = false;
	}
}

END_NAME_SPACE(tGis, Gui)

