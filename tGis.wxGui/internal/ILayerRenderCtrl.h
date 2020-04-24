#ifndef __I_LayerRenderCtrl_H__
#define __I_LayerRenderCtrl_H__


#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;


struct ILayerRenderCtrl
{
public:
	virtual const char* GetLayerRenderName() = 0;
	virtual bool IsSupportLayerExactly(ILayer* layer) = 0;
	virtual bool IsSupportLayer(ILayer* layer) = 0;
	virtual void SetLayer(ILayer* layer) = 0;
	virtual void UpdateLayerRender() = 0;
};

#endif
