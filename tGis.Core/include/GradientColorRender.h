#pragma once

#ifndef __GradientColorRender_H__
#define __GradientColorRender_H__


#include "Helper.h"
#include "ILayerRender.h"


#include "ogr_core.h"
#include <assert.h>

BEGIN_NAME_SPACE(tGis, Core)

struct ILayer;

class GradientColor;

class TGIS_CORE_API GradientColorRender : public ILayerRender
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	GradientColorRender(ILayer* layer, GradientColor* color = nullptr);
	~GradientColorRender();

public:
	void SetGradientColor(GradientColor* color);

	void SetHeight(int height);

	int GetHeight()
	{
		return _height;
	}

private:
	ILayer* _layer;
	GradientColor* _color;
	OGREnvelope _envelope;
	int _height;

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
