#pragma once

#ifndef __I_SYMBOL_H__
#define __I_SYMBOL_H__

#include "Helper.h"
#include "ISymbolLibrary.h"
#include "IReferenceObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

//����һ�㶼�Ǵӷ��ſ�ȡ���ģ��ӷ��ſ�ȡ���ķ��Ŷ���new������
//�����޷������Բ���Լ��һ��������඼ֻ��new��
//ֻ�ܾ��������Բ���Լ�������ȷʹ�÷�ʽ

//������Ҫ����Release�����ö���Ҫ���ô˽ӿ�
//����ֱ�Ӵӷ��ſ���ȡ����

//���ڴӷ��ſ�ȡ���ķ��ű�����ôνӿ��ͷ�
//ͨ�����Ŷ��Ǵӷ��ſ�ȡ����
//���Ǵӷ��ſ�ȡ���ķ��ţ������������ͷ�(���ص��ô˽ӿ�)

struct TGIS_CORE_API ISymbol : public IReferenceObject
{
	ISymbol(const ISymbolLibrary* lib)
	{
		_lib = lib;
	};
	virtual ~ISymbol() {};

	virtual const ISymbolLibrary* GetSymbolLibrary()
	{
		return _lib;
	};

	virtual int GetId() = 0;
	virtual SymbolType GetType() = 0;

protected:
	const ISymbolLibrary* _lib;
};


typedef ISymbol* ISymbolPtr;

END_NAME_SPACE(tGis, Core)


#endif
