#include "MemoryDataSourceProvider.h"
#include "MemoryDataSource.h"
#include "RasterBandSeqPixelReader.h"

#include <cassert>

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


const char* const MemoryDataSourceProvider::_name = "Memory";
const char* const MemoryDataSourceProvider::_type = "5EE4C82D-7773-4BA9-BF97-D5DD2EF4DFAD";

MemoryDataSourceProvider* MemoryDataSourceProvider::_instance = nullptr;


MemoryDataSourceProvider & MemoryDataSourceProvider::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new MemoryDataSourceProvider();
		static _tGisObjectDestructor<MemoryDataSourceProvider> shit(_instance);
	}

	return *_instance;
}

MemoryDataSourceProvider::MemoryDataSourceProvider()
{
}


MemoryDataSourceProvider::~MemoryDataSourceProvider()
{
	for (vector<IDataset*>::iterator it = _vecOpenedDataset.begin(); it != _vecOpenedDataset.end(); it++)
	{
		IDataset* dt = *it;
		if (dt->GetDataSource() == nullptr)
		{
			ReleaseMemRasterDataset(dt);
		}
	}
}

const char * MemoryDataSourceProvider::GetSupportedDataSourceType()
{
	return MemoryDataSource::S_GetType();
}

const char * MemoryDataSourceProvider::GetName()
{
	return _name;
}

const char * MemoryDataSourceProvider::GetType()
{
	return _type;
}


bool MemoryDataSourceProvider::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

bool MemoryDataSourceProvider::IsTypeOf(ITGisObject * object)
{
	if (strcmp(object->GetType(), _type) == 0)
		return true;
	return false;
}

class WriteBlockHandler
{
private:
	int _xOffset;//输出影像在输入影像左上角的x坐标
	int _yOffset;//输出影像在输入影像左上角的y坐标

	int _startProgress;
	int _maxProgress;
	int _processedPixelCount;
	int _totalPixelCount;
	ProgressEvent* _progressEvent;

	GDALDataType _dataType;
	int _dataBytes;

	GDALRasterBand* _outBand;

public:
	WriteBlockHandler(
		int xOffset,
		int yOffset,
		GDALRasterBand* outBand,
		GDALDataType dataType,
		int startProgress,
		int maxProgress,
		int totalPixelCount,
		ProgressEvent* progressEvent)
	{
		_xOffset = xOffset;
		_yOffset = yOffset;
		_outBand = outBand;
		_dataType = dataType;
		_dataBytes = GDALGetDataTypeSizeBytes(_dataType);

		_startProgress = startProgress;
		_maxProgress = maxProgress;
		_processedPixelCount = 0;
		_totalPixelCount = totalPixelCount;
		_progressEvent = progressEvent;
	}

	~WriteBlockHandler()
	{
	}

	static void WriteBlock(void* user, GDALRasterBand* band, StorageBlockBuffer* block)
	{
		WriteBlockHandler* wbh = (WriteBlockHandler*)user;
		int curXBlockLTC = block->xBlockLTC + block->xStartPosIB - wbh->_xOffset;
		int curYBlockLTC = block->yBlockLTC + block->yStartPosIB - wbh->_yOffset;

		int dx = block->xValidBlockSize;
		int dy = block->yValidBlockSize;

		int ls = block->xBlockSize*wbh->_dataBytes;
		char* vb = ((char*)block->Buffer) + block->yStartPosIB*ls + block->xStartPosIB*wbh->_dataBytes;

		if (dy == 1
			|| (block->xStartPosIB == 0
				&& block->yEndPosIB == (block->yBlockSize - 1)))
		{
			wbh->_outBand->RasterIO(GF_Write, curXBlockLTC, curYBlockLTC, dx, dy, vb, dx, dy, wbh->_dataType, 0, 0);
		}
		else
		{
			for (int i = 0; i< dy; i++)
			{
				wbh->_outBand->RasterIO(GF_Write, curXBlockLTC, curYBlockLTC, dx, 1, vb, dx, 1, wbh->_dataType, 0, 0);
				curYBlockLTC++;
				vb += ls;
			}
		}

		if (wbh->_progressEvent != nullptr)
		{
			wbh->_processedPixelCount += dx*dy;

			int v = wbh->_startProgress + wbh->_processedPixelCount * 100 / wbh->_totalPixelCount;
			Progress progress(v, wbh->_maxProgress);
			wbh->_progressEvent->Raise(progress);
		}
	}
};


MyGDALMemRasterDataset * MemoryDataSourceProvider::CreateMemRasterDataset(MyGDALRasterDataset * dataset, int left, int top, int width, int height, int b, int * bands, bool addToOpened, ProgressEvent * progressEvent)
{
	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("MEM");
	if (poDriver == nullptr)
		return nullptr;
	GDALDataset* src = dataset->GetGDALDataset();
	int totalBandCount = src->GetRasterCount();
	int bandCount = b == -1 ? totalBandCount : (b > totalBandCount ? totalBandCount : b);
	int* bandIndex = new int[bandCount];
	GDALDataset* dst = nullptr;
	int maxProgress = bandCount * 100;
	int totalPixelCount = width*height;
	for (int i = 0; i < bandCount; i++)
	{
		bandIndex[i] = b == -1 ? i + 1 : bands[i];
		GDALRasterBand* srcBand = src->GetRasterBand(bandIndex[i]);
		GDALDataType dataType = srcBand->GetRasterDataType();
		if (dst == nullptr)
		{
			dst = poDriver->Create(dataset->GetName(), width, height, 1, dataType, NULL);
			if (dst == nullptr)
			{
				break;
			}
			double geoTransform[6];
			src->GetGeoTransform(geoTransform);
			dataset->Pixel2Spatial(left, top, geoTransform, geoTransform + 3);
			dst->SetGeoTransform(geoTransform);
			const char* p = src->GetProjectionRef();
			if (p != nullptr && strcmp(p, "") != 0)
				dst->SetProjection(p);
		}
		else
		{
			dst->AddBand(dataType);
		}

		GDALRasterBand* dstBand = dst->GetRasterBand(i + 1);

		RasterBandSeqPixelReader reader(src, srcBand, left, top, width, height);
		WriteBlockHandler writer(left, top, dstBand, dataType, i * 100, maxProgress, totalPixelCount, progressEvent);
		reader.ForEachBlock(&WriteBlockHandler::WriteBlock, &writer);
		dstBand->FlushCache();
	}

	delete[] bandIndex;

	if (dst == nullptr)
		return nullptr;

	MyGDALMemRasterDataset* ret = new MyGDALMemRasterDataset(nullptr, dataset->GetName());
	ret->Attach(dst, true);
	if(addToOpened)
		this->AddOpenedDataset(ret);

	return ret;
}

void MemoryDataSourceProvider::ReleaseMemRasterDataset(IDataset * dataset, bool removeFromOpened)
{
	if(removeFromOpened)
		this->RemoveOpenedDataset(dataset);
	delete dataset;
}

END_NAME_SPACE(tGis, Core)


