#pragma once

#ifndef __I_FillSymbol_H__
#define __I_FillSymbol_H__

#include "Helper.h"
#include "ISymbol.h"

BEGIN_NAME_SPACE(tGis, Core)


struct TGIS_CORE_API IFillSymbol : public ISymbol
{
	IFillSymbol(const ISymbolLibrary* lib)
		:ISymbol(lib)
	{
	};

	SymbolType GetType() override
	{
		return SymbolType::Fill;
	};

	virtual void GetColor(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a) = 0;
	virtual void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;
	
	//一次画一个没有洞的多边形
	virtual void Paint(ISurface* surf, int count, int* x, int* y) = 0;

	//一次画一个可以带洞的多边形
	//x[0] y[0] 表示外部边界 count[0]表示外部范围的顶点数量
	//其它都是内部边界
	virtual void Paint(ISurface* surf, int contourCount, int* ptCount, int** x, int** y) = 0;
};


END_NAME_SPACE(tGis, Core)


#endif
