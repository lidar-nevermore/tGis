#include "VisualizeBufferManager.h"
#include <cstdlib>
#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

VisualizeBufferManager* VisualizeBufferManager::_instance = nullptr;


VisualizeBufferManager & VisualizeBufferManager::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new VisualizeBufferManager();
		static _tGisObjectDestructor<VisualizeBufferManager> shit(_instance);
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

END_NAME_SPACE(tGis, Core)

