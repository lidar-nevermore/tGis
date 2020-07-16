#pragma once

#ifndef __OverlayPolygon_H__
#define __OverlayPolygon_H__

#include "Helper.h"

#include "IOverlayObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IGeoSurface;
struct ILineSymbol;
struct IFillSymbol;

class OverlayPolygonImpl;

class TGIS_CORE_API OverlayPolygon : public IOverlayObject
{
public:
	OverlayPolygon();
	OverlayPolygon(double* x, double* y, int size);
	virtual ~OverlayPolygon();

	virtual bool GetVisible();
	virtual void SetVisible(bool);

	virtual void Paint(IGeoSurface*);

	void SetLineSymbol(ILineSymbol* lineSymbol);
	void SetFillSymbol(IFillSymbol* fillSymbol);

public:
	size_t GetVertexCount();
	void GetVertex(size_t pos, double* x, double* y);
	void AddVertex(double x, double y);
	void SetVertex(size_t pos, double x, double y);
	void RemoveVertex(size_t pos);
	void ClearVertex();

private:
	bool _visible;
	ILineSymbol* _lineSymbol;
	IFillSymbol* _fillSymbol;
	OverlayPolygonImpl* _impl_;
};

END_NAME_SPACE(tGis, Core)


#endif

