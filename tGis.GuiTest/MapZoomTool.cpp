#include "MapZoomTool.h"

#include "qevent.h"
#include "tUtility.h"


MapZoomTool::MapZoomTool()
{
}


MapZoomTool::~MapZoomTool()
{
}

void MapZoomTool::MouseWheel(void *ev)
{
	QWheelEvent* e = (QWheelEvent*)ev;
	IGeoSurface* surface = _mapWidget->GetGeoSurface();
	double resolution;
	surface->GetViewPort(nullptr, nullptr, nullptr, nullptr, &resolution);

	int mx = e->x();
	int my = e->y();
	double mouseX1;
	double mouseY1;
	surface->Surface2Spatial(mx, my, &mouseX1, &mouseY1);

	QPoint numDegrees = e->angleDelta();

	double newresolution = resolution*(1.0 + numDegrees.ry() / 960.0);

	if (newresolution < 0.0)
	{
		newresolution = 0.0000000000000000001;
	}
	surface->SetViewResolution(newresolution);

	double mouseX2;
	double mouseY2;
	surface->Surface2Spatial(mx, my, &mouseX2, &mouseY2);
	double spatialCenterX;
	double spatialCenterY;
	surface->GetViewPort(&spatialCenterX, &spatialCenterY, nullptr, nullptr, nullptr);
	surface->SetViewCenter(spatialCenterX + mouseX1 - mouseX2, spatialCenterY + mouseY1 - mouseY2);
	if (newresolution > resolution)
	{
		_mapWidget->RepaintMap();
	}
	else
	{
		_mapWidget->PresentSurface();
	}
}
