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

	//凡是需要调用Release的引用都需要调用此接口
	//除非直接从符号库中取出的
	virtual void Reference()
	{
		_refCount++;
	}

	//对于从符号库取出的符号必须调用次接口释放
	//通常符号都是从符号库取出的
	//不是从符号库取出的符号，创建方负责释放(不必调用此接口)
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
