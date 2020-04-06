#pragma once

#include <windows.h>
#include "GeoSurface.h"
#include <GdiPlus.h>  

using namespace tGis::Core;

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
	// Í¨¹ý GeoSurface ¼Ì³Ð
	virtual void Present(IWidget * w, int wX, int wY) override;

	virtual void Present(IWidget * w, int wX, int wY, int wW, int wH) override;

	virtual void BeginPaint(IWidget * w, bool isCache) override;

	virtual void EndPaint(IWidget * w, bool isCache) override;

	virtual void DrawPolyline(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt) override;

	virtual void DrawPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt) override;

	virtual void FillPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft) override;

	virtual void DrawEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt) override;

	virtual void FillEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft) override;

	virtual void DrawRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt) override;

	virtual void FillRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft) override;

	virtual void DrawImage(const unsigned char * buf, int surfX, int surfY, int width, int height) override;
};

