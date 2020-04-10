#pragma once

#include <windows.h>
#include "GeoSurface.h"
#include <GdiPlus.h>  

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, wGui)

class GdiGeoSurface : public GeoSurface
{
	friend class MFCMapWidget;
public:
	GdiGeoSurface();
	~GdiGeoSurface();

	HWND _win;

protected:
	Gdiplus::Bitmap *_bitmap;

protected:
	void SetViewPort(GeoViewPort* viewPort);

public:
	// ͨ�� GeoSurface �̳�
	virtual void Present(IWidget * w, int wX, int wY) override;

	virtual void Present(IWidget * w, int wX, int wY, int wW, int wH) override;

	virtual void BeginPaint(IWidget * w, bool isCache) override;

	virtual void EndPaint(IWidget * w, bool isCache) override;

	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY);
	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY, int surfW, int surfH);
};

END_NAME_SPACE(tGis, wGui)
