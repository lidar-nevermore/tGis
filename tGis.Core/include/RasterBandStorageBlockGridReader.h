#pragma once

#ifndef __I_RASTERBANDSTORAGEBLOCKGRIDREADER_H__
#define __I_RASTERBANDSTORAGEBLOCKGRIDREADER_H__

#include "Helper.h"
#include "gdal.h"
#include "gdal_priv.h"

BEGIN_NAME_SPACE(tGis, Core)

//�ֿ�洢��ʽ�洢��դ�����ݵĴ洢�����ʽ��ȡ��
//
//���ݰ����ȡ����ʱ��ȡ��Ĵ�С�������ѵĴ洢�黺������С�ʹ洢�������ʽ
//Ȼ���տ������ʽ��ȡ�ļ��е����д洢��
//�洢���ʾ���շֿ�洢��ʽ�洢դ������ʱ�����ֳ������ݿ�
class TGIS_API RasterBandStorageBlockGridReader
{
public:
	// width ÿ����Ҫ��ȡ��Ŀ�� ��λ����
	// height ÿ����Ҫ��ȡ��ĸ߶� ��λ����
	// mlimit �ڴ滺����ռ������ ��λMB
	// left ��ȡ��Χ�����������(��)  ��λ����
	// top ��ȡ��Χ���ϲ�����(��) ��λ����
	// right ��ȡ��Χ���Ҳ�����(����) ��λ����
	// bottom ��ȡ��Χ��ײ�����(����) ��λ����
	RasterBandStorageBlockGridReader(GDALRasterBand* band,int width,int height,int left = 0,int top = 0,int right = -1,int bottom = -1,int mlimit = 16);
	~RasterBandStorageBlockGridReader(void);

	//�������� ORDER_ROW ��������� ORDER_COL ���������
	int GetMajorOrder();

	//��ȡ�洢�黺�����ˮƽ������ռ�ݵĿ�����
	int GetStorageBlockCountX();

	//��ȡ�洢�黺�������ֱ������ռ�ݵĿ�����
	int GetStorageBlockCountY();

	//��ȡ�洢�黺������ָ��
	void** GetGridHandler();

	//��ȡ�洢�黺�������ˮƽ��storageBlockPosX����Ŀ�� ��λ����
	int GetStorageBlockSizeX(int storageBlockPosX);

	//��ȡ�洢�黺���������ֱ��bufferBlockPosY����ĸ߶� ��λ����
	int GetStorageBlockSizeY(int storageBlockPosY);

	//���¿�ʼ������ȡ���½�֮��Ķ����õ��������������ֱ�Ӷ�ȡ
	void ResetReading();

	//���ö�ȡ��Ĵ�С������С�ڹ����˶�ȡ��ʱ����Ķ�ȡ���С
	void SetReadingBlockSize(int width,int height);

	bool ContainReadingBlock(int x,int y);

	//��ȡȡ��x��yΪ���Ͻ������߷ֱ���width��height��һ�����أ��ڴ����ڲ����й�������Ҫ�ͷ�
	void* GetOneReadingBlock(int x,int y);

	//��ȡ��ǰ�洢�黺��������ʼX���� ��λ����
	int GetStorageBlockGridBeginX();

	//��ȡ��ǰ�洢�黺��������ʼY���� ��λ����
	int GetStorageBlockGridBeginY();

	//��ȡ��ǰ�洢�黺��������ֹX���� ��λ���أ��������ڻ������
	int GetStorageBlockGridEndX();

	//��ȡ��ǰ�洢�黺��������ֹY���� ��λ���أ��������ڻ������
	int GetStorageBlockGridEndY();

	//���ձ�����������ǰ������ȡһ�л���һ�п�
	//���洢�黺������ڲ������ݶ�ȡ���֮�󣬵��ô˽ӿ�
	int MoveNext();

public:
	bool MoveUp();

	bool MoveDown();

	bool MoveRight();

	bool MoveLeft();

