
#include "RasterBandStorageBlockGridReader.h"
#include <cassert>
#include <algorithm>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

RasterBandStorageBlockGridReader::RasterBandStorageBlockGridReader( GDALRasterBand* band,int width,int height,int left /*= 0*/,int top /*= 0*/,int right /*= -1*/,int bottom /*= -1*/,int mlimit /*= 16*/ )
{
	assert(left>=0);
	assert(top>=0);
	_readBuffer = NULL;
	_gridHandler = NULL;
	_gridHandlerFlag = NULL;

	_band = band;
	_xRasterSize = _band->GetDataset()->GetRasterXSize();
	_yRasterSize = _band->GetDataset()->GetRasterYSize();

	assert(width<=_xRasterSize);
	assert(height<=_yRasterSize);

	assert(right<=_xRasterSize);
	assert(bottom<=_yRasterSize);

	_band->GetBlockSize(&_xBlockSize,&_yBlockSize);
	_dataType = _band->GetRasterDataType();
	_dataBytes = GDALGetDataTypeSizeBytes(_dataType);

	int xBlockCount = 0;
	if(_xBlockSize==_xRasterSize||width==1)
	{
		xBlockCount = 1;
	}
	else
	{
		xBlockCount = (width+_xBlockSize-1)/_xBlockSize;
		xBlockCount = xBlockCount==1?2:xBlockCount;
	}
	int yBlockCount = 0;
	if(_yBlockSize==_yRasterSize||height==1)
	{
		yBlockCount = 1;
	}
	else
	{
		yBlockCount = ((height+_yBlockSize-1)/_yBlockSize);
		yBlockCount = yBlockCount==1?2:yBlockCount;
	}
	int blockBytes = _xBlockSize*_yBlockSize*_dataBytes;
	int maxMemBytes = mlimit*1048576;

	if (xBlockCount*yBlockCount*blockBytes > maxMemBytes)
	{
		throw std::exception("指定读取的块超过缓冲区限制");
	}

	int maxTotalBlockCount = maxMemBytes/blockBytes;

	if (right < 0 || right > _xRasterSize)
	{
		right = _xRasterSize;
	}

	if (bottom < 0 || bottom > _yRasterSize)
	{
		bottom = _yRasterSize;
	}

	_xAoiBegin = left;
	_xAoiEnd = right;
	_yAoiBegin = top;
	_yAoiEnd = bottom;
	_xAoiSize = _xAoiEnd - _xAoiBegin;
	_yAoiSize = _yAoiEnd - _yAoiBegin;

	_xAoiBlockBegin = _xAoiBegin/_xBlockSize;
	_yAoiBlockBegin = _yAoiBegin/_yBlockSize;
	_xAoiBlockEnd = (_xAoiEnd+_xBlockSize-1)/_xBlockSize;
	_yAoiBlockEnd = (_yAoiEnd+_yBlockSize-1)/_yBlockSize;

	_xAoiBlockCount = _xAoiBlockEnd - _xAoiBlockBegin;
	_yAoiBlockCount = _yAoiBlockEnd - _yAoiBlockBegin;

	int maxBlockCountX = maxTotalBlockCount/yBlockCount;
	maxBlockCountX = min(maxBlockCountX,_xAoiBlockCount);

	int maxBlockCountY = maxTotalBlockCount/xBlockCount;
	maxBlockCountY = min(maxBlockCountY,_yAoiBlockCount);

	//以下遍历找出总内存消耗最少，读取字节数量最少的缓冲方案
	int minTotalReadBytes = INT_MAX;

	_xBlockCount = xBlockCount;
	_yBlockCount = yBlockCount;

	for (int i = xBlockCount; i <= maxBlockCountX; i++)
	{
		int curMaxBlockCountY = maxTotalBlockCount/i;
		if (curMaxBlockCountY < yBlockCount) break;
		curMaxBlockCountY = min(curMaxBlockCountY,maxBlockCountY);
		for (int j = yBlockCount; j <= curMaxBlockCountY; j++)
		{
			int xMoveTotalReadBytes = i*j + j*(_xAoiBlockCount-i)*(_yAoiBlockCount-j) + (_yAoiBlockCount-j)*i;
			int yMoveTotalReadBytes = i*j + i*(_xAoiBlockCount-i)*(_yAoiBlockCount-j) + (_xAoiBlockCount-i)*j;

			if (xMoveTotalReadBytes < minTotalReadBytes)
			{
				minTotalReadBytes = xMoveTotalReadBytes;
				_majorOrder = ORDER_ROW;
				_xBlockCount = i;
				_yBlockCount = j;
			}

			if (yMoveTotalReadBytes < minTotalReadBytes)
			{
				minTotalReadBytes = yMoveTotalReadBytes;
				_majorOrder = ORDER_COL;
				_xBlockCount = i;
				_yBlockCount = j;
			}
		}
	}

	_xReadSize = width;
	_yReadSize = height;
	_xMaxReadSize = width;
	_yMaxReadSize = height;

	_gridHandlerFlag = (unsigned char*)malloc(_xBlockCount*_yBlockCount);

	if (_gridHandlerFlag == NULL)
	{
		throw std::bad_alloc();
	}

	_readBuffer = malloc(_xReadSize*_yReadSize*_dataBytes);

	if (_readBuffer == NULL)
	{
		FreeMemory();
		throw std::bad_alloc();
	}

	//以下申请缓冲区
	int totalBlocksCount = _xBlockCount*_yBlockCount;
	_gridHandler = malloc(totalBlocksCount*sizeof(void*));

	if (_gridHandler == NULL)
	{
		FreeMemory();
		throw std::bad_alloc();
	}

	memset(_gridHandler,0,totalBlocksCount*sizeof(void*));

	void** blockBuffer = (void**)_gridHandler;

	for (int i = 0; i < _xBlockCount; i++)
	{
		for (int j = 0; j < _yBlockCount; j++)
		{
			*blockBuffer = malloc(blockBytes);

			if (*blockBuffer == NULL)
			{				
				FreeMemory();
				throw std::bad_alloc();
			}

			blockBuffer++;
		}
	}

	ResetReading();
}


