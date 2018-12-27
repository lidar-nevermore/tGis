#pragma once

#ifndef __LAYERPROVIDER_H__
#define __LAYERPROVIDER_H__


#include "Helper.h"

#include "ILayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_CORE_API LayerProvider : public ILayerProvider
{
public:
	typedef ILayer*(*CreationUI)(ILayerProvider*, IDataset*);
	typedef void(*PropertyUI)(ILayerProvider*, ILayer*);

protected:
	LayerProvider();
	LayerProvider(const LayerProvider &) = delete;
	LayerProvider &operator=(const LayerProvider &) = delete;

public:
	virtual ~LayerProvider();

protected:
	CreationUI _uiCreation;
	PropertyUI _uiProperty;

public:
	void SetCreationUI(const CreationUI ui);
	virtual ILayer* UI_CreateLayer(IDataset* dataset);
	void SetPropertyUI(const PropertyUI ui);
	virtual void UI_LayerProperty(ILayer* layer);

	virtual ILayer* CreateLayer(IDataset* dataset, const char* creationString);
	virtual void ReleaseLayer(ILayer*);

	virtual void Release();
};


END_NAME_SPACE(tGis, Core)


#endif


