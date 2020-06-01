#include "RasterBandStorageBlockWalker.h"
#include "MyGDALRasterDataset.h"
#include "MultiRasterBandStorageBlockWalker.h"

#include <assert.h>
#include <exception>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

MultiRasterBandStorageBlockWalker::MultiRasterBandStorageBlockWalker(
	GDALDataset * raster, int * bands, int bandCount, GDALDataset * aoiRaster, int aoiBand)
	:RasterBandStorageBlockWalker(raster, bands[0], aoiRaster, aoiBand)
{
	assert(bandCount > 1);
	_bandCount = bandCount;
	_bands = (GDALRasterBand**)malloc(bandCount * sizeof(GDALRasterBand*));
	_bands[0] = _band;
	_bandDataType = (GDALDataType*)malloc(bandCount * sizeof(GDALDataType));
	_bandDataType[0] = _dataType;
	for (int i = 1; i < bandCount; i++)
	{
		_bands[i] = raster->GetRasterBand(bands[i]);
		if (_bands[i] == nullptr)
		{
			free(_bands);
			free(_bandDataType);
			throw std::exception("Band is invalid!");
		}
		_bandDataType[i] = _bands[i]->GetRasterDataType();

		if (_bandDataType[i] > 7 || _bandDataType[i] == 0)
		{
			free(_bands);
			free(_bandDataType);
			throw std::exception("不支持复数和未定义的像素格式");
		}
	}

	_blockReading = (bool*)malloc(bandCount * sizeof(bool));
	_blockBuffer = (void**)malloc(bandCount * sizeof(void*));	
	_bandDataBytes = (int*)malloc(bandCount * sizeof(int));
	_bandDataBytes[0] = _dataBytes;
	int blockSize = _xBlockSize*_yBlockSize;
	int blockBytes = blockSize*_dataBytes;
	for (int i = 1; i < bandCount; i++)
	{
		int xBlockSize;
		int yBlockSize;		
		_bands[i]->GetBlockSize(&xBlockSize, &yBlockSize);

		if (xBlockSize != _xBlockSize
			|| yBlockSize != _yBlockSize)
			_blockReading[i] = false;
		else
			_blockReading[i] = true;
		_bandDataBytes[i] = GDALGetDataTypeSizeBytes(_bandDataType[i]);
		int rblockBytes = blockBytes;
		if (_bandDataBytes[i] != _dataBytes)
			rblockBytes = blockSize*_bandDataBytes[i];
		_blockBuffer[i] = malloc(rblockBytes);
	}

	_blockPixel = (double*)malloc(bandCount * sizeof(double));
	_blockBufferPos = (char**)malloc(bandCount * sizeof(char*));
}

MultiRasterBandStorageBlockWalker::~MultiRasterBandStorageBlockWalker(void)
{
	free(_bands);
	free(_bandDataType);
	free(_bandDataBytes);
	free(_blockReading);
	free(_blockBuffer);

	free(_blockPixel);
	free(_blockBufferPos);
}

struct __UserData
{
	union
	{
		MultiRasterBandStorageBlockWalker::M_FOREACHPIXEL_FUNC _pixel_proc;
		MultiRasterBandStorageBlockWalker::M_FOREACHBLOCK_FUNC _block_proc;
	};
	MultiRasterBandStorageBlockWalker* _pThis;
	void* _cliUser;
};