void RasterBandStorageBlockGridReader::FreeMemory()
{
	if (_gridHandlerFlag != NULL)
	{
		free(_gridHandlerFlag);
	}

	if(_readBuffer != NULL)
	{
		free(_readBuffer);
	}

	if (_gridHandler != NULL)
	{
		void** blockBuffer = (void**)_gridHandler;

		for (int i = 0; i < _xBlockCount; i++)
		{
			for (int j = 0; j < _yBlockCount; j++)
			{
				if (*blockBuffer != NULL)
				{
					free(*blockBuffer);
				}

				blockBuffer++;
			}
		}

		free(_gridHandler);
	}
}


RasterBandStorageBlockGridReader::~RasterBandStorageBlockGridReader(void)
{
	FreeMemory();
}

void RasterBandStorageBlockGridReader::ResetReading()
{
	void** blockBuffer = (void**)_gridHandler;

	for (int j = 0; j < _yBlockCount; j++)
	{
		for (int i = 0; i < _xBlockCount; i++)
		{
			_band->ReadBlock(_xAoiBlockBegin+i,_yAoiBlockBegin+j,*blockBuffer);
			blockBuffer++;
		}
	}

	_xBlockBegin = _xAoiBlockBegin;
	_yBlockBegin = _yAoiBlockBegin;
	_xBlockEnd = _xBlockBegin+_xBlockCount;
	_yBlockEnd = _yBlockBegin+_yBlockCount;

	_band->GetActualBlockSize(_xBlockEnd-1,_yBlockEnd-1,&_xEndBlockSize,&_yEndBlockSize);

	_xBegin = _xBlockBegin*_xBlockSize;
	_yBegin = _yBlockBegin*_yBlockSize;

	_xEnd = _xBegin + (_xBlockCount-1)*_xBlockSize + _xEndBlockSize;
	_yEnd = _yBegin + (_yBlockCount-1)*_yBlockSize + _yEndBlockSize;

	_lastMoveDirection = MOV_NONE;
}

