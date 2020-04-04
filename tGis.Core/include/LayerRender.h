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


//��ȡ���ݼ���buffer �� �����ݼ�ת��Ϊ����ͼ���buffer
//ͼ�㲻��ͬʱ���л��ƣ�����ֻ��Ҫһ��buffer������LayerRender�й���
protected:
	//LayerRenderBuffer�����ڳɶԵĵ��û�ȡ�͹黹buffer
	//��Ҫ��Ϊc++û��try finally�﷨
	//�ɶԵ��÷���Ҳ�Ǹ��ǳ��鷳����
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
		//��ȡ����ͼ���buffer
		unsigned char * AcquireSurfaceBuffer();
		//�黹����ͼ���buffer
		static void RevertSurfaceBuffer();
		//��ȡ��ȡ���ݼ���buffer 
		static unsigned char * AcquireDatasetBuffer();
		//�黹��ȡ���ݼ���buffer
		static void RevertDatasetBuffer();
	};

private:
	static LayerRenderBuffer _g_rbuf;

protected:
	//������������� ���ݼ���buffer �� �����ݼ�ת��Ϊ����ͼ���buffer
	//���軺��Ĵ�С����ʾ����С�й�ϵ��
	//ϵͳ��ʼ��ʱ������ʾ����С����һ�ݾͿ���������С���ڵ�ʹ��Ҫ��
	static void SetMaxSurfaceSize(int width, int height);
};

END_NAME_SPACE(tGis, Core)

#endif
