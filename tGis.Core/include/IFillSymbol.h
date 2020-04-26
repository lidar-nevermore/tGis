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
	//һ�λ�һ��û�ж��Ķ����
	virtual void Paint(ISurface* surf, int count, int* x, int* y) = 0;

	//һ�λ�һ�����Դ����Ķ����
	//x[0] y[0] ��ʾ�ⲿ�߽� count[0]��ʾ�ⲿ��Χ�Ķ�������
	//���������ڲ��߽�
	virtual void Paint(ISurface* surf, int contourCount, int* ptCount, int** x, int** y) = 0;

private:
	static const char* const _type;
};


END_NAME_SPACE(tGis, Core)


#endif
