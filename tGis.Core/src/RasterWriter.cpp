#include "RasterWriter.h"
#include "MyGDALDataset.h"
#include "MyGDALRasterDataset.h"
#include "Event.h"

BEGIN_NAME_SPACE(tGis, Core)


struct MyGDALProgressArg
{
	Progress* progress;
	ProgressEvent* progressEvent;
};

int CPL_STDCALL MyGDALProgressFunc(double dfComplete, const char *pszMessage, void *pProgressArg)
{
	MyGDALProgressArg* arg = (MyGDALProgressArg*)pProgressArg;
	arg->progress->Value += int(100 * dfComplete);
	arg->progressEvent->Raise(*(arg->progress));
	return CE_None;
}

TGIS_API void WriteMemoryBlock(
	const char* path, 
	void** mem, GDALDataType dt, int w, int h, int count,
	ProgressEvent * progressEvent,
	GDALDataset * raster, int xOffset, int yOffset)
{
	bool supported = false;
	int extPos = 1 + _tgis_find_last_of(path, ".", 0);
	size_t drvPos = MyGDALDataset::GetSupportedRasterFormatPos(path + extPos, &supported);
	bool creatable = false;
	const char* drvName = "GTiff";
	if(supported)
		creatable = MyGDALDataset::GetSupportedRasterFormatCreatable(drvPos);
	if (creatable)
		drvName = MyGDALDataset::GetSupportedRasterFormatName(drvPos);

	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(drvName);
	char **papszOptions = nullptr;
	GDALDataset* outRaster = poDriver->Create(path, w, h, count, dt, papszOptions);
	if (raster != nullptr)
	{
		const char* p = raster->GetProjectionRef();
		if (p != nullptr && strcmp(p, "") != 0)
			outRaster->SetProjection(p);

		double geoTransform[6];
		raster->GetGeoTransform(geoTransform);
		if (xOffset > 0 || yOffset > 0)
		{
			MyGDALRasterDataset rdt(nullptr);
			rdt.Attach(raster);
			rdt.Pixel2Spatial(xOffset, yOffset, geoTransform, geoTransform + 3);
		}
		outRaster->SetGeoTransform(geoTransform);
	}

	Progress prog(0, 100 * count);
	MyGDALProgressArg proga;
	proga.progress = &prog;
	proga.progressEvent = progressEvent;

	GDALRasterIOExtraArg rasterIoArg;
	INIT_RASTERIO_EXTRA_ARG(rasterIoArg);
	rasterIoArg.pfnProgress = MyGDALProgressFunc;
	GDALRasterIOExtraArg *pRasterIoArg = progressEvent == nullptr ? nullptr : &rasterIoArg;

	for (int i = 1; i < count; i++)
	{
		GDALRasterBand* outBand = outRaster->GetRasterBand(i);
		outBand->RasterIO(GF_Write, 0, 0, w, h, mem + (i - 1), w, h, dt, 0, 0, pRasterIoArg);
		outBand->FlushCache();
	}

	outRaster->FlushCache();
	GDALClose((GDALDatasetH)outRaster);
}

TGIS_API void CopyRaster(const char* path,
	GDALDataset * raster,
	int xOffset, int yOffset, int xSize, int ySize,
	int dstXSize, int dstYSize,
	int* bands, int count,
	ProgressEvent * progressEvent)
{
}

END_NAME_SPACE(tGis, Core)
