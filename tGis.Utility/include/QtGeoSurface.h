#pragma once

#ifndef __QTGEOSURFACE_H__
#define __QTGEOSURFACE_H__


#include <QPainter>
#include <QPixmap>
#include "tEntity.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

class QtGeoSurface : public GeoSurface
{
	friend class QMapWidget;
private:
	QtGeoSurface(QMapWidget* mapWidget);
	~QtGeoSurface();

private:
	QMapWidget* _mapWidget;
	bool _paintOnAttachedQPainter;
	QPainter* _painter;
	QPixmap* _osSurf4Paint;
	QPixmap* _osSurf4Present;

	unsigned char _surfBackgroundR;
	unsigned char _surfBackgroundG;
	unsigned char _surfBackgroundB;

private:
	inline void EnsurePaintSurfaceValid();
	inline Qt::PenStyle TranslateLineStyle(int lt);
	inline Qt::BrushStyle TranslateFillStyle(int ft);
	inline QPoint* CreateQPoints(int count, int* surfX, int* surfY);
	inline void DeleteQPoints(QPoint* pts);

	void SetViewSize(int surfW, int surfH);
	void AttachQPainter(QPainter* painter);
	void DetachQPainter();
	void PresentMap();
	void SwithSurface();
	void BeginPaintOnAttachedQPainter();
	void EndPaintOnAttachedQPainter();

public:
	void DrawPolyline(int count, int* surfX, int* surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt);
	void DrawPolygon(int count, int* surfX, int* surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt);
	void FillPolygon(int count, int* surfX, int* surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft);
	void DrawEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt);
	void FillEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft);
	void DrawRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt);
	void FillRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft);

	void DrawImage(const unsigned char* buf, int surfX, int surfY, int width, int height);
};


END_NAME_SPACE(tGis, Utility)


#endif

