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
	void SetSymbolLibrary(ISymbolLibrary* symLib);

	void SetSymbolWidth(int width);

	int GetSymbolWidth()
	{
		return _symWidth;
	}

	void SetSymbolSpan(int span);

	int GetSymbolSpan()
	{
		return _symSpan;
	}

	void UpdateEnvelope(int surfW, int surfH);

	//ѡ��Surface����(x,y)���ķ��� ���ط���id
	int SelectSymbol(int x, int y);

	int SelectSymbol(int symId);

private:
	ILayer* _layer;
	ISymbolLibrary* _symLib;
	OGREnvelope _envelope;
	int _symWidth;
	int _symHalfWidth;
	int _symSpan;
	int _symOccupy;

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
