#pragma once

#ifndef __I_MarkerSymbol_H__
#define __I_MarkerSymbol_H__

#include "Helper.h"
#include "ISymbol.h"

BEGIN_NAME_SPACE(tGis, Core)


struct TGIS_CORE_API IMarkerSymbol : public ISymbol
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

	virtual void GetColor(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a) = 0;
	virtual void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;
	virtual int GetXOffset() = 0;
	virtual void SetXOffset(int xOff) = 0;
	virtual int GetYOffset() = 0;
	virtual void SetYOffset(int yOff) = 0;

	//����marker������˵���������Բ����Ǳ����
	//��ʱ���С��������ϸ��ͬ�����ڲ�ͬ�ķ�����
	virtual int GetWidth() = 0;
	virtual void SetWidth(int w) = 0;
	virtual int GetHeight() = 0;
	virtual void SetHeight(int h) = 0;
	virtual int GetLineWidth() = 0;
	virtual void SetLineWidth(int lw) = 0;

	//һ�λ����marker����
	virtual void Paint(ISurface* surf, int count, int* x, int* y) = 0;
	//һ�λ�һ��marker���ŵ�
	virtual void Paint(ISurface* surf, int x, int y) = 0;

private:
	static const char* const _type;
};


END_NAME_SPACE(tGis, Core)


#endif