void* RasterBandStorageBlockGridReader::GetOneReadingBlock( int x,int y )
{
	assert(x>=_xBegin);
	assert(y>=_yBegin);
	assert(x+_xReadSize<=_xEnd);
	assert(y+_yReadSize<=_yEnd);

	int rxBlockBegin = (x-_xBegin)/_xBlockSize;
	int rxBlockEnd = rxBlockBegin+1;
	
	if (_xReadSize != 1)
	{
		rxBlockEnd = (x-_xBegin+_xReadSize+_xBlockSize-1)/_xBlockSize;
	}

	int ryBlockBegin = (y-_yBegin)/_yBlockSize;
	int ryBlockEnd = ryBlockBegin+ 1;

	if (_yReadSize != 1)
	{
		ryBlockEnd = (y-_yBegin+_yReadSize+_yBlockSize-1)/_yBlockSize;
	}
	
	int lrySize = _yReadSize;
	
	for (int i = ryBlockBegin; i < ryBlockEnd; i++)
	{
		int yBlockSize = _yBlockSize;

		if (i == _yBlockCount-1)
		{
			yBlockSize = _yEndBlockSize;
		}

		int ryPosIB = y+_yReadSize-lrySize-_yBegin-i*_yBlockSize;
		int rySize = yBlockSize-ryPosIB;

		if (lrySize < rySize)
		{
			rySize = lrySize;
		}

		
		int lrxSize = _xReadSize;
		for (int j = rxBlockBegin; j < rxBlockEnd; j++)
		{
			int xBlockSize = _xBlockSize;

			if (j == _xBlockCount-1)
			{
				xBlockSize = _xEndBlockSize;
			}

			int rxPosIB = x+_xReadSize-lrxSize-_xBegin-j*_xBlockSize;
			int rxSize = xBlockSize-rxPosIB;

			if (lrxSize < rxSize)
			{
				rxSize = lrxSize;
			}

			void** blockBuffer = (void**)_gridHandler;

			blockBuffer += i*_xBlockCount+j;

			char* rPtIB = (char*)(*blockBuffer) + ryPosIB*_xBlockSize*_dataBytes+rxPosIB*_dataBytes;
			char* rPtIR = (char*)_readBuffer + (_yReadSize-lrySize)*_xReadSize*_dataBytes+(_xReadSize-lrxSize)*_dataBytes;
			
			for (int k = 0; k < rySize; k++)
			{
				memcpy(rPtIR,rPtIB,rxSize*_dataBytes);
				rPtIR += _xReadSize*_dataBytes;
				rPtIB += xBlockSize*_dataBytes;
			}

			lrxSize -= rxSize;
		}

		lrySize -= rySize;
	}

	return _readBuffer;
}

