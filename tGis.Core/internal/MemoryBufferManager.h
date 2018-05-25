#pragma once


#ifndef __MEMPOOLMANAGER_H__
#define __MEMPOOLMANAGER_H__

#include "Helper.h"

#include "elr_mpl.h"

BEGIN_NAME_SPACE(tGis, Core)

class MemoryBufferManager
{
public:
	static MemoryBufferManager* _instance;
	static MemoryBufferManager& INSTANCE();
	const static int RasterLayerBufferWidth = 256;

private:
	MemoryBufferManager();

public:
	~MemoryBufferManager();

public:
	unsigned char* AllocRasterSurfaceBuffer();
	void FreeRasterSurfaceBuffer(unsigned char* buf);
	unsigned char* AllocRasterDatasetBuffer(int pixBytes);
	void FreeRasterDatasetBuffer(unsigned char* buf);

private:
	elr_mpl_t _mplRasterLayer;
	elr_mpl_t _mplVectorLayer;
};


END_NAME_SPACE(tGis, Core)

#endif

