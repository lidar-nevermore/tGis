#include "VisualizeBufferManager.h"
#include <cstdlib>
#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

VisualizeBufferManager* VisualizeBufferManager::_instance = nullptr;

static PtrDestructor<VisualizeBufferManager> shit(VisualizeBufferManager::_instance);

VisualizeBufferManager & VisualizeBufferManager::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new VisualizeBufferManager();
	}

	return *_instance;
}

VisualizeBufferManager::VisualizeBufferManager()
{
	_surfaceBuffer = nullptr;
	_datasetBuffer = nullptr;
	_maxSurfaceWidth = 0;
	_maxSurfaceHeight = 0;
	_surfaceBufferInUsing = ELR_ATOMIC_ZERO;
	_datasetBufferInUsing = ELR_ATOMIC_ZERO;
	elr_mtx_init(&_mtx);
}


VisualizeBufferManager::~VisualizeBufferManager()
{
	if (_surfaceBuffer != nullptr)
	{
		free(_surfaceBuffer);
	}
	if (_datasetBuffer != nullptr)
	{
		free(_datasetBuffer);
	}
	elr_mtx_finalize(&_mtx);
}

//inline void VisualizeBufferManager::SetMaxSurfaceSize(int width, int height)
//{
//	size_t surfaceBufferSize = width*height*4;
//	if (surfaceBufferSize > _surfaceBufferSize)
//	{
//		_surfaceBufferSize = surfaceBufferSize;
//		_datasetBufferSize = surfaceBufferSize * 2;
//
//		_maxSurfaceWidth = width;
//		_maxSurfaceHeight = height;
//
//		bool ok = false;
//		if (_surfaceBufferInUsing == 0)
//		{
//			elr_mtx_lock(&_mtx);
//			if (_surfaceBufferInUsing == 0)
//			{
//				elr_atomic_inc(&_surfaceBufferInUsing);
//				ok = true;
//				if (_surfaceBuffer != nullptr)
//					free(_surfaceBuffer);
//			}
//			elr_mtx_lock(&_mtx);
//		}
//
//		_surfaceBuffer = (unsigned char*)malloc(width*height * 4);
//
//		if(ok)
//			elr_atomic_dec(&_surfaceBufferInUsing);
//
//		ok = false;
//		if (_datasetBufferInUsing == 0)
//		{
//			elr_mtx_lock(&_mtx);
//			if (_datasetBufferInUsing == 0)
//			{
//				elr_atomic_inc(&_datasetBufferInUsing);
//				ok = true;
//				if(_datasetBuffer != nullptr)
//					free(_datasetBuffer);
//			}
//			elr_mtx_lock(&_mtx);
//		}
//
//		_datasetBuffer = (unsigned char*)malloc(width*height * 8);
//
//		if (ok)
//			elr_atomic_dec(&_datasetBufferInUsing);
//	}
//}
//
//inline unsigned char * VisualizeBufferManager::AllocSurfaceBuffer()
//{
//	bool ok = false;
//	if (_surfaceBufferInUsing == 0)
//	{
//		elr_mtx_lock(&_mtx);
//		if (_surfaceBufferInUsing == 0)
//		{
//			elr_atomic_inc(&_surfaceBufferInUsing);
//			ok = true;
//		}
//		elr_mtx_lock(&_mtx);
//	}
//
//	if(ok)
//		return _surfaceBuffer;
//
//	return nullptr;
//}
//
//inline size_t VisualizeBufferManager::SurfaceBufferSize()
//{
//	return _surfaceBufferSize;
//}
//
//inline void VisualizeBufferManager::FreeSurfaceBuffer(void * buf)
//{
//	elr_atomic_dec(&_surfaceBufferInUsing);
//	if (buf != _surfaceBuffer)
//		free(buf);
//}
//
//inline unsigned char * VisualizeBufferManager::AllocDatasetBuffer(int pixBytes)
//{
//	assert(pixBytes == 1 || pixBytes == 2 || pixBytes == 4 || pixBytes == 8);
//
//	bool ok = false;
//	if (_datasetBufferInUsing == 0)
//	{
//		elr_mtx_lock(&_mtx);
//		if (_datasetBufferInUsing == 0)
//		{
//			elr_atomic_inc(&_datasetBufferInUsing);
//			ok = true;
//		}
//		elr_mtx_lock(&_mtx);
//	}
//
//	if (ok)
//		return _datasetBuffer;
//
//	return nullptr;
//}
//
//inline size_t VisualizeBufferManager::DatasetBufferSize()
//{
//	return _datasetBufferSize;
//}
//
//inline void VisualizeBufferManager::FreeDatasetBuffer(void * buf)
//{
//	elr_atomic_dec(&_datasetBufferInUsing);
//	if (buf != _datasetBuffer)
//		free(buf);
//}


END_NAME_SPACE(tGis, Core)

