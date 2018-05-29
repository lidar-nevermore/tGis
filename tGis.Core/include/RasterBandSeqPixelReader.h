#pragma once

#ifndef __I_RASTERBANDSEQPIXELREADER_H__
#define __I_RASTERBANDSEQPIXELREADER_H__

#include "Helper.h"

#include "gdal.h"
#include "gdal_priv.h"


BEGIN_NAME_SPACE(tGis, Core)

typedef struct _StorageBlockBuffer
{
	//��ǰ�������Ŀ�ָ��
	void* Buffer; 
	//xBlockOff,yBlockOff ��ǰ��������ı�ţ�Ҳ���ǵڼ�����
	int xBlockOff; 
	int yBlockOff;
	//xBlockSize,yBlockSize ��ǰ������������سߴ�
	int xBlockSize;
	int yBlockSize;
	//xBlock,yBlock ��ǰ�����Ͻǵ� ��������
	int xBlockLTC;
	int yBlockLTC;
	//xValidBlockSize,yValidBlockSize ��ǰ���� ����AOI��Ӿ����еĲ��� �����سߴ�
	int xValidBlockSize;
	int yValidBlockSize;
	//xStartPosIB,yStartPosIB ��ǰ���� ����AOI��Ӿ����еĲ��� �����Ͻ� ��Block�е�λ��
	int xStartPosIB;
	int yStartPosIB;
	//xEndPosIB,yEndPosIB ��ǰ���� ����AOI��Ӿ����еĲ��� �����½� ��Block�е�λ��
	int xEndPosIB;
	int yEndPosIB;
} StorageBlockBuffer;


class TGIS_API RasterBandSeqPixelReader
{
public:
	typedef void(*FOREACHPIXEL_FUNC)(void* user, GDALRasterBand* band, double pix, int x, int y, void* orgPix, StorageBlockBuffer* block, int xPosIB, int yPosIB);
	typedef void(*FOREACHBLOCK_FUNC)(void* user, GDALRasterBand* band, StorageBlockBuffer* block);

public:
	RasterBandSeqPixelReader(GDALDataset* raster,int band,GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	RasterBandSeqPixelReader(GDALDataset* raster,GDALRasterBand* band,GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	RasterBandSeqPixelReader(GDALDataset* raster,int band,int xOffset, int yOffset, int xSize, int ySize);
	RasterBandSeqPixelReader(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xSize, int ySize);
	~RasterBandSeqPixelReader(void);

public:
	int GetXOffset() { return _xOffset; }

	int GetYOffset() { return _yOffset; }

	int GetXSize() { return _xSize; }

	int GetYSize() { return _ySize; }

	void ResetPixelReading();

	//��������; raster ��ǰ������Ӱ�� band ��ǰ�����Ĳ��� pix ����ֵ x,y ���ص�ͼ������ orgPix ԭʼ����ֵָ��   xPos,yPos ��ǰ�����������ڿ�������
	void ForEachPixel(FOREACHPIXEL_FUNC proc, void* user);

	void ResetBlockReading();

	//�������ؿ�; 
	void ForEachBlock(FOREACHBLOCK_FUNC proc, void* user);

private:
	int _xOffset;
	int _yOffset;
	int _xEnd;
	int _yEnd;
	int _xSize;
	int _ySize;

	int _xBlockSize;
	int _yBlockSize;

	unsigned char* _blockBuffer;

	GDALDataType _dataType;
	int _dataBytes;

	GDALDataset* _raster;
	GDALRasterBand* _band;

	int _xAoiOffset;
	int _yAoiOffset;
	int _xAoiEnd;
	int _yAoiEnd;
	int _xAoiSize;
	int _yAoiSize;

	double _xAoiPixScale;
	double _yAoiPixScale;

	GDALDataset* _aoiRaster;
	GDALRasterBand* _aoiBand;
	double _aoiNoDataValue;
	bool _aoiNoDataValueValid;

	unsigned char* _aoiBlockBuffer;


private:
	int _startXBlockOff; //����������������Ͻǿ��ˮƽ����
	int _startYBlockOff; //����������������Ͻǿ����ֱ����
	int _endXBlockOff; //����������������½ǿ��ˮƽ����
	int _endYBlockOff; //����������������½ǿ����ֱ����

	int _curBlockX; //��ǰ�����Ͻ�X��������
	int _curBlockY; //��ǰ�����Ͻ�Y��������

	int _curStartXPosIB; //��ǰ��ʼ����������Block�е�Xλ��
	int _curStartYPosIB; //��ǰ��ʼ����������Block�е�Yλ��
	int _curEndXPosIB; //��ǰ��������������Block�е�Xλ��
	int _curEndYPosIB; //��ǰ��������������Block�е�Yλ��

	void Init(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xEnd, int yEnd);

private:
	void CalcPixelTraverseParam(int xBlockOff,int yBlockOff,int xBlockSize,int yBlockSize);

	double GetPixelValue(void* pix);

private:
    CPL_DISALLOW_COPY_ASSIGN(RasterBandSeqPixelReader)

};



END_NAME_SPACE(tGis, Core)


#endif