int RasterBandStorageBlockGridReader::MoveNext()
{
	int curMoveDirection = MOV_NONE;

	if (_xBlockBegin == _xAoiBlockBegin)
	{
		if (_xBlockEnd == _xAoiBlockEnd)
		{
			curMoveDirection = _yBlockEnd < _yAoiBlockEnd?MOV_DOWN:MOV_NONE;
		}
		else //(_xBufferBlockEnd < _xAoiBlockEnd)
		{
			if (_yBlockBegin = _yAoiBlockBegin)
			{
				curMoveDirection = (_majorOrder == ORDER_COL && _yBlockEnd < _yAoiBlockEnd)?MOV_DOWN:MOV_RIGHT;
			}
			else //(_yBufferBlockBegin > _yAoiBlockBegin)
			{
				if(_yBlockEnd < _yAoiBlockEnd)
				{
					curMoveDirection = (_majorOrder == ORDER_ROW && _lastMoveDirection == MOV_DOWN)?MOV_RIGHT:MOV_DOWN;
				}
				else //(_yBufferBlockEnd == _yAoiBlockEnd)
				{
					curMoveDirection = (_majorOrder == ORDER_ROW && _lastMoveDirection == MOV_LEFT)?MOV_NONE:MOV_RIGHT;
				}
			}			
		}
	}
	else //(_xBufferBlockBegin > _xAoiBlockBegin)
	{
		if (_xBlockEnd == _xAoiBlockEnd)
		{
			if (_yBlockBegin = _yAoiBlockBegin)
			{
				if (_yBlockEnd < _yAoiBlockEnd)
				{
					curMoveDirection = (_majorOrder == ORDER_COL && _lastMoveDirection == MOV_UP)?MOV_NONE:MOV_DOWN;
				}
				else
				{
					curMoveDirection = MOV_NONE;
				}
			}
			else //(_yBufferBlockBegin > _yAoiBlockBegin)
			{
				if (_yBlockEnd = _yAoiBlockEnd)
				{
					if (_majorOrder == ORDER_COL && _lastMoveDirection == MOV_RIGHT)
					{
						curMoveDirection = MOV_UP;
					}
					else if (_majorOrder == ORDER_ROW && _lastMoveDirection == MOV_DOWN)
					{
						curMoveDirection = MOV_LEFT;
					}
					else
					{
						curMoveDirection = MOV_NONE;
					}					
				}
				else //_yBufferBlockEnd < _yAoiBlockEnd
				{
					if (_majorOrder == ORDER_COL)
					{
						curMoveDirection = _lastMoveDirection;
					}
					else //if (_majorOrder == ORDER_ROW)
					{
						if (_lastMoveDirection == MOV_DOWN)
						{
							curMoveDirection = MOV_LEFT;
						}
						else //if (_lastMoveDirection = MOV_RIGHT)
						{
							curMoveDirection = MOV_DOWN;
						}						
					}		
				}				
			}			
		}
		else //(_xBufferBlockEnd < _xAoiBlockEnd)
		{
			if (_yBlockBegin = _yAoiBlockBegin)
			{
				if (_majorOrder == ORDER_COL)
				{
					curMoveDirection = MOV_DOWN;
				}
				else
				{
					curMoveDirection = MOV_RIGHT;
				}
			}
			else //(_yBufferBlockBegin > _yAoiBlockBegin)
			{
				if (_yBlockEnd = _yAoiBlockEnd)
				{
					if (_majorOrder == ORDER_ROW)
					{
						curMoveDirection = _lastMoveDirection;
					}
					else //if (_majorOrder == ORDER_COL)
					{
						if (_lastMoveDirection == MOV_DOWN)
						{
							curMoveDirection = MOV_RIGHT;
						}
						else  //if (_lastMoveDirection = MOV_RIGHT)
						{
							curMoveDirection = MOV_UP;
						}						
					}	
				}
				else //(_yBufferBlockEnd < _yAoiBlockEnd)
				{
					curMoveDirection = _lastMoveDirection;
				}
			}
		}
	}

	if (curMoveDirection == MOV_UP)
	{
		MoveUp();
	}
	else if (curMoveDirection == MOV_DOWN)
	{
		MoveDown();
	}
	else if (curMoveDirection == MOV_LEFT)
	{
		MoveLeft();
	}
	else if (curMoveDirection == MOV_RIGHT)
	{
		MoveRight();
	}

	return curMoveDirection;
}

bool RasterBandStorageBlockGridReader::MoveUp()
{
	if (_yBlockBegin == _yAoiBlockBegin)
		return false;
	
	int totalcount = _xBlockCount*(_yBlockCount - 1);
	for (int i = _xBlockCount; i > 0; i--)
	{
		void** blockBuffer = (void**)_gridHandler + totalcount+i;
		for (int j = 0; j < totalcount; j++)
		{
			void* temp = *(blockBuffer-j);
			*(blockBuffer-j) = *(blockBuffer-j-1);
			*(blockBuffer-j-1) = temp;
		}
	}

	_yBlockBegin--;
	_yBlockEnd--;

	_band->GetActualBlockSize(_xBlockEnd-1,_yBlockEnd-1,&_xEndBlockSize,&_yEndBlockSize);

	void** blockBuffer = (void**)_gridHandler;

	for (int i = 0; i < _xBlockCount; i++)
	{
		blockBuffer += i;
		_band->ReadBlock(_xBlockBegin+i,_yBlockBegin,*blockBuffer);
	}

	_xBegin = _xBlockBegin*_xBlockSize;
	_yBegin = _yBlockBegin*_yBlockSize;

	_xEnd = _xBegin + (_xBlockCount-1)*_xBlockSize + _xEndBlockSize;
	_yEnd = _yBegin + (_yBlockCount-1)*_yBlockSize + _yEndBlockSize;

	_lastMoveDirection = MOV_DOWN;

	return true;
}

