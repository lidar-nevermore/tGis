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
	void FillRect(int surfX, int surfY, int width, int height, unsigned char R, unsigned char G, unsigned char B, unsigned char A);
	void DrawImage(const unsigned char* buf, int surfX, int surfY, int width, int height);
};

