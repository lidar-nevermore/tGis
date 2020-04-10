#pragma once

#include "stdafx.h"
#include <windows.h>
#include "tGis_Visualize.h"
#include "GdiGeoSurface.h"

using namespace tGis::Core;

class RasterLayer;

BEGIN_NAME_SPACE(tGis, wGui)

#define MAP_WIDGET_CLASS _T("MFCMapWidget")
// MFCMapWidget

class MFCMapWidget : public CWnd, public MapWidget
{
	friend class GdiGeoSurface;

	DECLARE_DYNAMIC(MFCMapWidget)

public:
	MFCMapWidget();
	virtual ~MFCMapWidget();

public:
	BOOL RegisterWndClass();

private:
	bool _repaint;

public:
	virtual void RepaintMap();
	virtual void PresentMap();

protected:
	Map _thisMap;
	GdiGeoSurface _thisGeoSurface;

private:
	int _mouseDownX;
	int _mouseDownY;

	double _mouseDownSpatialX;
	double _mouseDownSpatialY;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// Í¨¹ý MapWidget ¼Ì³Ð
	virtual void Client2Screen(int cliX, int cliY, int * scrX, int * scrY) override;
	virtual void Screen2Client(int scrX, int scrY, int * cliX, int * cliY) override;
};

END_NAME_SPACE(tGis, wGui)