bool RasterBandStorageBlockGridReader::MoveDown()
{
	if (_yBlockEnd == _yAoiBlockEnd)
		return false;

	int totalcount = _xBlockCount*(_yBlockCount - 1);
	for (int i = _xBlockCount; i > 0; i--)
	{
		void** blockBuffer = (void**)_gridHandler + i;
		for (int j = 0; j < totalcount; j++)
		{
			void* temp = *(blockBuffer+j);
			*(blockBuffer+j) = *(blockBuffer+j-1);
			*(blockBuffer+j-1) = temp;
		}
	}

	_band->GetActualBlockSize(_xBlockEnd-1,_yBlockEnd,&_xEndBlockSize,&_yEndBlockSize);

	void** blockBuffer = (void**)_gridHandler + totalcount;

	for (int i = 0; i < _xBlockCount; i++)
	{
		blockBuffer += i;
		_band->ReadBlock(_xBlockBegin+i,_yBlockEnd,*blockBuffer);
	}

	_yBlockBegin++;
	_yBlockEnd++;

	_xBegin = _xBlockBegin*_xBlockSize;
	_yBegin = _yBlockBegin*_yBlockSize;

	_xEnd = _xBegin + (_xBlockCount-1)*_xBlockSize + _xEndBlockSize;
	_yEnd = _yBegin + (_yBlockCount-1)*_yBlockSize + _yEndBlockSize;

	_lastMoveDirection = MOV_DOWN;

	return true;
}

bool RasterBandStorageBlockGridReader::MoveLeft()
{
	if (_xBlockBegin == _xAoiBlockBegin)
		return false;

	for (int i = 0; i < _yBlockCount; i++)
	{
		void** blockBuffer = (void**)_gridHandler + i*_xBlockCount;
		for (int j = _xBlockCount-2; j > -1; j--)
		{
			void* temp = *(blockBuffer+j);
			*(blockBuffer+j) = *(blockBuffer+j+1);
			*(blockBuffer+j+1) = temp;
		}
	}

	_xBlockBegin--;
	_xBlockEnd--;

	_band->GetActualBlockSize(_xBlockEnd-1,_yBlockEnd-1,&_xEndBlockSize,&_yEndBlockSize);

	void** blockBuffer = (void**)_gridHandler;

	for (int i = 0; i < _yBlockCount; i++)
	{
		blockBuffer += i*_xBlockCount;
		_band->ReadBlock(_xBlockBegin,_yBlockBegin+i,*blockBuffer);
	}

	_xBegin = _xBlockBegin*_xBlockSize;
	_yBegin = _yBlockBegin*_yBlockSize;

	_xEnd = _xBegin + (_xBlockCount-1)*_xBlockSize + _xEndBlockSize;
	_yEnd = _yBegin + (_yBlockCount-1)*_yBlockSize + _yEndBlockSize;

	_lastMoveDirection = MOV_LEFT;

	return true;
}

bool RasterBandStorageBlockGridReader::MoveRight()
{
	if (_xBlockEnd == _xAoiBlockEnd)
		return false;

	for (int i = 0; i < _yBlockCount; i++)
	{
		void** blockBuffer = (void**)_gridHandler + i*_xBlockCount;
		for (int j = 1; j < _xBlockCount; j++)
		{
			void* temp = *(blockBuffer+j);
			*(blockBuffer+j) = *(blockBuffer+j-1);
			*(blockBuffer+j-1) = temp;
		}
	}

	_band->GetActualBlockSize(_xBlockEnd,_yBlockEnd-1,&_xEndBlockSize,&_yEndBlockSize);

	void** blockBuffer = (void**)_gridHandler + _xBlockCount-1;

	for (int i = 0; i < _yBlockCount; i++)
	{
		blockBuffer += i*_xBlockCount;
		_band->ReadBlock(_xBlockEnd,_yBlockBegin+i,*blockBuffer);
	}

	_xBlockBegin++;
	_xBlockEnd++;

	_xBegin = _xBlockBegin*_xBlockSize;
	_yBegin = _yBlockBegin*_yBlockSize;

	_xEnd = _xBegin + (_xBlockCount-1)*_xBlockSize + _xEndBlockSize;
	_yEnd = _yBegin + (_yBlockCount-1)*_yBlockSize + _yEndBlockSize;

	_lastMoveDirection = MOV_RIGHT;

	return true;
}

