#pragma once

#ifndef __OVERLAYRECT_H__
#define __OVERLAYRECT_H__

#include "Helper.h"

#include "IOverlayObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IGeoSurface;
struct ISymbol;

class TGISCORE_API OverlayRect : public IOverlayObject
{
public:
	OverlayRect();
	OverlayRect(int left, int top, int right, int bottom);
	virtual ~OverlayRect();

	virtual bool GetVisible();
	virtual void SetVisible(bool);

	virtual void Paint(IGeoSurface*);

	void SetLineSymbol(ISymbol* lineSymbol);
	void SetFillSymbol(ISymbol* fillSymbol);

public:
	int _left;
	int _right;
	int _top;
	int _bottom;

private:
	bool _visible;
	ISymbol* _lineSymbol;
	ISymbol* _fillSymbol;
};

END_NAME_SPACE(tGis, Core)


#endif

