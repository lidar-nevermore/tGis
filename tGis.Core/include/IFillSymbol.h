#pragma once

#ifndef __I_FillSymbol_H__
#define __I_FillSymbol_H__

#include "Helper.h"
#include "ISymbol.h"

BEGIN_NAME_SPACE(tGis, Core)


struct TGIS_CORE_API IFillSymbol : public ISymbol
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);
	//一次画一个没有洞的多边形
	virtual void Paint(ISurface* surf, int count, int* x, int* y) = 0;

	//一次画一个可以带洞的多边形
	//x[0] y[0] 表示外部边界 count[0]表示外部范围的顶点数量
	//其它都是内部边界
	virtual void Paint(ISurface* surf, int contourCount, int* ptCount, int** x, int** y) = 0;

private:
	static const char* const _type;
};


END_NAME_SPACE(tGis, Core)


#endif
