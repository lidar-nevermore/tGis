#include "MapZoomTool.h"

#include "qevent.h"
#include "tVisualize.h"


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

	int degree = numDegrees.ry();
	int step = (int)(abs(degree / 120.0));
	step = step == 0 ? 1 : step;

	double newresolution = resolution;

	for (int i = 0; i < step; i++)
	{
		if (degree > 0)
		{
			newresolution *= 1.042; //375/360
		}
		else
		{
			newresolution *= 0.96; //360/375
		}
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
