#include "RasterWriter.h"
#include "MyGDALDataset.h"
#include "MyGDALRasterDataset.h"
#include "Event.h"

BEGIN_NAME_SPACE(tGis, Core)


struct WriteMemoryBlockPrgArg
{
	WriteMemoryBlockPrgArg()
	{
		stepInfo = nullptr;
		maxValue = 100;
		totalValue = 0;
	}
	char* stepInfo;
	int maxValue;
	int totalValue;
	double stepRatio;
	ProgressEventHandler* progressHandler;
};

int CPL_STDCALL WriteMemoryBlockPrgFunc(double dfComplete, const char *pszMessage, void *pProgressArg)
{
	WriteMemoryBlockPrgArg* arg = (WriteMemoryBlockPrgArg*)pProgressArg;
	int stepValue = int(arg->maxValue * dfComplete);
	int totalValue = arg->totalValue + int(arg->stepRatio*stepValue);
	Progress prg(totalValue,
		stepValue,
		arg->maxValue,
		pszMessage,
		arg->stepInfo);
	(*(arg->progressHandler))(prg);

	return TRUE;
}

TGIS_CORE_API void WriteMemoryBlock(
	const char* path, 
	void** mem, GDALDataType dt, int w, int h, int count, double noDataValue,
	ProgressEventHandler * progressHandler,
	GDALDataset * raster, int xOffset, int yOffset, int xSize, int ySize)
{
	bool supported = false;
	int extPos = 1 + _tgis_find_last_of(path, ".", 0);
	size_t drvPos = MyGDALDataset::GetSupportedRasterFormatPos(path + extPos, &supported);
	bool creatable = false;
	int drvIndex = -1;
	if(supported)
		creatable = MyGDALDataset::GetSupportedRasterFormatCreatable(drvPos);
	if (creatable)
		drvIndex = MyGDALDataset::GetSupportedRasterFormatDriverIndex(drvPos);

	GDALDriver *poDriver = nullptr;
	char **papszOptions = nullptr;
	double geoTransform[6] = { 0,1,0,0,0,1 };
	if (drvIndex > -1)
		poDriver = GetGDALDriverManager()->GetDriver(drvIndex);
	else
		poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");	
	GDALDataset* outRaster = poDriver->Create(path, w, h, count, dt, papszOptions);
	if (raster != nullptr)
	{
		const char* p = raster->GetProjectionRef();
		if (p != nullptr && strcmp(p, "") != 0)
			outRaster->SetProjection(p);

		raster->GetGeoTransform(geoTransform);
		if (xOffset > 0 || yOffset > 0)
		{
			MyGDALRasterDataset rdt;
			rdt.Attach(raster);
			rdt.Pixel2Spatial(xOffset, yOffset, geoTransform, geoTransform + 3);
		}

		if (xSize != w)
		{
			double xRatio = (double)xSize / w;
			geoTransform[1] *= xRatio;
			geoTransform[2] *= xRatio;
		}

		if (ySize != h)
		{
			double yRatio = (double)ySize / h;
			geoTransform[4] *= yRatio;
			geoTransform[5] *= yRatio;
		}
	}
	outRaster->SetGeoTransform(geoTransform);

	WriteMemoryBlockPrgArg prga;
	prga.progressHandler = progressHandler;
	prga.stepRatio = 1.0 / count;

	GDALRasterIOExtraArg rasterIoArg;
	INIT_RASTERIO_EXTRA_ARG(rasterIoArg);
	rasterIoArg.pfnProgress = WriteMemoryBlockPrgFunc;
	rasterIoArg.pProgressData = &prga;
	GDALRasterIOExtraArg *pRasterIoArg = progressHandler == nullptr ? nullptr : &rasterIoArg;
	char stepInfoBuf[1000];
	for (int i = 1; i <= count; i++)
	{
		sprintf(stepInfoBuf, "正在写入第%d通道，共%d通道", i, count);
		prga.stepInfo = stepInfoBuf;
		GDALRasterBand* outBand = outRaster->GetRasterBand(i);
		outBand->SetNoDataValue(noDataValue);
		outBand->RasterIO(GF_Write, 0, 0, w, h, mem[i - 1], w, h, dt, 0, 0, pRasterIoArg);
		outBand->FlushCache();

		prga.totalValue += prga.maxValue*i/count;
	}

	if (progressHandler != nullptr)
	{
		Progress prg(prga.maxValue,
			prga.maxValue,
			prga.maxValue,
			nullptr,
			stepInfoBuf);
		(*progressHandler)(prg);
	}

	outRaster->FlushCache();
	GDALClose((GDALDatasetH)outRaster);
}

TGIS_CORE_API void CopyRaster(const char* path,
	GDALDataset * raster,
	int xOffset, int yOffset, int xSize, int ySize,
	int dstXSize, int dstYSize,
	int* bands, int count,
	ProgressEventHandler * progressHandler)
{
}

END_NAME_SPACE(tGis, Core)
