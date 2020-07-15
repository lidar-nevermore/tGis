#pragma once

#ifndef __I_LineSymbol_H__
#define __I_LineSymbol_H__

#include "Helper.h"
#include "ISymbol.h"

BEGIN_NAME_SPACE(tGis, Core)


struct TGIS_CORE_API ILineSymbol : public ISymbol
{
	ILineSymbol(const ISymbolLibrary* lib)
		:ISymbol(lib)
	{
	};

	SymbolType GetType() override
	{
		return SymbolType::Line;
	};

	virtual void GetColor(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a) = 0;
	virtual void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;
	virtual int GetWidth() = 0;
	virtual void SetWidth(int w) = 0;

	//一次画一条线
	virtual void Paint(ISurface* surf, int count, int* x, int* y, bool close = false) = 0;

	//调用方的顶点可能不是放在连续的内存空间的
	//提供这些接口可以避免拼接顶点数组
	virtual void BeginPaint(ISurface* surf, bool close = false) = 0;

	virtual void AppendVertex(int count, int* x, int* y) = 0;

	virtual void AppendVertex(int x, int y) = 0;

	virtual void EndPaint(ISurface* surf) = 0;
};


END_NAME_SPACE(tGis, Core)


#endif
