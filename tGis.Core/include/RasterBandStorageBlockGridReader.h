#pragma once

#ifndef __RASTERBANDSTORAGEBLOCKGRIDREADER_H__
#define __RASTERBANDSTORAGEBLOCKGRIDREADER_H__

#include "Helper.h"
#include "gdal.h"
#include "gdal_priv.h"
#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

//分块存储方式存储的栅格数据的存储块矩阵式读取器
//
//根据按块读取波段时读取块的大小计算出最佳的存储块缓存矩阵大小和存储块遍历方式
//然后按照块遍历方式读取文件中的所有存储块
//存储块表示按照分块存储方式存储栅格数据时所划分出的数据块
class TGIS_API RasterBandStorageBlockGridReader
{
public:
	// width 每次需要读取块的宽度 单位像素
	// height 每次需要读取块的高度 单位像素
	// mlimit 内存缓冲区占用限制 单位MB
	// left 读取范围的最左侧坐标(含)  单位像素
	// top 读取范围最上侧坐标(含) 单位像素
	// right 读取范围最右侧坐标(不含) 单位像素
	// bottom 读取范围最底部坐标(不含) 单位像素
	RasterBandStorageBlockGridReader(GDALRasterBand* band,int width,int height,int left = 0,int top = 0,int right = -1,int bottom = -1,int mlimit = 16);
	~RasterBandStorageBlockGridReader(void);

	//遍历方向 ORDER_ROW 行主序遍历 ORDER_COL 列主序遍历
	int GetMajorOrder()
	{
		return _majorOrder;
	}

	//获取存储块缓存矩阵水平方向上占据的块数量
	int GetStorageBlockCountX()
	{
		return _xBlockCount;
	}

	//获取存储块缓存矩阵竖直方向上占据的块数量
	int GetStorageBlockCountY()
	{
		return _yBlockCount;
	}

	//获取存储块缓存矩阵的指针
	void** GetGridHandler()
	{
		return (void**)_gridHandler;
	}

	int GetStorageBlockSizeX()
	{
		return _xBlockSize;
	}

	int GetStorageBlockSizeY()
	{
		return _yBlockSize;
	}

	//获取存储块缓存矩阵中水平第storageBlockPosX个块的宽度 单位像素
	int GetStorageBlockSizeX(int storageBlockPosX)
	{
		if (storageBlockPosX == _xBlockCount - 1)
			return _xEndBlockSize;
		else return _xBlockSize;
	}

	//获取存储块缓存矩阵中竖直第bufferBlockPosY个块的高度 单位像素
	int GetStorageBlockSizeY(int storageBlockPosY)
	{
		if (storageBlockPosY == _yBlockCount - 1)
			return _yEndBlockSize;
		else return _yBlockSize;
	}

	//重新开始遍历读取，新建之后的对象不用调用这个方法即可直接读取
	void ResetReading();

	//设置读取块的大小，必须小于构建此读取器时传入的读取块大小
	void SetReadingBlockSize(int width, int height)
	{
		assert(width >= 0);
		assert(height >= 0);
		assert(width <= _xMaxReadSize);
		assert(height <= _yMaxReadSize);

		_xReadSize = width;
		_yReadSize = height;
	}

	//所要读取的读取块是否包含在当前缓存矩阵中 如果不包含需要调用Move??方法使包含之
	bool ContainReadingBlock(int x, int y)
	{
		if (x >= _xBegin
			&& y >= _yBegin
			&& x + _xReadSize <= _xEnd
			&& y + _yReadSize <= _yEnd)
		{
			return true;
		}

		return false;
	}

	//获取取以x和y为左上角坐标宽高分别是width和height的一块像素，内存在内部进行管理，不需要释放
	void* GetOneReadingBlock(int x,int y);

	//获取当前存储块缓存矩阵的起始X坐标 单位像素
	int GetStorageBlockGridBeginX()
	{
		return _xBegin;
	}

