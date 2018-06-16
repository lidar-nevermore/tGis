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

class TGIS_API VectorSimpleLayerProvider : public ILayerProvider
{
public:
	typedef ILayer*(*CreationUI)(VectorSimpleLayerProvider*,IDataset*);
	typedef void*(*PropertyUI)(VectorSimpleLayerProvider*, ILayer*);

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
	void SetCreationUI(const CreationUI ui);
	ILayer* UI_CreateLayer(IDataset* dataset);
	void SetPropertyUI(const PropertyUI ui);
	void UI_LayerProperty(ILayer* layer);
	ILayer* CreateLayer(MyGDALVectorDataset* vector, OGRLayer* layer, SimpleMarkerSymbol* markerSymbol, SimpleLineSymbol* lineSymbol, SimpleFillSymbol* fillSymbol, int geometryField, int labelField = -1);
	ILayer* CreateLayer(IDataset* dataset, const char* creationString);
	void ReleaseLayer(ILayer*);

	void Release();

private:
	CreationUI _uiCreation;
	PropertyUI _uiProperty;

private:
	static const char* const _name;
	static const char* const _type;
};

END_NAME_SPACE(tGis, Core)


#endif

