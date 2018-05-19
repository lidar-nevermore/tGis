#pragma once

#include <QPainter>
#include <QPixmap>
#include "tEntity.h"

using namespace tGis::Core;

class QtGeoSurface : public GeoSurface
{
public:
	QtGeoSurface();
	~QtGeoSurface();

public:
	void SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B);

private:
	QPainter* _painter;
	QPixmap* _osSurf4Paint;
	QPixmap* _osSurf4Present;

	unsigned char _surfBackgroundR;
	unsigned char _surfBackgroundG;
	unsigned char _surfBackgroundB;

private:
	void EnsurePaintSurfaceValid();

public:
	void AttachQPainter(QPainter* painter);
	void DetachQPainter();
	void PresentSurface();
	void SwithSurface();


	void DrawPolyline(int count, int* surfX, int* surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt);
	void DrawPolygon(int count, int* surfX, int* surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt);
	void FillPolygon(int count, int* surfX, int* surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft);
	void DrawEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt);
	void FillEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft);
	void DrawRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt);
	void FillRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft);

	void DrawImage(const unsigned char* buf, int surfX, int surfY, int width, int height);
};

