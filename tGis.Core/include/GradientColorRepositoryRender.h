#pragma once

#ifndef __GradientColorRepositoryRender_H__
#define __GradientColorRepositoryRender_H__

#include "Helper.h"
#include "ILayerRender.h"


#include "ogr_core.h"
#include <assert.h>


BEGIN_NAME_SPACE(tGis, Core)

struct ILayer;
class GradientColor;
class GradientColorRepositoryRenderImpl;

class TGIS_CORE_API GradientColorRepositoryRender : public ILayerRender
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	GradientColorRepositoryRender(ILayer* layer);
	~GradientColorRepositoryRender();

public:
	void SetHeight(int height);

	int GetHeight()
	{
		return _height;
	}

	void SetMargin(int margin);

	int GetMargi()
	{
		return _margin;
	}

	void UpdateEnvelope(int surfW, int surfH);

	//选择Surface坐标(x,y)处的色条
	GradientColor* SelectGradientColor(int x, int y);

	void SelectGradientColor(GradientColor* color);

private:
	ILayer* _layer;
	OGREnvelope _envelope;
	int _height;
	int _margin;
	int _occupy;
	//Surface上当前起始绘制的y坐标，可能小于零，只有一半露在外面时就小于零
	int _drwTopSurfY;
	size_t _topColorIdx;
	GradientColor* _selColor;
	GradientColorRepositoryRenderImpl* _impl_;

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
