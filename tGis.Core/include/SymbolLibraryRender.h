#pragma once

#ifndef __SymbolLibraryRender_H__
#define __SymbolLibraryRender_H__


#include "Helper.h"
#include "ILayerRender.h"

#include "ogr_core.h"
#include <assert.h>


BEGIN_NAME_SPACE(tGis, Core)

struct ILayer;
struct ISymbolLibrary;
struct IMarkerSymbol;
struct ILineSymbol;
struct IFillSymbol;

class SymbolLibraryRenderImpl;

class TGIS_CORE_API SymbolLibraryRender : public ILayerRender
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	SymbolLibraryRender(ILayer* layer, ISymbolLibrary* symLib = nullptr);
	~SymbolLibraryRender();

public:
	void SetSymbolLibrary(ISymbolLibrary* symLib)
	{
		_symLib = symLib;
	}

	void SetSymbolWidth(int width)
	{
		assert(width > 3);
		_symWidth = width;
		_symHalfWidth = _symWidth / 2;
	}

	int GetSymbolWidth()
	{
		return _symWidth;
	}

	void SetSymbolSpan(int span)
	{
		assert(span > 3);
		_symSpan = span;
	}

	int GetSymbolSpan()
	{
		return _symSpan;
	}

	//选择Surface坐标(x,y)处的符号 返回符号id
	int SelectSymbol(int x, int y);

	int SelectSymbol(int symId);

private:
	ILayer* _layer;
	ISymbolLibrary* _symLib;
	OGREnvelope _envelope;
	int _symWidth;
	int _symHalfWidth;
	int _symSpan;

	SymbolLibraryRenderImpl* _impl_;

private:
	void DrawMarkerSymbol(IGeoSurface*, IMarkerSymbol* sym, int x, int y);
	void DrawLineSymbol(IGeoSurface*, ILineSymbol* sym, int x, int y);
	void DrawFillSymbol(IGeoSurface*, IFillSymbol* sym, int x, int y);

public:
	virtual void Paint(IGeoSurface*);

	virtual const OGREnvelope * GetEnvelope() override;

	virtual const OGRSpatialReference * GetSpatialReference() override;

	virtual bool CanTransformTo(const OGRSpatialReference *) override;

	virtual float GetOpacity() override;

	virtual void SetOpacity(float) override;

};


END_NAME_SPACE(tGis, Core)

#endif
