#pragma once

#ifndef __I_SYMBOL_H__
#define __I_SYMBOL_H__

#include "Helper.h"
#include "ISymbolLibrary.h"
#include "IReferenceObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

//符号一般都是从符号库取出的，从符号库取出的符号都是new出来的
//由于无法从语言层面约束一个类的子类都只能new，
//只能尽量从语言层面约束类的正确使用方式

//凡是需要调用Release的引用都需要调用此接口
//除非直接从符号库中取出的

//对于从符号库取出的符号必须调用次接口释放
//通常符号都是从符号库取出的
//不是从符号库取出的符号，创建方负责释放(不必调用此接口)

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
