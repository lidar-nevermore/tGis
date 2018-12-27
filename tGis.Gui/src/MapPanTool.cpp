#include "qevent.h"
#include "tVisualize.h"

#include "MapPanTool.h"

BEGIN_NAME_SPACE(tGis, Gui)

MapPanTool::MapPanTool()
{
	_mouseDownX = -1;
	_mouseDownY = -1;
}


MapPanTool::~MapPanTool()
{
}

void MapPanTool::MouseDown(void * ev)
{
	QMouseEvent* e = (QMouseEvent*)ev;
	Qt::MouseButtons buttons = e->buttons();
	if(buttons & Qt::LeftButton)
	{ 
		GeoViewPort* viewPort = _mapWidget->GetViewPort();
		_mouseDownX = e->x();
		_mouseDownY = e->y();
		viewPort->Surface2Spatial(_mouseDownX, _mouseDownY, &_mouseDownSpatialX, &_mouseDownSpatialY);
	}
}

void MapPanTool::MouseMove(void *ev)
{
	QMouseEvent* e = (QMouseEvent*)ev;
	Qt::MouseButtons buttons = e->buttons();
	if (buttons & Qt::LeftButton)
	{
		int mx = e->x();
		int my = e->y();
		if (mx != _mouseDownX && my != _mouseDownY)
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
}

void MapPanTool::MouseUp(void* ev)
{
	_mapWidget->RepaintMap();
}

END_NAME_SPACE(tGis, Gui)

