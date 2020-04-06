#pragma once

#ifndef __I_SYMBOLLIBRARY_H__
#define __I_SYMBOLLIBRARY_H__

#include "Helper.h"
#include "IInclusionObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISymbol;

struct TGIS_CORE_API ISymbolLibrary : public IInclusionObject
{
	friend class SymbolLibraryRepository;

	ISymbolLibrary() {};
	virtual ~ISymbolLibrary() {};

	//���뱣֤�ýӿ��κ�ʱ�򶼷���ͬһ��ָ��
	//ֻ���ַ���ֵһ����ŵ�ַ��һ��Ҳ�ǲ��Ե�
	//���SymbolLibraryһ�����������Ʋ����޸�
	virtual const char* GetName() const = 0;
	virtual int GetSymbolCount() const = 0;
	virtual ISymbol* GetSymbol(int id) const = 0;
	//�����ſ��еķ���Id��������ʱ�������ٱ������ſ�
	//��һ��������ǰ��Ҫ��ȡ�ķ���Id
	//�ڶ��������´ο��Ի�ȡ�ķ���Id��
	//����ڶ�����������ֵΪ����˵�����ſ����Ѿ�û�и���id�ķ�����
	//
	//������ʱ���0��ʼ��ÿ�ζ���ȡ��һ�ν�Ҫ��ȡ�ķ���Id��
	virtual ISymbol* GetSymbol(int id,int* nextId) const = 0;
	//���дӷ��ſ���ȡ�����ķ���
	//������ʹ��ʱ����Ҫ��������ӿڷ��������ſ�
	virtual void RevertSymbol(ISymbol*) const = 0;
};

typedef ISymbolLibrary* ISymbolLibraryPtr;

END_NAME_SPACE(tGis, Core)


#endif