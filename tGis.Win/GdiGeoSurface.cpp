#include "stdafx.h"
#include "GdiGeoSurface.h"
#include "MFCMapWidget.h"
#include <GdiPlus.h>  

#pragma comment(lib,"Gdiplus.lib")  

BEGIN_NAME_SPACE(tGis, wGui)

//该类主要用来在程序启动时自动执行GdiplusStartup，程序结束时执行GdiplusShutdown  
//使得GDI+可以正常工作，也可以不定义该类，而在自己的代码中自行调用GdiplusStartup和GdiplusShutdown  
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
@brief   取图片类型GLSID
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

void GdiGeoSurface::Present(int wX, int wY)
{
	if (_bitmap == NULL)
		return;

	Gdiplus::Graphics gps(_mapWidget->GetSafeHwnd());
	gps.DrawImage(_bitmap, wX, wY);
}

void GdiGeoSurface::Present(int wX, int wY, int wW, int wH)
{
	if (_bitmap == NULL)
		return;

	CRect rect;
	_mapWidget->GetClientRect(&rect);
	Gdiplus::Bitmap buf(rect.Width(), rect.Height(), PixelFormat24bppRGB);
	Gdiplus::Graphics gps(&buf);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
	gps.FillRectangle(&brush, 0, 0, rect.Width(), rect.Height());
	gps.DrawImage(_bitmap, wX, wY, wW, wH);
	//Gdiplus::Rect rc(wX, wY, wW, wH);
	//gps.DrawImage(_bitmap, rc, surfX, surfY, surfW, surfH, Gdiplus::Unit::UnitPixel);

	Gdiplus::Graphics gw(_mapWidget->GetSafeHwnd());
	gw.DrawImage(&buf, 0, 0);
}

void GdiGeoSurface::BeginPaint(bool isCache)
{
	GeoSurface::BeginPaint(isCache);

	if (isCache == false)
	{
		if (_bitmap == NULL)
			return;

		//全新绘制时，作为缓存的_bitmap尺寸等于surface尺寸

		int surfWidth;
		int surfHeight;
		_viewPort.GetSurfaceSize(&surfWidth, &surfHeight);
		Gdiplus::Graphics gps(_bitmap);
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, _mapWidget->_backgroundR, _mapWidget->_backgroundG, _mapWidget->_backgroundB));
		gps.FillRectangle(&brush, 0, 0, surfWidth, surfHeight);
	}
	else
	{
		//CRect rect;
		//_mapWidget->GetClientRect(&rect);
		//Gdiplus::Graphics gps(_mapWidget->GetSafeHwnd());
		//Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
		//gps.FillRectangle(&brush, 0, 0, rect.Width(), rect.Height());
	}
}

void GdiGeoSurface::EndPaint(bool isCache)
{
	if (isCache == false)
		Present(0, 0);
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
