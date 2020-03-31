#pragma once

#ifndef __I_SURFACE_H__
#define __I_SURFACE_H__

#include "Helper.h"
#include "tGisCoreCfg.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IWidget;

struct TGIS_CORE_API ISurface
{
	//��Surface��ǰ�������ݰ�1:1���Ƶ��ؼ���
	virtual void Present(IWidget* w, int wX, int wY) = 0;
	//��Surface��ǰ�������ݻ��Ƶ��ؼ�ָ���ķ�Χ�ڣ������ˣ�
	virtual void Present(IWidget* w, int wX, int wY, int wW, int wH) = 0;
	//��ʼһ�����ƣ���ÿһ������֮ǰ����
	//���ʹ��OpenGL����DirectX��Ⱦ���棬������BeginPaint��׼��������������
	//modeָ������ģʽ��isCache��ʾ�Ƿ��ǻ���ͼƬ���壬��isCache==falseʱ����ͼƬ����
	virtual void BeginPaint(IWidget* w, bool isCache) = 0;
	//����һ�����ƣ���ÿһ���������֮�����
	//����ͼ����ʱͼ��ȣ��������������
	//���ʹ��OpenGL����DirectX��Ⱦ���棬�Դ�˫���壬�����ｫһ��������ʾ����
	virtual void EndPaint(IWidget* w, bool isCache) = 0;

	virtual void DrawPolyline(int count, int* surfX, int* surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt) = 0;
	virtual void DrawPolygon(int count, int* surfX, int* surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt) = 0;
	virtual void FillPolygon(int count, int* surfX, int* surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft) = 0;
	virtual void DrawEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt) = 0;
	virtual void FillEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft) = 0;
	virtual void DrawRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt) = 0;
	virtual void FillRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft) = 0;
	
	virtual void DrawImage(const unsigned char* buf, int surfX, int surfY, int width, int height) = 0;

protected:
	ISurface() {};
	virtual ~ISurface() {};
private:
	ISurface(const ISurface &) = delete;
	ISurface &operator=(const ISurface &) = delete;
};

typedef ISurface* ISurfacePtr;

END_NAME_SPACE(tGis, Core)


#endif
