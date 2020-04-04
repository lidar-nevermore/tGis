#pragma once

#ifndef __LAYERRENDER_H__
#define __LAYERRENDER_H__

#include "Helper.h"

#include "ILayerRender.h"

#include "elr_mtx.h"
#include <cstdlib>
#include <cassert>
#include <string>

using namespace std;

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
struct ILayer;

class TGIS_CORE_API LayerRender : public ILayerRender
{
	friend class MapWidget;
	friend class Map;
public:
	LayerRender(ILayer* layer);
	virtual ~LayerRender();

public:
	virtual float GetOpacity();
	virtual void SetOpacity(float);

protected:
	float _opacity;
	unsigned char _alpha;
	ILayer* _layer;


//读取数据集的buffer 和 将数据集转换为可视图像的buffer
//图层不会同时进行绘制，所以只需要一份buffer在所有LayerRender中共享
protected:
	//LayerRenderBuffer还用于成对的调用获取和归还buffer
	//主要因为c++没有try finally语法
	//成对调用方法也是个非常麻烦的事
	class LayerRenderBuffer
	{
		friend class LayerRender;

	public:
		unsigned char * _acquiredSurfaceBuffer;
		unsigned char * _acquiredDatasetBuffer;
		LayerRenderBuffer();
		~LayerRenderBuffer();

	private:
		static int _ins_count;
		static elr_mtx _mtx;
		static elr_atomic_t _surfaceBufferInUsing;
		static elr_atomic_t _datasetBufferInUsing;
		static unsigned char* _surfaceBuffer;
		static unsigned char* _datasetBuffer;
		static int _maxSurfaceWidth;
		static int _maxSurfaceHeight;
		static size_t _surfaceBufferSize;
		static size_t _datasetBufferSize;

	private:
		//获取可视图像的buffer
		unsigned char * AcquireSurfaceBuffer();
		//归还可视图像的buffer
		static void RevertSurfaceBuffer();
		//获取读取数据集的buffer 
		static unsigned char * AcquireDatasetBuffer();
		//归还读取数据集的buffer
		static void RevertDatasetBuffer();
	};

private:
	static LayerRenderBuffer _g_rbuf;

protected:
	//这里才真正申请 数据集的buffer 和 将数据集转换为可视图像的buffer
	//所需缓存的大小与显示屏大小有关系，
	//系统初始化时按照显示屏大小申请一份就可以满足缩小窗口的使用要求
	static void SetMaxSurfaceSize(int width, int height);
};

END_NAME_SPACE(tGis, Core)

#endif
