#pragma once

#ifndef __I_SYMBOLLIBRARY_H__
#define __I_SYMBOLLIBRARY_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISymbol;

struct TGIS_API ISymbolLibrary
{
	virtual const char* GetType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetSymbolCount() const = 0;
	virtual ISymbol* GetSymbol(int) const = 0;
	//�����ſ��еķ���Id��������ʱ�������ٱ������ſ�
	//��һ��������ǰ��Ҫ��ȡ�ķ���Id
	//�ڶ��������´ο��Ի�ȡ�ķ���Id���������ֵΪ�����ſ����Ѿ�û�и���id�ķ�����
	//
	//������ʱ���0��ʼ��ÿ�ζ���ȡ��һ�ν�Ҫ��ȡ�ķ���Id��
	virtual ISymbol* GetSymbol(int,int*) const = 0;
	//���дӷ��ſ���ȡ�����ķ���
	//������ʹ��ʱ����Ҫ��������ӿڷ��������ſ�
	virtual void ReclaimSymbol(ISymbol*) const = 0;

	virtual void Release() = 0;

	virtual ~ISymbolLibrary() {};
};

typedef ISymbolLibrary* ISymbolLibraryPtr;

END_NAME_SPACE(tGis, Core)


#endif