#pragma once

#ifndef __I_SURFACE_H__
#define __I_SURFACE_H__

#include "Helper.h"

#ifndef __gl_h_
typedef float GLfloat;
typedef double GLdouble;
#endif 

BEGIN_NAME_SPACE(tGis, Core)

//ISurface应当实现为状态机
//凡是直接和间接调用ISurface接口的方法都不支持多线程调用
//Surface的坐标左上角(0,0), x方向朝右，y方向朝下，单位像素
struct TGIS_CORE_API ISurface
{
	virtual void GetSize(int* surfW, int* surfH) const = 0;

	//将Surface当前内容按1:1绘制到控件上
	virtual void Present(int wX, int wY) = 0;
	//将Surface当前内容绘制到控件指定的范围内（缩放了）
	virtual void Present(int wX, int wY, int wW, int wH) = 0;
	//开始一屏绘制，在每一屏绘制之前调用
	//如果使用OpenGL或者DirectX渲染引擎，可以在BeginPaint里准备画布、上下文
	//isCache表示是否是将图片缓冲显示出来，在isCache==false时制作图片缓冲
	//BeginPaint里需要将_tess准备好
	virtual void BeginPaint(bool isCache) = 0;
	//结束一屏绘制，在每一屏绘制完成之后调用
	//叠加图层、屏幕图层等，可以在这里绘制
	//如果使用OpenGL或者DirectX渲染引擎，自带双缓冲，在这里将一屏内容显示出来
	virtual void EndPaint(bool isCache) = 0;

	//buf 四通道RGBA紧蹙格式图像 行的长度单字节对齐的 (OpenGL早期版本不支持BGRA)
	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY) = 0;
	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY, int surfW, int surfH) = 0;

	//Surface Coordinate -> OpenGL Normalized Device Coordinate
	virtual void Surface2glndc(int surfX, int surfY, GLfloat* ndcX, GLfloat* ndcY) = 0;
	virtual void Surface2glndc(int surfX, int surfY, GLdouble* ndcX, GLdouble* ndcY) = 0;

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
