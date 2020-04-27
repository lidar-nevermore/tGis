#pragma once

#ifndef __I_SYMBOL_H__
#define __I_SYMBOL_H__

#include "Helper.h"
#include "ISymbolLibrary.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

struct TGIS_CORE_API ISymbol
{
	virtual const ISymbolLibrary* GetSymbolLibrary()
	{
		return _lib;
	};

	virtual int GetId() = 0;
	virtual SymbolType GetType() = 0;

	//������Ҫ����Release�����ö���Ҫ���ô˽ӿ�
	//����ֱ�Ӵӷ��ſ���ȡ����
	virtual void Reference()
	{
		_refCount++;
	}

	//���ڴӷ��ſ�ȡ���ķ��ű�����ôνӿ��ͷ�
	//ͨ�����Ŷ��Ǵӷ��ſ�ȡ����
	//���Ǵӷ��ſ�ȡ���ķ��ţ������������ͷ�(���ص��ô˽ӿ�)
	virtual void Release()
	{
		if(_refCount > 0)
			_refCount--;
		if (_refCount == 0 && _lib != nullptr)
			_lib->ReleaseSymbol(this);
	};

	ISymbol(const ISymbolLibrary* lib)
	{
		_refCount = 1;
		_lib = lib;
	};
	virtual ~ISymbol() {};

private:
	int _refCount;

protected:
	const ISymbolLibrary* _lib;
};


typedef ISymbol* ISymbolPtr;

END_NAME_SPACE(tGis, Core)


#endif