void MultiRasterBandStorageBlockWalker::FOREACHPIXEL_OneBand(void* user,
	GDALRasterBand* band,
	StorageBlockInfo* blockInfo, void* blockBuffer,
	GDALDataType dataType, int dataBytes,
	unsigned char* aoiBuffer, double aoiNoDataValue,
	double progress)
{
	__UserData* ud = (__UserData*)user;

	for (int i = 1; i < ud->_pThis->_bandCount; i++)
	{
		if (ud->_pThis->_blockReading[i])
			ud->_pThis->_bands[i]->ReadBlock(blockInfo->xBlockOff, blockInfo->yBlockOff, ud->_pThis->_blockBuffer[i]);
		else
			ud->_pThis->_bands[i]->RasterIO(GF_Read, blockInfo->xBlockLTC, blockInfo->yBlockLTC, blockInfo->xBlockSize, blockInfo->yBlockSize,
				ud->_pThis->_blockBuffer[i], blockInfo->xBlockSize, blockInfo->yBlockSize, ud->_pThis->_bandDataType[i], 0, 0);
	}

	ud->_pThis->_blockBuffer[0] = blockBuffer;

	int curStartXPosIB = blockInfo->xStartPosIB;
	int curStartYPosIB = blockInfo->yStartPosIB;
	int curEndXPosIB = blockInfo->xEndPosIB;
	int curEndYPosIB = blockInfo->yEndPosIB;
	int curBlockX = blockInfo->xBlockLTC;
	int curBlockY = blockInfo->yBlockLTC;

	unsigned char* curAoi = aoiBuffer;

	for (int yp = curStartYPosIB; yp <= curEndYPosIB; yp++)
	{
		for (int i = 0; i < ud->_pThis->_bandCount; i++)
			ud->_pThis->_blockBufferPos[i] = (char*)(ud->_pThis->_blockBuffer[i]) + yp*blockInfo->xBlockSize*ud->_pThis->_bandDataBytes[i];

		for (int xp = curStartXPosIB; xp <= curEndXPosIB; xp++)
		{
			if (curAoi == nullptr || (*curAoi > 0 && *curAoi != ud->_pThis->_aoiNoDataValue))
			{
				for (int i = 0; i < ud->_pThis->_bandCount; i++)
				{
					char* curPix = ud->_pThis->_blockBufferPos[i] + xp*ud->_pThis->_bandDataBytes[i];
					ud->_pThis->_blockPixel[i] = MyGDALGetPixelValue(ud->_pThis->_bandDataType[i], curPix);
				}
				ud->_pixel_proc(ud->_cliUser,
					ud->_pThis->_bands,
					ud->_pThis->_blockPixel, curBlockX + xp, curBlockY + yp, xp, yp, (void**)ud->_pThis->_blockBufferPos,
					blockInfo, ud->_pThis->_blockBuffer, 
					ud->_pThis->_bandDataType, ud->_pThis->_bandDataBytes, 
					progress);
			}

			if (curAoi != nullptr)
				curAoi += 1;
		}
	}
}

void MultiRasterBandStorageBlockWalker::FOREACHBLOCK_OneBand(void* user,
	GDALRasterBand* band,
	StorageBlockInfo* blockInfo, void* blockBuffer,
	GDALDataType dataType, int dataBytes,
	unsigned char* aoiBuffer, double aoiNoDataValue,
	double progress) 
{
	__UserData* ud = (__UserData*)user;

	for (int i = 1; i < ud->_pThis->_bandCount; i++)
	{
		if (ud->_pThis->_blockReading[i])
			ud->_pThis->_bands[i]->ReadBlock(blockInfo->xBlockOff, blockInfo->yBlockOff, ud->_pThis->_blockBuffer[i]);
		else
			ud->_pThis->_bands[i]->RasterIO(GF_Read, blockInfo->xBlockLTC, blockInfo->yBlockLTC, blockInfo->xBlockSize, blockInfo->yBlockSize,
				ud->_pThis->_blockBuffer[i], blockInfo->xBlockSize, blockInfo->yBlockSize, ud->_pThis->_bandDataType[i], 0, 0);
	}

	ud->_pThis->_blockBuffer[0] = blockBuffer;

	ud->_block_proc(ud->_cliUser,
		ud->_pThis->_bands,
		blockInfo, ud->_pThis->_blockBuffer,
		ud->_pThis->_bandDataType, ud->_pThis->_bandDataBytes,
		aoiBuffer, aoiNoDataValue,
		progress);
}

void MultiRasterBandStorageBlockWalker::ForEachPixel(M_FOREACHPIXEL_FUNC proc, void* user)
{
	__UserData ud;
	ud._pixel_proc = proc;
	ud._pThis = this;
	ud._cliUser = user;

	RasterBandStorageBlockWalker::ForEachBlock(FOREACHBLOCK_OneBand, &ud);
}

void MultiRasterBandStorageBlockWalker::ForEachBlock(M_FOREACHBLOCK_FUNC proc, void* user)
{
	__UserData ud;
	ud._block_proc = proc;
	ud._pThis = this;
	ud._cliUser = user;

	RasterBandStorageBlockWalker::ForEachBlock(FOREACHBLOCK_OneBand, &ud);
}

END_NAME_SPACE(tGis, Core)
