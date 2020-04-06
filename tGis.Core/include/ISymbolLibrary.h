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

	//必须保证该接口任何时候都返回同一个指针
	//只是字符串值一样存放地址不一样也是不对的
	//因此SymbolLibrary一旦创建其名称不得修改
	virtual const char* GetName() const = 0;
	virtual int GetSymbolCount() const = 0;
	virtual ISymbol* GetSymbol(int id) const = 0;
	//当符号库中的符号Id不连续的时候辅助快速遍历符号库
	//第一个参数当前将要获取的符号Id
	//第二个参数下次可以获取的符号Id，
	//如果第二个参数返回值为负，说明符号库里已经没有更大id的符号了
	//
	//遍历的时候从0开始，每次都获取下一次将要获取的符号Id。
	virtual ISymbol* GetSymbol(int id,int* nextId) const = 0;
	//所有从符号库里取出来的符号
	//当不再使用时都需要调用这个接口返还给符号库
	virtual void RevertSymbol(ISymbol*) const = 0;
};

typedef ISymbolLibrary* ISymbolLibraryPtr;

END_NAME_SPACE(tGis, Core)


#endif