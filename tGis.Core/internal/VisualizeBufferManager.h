#pragma once


#ifndef __MEMPOOLMANAGER_H__
#define __MEMPOOLMANAGER_H__

#include "Helper.h"

#include "elr_mtx.h"

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
	void SetMaxSurfaceSize(int width, int height);
	unsigned char* AllocSurfaceBuffer();
	void FreeSurfaceBuffer(unsigned char* buf);
	unsigned char* AllocDatasetBuffer(int pixBytes);
	void FreeDatasetBuffer(unsigned char* buf);

private:
	elr_mtx _mtx;
	elr_atomic_t _surfaceBufferInUsing;
	elr_atomic_t _datasetBufferInUsing;
	unsigned char* _surfaceBuffer;
	unsigned char* _datasetBuffer;
	int _maxSurfaceWidth;
	int _maxSurfaceHeight;
};


END_NAME_SPACE(tGis, Core)

#endif

