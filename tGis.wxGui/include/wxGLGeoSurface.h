#pragma once

#ifndef __WXGL_GEOSURFACE_H__
#define __WXGL_GEOSURFACE_H__

#include "tGis_wxGuiCfg.h"

#include "wx/glcanvas.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class wxGLMapWidget;

class TGIS_GUI_API wxGLGeoSurface : public GeoSurface
{
public:
	wxGLGeoSurface(wxGLMapWidget* mapWidget);
	virtual ~wxGLGeoSurface();

protected:
	wxGLMapWidget* _mapWidget;
	wxGLContext _glContext;
	void* _mapBuffer;
	int _bufWidth;
	int _bufHeight;
	int _mapWidth;
	int _mapHeight;

public:
	// 通过 GeoSurface 继承
	virtual void Present(int wX, int wY) override;
	virtual void Present(int wX, int wY, int wW, int wH) override;
	virtual void BeginPaint(bool isCache) override;
	virtual void EndPaint(bool isCache) override;

	//绘制接口
	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY) override;
	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY, int surfW, int surfH) override;
};

END_NAME_SPACE(tGis, Gui)

#endif