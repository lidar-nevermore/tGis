#include "stdafx.h"
#include "GdiGeoSurface.h"
#include "MFCMapWidget.h"
#include <GdiPlus.h>  

#pragma comment(lib,"Gdiplus.lib")  

BEGIN_NAME_SPACE(tGis, wGui)

//������Ҫ�����ڳ�������ʱ�Զ�ִ��GdiplusStartup���������ʱִ��GdiplusShutdown  
//ʹ��GDI+��������������Ҳ���Բ�������࣬�����Լ��Ĵ��������е���GdiplusStartup��GdiplusShutdown  
class GdiPlusInit
{
public:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiPlusInit()
	{
		gdiplusToken = 0;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~GdiPlusInit()
	{
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
	static GdiPlusInit Instance;
};

GdiPlusInit GdiPlusInit::Instance;


GdiGeoSurface::GdiGeoSurface()
{
	if (GdiPlusInit::Instance.gdiplusToken == 0)
		throw;
	_bitmap = NULL;
}


GdiGeoSurface::~GdiGeoSurface()
{
	if (_bitmap != NULL)
		delete _bitmap;
}

/**
@name    GetEncoderClsid
@brief   ȡͼƬ����GLSID
@param[in]        const WCHAR * format
@param[in]        CLSID * pClsid
@return           int
*/
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;
	UINT  size = 0;
	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}

void GdiGeoSurface::SetViewPort(GeoViewPort* viewPort)
{
	_viewPort = *viewPort;
	int surfWidth;
	int surfHeight;
	_viewPort.GetSurfaceSize(&surfWidth, &surfHeight);

	if(0 == surfWidth || 0 == surfHeight)
		return;

	if (_bitmap != NULL)
	{
		UINT w = _bitmap->GetWidth();
		UINT h = _bitmap->GetHeight();
		if(w != surfWidth || h != surfHeight)
		{
			delete _bitmap;
			_bitmap = NULL;
		}
	}

	if (_bitmap == NULL)
	{
		_bitmap = new Gdiplus::Bitmap(
			surfWidth, 
			surfHeight,
			PixelFormat32bppARGB);
	}
}

void GdiGeoSurface::Present(IWidget * w, int wX, int wY)
{
	MFCMapWidget* cw = (MFCMapWidget*)w;

	if (_bitmap == NULL)
		return;

	Gdiplus::Graphics gps(cw->GetSafeHwnd());
	gps.DrawImage(_bitmap, wX, wY);
}

void GdiGeoSurface::Present(IWidget * w, int wX, int wY, int wW, int wH)
{
	MFCMapWidget* cw = (MFCMapWidget*)w;
	if (_bitmap == NULL)
		return;

	CRect rect;
	cw->GetClientRect(&rect);
	Gdiplus::Bitmap buf(rect.Width(), rect.Height(), PixelFormat24bppRGB);
	Gdiplus::Graphics gps(&buf);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
	gps.FillRectangle(&brush, 0, 0, rect.Width(), rect.Height());
	gps.DrawImage(_bitmap, wX, wY, wW, wH);
	//Gdiplus::Rect rc(wX, wY, wW, wH);
	//gps.DrawImage(_bitmap, rc, surfX, surfY, surfW, surfH, Gdiplus::Unit::UnitPixel);

	Gdiplus::Graphics gw(cw->GetSafeHwnd());
	gw.DrawImage(&buf, 0, 0);
}

void GdiGeoSurface::BeginPaint(IWidget * w, bool isCache)
{
	GeoSurface::BeginPaint(w, isCache);

	MFCMapWidget* cw = (MFCMapWidget*)w;
	if (isCache == false)
	{
		if (_bitmap == NULL)
			return;

		//ȫ�»���ʱ����Ϊ�����_bitmap�ߴ����surface�ߴ�

		int surfWidth;
		int surfHeight;
		_viewPort.GetSurfaceSize(&surfWidth, &surfHeight);
		Gdiplus::Graphics gps(_bitmap);
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, cw->_backgroundR, cw->_backgroundG, cw->_backgroundB));
		gps.FillRectangle(&brush, 0, 0, surfWidth, surfHeight);
	}
	else
	{
		//CRect rect;
		//cw->GetClientRect(&rect);
		//Gdiplus::Graphics gps(cw->GetSafeHwnd());
		//Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
		//gps.FillRectangle(&brush, 0, 0, rect.Width(), rect.Height());
	}
}

void GdiGeoSurface::EndPaint(IWidget * w, bool isCache)
{
	if (isCache == false)
		Present(w, 0, 0);
}

void GdiGeoSurface::DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY)
{
	if (_bitmap == NULL)
		return;

	Gdiplus::Bitmap bitmap(width, height, width * 4, PixelFormat32bppARGB, (BYTE*)buf);
	//CLSID encoderClsid;
	//GetEncoderClsid(L"image/bmp", &encoderClsid);
	//bitmap.Save(L"E:\\fu.bmp",&encoderClsid, nullptr);
	Gdiplus::Graphics gps(_bitmap);
	gps.DrawImage(&bitmap, surfX, surfY);
}

void GdiGeoSurface::DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY, int surfW, int surfH)
{
	if (_bitmap == NULL)
		return;

	Gdiplus::Bitmap bitmap(width, height, width * 4, PixelFormat32bppARGB, (BYTE*)buf);
	//CLSID encoderClsid;
	//GetEncoderClsid(L"image/bmp", &encoderClsid);
	//bitmap.Save(L"E:\\fu.bmp",&encoderClsid, nullptr);
	Gdiplus::Graphics gps(_bitmap);
	gps.DrawImage(&bitmap, surfX, surfY, surfW, surfH);
}

END_NAME_SPACE(tGis, wGui)
