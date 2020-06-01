#pragma once

#ifndef __MultiRasterBandStorageBlockWalker_H__
#define __MultiRasterBandStorageBlockWalker_H__


#include "Helper.h"
#include "tGis_CoreCfg.h"

#include "gdal.h"
#include "gdal_priv.h"

#include "RasterBandStorageBlockWalker.h"

BEGIN_NAME_SPACE(tGis, Core)

class MultiRasterBandStorageBlockWalker : public RasterBandStorageBlockWalker
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
	typedef void(__stdcall *M_FOREACHPIXEL_FUNC)(void* user,
		GDALRasterBand** band,
		double* pix, int xIM, int yIM, int xIB, int yIB, void** orgPix,
		StorageBlockInfo* blockInfo, void** blockBuffer,
		GDALDataType* bandDataType, int* bandDataBytes,
		double progress);

	//�������ؿ�ص�
	//blockInfo ��ǰ����Ϣ 
	//blockBuffer ��ǰ���ڴ� 
	//dataType ���ε���������
	//dataBytes ��������ռ�ݵ��ֽ���Ŀ
	//aoiBuffer ��ǰ������AOI��Χ�ڵĲ��ֶ�Ӧ��AOI���ػ���
	//aoiNoDataValue aoi���ص���Чֵ��û����Чֵ���ᴫ��INT_MIN
	//progress �������� ��������һ�ν��ȣ��������ֵû�иı䲻��Ҫ����ui�ϵĽ�����ʾ�����½�����ʾ������Ч��
	typedef void(__stdcall *M_FOREACHBLOCK_FUNC)(void* user,
		GDALRasterBand** band,
		StorageBlockInfo* blockInfo, void** blockBuffer,
		GDALDataType* bandDataType, int* bandDataBytes,
		unsigned char* aoiBuffer, double aoiNoDataValue,
		double progress);

public:
	MultiRasterBandStorageBlockWalker(GDALDataset* raster, int* bands, int bandCount, GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	~MultiRasterBandStorageBlockWalker(void);

private:
	static void __stdcall FOREACHPIXEL_OneBand(void* user,
		GDALRasterBand* band,
		StorageBlockInfo* blockInfo, void* blockBuffer,
		GDALDataType dataType, int dataBytes,
		unsigned char* aoiBuffer, double aoiNoDataValue,
		double progress);

	static void __stdcall FOREACHBLOCK_OneBand(void* user,
		GDALRasterBand* band,
		StorageBlockInfo* blockInfo, void* blockBuffer,
		GDALDataType dataType, int dataBytes,
		unsigned char* aoiBuffer, double aoiNoDataValue,
		double progress);

public:
	int GetBandCount() { return _bandCount; }

	void ForEachPixel(M_FOREACHPIXEL_FUNC proc, void* user);

	void ForEachBlock(M_FOREACHBLOCK_FUNC proc, void* user);

private:
	int _bandCount;
	//�����ο黺��
	void** _blockBuffer;
	//�������Ƿ�����ð����ȡ
	bool* _blockReading;
	//������ָ��
	GDALRasterBand** _bands;
	//��������������
	GDALDataType* _bandDataType;
	//���������ݳߴ�
	int* _bandDataBytes;
	//��������ʱ����������
	double* _blockPixel;
	//���ڱ�������ʱ��blockBuffer���ƶ�
	char** _blockBufferPos;

private:
    CPL_DISALLOW_COPY_ASSIGN(MultiRasterBandStorageBlockWalker)

};

END_NAME_SPACE(tGis, Core)

#endif