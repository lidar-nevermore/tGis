#pragma once

#ifndef __VECTORSIMPLELAYERPROVIDER_H__
#define __VECTORSIMPLELAYERPROVIDER_H__


#include "Helper.h"

#include "ILayerProvider.h"

class OGRLayer;

BEGIN_NAME_SPACE(tGis, Core)

class VectorSimpleLayer;
class MyGDALVectorDataset;
class SimpleMarkerSymbol;
class SimpleLineSymbol;
class SimpleFillSymbol;

class TGISCORE_API VectorSimpleLayerProvider : public ILayerProvider
{
public:
	typedef VectorSimpleLayer*(*UI)();

public:
	static VectorSimpleLayerProvider INSTANCE;

private:
	VectorSimpleLayerProvider();
	~VectorSimpleLayerProvider();
	VectorSimpleLayerProvider(const VectorSimpleLayerProvider &) = delete;
	VectorSimpleLayerProvider &operator=(const VectorSimpleLayerProvider &) = delete;

public:
	const char* GetType();
	const char* GetName();

	const char* GetSupportedLayerType();

	const char* GetSupportedDatasetType();

	void SetCreationUI(const UI ui);
	ILayer* UI_CreateLayer(IDataset* dataset);
	ILayer* CreateLayer(MyGDALVectorDataset* vector, OGRLayer* layer, SimpleMarkerSymbol* markerSymbol, SimpleLineSymbol* lineSymbol, SimpleFillSymbol* fillSymbol, int geometryField, int labelField = -1);
	ILayer* CreateLayer(IDataset* dataset, const char* creationString);
	void ReleaseLayer(ILayer*);

	void Release();

private:
	UI _ui;

private:
	static const char* const _name;
	static const char* const _type;
};

END_NAME_SPACE(tGis, Core)


#endif

