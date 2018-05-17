#pragma once

#include "Helper.h"

#include "RasterLayer.h"

#include <string>

using namespace std;

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)


class TGISCORE_API RasterGrayScaleLayer : public RasterLayer
{
public:
	RasterGrayScaleLayer();
	RasterGrayScaleLayer(MyGDALRasterDataset* dataset, int band);
	~RasterGrayScaleLayer();
	void SetDataset(MyGDALRasterDataset* dataset, int band);

private:
	using RasterLayer::SetDataset;

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	virtual void SetOpacity(float);

	virtual void Paint(IGeoSurface*);
	virtual void PaintByOuterResample(IGeoSurface*);
	virtual void PaintByIOResample(IGeoSurface*);

private:
	const static int _bufferAreaWidth = 352;

	//_bufferAreaWidth*_bufferAreaWidth ARGB
	unsigned char* _surfBuffer;
	//_bufferAreaWidth*_bufferAreaWidth GRAY
	unsigned char* _pixBuffer;

	GDALRasterBand* _band;
	int _bandIndex;

};

END_NAME_SPACE(tGis, Core)
