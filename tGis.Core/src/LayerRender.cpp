#include "LayerRender.h"
#include "IDataset.h"
#include "ILayer.h"

BEGIN_NAME_SPACE(tGis, Core)

LayerRender::LayerRender(ILayer* layer)
{
	_opacity = 1.0;
	_alpha = 255;
	_layer = layer;
	_layer->SetRender(this);
}

LayerRender::~LayerRender()
{
}

float LayerRender::GetOpacity()
{
	return _opacity;
}

void LayerRender::SetOpacity(float opacity)
{
	_opacity = opacity;
	int alpha = int(255 * _opacity);
	if (alpha > 255)
		alpha = 255;
	_alpha = (unsigned char)alpha;
}

int LayerRender::LayerRenderBuffer::_ins_count = 0;
elr_mtx LayerRender::LayerRenderBuffer::_mtx;
elr_atomic_t LayerRender::LayerRenderBuffer::_surfaceBufferInUsing = 0;
elr_atomic_t LayerRender::LayerRenderBuffer::_datasetBufferInUsing = 0;
unsigned char* LayerRender::LayerRenderBuffer::_surfaceBuffer = nullptr;
unsigned char* LayerRender::LayerRenderBuffer::_datasetBuffer = nullptr;
int LayerRender::LayerRenderBuffer::_maxSurfaceWidth = 0;
int LayerRender::LayerRenderBuffer::_maxSurfaceHeight = 0;
size_t LayerRender::LayerRenderBuffer::_surfaceBufferSize = 0;
size_t LayerRender::LayerRenderBuffer::_datasetBufferSize = 0;

LayerRender::LayerRenderBuffer::LayerRenderBuffer()
{
	if (_ins_count == 0)
	{
		elr_mtx_init(&_mtx);
		_ins_count = 1;
		_acquiredSurfaceBuffer = nullptr;
		_acquiredDatasetBuffer = nullptr;
	}
	else
	{
		_ins_count = 2;
		_acquiredSurfaceBuffer = AcquireSurfaceBuffer();
		_acquiredDatasetBuffer = AcquireDatasetBuffer();
	}
}

LayerRender::LayerRenderBuffer::~LayerRenderBuffer()
{
	if (_ins_count == 2)
	{
		_ins_count = 1;
		if (_acquiredSurfaceBuffer != nullptr)
		{
			ReleaseSurfaceBuffer();
			_acquiredSurfaceBuffer = nullptr;
		}
		if (_acquiredDatasetBuffer != nullptr)
		{
			ReleaseDatasetBuffer();
			_acquiredDatasetBuffer = nullptr;
		}
	}
	else
	{
		_ins_count = 0;
		if (_surfaceBuffer != nullptr)
			free(_surfaceBuffer);
		if (_datasetBuffer != nullptr)
			free(_datasetBuffer);
		elr_mtx_finalize(&_mtx);
	}
}

unsigned char * LayerRender::LayerRenderBuffer::AcquireSurfaceBuffer()
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

void LayerRender::LayerRenderBuffer::ReleaseSurfaceBuffer()
{
	elr_atomic_dec(&_surfaceBufferInUsing);
}

unsigned char * LayerRender::LayerRenderBuffer::AcquireDatasetBuffer()
{
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

void LayerRender::LayerRenderBuffer::ReleaseDatasetBuffer()
{
	elr_atomic_dec(&_datasetBufferInUsing);
}


//LayerRender::LayerRenderBuffer g_layerRenderBuffer;

LayerRender::LayerRenderBuffer LayerRender::_g_rbuf;

//elr_mtx* LayerRender::_mtx;
//elr_atomic_t LayerRender::_surfaceBufferInUsing = 0;
//elr_atomic_t LayerRender::_datasetBufferInUsing = 0;
//unsigned char* LayerRender::_surfaceBuffer = nullptr;
//unsigned char* LayerRender::_datasetBuffer = nullptr;
//int LayerRender::_maxSurfaceWidth = 0;
//int LayerRender::_maxSurfaceHeight = 0;
//size_t LayerRender::_surfaceBufferSize = 0;
//size_t LayerRender::_datasetBufferSize = 0;

void LayerRender::SetMaxSurfaceSize(int width, int height)
{
	size_t surfaceBufferSize = width*height * 4;
	if (surfaceBufferSize > _g_rbuf._surfaceBufferSize)
	{
		size_t datasetBufferSize = surfaceBufferSize * 2;

		//所有的render都在单一UI线程里运行，这些骚操作其实没必要
		elr_mtx_lock(&_g_rbuf._mtx);

		while (_g_rbuf._surfaceBufferInUsing != 0);

		if (_g_rbuf._surfaceBuffer != nullptr)
			free(_g_rbuf._surfaceBuffer);
		
		_g_rbuf._surfaceBuffer = (unsigned char*)malloc(surfaceBufferSize);

		while (_g_rbuf._datasetBufferInUsing != 0);

		if (_g_rbuf._datasetBuffer != nullptr)
			free(_g_rbuf._datasetBuffer);

		_g_rbuf._datasetBuffer = (unsigned char*)malloc(datasetBufferSize);

		_g_rbuf._surfaceBufferSize = surfaceBufferSize;
		_g_rbuf._datasetBufferSize = datasetBufferSize;
		_g_rbuf._maxSurfaceWidth = width;
		_g_rbuf._maxSurfaceHeight = height;

		elr_mtx_lock(&_g_rbuf._mtx);
	}
}


END_NAME_SPACE(tGis, Core)


