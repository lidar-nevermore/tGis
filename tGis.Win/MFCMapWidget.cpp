// MapWidget.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCMapWidget.h"

#define _isinf(a)   ((_fpclass(a) == _FPCLASS_PINF) || (_fpclass(a) == _FPCLASS_NINF))

inline double round(double val, int places = 0) {
	double t;
	double f = pow(10.0, (double)places);
	double x = val * f;

	if (_isinf(x) || _isnan(x)) {
		return val;
	}

	if (x >= 0.0) {
		t = ceil(x);
		if ((t - x) > 0.50000000001) {
			t -= 1.0;
		}
	}
	else {
		t = ceil(-x);
		if ((t + x) > 0.50000000001) {
			t -= 1.0;
		}
		t = -t;
	}
	x = t / f;

	return !_isnan(x) ? x : t;
}

// MFCMapWidget

IMPLEMENT_DYNAMIC(MFCMapWidget, CWnd)

MFCMapWidget::MFCMapWidget()
{
	_repaint = false;
	_mouseDownX = -1;
	_mouseDownY = -1;
	SetMap(&_thisMap);
	_geoSurface = &_thisGeoSurface;

	RegisterWndClass();
}

MFCMapWidget::~MFCMapWidget()
{
}


BEGIN_MESSAGE_MAP(MFCMapWidget, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
    ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL MFCMapWidget::RegisterWndClass()
{
	WNDCLASS windowclass;
	HINSTANCE hInst = AfxGetInstanceHandle();

	//Check weather the class is registerd already
	if (!(::GetClassInfo(hInst, MAP_WIDGET_CLASS, &windowclass)))
	{
		//If not then we have to register the new class
		windowclass.style = CS_DBLCLKS;// | CS_HREDRAW | CS_VREDRAW;
		windowclass.lpfnWndProc = ::DefWindowProc;
		windowclass.cbClsExtra = windowclass.cbWndExtra = 0;
		windowclass.hInstance = hInst;
		windowclass.hIcon = NULL;
		windowclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		windowclass.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);
		windowclass.lpszMenuName = NULL;
		windowclass.lpszClassName = MAP_WIDGET_CLASS;

		if (!AfxRegisterClass(&windowclass))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
    return TRUE;  
}

void MFCMapWidget::RepaintMap()
{
	_repaint = true;
}

void MFCMapWidget::PresentMap()
{

}



// MFCMapWidget 消息处理程序

void MFCMapWidget::OnPaint()
{
	if (_repaint == true)
	{
		_repaint = false;
		MapWidget::RepaintMap();
	}
	else
	{
		MapWidget::PresentMap();
	}
}


void MFCMapWidget::OnSize(UINT nType, int cx, int cy)
{
	// TODO: 在此处添加消息处理程序代码
	CWnd::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&rect);
	SetSurfaceSize(rect.Width(), rect.Height());
	RepaintMap();
}


void MFCMapWidget::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (_thisMap.GetLayerCount() > 0)
	{
		_mouseDownX = point.x;
		_mouseDownY = point.y;
		_viewPort.Surface2Spatial(_mouseDownX, _mouseDownY, &_mouseDownSpatialX, &_mouseDownSpatialY);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void MFCMapWidget::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( (nFlags&MK_LBUTTON) && (_mouseDownX != -1 || _mouseDownY != -1) )
	{
		int mx = point.x;
		int my = point.y;
		if (mx != _mouseDownX && my != _mouseDownY)
		{
			GeoViewPort* viewPort = &_viewPort;
			double mouseX;
			double mouseY;
			viewPort->Surface2Spatial(mx, my, &mouseX, &mouseY);

			double spatialCenterX;
			double spatialCenterY;
			viewPort->GetSpatialCenter(&spatialCenterX, &spatialCenterY);
			viewPort->SetSpatialCenter(spatialCenterX + _mouseDownSpatialX - mouseX, spatialCenterY + _mouseDownSpatialY - mouseY);
			PresentMap();
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}


void MFCMapWidget::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (_thisMap.GetLayerCount() > 0)
		RepaintMap();

	CWnd::OnLButtonUp(nFlags, point);
}


BOOL MFCMapWidget::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (_thisMap.GetLayerCount() == 0)
	    return CWnd::OnMouseWheel(nFlags, zDelta, pt);

	GeoViewPort* viewPort = &_viewPort;
	double scale;
	viewPort->GetViewScale(&scale);

	ScreenToClient(&pt);

	int mx = pt.x;
	int my = pt.y;
	double mouseX1;
	double mouseY1;
	viewPort->Surface2Spatial(mx, my, &mouseX1, &mouseY1);

	int step = (int)(abs(zDelta / 120.0));
	step = step == 0 ? 1 : step;

	double newScale = scale;

	for (int i = 0; i < step; i++)
	{
		if (zDelta > 0)
		{
			newScale *= 0.96; //360/375
		}
		else
		{
			newScale *= 1.042; //375/360
		}
	}

	viewPort->SetViewScale(newScale);

	double mouseX2;
	double mouseY2;
	viewPort->Surface2Spatial(mx, my, &mouseX2, &mouseY2);
	double spatialCenterX;
	double spatialCenterY;
	viewPort->GetSpatialCenter(&spatialCenterX, &spatialCenterY);
	viewPort->SetSpatialCenter(spatialCenterX + mouseX1 - mouseX2, spatialCenterY + mouseY1 - mouseY2);
	if (newScale > scale)
	{
		RepaintMap();
	}
	else
	{
		PresentMap();
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL MFCMapWidget::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

void MFCMapWidget::Client2Screen(int cliX, int cliY, int * scrX, int * scrY)
{
}

void MFCMapWidget::Screen2Client(int scrX, int scrY, int * cliX, int * cliY)
{
}
