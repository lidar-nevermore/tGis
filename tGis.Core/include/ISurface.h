#pragma once

#ifndef __I_SURFACE_H__
#define __I_SURFACE_H__

#include "Helper.h"

#define __gl_h_
typedef float GLfloat;
#endif 

BEGIN_NAME_SPACE(tGis, Core)

//ISurfaceӦ��ʵ��Ϊ״̬��
//Surface���������Ͻ�(0,0), x�����ң�y�����£���λ����
struct TGIS_CORE_API ISurface
{
	virtual void GetSize(int* surfW, int* surfH) const = 0;

	//��Surface��ǰ�������ݰ�1:1���Ƶ��ؼ���
	virtual void Present(int wX, int wY) = 0;
	//��Surface��ǰ�������ݻ��Ƶ��ؼ�ָ���ķ�Χ�ڣ������ˣ�
	virtual void Present(int wX, int wY, int wW, int wH) = 0;
	//��ʼһ�����ƣ���ÿһ������֮ǰ����
	//���ʹ��OpenGL����DirectX��Ⱦ���棬������BeginPaint��׼��������������
	//isCache��ʾ�Ƿ��ǽ�ͼƬ������ʾ��������isCache==falseʱ����ͼƬ����
	virtual void BeginPaint(bool isCache) = 0;
	//����һ�����ƣ���ÿһ���������֮�����
	//����ͼ�㡢��Ļͼ��ȣ��������������
	//���ʹ��OpenGL����DirectX��Ⱦ���棬�Դ�˫���壬�����ｫһ��������ʾ����
	virtual void EndPaint(bool isCache) = 0;

	//buf ��ͨ��RGBA������ʽͼ�� �еĳ��ȵ��ֽڶ���� (ĳЩGLͷ�ļ����ܲ�֧��BGRA)
	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY) = 0;
	virtual void DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY, int surfW, int surfH) = 0;

	virtual void Surface2glndc(int surfX, int surfY, GLfloat* ndcX, GLfloat* ndcY) = 0;

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
