#pragma once

#ifndef __I_SURFACE_H__
#define __I_SURFACE_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IWidget;

struct TGIS_CORE_API ISurface
{
	//将Surface当前缓冲内容按1:1绘制到控件上
	virtual void Present(IWidget* w, int wX, int wY) = 0;
	//将Surface当前缓冲内容绘制到控件指定的范围内（缩放了）
	virtual void Present(IWidget* w, int wX, int wY, int wW, int wH) = 0;
	//开始一屏绘制，在每一屏绘制之前调用
	//如果使用OpenGL或者DirectX渲染引擎，可以在BeginPaint里准备画布、上下文
	//isCache表示是否是将图片缓冲显示出来，在isCache==false时制作图片缓冲
	virtual void BeginPaint(IWidget* w, bool isCache) = 0;
	//结束一屏绘制，在每一屏绘制完成之后调用
	//叠加图层、屏幕图层等，可以在这里绘制
	//如果使用OpenGL或者DirectX渲染引擎，自带双缓冲，在这里将一屏内容显示出来
	virtual void EndPaint(IWidget* w, bool isCache) = 0;

	//buf 四通道RGBA紧蹙格式图像 行的长度单字节对齐的 (某些GL头文件可能不支持BGRA)
	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY) = 0;
	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY, int surfW, int surfH) = 0;

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
