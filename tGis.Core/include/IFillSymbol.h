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
	
	//һ�λ�һ��û�ж��Ķ����
	virtual void Paint(ISurface* surf, int count, int* x, int* y) = 0;

	//һ�λ�һ�����Դ����Ķ����
	//x[0] y[0] ��ʾ�ⲿ�߽� ptCount[0]��ʾ�ⲿ��Χ�Ķ�������
	//���������ڲ��߽�
	virtual void Paint(ISurface* surf, int contourCount, int* ptCount, int** x, int** y) = 0;


	//���÷��Ķ�����ܲ��Ƿ����������ڴ�ռ��
	//�ṩ��Щ�ӿڿ��Ա���ƴ�Ӷ�������
	virtual void BeginPaint(ISurface* surf) = 0;

	//AppendVertex������BeginContour��EndContour֮�����
	//��һ��BeginContour��EndContour֮�������߽磬�������ڱ߽�
	virtual void BeginContour(ISurface* surf) = 0;

	virtual void AppendVertex(int count, int* x, int* y) = 0;

	virtual void AppendVertex(int x, int y) = 0;

	virtual void EndContour(ISurface* surf) = 0;

	virtual void EndPaint(ISurface* surf) = 0;

};


END_NAME_SPACE(tGis, Core)


#endif