bool RasterBandStorageBlockGridReader::MoveArbitrarily( int x, int y,int preferredDirection /*= MOV_NONE*/ )
{
	int xReadEnd = x+_xReadSize;
	int yReadEnd = y+_yReadSize;

	if(x<_xAoiBegin)
		return false;
	if(y<_yAoiBegin)
		return false;

	if(xReadEnd>_xAoiEnd)
		return false;
	if(yReadEnd>_yAoiEnd)
		return false;

	int xReadBlockBegin = x/_xBlockSize;
	int yReadBlockBegin = y/_yBlockSize;
	int xReadBlockEnd = (xReadEnd+_xBlockSize-1)/_xBlockSize;
	int yReadBlockEnd = (yReadEnd+_yBlockSize-1)/_yBlockSize;

	//移动之后 存储块缓存矩阵 左上右下角的块索引坐标
	int xBlockBegin = _xBlockBegin;
	int yBlockBegin = _yBlockBegin;
	int xBlockEnd = _xBlockEnd;
	int yBlockEnd = _yBlockEnd;

	if (preferredDirection == MOV_NONE)
	{
		//如果 块缓存矩阵 需要完全移动 会尽量将 块缓存矩阵 的中心位置对齐 x y

		if (xReadBlockBegin >= _xBlockEnd)
		{
			xBlockBegin = xReadBlockBegin - (_xBlockCount-xReadBlockEnd+xReadBlockBegin)/2;
			xBlockEnd = xBlockBegin + _xBlockCount;
		}

		else if (xReadBlockEnd <= _xBlockBegin)
		{
			xBlockEnd = xReadBlockEnd + (_xBlockCount-xReadBlockEnd+xReadBlockBegin)/2;
			xBlockBegin = xBlockEnd - _xBlockCount;
		}

		if (yReadBlockBegin >= _yBlockEnd)
		{
			yBlockBegin = yReadBlockBegin - (_yBlockCount-yReadBlockEnd+yReadBlockBegin)/2;
			yBlockEnd = yBlockBegin + _yBlockCount;
		}

		else if (yReadBlockEnd <= _yBlockBegin)
		{
			yBlockEnd = yReadBlockEnd + (_yBlockCount-yReadBlockEnd+yReadBlockBegin)/2;
			yBlockBegin = yBlockEnd - _yBlockCount;
		}
	}
	else
	{
		//如果 块缓存矩阵 不需要完全移动 会以最小移动将 块缓存矩阵 移动至恰好包含 x y

		if (xReadBlockBegin < _xBlockBegin)
		{
			xBlockBegin = xReadBlockBegin;
			xBlockEnd = xBlockBegin + _xBlockCount;
		}

		else if (xReadBlockEnd > _xBlockEnd)
		{
			xBlockEnd = xReadBlockEnd;
			xBlockBegin = xBlockEnd - _xBlockCount;
		}

		if (yReadBlockBegin < _yBlockBegin)
		{
			yBlockBegin = yReadBlockBegin;
			yBlockEnd = yBlockBegin + _yBlockCount;
		}

		else if (yReadBlockEnd > _yBlockEnd)
		{
			yBlockEnd = yReadBlockEnd;
			yBlockBegin = yBlockEnd - _yBlockCount;
		}
	}

	//当preferredDirection不为MOV_NONE时
	//——在 preferredDirection 指定的方向执行一个最大移动
	//——在 preferredDirection 正交的方向执行最小移动（最小移动在前一个else语句已经执行）
	if (preferredDirection == MOV_UP)
	{
		yBlockEnd = yReadBlockBegin+1;
		yBlockBegin = yBlockEnd - _yBlockCount;
	}

	else if (preferredDirection == MOV_DOWN)
	{
		yBlockBegin = yReadBlockEnd-1;
		yBlockEnd = yBlockBegin + _yBlockCount;
	}

	else if (preferredDirection == MOV_LEFT)
	{
		xBlockEnd = xReadBlockBegin+1;
		xBlockBegin = xBlockEnd - _xBlockCount;
	}

	else if (preferredDirection == MOV_RIGHT)
	{
		xBlockBegin = xReadBlockEnd - 1;
		xBlockEnd = xBlockBegin + _xBlockCount;
	}

	//如果超出AOI范围，再以最小的移动调整回AOI范围内
	if (xBlockBegin < _xAoiBlockBegin)
	{
		xBlockBegin = _xAoiBlockBegin;
		xBlockEnd = xBlockBegin + _xBlockCount;
	}

	else if (xBlockEnd > _xAoiBlockEnd)
	{
		xBlockEnd = _xAoiBlockEnd;
		xBlockBegin = xBlockEnd - _xBlockCount;
	}

	if (yBlockBegin < _yAoiBlockBegin)
	{
		yBlockBegin = _yAoiBlockBegin;
		yBlockEnd = yBlockBegin + _yBlockCount;
	}

	else if (yBlockEnd > _yAoiBlockEnd)
	{
		yBlockEnd = _yAoiBlockEnd;
		yBlockBegin = yBlockEnd - _yBlockCount;
	}

	memset(_gridHandlerFlag,0,_xBlockCount*_yBlockCount);

	void** blockBuffer = (void**)_gridHandler;
	unsigned char* blockBufferFlag = _gridHandlerFlag;

	for (int i = 0; i < _yBlockCount; i++)
	{
		for (int j = 0; j < _xBlockCount; j++)
		{
			if (*blockBufferFlag != 1)
			{
				//递归地将旧存储块矩阵中仍然有效的存储块挪动到新存储块矩阵的中

				//指向 旧存储块缓存矩阵 中的指针，初始化到 (i,j) 处
				void** blockBufferO = blockBuffer;
				//将起始挪动的位置初始化为 (i,j) 
				int m = j;
				int n = i;

				bool goOn = true;

				while (goOn)
				{
					//存储块缓存矩阵 中 (m,n) 处的块 在 图像上的块坐标
					int xBlock = _xBlockBegin + m;
					int yBlock = _yBlockBegin + n;
					//旧存储块缓存矩阵 中 (m,n) 处的块 在 新存储块缓存矩阵 中的坐标
					m = xBlock - xBlockBegin;
					n = yBlock - yBlockBegin;

					if (m > 0
						&& m < _xBlockCount
						&& n > 0
						&& n < _yBlockCount)
					{
						goOn = true;
						*(_gridHandlerFlag + n*_xBlockCount + m) = 1;

						//存储块缓存矩阵 中 指向 (m,n) 处的块的指针
						void** blockBufferP = (void**)_gridHandler + n*_xBlockCount + m;

						void* blockTemp = *blockBufferP;
						*blockBufferP = *blockBufferO;
						*blockBufferO = blockTemp;
					}
					else
					{
						goOn = false;
						*(_gridHandlerFlag + i*_xBlockCount + j) = 1;
						_band->ReadBlock(xBlockBegin + j, yBlockBegin + i, *blockBuffer);
					}
				};
			}

			blockBuffer++;
			blockBufferFlag++;
		}
	}

	_xBlockBegin = xBlockBegin;
	_yBlockBegin = yBlockBegin;
	_xBlockEnd = xBlockEnd;
	_yBlockEnd = yBlockEnd;

	_band->GetActualBlockSize(_xBlockEnd-1,_yBlockEnd-1,&_xEndBlockSize,&_yEndBlockSize);

	_xBegin = _xBlockBegin*_xBlockSize;
	_yBegin = _yBlockBegin*_yBlockSize;

	_xEnd = _xBegin + (_xBlockCount-1)*_xBlockSize + _xEndBlockSize;
	_yEnd = _yBegin + (_yBlockCount-1)*_yBlockSize + _yEndBlockSize;

	return true;
}

END_NAME_SPACE(tGis, Core)




