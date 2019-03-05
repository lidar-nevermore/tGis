#include "RasterBinarize.h"

#include "ogrsf_frmts.h"
#include <algorithm>

using namespace std;

void RasterBinarize::Init()
{
	_inBand->GetBlockSize(&_xBufferSize,&_yBufferSize);
	_outBufferSize = _xBufferSize*_yBufferSize;
	_outBuffer = new unsigned char[_outBufferSize]();
}

RasterBinarize::RasterBinarize(const char* inRasterPath,const int band,const char* aoiRasterPath,const int aoiBand)
	:_inRasterWraper(),_aoiRasterWraper()
{
	_aoiRaster = nullptr;
	_aoiBand = aoiBand;
	_inRasterWraper.Attach(inRasterPath,GA_ReadOnly,true);
	_inRaster = _inRasterWraper.GetGDALDataset();
	_inBand = _inRaster->GetRasterBand(band);
	if(aoiRasterPath != nullptr)
	{
		_aoiRasterWraper.Attach(aoiRasterPath,GA_ReadOnly,true);
		_aoiRaster = _aoiRasterWraper.GetGDALDataset();
	}
	Init();
}

RasterBinarize::RasterBinarize(GDALDataset* inRaster,int band,GDALDataset* aoiRaster,const int aoiBand)
	:_inRasterWraper(),_aoiRasterWraper()
{
	_aoiRaster = aoiRaster;
	_aoiBand = aoiBand;
	_inRaster = inRaster;
	_inBand = _inRaster->GetRasterBand(band);
	_inRasterWraper.Attach(_inRaster,false);

	if(_aoiRaster != nullptr)
		_aoiRasterWraper.Attach(_aoiRaster,false);

	Init();
}

RasterBinarize::~RasterBinarize(void)
{
	delete [] _outBuffer;
}

class BiPixelFunctor
{
private:
	int _curXBlockOff;
	int _curYBlockOff;

	int _curXBlockLTC;
	int _curYBlockLTC;
	int _curXBlockSize;
	int _curYBlockSize;

	int _xOffset;//输出影像在输入影像左上角得x坐标
	int _yOffset;//输出影像在输入影像左上角得y坐标

	unsigned char _fValue;
	unsigned char* _outBuffer;
	int _outBufferSize;
	bool _save;

	GDALRasterBand* _outBand;

	int* _pixCount;

	const std::function<int(double pix)> _func;

public:
	BiPixelFunctor(
		unsigned char *outBuffer,
		int outBufferSize,
		int xOffset,
	    int yOffset,
		GDALRasterBand* outBand,
		int* pixCount,
		unsigned char fPixValue,
		const std::function<int(double pix)>& func)
		:_func(func)
	{
	 _outBuffer = outBuffer;
	 _outBufferSize = outBufferSize;

	 _xOffset = xOffset;
	 _yOffset = yOffset;
	 _fValue = fPixValue;
	 _outBand = outBand;
	 _pixCount = pixCount;
	 _curXBlockOff = -1;
	 _curYBlockOff = -1;
	 _save = false;
	 memset(_outBuffer,0,_outBufferSize);
	}

	~BiPixelFunctor()
	{

	}

	void FlushCache()
	{
		if (_save)
		{
			_save = false;
			_outBand->RasterIO(GF_Write, _curXBlockLTC, _curYBlockLTC, _curXBlockSize, _curYBlockSize, _outBuffer, _curXBlockSize, _curYBlockSize, GDT_Byte, 0, 0);
		}
	}

	void operator ()(GDALRasterBand* band,double pix, int x,int y, void* orgPix, StorageBlockBuffer* block, int xPosIB, int yPosIB)
	{
		if(_curXBlockOff != block->xBlockOff || _curYBlockOff != block->yBlockOff)
		{
			if(_save)
			    _outBand->RasterIO(GF_Write,_curXBlockLTC,_curYBlockLTC,_curXBlockSize,_curYBlockSize,_outBuffer,_curXBlockSize,_curYBlockSize,GDT_Byte,0,0);
			_save = true;

			_curXBlockLTC = block->xBlockLTC+block->xStartPosIB-_xOffset;
			_curYBlockLTC = block->yBlockLTC+block->yStartPosIB-_yOffset;
			_curXBlockOff = block->xBlockOff;
			_curYBlockOff = block->yBlockOff;
			_curXBlockSize = block->xValidBlockSize;
			_curYBlockSize = block->yValidBlockSize;

			memset(_outBuffer,0,_outBufferSize);
		}

		double nov = band->GetNoDataValue();
		if(pix != nov)
		{
			bool npix = _func(pix);
			if(npix)
			{
				*(_outBuffer+(yPosIB-block->yStartPosIB)*block->xValidBlockSize+(xPosIB-block->xStartPosIB)) = _fValue;
				(*_pixCount)++;
			}
		}
	}
};

void PixelBinarize(void* user, GDALRasterBand* band, double pix, int x, int y, void* orgPix, StorageBlockBuffer* block, int xPosIB, int yPosIB)
{
	BiPixelFunctor* functor = (BiPixelFunctor*)user;

	functor->operator()(band, pix, x, y, orgPix, block, xPosIB, yPosIB);
}

int RasterBinarize::Process(const char* outRasterPath,const unsigned char fPixValue,const std::function<bool(double pix)>& tester)
{
	RasterBandSeqPixelReader reader(_inRaster,_inBand,_aoiRaster,_aoiBand);

	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	char **papszOptions = nullptr;

	GDALDataset* outRaster = poDriver->Create(outRasterPath,reader.GetXSize(),reader.GetYSize(),1,GDT_Byte,papszOptions);
	double geoTransform[6];
	_inRaster->GetGeoTransform(geoTransform);

	_inRasterWraper.Pixel2Spatial(reader.GetXOffset(),reader.GetYOffset(),geoTransform,geoTransform+3);
	outRaster->SetGeoTransform(geoTransform);
	const char* p = _inRaster->GetProjectionRef();
	if(p != nullptr && strcmp(p,"")!=0)
		outRaster->SetProjection(p);
	GDALRasterBand* outBand = outRaster->GetRasterBand(1);

	int pixCount = 0;

	BiPixelFunctor functor(_outBuffer,_outBufferSize,reader.GetXOffset(),reader.GetYOffset(),outBand,&pixCount,fPixValue, tester);
	reader.ForEachPixel(PixelBinarize, &functor);
	functor.FlushCache();
    outBand->FlushCache();
	outRaster->FlushCache();
	GDALClose((GDALDatasetH)outRaster);
	return pixCount;
}