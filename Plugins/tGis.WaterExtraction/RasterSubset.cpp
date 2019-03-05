#include "RasterSubset.h"


RasterSubset::RasterSubset(GDALDataset* raster,int xOffset, int yOffset, int xSize, int ySize)
	:_inRasterWraper()
{
	_inRaster = raster;
	_inRasterWraper.Attach(_inRaster,false);
	_xOffset = xOffset;
	_yOffset = yOffset;
	_xSize = xSize;
	_ySize = ySize;
}


RasterSubset::~RasterSubset(void)
{
}

class WriteBlockFunctor
{
private:
	int _xOffset;//输出影像在输入影像左上角得x坐标
	int _yOffset;//输出影像在输入影像左上角得y坐标

	GDALDataType _dataType;
	int _dataBytes;

	GDALRasterBand* _outBand;

public:
	WriteBlockFunctor(
		int xOffset,
	    int yOffset,
		GDALRasterBand* outBand,
		GDALDataType dataType)
	{
	 _xOffset = xOffset;
	 _yOffset = yOffset;
	 _outBand = outBand;
	 _dataType = dataType;
	 _dataBytes = GDALGetDataTypeSizeBytes(_dataType);
	}

	~WriteBlockFunctor()
	{
	}

	void operator ()(GDALRasterBand* band, StorageBlockBuffer* block)
	{
		int curXBlockLTC = block->xBlockLTC+block->xStartPosIB - _xOffset;
		int curYBlockLTC = block->yBlockLTC+block->yStartPosIB - _yOffset;

		int dx = block->xValidBlockSize;
		int dy = block->yValidBlockSize;

		int ls = block->xBlockSize*_dataBytes;
		char* vb = ((char*)block->Buffer)+block->yStartPosIB*ls+block->xStartPosIB*_dataBytes;

		if(dy==1 
			|| ( block->xStartPosIB == 0 
			&& block->xEndPosIB == (block->xBlockSize-1)))
		{
			_outBand->RasterIO(GF_Write, curXBlockLTC, curYBlockLTC, dx, dy, vb, dx, dy, _dataType,0,0);
		}
		else
		{
			for(int i = 0 ; i< dy; i++)
			{
				_outBand->RasterIO(GF_Write, curXBlockLTC, curYBlockLTC, dx, 1, vb, dx, 1, _dataType,0,0);
				curYBlockLTC++;
				vb += ls;
			}
		}		
	}
};

void WriteSubsetBlock(void* user, GDALRasterBand* band, StorageBlockBuffer* block)
{
	WriteBlockFunctor* functor = (WriteBlockFunctor*)user;
	functor->operator()(band, block);
}

void RasterSubset::Process(const char* outRasterPath)
{
	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	char **papszOptions = nullptr;

	GDALDataset* outRaster = poDriver->Create(outRasterPath,_xSize,_ySize,_inRaster->GetRasterCount(),_inRaster->GetRasterBand(1)->GetRasterDataType(),papszOptions);
	double geoTransform[6];
	_inRaster->GetGeoTransform(geoTransform);

	_inRasterWraper.Pixel2Spatial(_xOffset,_yOffset,geoTransform,geoTransform+3);
	outRaster->SetGeoTransform(geoTransform);
	const char* p = _inRaster->GetProjectionRef();
	if(p != nullptr && strcmp(p,"")!=0)
		outRaster->SetProjection(p);

	int rasterCount = _inRaster->GetRasterCount();

	for(int i = 1; i <= rasterCount; i++)
	{
		GDALRasterBand* inBand = _inRaster->GetRasterBand(i);
		//outRaster->AddBand(inBand->GetRasterDataType());
		GDALRasterBand* outBand = outRaster->GetRasterBand(i);

		int suc = 0;
		double noDataValue = inBand->GetNoDataValue(&suc);
		if (suc != 0)
			outBand->SetNoDataValue(noDataValue);

		RasterBandSeqPixelReader reader(_inRaster,i,_xOffset,_yOffset,_xSize,_ySize);
		WriteBlockFunctor functor(_xOffset,_yOffset,outBand,inBand->GetRasterDataType());
		reader.ForEachBlock(WriteSubsetBlock, &functor);
		outBand->FlushCache();
	}

	outRaster->FlushCache();
	GDALClose((GDALDatasetH)outRaster);
}
