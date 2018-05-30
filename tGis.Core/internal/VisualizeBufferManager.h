#pragma once


#ifndef __MEMPOOLMANAGER_H__
#define __MEMPOOLMANAGER_H__

#include "Helper.h"

#include "elr_mtx.h"
#include <cstdlib>
#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

class VisualizeBufferManager
{
public:
	static VisualizeBufferManager* _instance;
	static VisualizeBufferManager& INSTANCE();

private:
	VisualizeBufferManager();

public:
	~VisualizeBufferManager();

public:
	//inline void SetMaxSurfaceSize(int width, int height);
	//inline unsigned char* AllocSurfaceBuffer();
	//inline size_t SurfaceBufferSize();
	//inline void FreeSurfaceBuffer(void* buf);
	//inline unsigned char* AllocDatasetBuffer(int pixBytes);
	//inline size_t DatasetBufferSize();
	//inline void FreeDatasetBuffer(void* buf);

	inline void SetMaxSurfaceSize(int width, int height)
	{
		size_t surfaceBufferSize = width*height * 4;
		if (surfaceBufferSize > _surfaceBufferSize)
		{
			_surfaceBufferSize = surfaceBufferSize;
			_datasetBufferSize = surfaceBufferSize * 2;

			_maxSurfaceWidth = width;
			_maxSurfaceHeight = height;

			bool ok = false;
			if (_surfaceBufferInUsing == 0)
			{
				elr_mtx_lock(&_mtx);
				if (_surfaceBufferInUsing == 0)
				{
					elr_atomic_inc(&_surfaceBufferInUsing);
					ok = true;
					if (_surfaceBuffer != nullptr)
						free(_surfaceBuffer);
				}
				elr_mtx_lock(&_mtx);
			}

			_surfaceBuffer = (unsigned char*)malloc(width*height * 4);

			if (ok)
				elr_atomic_dec(&_surfaceBufferInUsing);

			ok = false;
			if (_datasetBufferInUsing == 0)
			{
				elr_mtx_lock(&_mtx);
				if (_datasetBufferInUsing == 0)
				{
					elr_atomic_inc(&_datasetBufferInUsing);
					ok = true;
					if (_datasetBuffer != nullptr)
						free(_datasetBuffer);
				}
				elr_mtx_lock(&_mtx);
			}

			_datasetBuffer = (unsigned char*)malloc(width*height * 8);

			if (ok)
				elr_atomic_dec(&_datasetBufferInUsing);
		}
	}

	inline unsigned char * AllocSurfaceBuffer()
	{
		bool ok = false;
		if (_surfaceBufferInUsing == 0)
		{
			elr_mtx_lock(&_mtx);
			if (_surfaceBufferInUsing == 0)
			{
				elr_atomic_inc(&_surfaceBufferInUsing);
				ok = true;
			}
			elr_mtx_lock(&_mtx);
		}

		if (ok)
			return _surfaceBuffer;

		return nullptr;
	}

	inline size_t SurfaceBufferSize()
	{
		return _surfaceBufferSize;
	}

	inline void FreeSurfaceBuffer(void * buf)
	{
		elr_atomic_dec(&_surfaceBufferInUsing);
		if (buf != _surfaceBuffer)
			free(buf);
	}

	inline unsigned char * AllocDatasetBuffer(int pixBytes)
	{
		assert(pixBytes == 1 || pixBytes == 2 || pixBytes == 4 || pixBytes == 8);

		bool ok = false;
		if (_datasetBufferInUsing == 0)
		{
			elr_mtx_lock(&_mtx);
			if (_datasetBufferInUsing == 0)
			{
				elr_atomic_inc(&_datasetBufferInUsing);
				ok = true;
			}
			elr_mtx_lock(&_mtx);
		}

		if (ok)
			return _datasetBuffer;

		return nullptr;
	}

	inline size_t DatasetBufferSize()
	{
		return _datasetBufferSize;
	}

	inline void FreeDatasetBuffer(void * buf)
	{
		elr_atomic_dec(&_datasetBufferInUsing);
		if (buf != _datasetBuffer)
			free(buf);
	}

private:
	elr_mtx _mtx;
	elr_atomic_t _surfaceBufferInUsing;
	elr_atomic_t _datasetBufferInUsing;
	unsigned char* _surfaceBuffer;
	unsigned char* _datasetBuffer;
	int _maxSurfaceWidth;
	int _maxSurfaceHeight;
	size_t _surfaceBufferSize;
	size_t _datasetBufferSize;
};


END_NAME_SPACE(tGis, Core)

#endif

