#pragma once

#ifndef __RASTERBANDSEQPIXELREADER_H__
#define __RASTERBANDSEQPIXELREADER_H__

#include "Helper.h"
#include "tGis_CoreCfg.h"

#include "gdal.h"
#include "gdal_priv.h"


BEGIN_NAME_SPACE(tGis, Core)

//Ӱ��һ��ᰴ����д洢��ÿ���洢��ռ�������ĵ�ַ�ռ�
//����readblock�ӿ�һ�ζ�ȡһ���飬���и��ߵĶ�дЧ��
struct StorageBlockInfo
{
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
} ;


class TGIS_CORE_API RasterBandStorageBlockWalker
{
public:
	//�������ػص�
	//band ��ǰ�����Ĳ��� 
	//pix ����ֵ 
	//xIM, yIM ���ص�ͼ������ 
	//xIB, yIB ��ǰ�����������ڿ������� 
	//orgPix ԭʼ����ֵָ��(ָ��blockBuffer��ĳ��������������band��DataTypeȷ��) 
	//blockInfo ��ǰ����Ϣ 
	//blockBuffer ��ǰ���ڴ� 
	//dataType ���ε���������
	//dataBytes ��������ռ�ݵ��ֽ���Ŀ
	//progress �������� ��������һ�ν��ȣ��������ֵû�иı䲻��Ҫ����ui�ϵĽ�����ʾ�����½�����ʾ������Ч��
	//
	//functor����lambda���ʽ���ñ�ֱ�Ӻ���ָ�����Ҫ���ܶ�
	//���Ƽ��øýӿ�
	typedef void(__stdcall *FOREACHPIXEL_FUNC)(void* user, 
		GDALRasterBand* band,
		double pix, int xIM, int yIM, int xIB, int yIB, void* orgPix,
		StorageBlockInfo* blockInfo, void* blockBuffer,
		GDALDataType dataType, int dataBytes,
		double progress);

	//�������ؿ�ص�
	//blockInfo ��ǰ����Ϣ 
	//blockBuffer ��ǰ���ڴ� 
	//dataType ���ε���������
	//dataBytes ��������ռ�ݵ��ֽ���Ŀ
	//aoiBuffer ��ǰ������AOI��Χ�ڵĲ��ֶ�Ӧ��AOI���ػ���
	//aoiNoDataValue aoi���ص���Чֵ��û����Чֵ���ᴫ��INT_MIN
	//progress �������� ��������һ�ν��ȣ��������ֵû�иı䲻��Ҫ����ui�ϵĽ�����ʾ�����½�����ʾ������Ч��
	typedef void(__stdcall *FOREACHBLOCK_FUNC)(void* user,
		GDALRasterBand* band,
		StorageBlockInfo* blockInfo, void* blockBuffer,
		GDALDataType dataType, int dataBytes,
		unsigned char* aoiBuffer, double aoiNoDataValue,
		double progress);

public:
	RasterBandStorageBlockWalker(GDALDataset* raster,int band,GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	RasterBandStorageBlockWalker(GDALDataset* raster,GDALRasterBand* band,GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	RasterBandStorageBlockWalker(GDALDataset* raster,int band,int xOffset, int yOffset, int xSize, int ySize);
	RasterBandStorageBlockWalker(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xSize, int ySize);
	~RasterBandStorageBlockWalker(void);

public:
	int GetXOffset() { return _xOffset; }

	int GetYOffset() { return _yOffset; }

	int GetXSize() { return _xSize; }

	int GetYSize() { return _ySize; }

	int GetXBlockSize() { return _xBlockSize; }

	int GetYBlockSize() { return _yBlockSize; }

	void ResetPixelReading();

	//��������
	void ForEachPixel(FOREACHPIXEL_FUNC proc, void* user);

	void ResetBlockReading();

	//�������ؿ�; 
	void ForEachBlock(FOREACHBLOCK_FUNC proc, void* user);

protected:
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

private:
    CPL_DISALLOW_COPY_ASSIGN(RasterBandStorageBlockWalker)

};



END_NAME_SPACE(tGis, Core)


#endif

