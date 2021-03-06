#ifndef __ColorRampLayerRenderCtrl__
#define __ColorRampLayerRenderCtrl__

/**
@file
Subclass of ColorRampLayerRenderCtrlBase, which is generated by wxFormBuilder.
*/

#include "DualRampLayerRenderCtrlBase.h"
#include "ILayerRenderCtrl.h"

//// end generated include

/** Implementing ColorRampLayerRenderCtrlBase */
class DualRampLayerRenderCtrl : public DualRampLayerRenderCtrlBase, public ILayerRenderCtrl
{
public:
	/** Constructor */
	DualRampLayerRenderCtrl(wxWindow* parent);
	~DualRampLayerRenderCtrl();
	//// end generated class members

public:
	// ͨ�� ILayerRenderCtrl �̳�
	virtual const char * GetLayerRenderName() override;

	virtual bool IsSupportLayerExactly(ILayer * layer) override;

	virtual bool IsSupportLayer(ILayer * layer) override;

	virtual void SetLayer(ILayer * layer) override;

	virtual void UpdateLayerRender() override;

private:
	void SetDataset(MyGDALRasterDataset* raster);
	void SetLayerRender(RasterDualRampLayerRender* render);

private:
	RasterDualRampLayerRender* _render;
	MyGDALRasterDataset* _raster;
	ILayer* _layer;

private:
	void _sldOpacity_scroll(wxCommandEvent& event);
	void _btnComputeStatistics_Clicked(wxCommandEvent& event);
};

#endif // __ColorRampLayerRenderCtrl__