	//ʹ�ö�ȡ���������ٵķ�ʽ�� �黺����� �ƶ��������������� x y
	//
	//��preferredDirectionΪMOV_NONEʱ
	//������� �黺����� �Ѿ����� x y �������ƶ�
	//������� �黺����� ��Ҫ��ȫ�ƶ� �ᾡ���� �黺����� ������λ�ö��� x y
	//������� �黺����� ����Ҫ��ȫ�ƶ� ������С�ƶ��� �黺����� �ƶ���ǡ�ð��� x y
	//
	//��preferredDirection��ΪMOV_NONEʱ
	//������ preferredDirection ָ���ķ���ִ��һ������ƶ�
	//������ preferredDirection �����ķ���ִ����С�ƶ�
	//�������磬����ΪMOV_DOWN�������ƶ� �黺����� ��������һ�д洢�������������x,y
	bool MoveArbitrarily(int x, int y,int preferredDirection = MOV_NONE);

public:
	const static int ORDER_ROW = 0;
	const static int ORDER_COL = 1;

	const static int MOV_NONE = -1;
	const static int MOV_UP = 0;
	const static int MOV_DOWN = 1;
	const static int MOV_LEFT = 2;
	const static int MOV_RIGHT = 3;

private:
	void FreeMemory();

private:
	CPL_DISALLOW_COPY_ASSIGN(RasterBandStorageBlockGridReader)

private:
	GDALRasterBand* _band;
	GDALDataType _dataType;
	int _dataBytes;
	//ͼ��Ŀ�� ��λ����
	int _xRasterSize;
	int _yRasterSize;
	//��ߴ� ��λ����
	int _xBlockSize;
	int _yBlockSize;

	//��ȡ������ߴ�
	int _xMaxReadSize;
	int _yMaxReadSize;
	//��ȡ���ʵ�ʳߴ�
	int _xReadSize;
	int _yReadSize;
	//�ݴ��ȡ�����ݵ��ڴ�
	void* _readBuffer;

	//����������End��β�ı������������ڻ���������Ч��Χ��

	//AOI���� �������½ǵ���������
	int _xAoiBegin;
	int _yAoiBegin;
	int _xAoiEnd;
	int _yAoiEnd;
	//AOI��������سߴ�
	int _xAoiSize;
	int _yAoiSize;

	//AOI���� �������½ǵĿ���������
	int _xAoiBlockBegin;
	int _yAoiBlockBegin;
	int _xAoiBlockEnd;
	int _yAoiBlockEnd;
	//AOI���� ˮƽ����ֱ������ռ�ݵĿ�����
	int _xAoiBlockCount;
	int _yAoiBlockCount;

#pragma region ����������Ϣ�����ڱ�����������еĵ�ǰֵ

	//�������� 0 ��������� 1 ���������
	int _majorOrder;
	//�洢�黺�����ָ������ ������洢
	void* _gridHandler;
	//�洢�黺������ÿһ���黺��ָ���Ƿ���Ч�ı�� ������洢
	//��ָ�����MoveArbitrarily������ʹ��
	unsigned char* _gridHandlerFlag;
	//�洢�黺����� ˮƽ����ֱ������ռ�ݵĿ�����
	int _xBlockCount;
	int _yBlockCount;

	//�洢�黺����� �������½ǵĿ���������
	int _xBlockBegin;
	int _yBlockBegin;
	int _xBlockEnd;
	int _yBlockEnd;
	//�洢�黺����� �������½ǵ���������
	int _xBegin;
	int _yBegin;
	int _xEnd;
	int _yEnd;
	//�洢�黺����� ���½ǵĿ�ߴ� ��λ����
	int _xEndBlockSize;
	int _yEndBlockSize;
	//�´��ƶ����� 
	int _lastMoveDirection;

#pragma endregion ����������Ϣ�����ڱ�����������еĵ�ǰֵ
};



END_NAME_SPACE(tGis, Core)


#endif


