#include "qevent.h"
#include "tVisualize.h"

#include "MapPanTool.h"

BEGIN_NAME_SPACE(tGis, Utility)

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
		IGeoSurface* surface = _mapWidget->GetGeoSurface();
		_mouseDownX = e->x();
		_mouseDownY = e->y();
		surface->Surface2Spatial(_mouseDownX, _mouseDownY, &_mouseDownSpatialX, &_mouseDownSpatialY);
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
			IGeoSurface* surface = _mapWidget->GetGeoSurface();
			double mouseX;
			double mouseY;
			surface->Surface2Spatial(mx, my, &mouseX, &mouseY);

			double spatialCenterX;
			double spatialCenterY;
			surface->GetViewPort(&spatialCenterX, &spatialCenterY, nullptr);
			surface->SetViewCenter(spatialCenterX + _mouseDownSpatialX - mouseX, spatialCenterY + _mouseDownSpatialY - mouseY);
			_mapWidget->PresentMap();
		}
	}
}

void MapPanTool::MouseUp(void* ev)
{
	_mapWidget->RepaintMap();
}

END_NAME_SPACE(tGis, Utility)

