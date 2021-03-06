#ifndef __PseudoColorLayerRenderCtrl__
#define __PseudoColorLayerRenderCtrl__

/**
@file
Subclass of GrayScaleLayerRenderCtrlBase, which is generated by wxFormBuilder.
*/

#include "ColorRampLayerRenderCtrlBase.h"
#include "ILayerRenderCtrl.h"

//// end generated include

/** Implementing GrayScaleLayerRenderCtrlBase */
class ColorRampLayerRenderCtrl : public ColorRampLayerRenderCtrlBase, public ILayerRenderCtrl
{
public:
	/** Constructor */
	ColorRampLayerRenderCtrl(wxWindow* parent);
	~ColorRampLayerRenderCtrl();
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
	void SetLayerRender(RasterColorRampLayerRender* render);

private:
	RasterColorRampLayerRender* _render;
	MyGDALRasterDataset* _raster;
	ILayer* _layer;

private:
	void _sldOpacity_scroll(wxCommandEvent& event);
	void _btnComputeStatistics_Clicked(wxCommandEvent& event);
	void _btnSelGradColor_clicked(wxCommandEvent& e);
};

#endif // __GrayScaleLayerRenderCtrl__
