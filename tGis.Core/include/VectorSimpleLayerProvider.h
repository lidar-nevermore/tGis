#pragma once

#ifndef __VECTORSIMPLELAYERPROVIDER_H__
#define __VECTORSIMPLELAYERPROVIDER_H__


#include "Helper.h"

#include "LayerProvider.h"

class OGRLayer;

BEGIN_NAME_SPACE(tGis, Core)

class VectorSimpleLayer;
class MyGDALVectorDataset;
class SimpleMarkerSymbol;
class SimpleLineSymbol;
class SimpleFillSymbol;

class TGIS_CORE_API VectorSimpleLayerProvider : public LayerProvider
{
private:
	static const char* const _name;
	static const char* const _type;

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

	bool IsSupportDataset(IDataset* dataset);
	virtual ILayer* UI_CreateLayer(IDataset* dataset);
	ILayer* CreateLayer(MyGDALVectorDataset* vector, OGRLayer* layer, SimpleMarkerSymbol* markerSymbol, SimpleLineSymbol* lineSymbol, SimpleFillSymbol* fillSymbol, int geometryField, int labelField = -1);
};

END_NAME_SPACE(tGis, Core)


#endif

