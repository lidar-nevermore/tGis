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

	//对于marker符号来说，下列属性并不是必须的
	//有时候大小、线条粗细不同就属于不同的符号了
	virtual int GetWidth() = 0;
	virtual void SetWidth(int w) = 0;
	virtual int GetHeight() = 0;
	virtual void SetHeight(int h) = 0;
	virtual int GetLineWidth() = 0;
	virtual void SetLineWidth(int lw) = 0;

	//一次画多个marker符号
	virtual void Paint(ISurface* surf, int count, int* x, int* y) = 0;
	//一次画一个marker符号的
	virtual void Paint(ISurface* surf, int x, int y) = 0;

private:
	static const char* const _type;
};


END_NAME_SPACE(tGis, Core)


#endif