	//获取当前存储块缓存矩阵的起始Y坐标 单位像素
	int GetStorageBlockGridBeginY()
	{
		return _yBegin;
	}

	//获取当前存储块缓存矩阵的终止X坐标 单位像素，不包含在缓存块里
	int GetStorageBlockGridEndX()
	{
		return _xEnd;
	}

	//获取当前存储块缓存矩阵的终止Y坐标 单位像素，不包含在缓存块里
	int GetStorageBlockGridEndY()
	{
		return _yEnd;
	}

	//按照遍历主序方向向前滚动读取一行或者一列块
	//当存储块缓存矩阵内部的数据读取完毕之后，调用此接口
	int MoveNext();

public:
	bool MoveUp();

	bool MoveDown();

	bool MoveRight();

	bool MoveLeft();

	//使用读取数据量最少的方式将 块缓存矩阵 移动到包含像素坐标 x y
	//
	//当preferredDirection为MOV_NONE时
	//——如果 块缓存矩阵 已经包含 x y 将不会移动
	//——如果 块缓存矩阵 需要完全移动 会尽量将 块缓存矩阵 的中心位置对齐 x y
	//——如果 块缓存矩阵 不需要完全移动 会以最小移动将 块缓存矩阵 移动至恰好包含 x y
	//
	//当preferredDirection不为MOV_NONE时
	//——在 preferredDirection 指定的方向执行一个最大移动
	//——在 preferredDirection 正交的方向执行最小移动
	//——比如，方向为MOV_DOWN，将会移动 块缓存矩阵 至最上面一行存储块包括像素坐标x,y
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
	//图像的宽高 单位像素
	int _xRasterSize;
	int _yRasterSize;
	//块尺寸 单位像素
	int _xBlockSize;
	int _yBlockSize;

	//读取块的最大尺寸
	int _xMaxReadSize;
	int _yMaxReadSize;
	//读取块的实际尺寸
	int _xReadSize;
	int _yReadSize;
	//暂存读取块数据的内存
	void* _readBuffer;

	//以下所有以End结尾的变量都不包含在缓冲区的有效范围内

	//AOI区域 左上右下角的像素坐标
	int _xAoiBegin;
	int _yAoiBegin;
	int _xAoiEnd;
	int _yAoiEnd;
	//AOI区域的像素尺寸
	int _xAoiSize;
	int _yAoiSize;

	//AOI区域 左上右下角的块索引坐标
	int _xAoiBlockBegin;
	int _yAoiBlockBegin;
	int _xAoiBlockEnd;
	int _yAoiBlockEnd;
	//AOI区域 水平和竖直方向上占据的块数量
	int _xAoiBlockCount;
	int _yAoiBlockCount;

#pragma region 缓存区块信息，用于保存遍历过程中的当前值

	//遍历方向 0 行主序遍历 1 列主序遍历
	int _majorOrder;
	//存储块缓存矩阵指针数组 行主序存储
	void* _gridHandler;
	//存储块缓存矩阵的每一个块缓存指针是否有效的标记 行主序存储
	//该指针仅在MoveArbitrarily方法中使用
	unsigned char* _gridHandlerFlag;
	//存储块缓存矩阵 水平和竖直方向上占据的块数量
	int _xBlockCount;
	int _yBlockCount;

	//存储块缓存矩阵 左上右下角的块索引坐标
	int _xBlockBegin;
	int _yBlockBegin;
	int _xBlockEnd;
	int _yBlockEnd;
	//存储块缓存矩阵 左上右下角的像素坐标
	int _xBegin;
	int _yBegin;
	int _xEnd;
	int _yEnd;
	//存储块缓存矩阵 右下角的块尺寸 单位像素
	int _xEndBlockSize;
	int _yEndBlockSize;
	//下次移动方向 
	int _lastMoveDirection;

#pragma endregion 缓存区块信息，用于保存遍历过程中的当前值
};



END_NAME_SPACE(tGis, Core)


#endif


