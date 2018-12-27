#include "MapZoomTool.h"

#include "qevent.h"
#include "tVisualize.h"


BEGIN_NAME_SPACE(tGis, Gui)


MapZoomTool::MapZoomTool()
{
}


MapZoomTool::~MapZoomTool()
{
}

void MapZoomTool::MouseWheel(void *ev)
{
	QWheelEvent* e = (QWheelEvent*)ev;
	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	double scale;
	viewPort->GetViewScale(&scale);

	int mx = e->x();
	int my = e->y();
	double mouseX1;
	double mouseY1;
	viewPort->Surface2Spatial(mx, my, &mouseX1, &mouseY1);

	QPoint numDegrees = e->angleDelta();

	int degree = numDegrees.ry();
	int step = (int)(abs(degree / 120.0));
	step = step == 0 ? 1 : step;

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
	}
}

END_NAME_SPACE(tGis, Gui)

