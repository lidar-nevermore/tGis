#ifndef __PalettedLayerRenderCtrl__
#define __PalettedLayerRenderCtrl__

/**
@file
Subclass of GrayScaleLayerRenderCtrlBase, which is generated by wxFormBuilder.
*/

#include "PalettedLayerRenderCtrlBase.h"
#include "ILayerRenderCtrl.h"

//// end generated include

/** Implementing GrayScaleLayerRenderCtrlBase */
class PalettedLayerRenderCtrl : public PalettedLayerRenderCtrlBase, public ILayerRenderCtrl
{
public:
	/** Constructor */
	PalettedLayerRenderCtrl(wxWindow* parent);
	~PalettedLayerRenderCtrl();
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
	void SetLayerRender(RasterPalettedLayerRender* render);

private:
	RasterPalettedLayerRender* _render;
	MyGDALRasterDataset* _raster;
	ILayer* _layer;
	Palette* _palette;

private:
	void _sldOpacity_scroll(wxCommandEvent& event);
	void _btnRandColor_clicked(wxCommandEvent& event);
	void _btnGradColor_clicked(wxCommandEvent& event);
	void _btnAdd_clicked(wxCommandEvent& event);
	void _btnEdit_clicked(wxCommandEvent& event);
	void _btnDelete_clicked(wxCommandEvent& event);
	void _btnClear_clicked(wxCommandEvent& event);
};

#endif // __GrayScaleLayerRenderCtrl__
