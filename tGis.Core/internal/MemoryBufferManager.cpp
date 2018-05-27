#include "MemoryBufferManager.h"
#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

MemoryBufferManager* MemoryBufferManager::_instance = nullptr;

static PtrDestructor<MemoryBufferManager> shit(MemoryBufferManager::_instance);

MemoryBufferManager & MemoryBufferManager::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new MemoryBufferManager();
	}

	return *_instance;
}

MemoryBufferManager::MemoryBufferManager()
{
	elr_mpl_init();
	_mplRasterLayer = elr_mpl_create_multi_2(NULL, NULL, NULL, 3,
		(RasterLayerBufferWidth + 3)*(RasterLayerBufferWidth + 3),
		(RasterLayerBufferWidth + 3)*(RasterLayerBufferWidth + 3) * 4,
		(RasterLayerBufferWidth + 3)*(RasterLayerBufferWidth + 3) * 8);
}


MemoryBufferManager::~MemoryBufferManager()
{
	elr_mpl_destroy(&_mplRasterLayer);
	elr_mpl_finalize();
}

unsigned char * MemoryBufferManager::AllocRasterSurfaceBuffer()
{
	return (unsigned char*)elr_mpl_alloc_multi(&_mplRasterLayer, RasterLayerBufferWidth*RasterLayerBufferWidth*4);
}

void MemoryBufferManager::FreeRasterSurfaceBuffer(unsigned char * buf)
{
	elr_mpl_free(buf);
}

unsigned char * MemoryBufferManager::AllocRasterDatasetBuffer(int pixBytes)
{
	assert(pixBytes == 1 || pixBytes == 2 || pixBytes == 4 || pixBytes == 8);

	return (unsigned char*)elr_mpl_alloc_multi(&_mplRasterLayer, (RasterLayerBufferWidth+2)*(RasterLayerBufferWidth+2)*pixBytes);
}

void MemoryBufferManager::FreeRasterDatasetBuffer(unsigned char * buf)
{
	elr_mpl_free(buf);
}


END_NAME_SPACE(tGis, Core)

