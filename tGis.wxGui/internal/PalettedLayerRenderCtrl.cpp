#include <wx/wx.h>
#include "PalettedLayerRenderCtrl.h"
#include <wx/progdlg.h>

#include "wxGridPaletteTable.h"
#include "wxGridCellColorEditor.h"
#include "wxGridCellColorRenderer.h"

using namespace tGis::Gui;

PalettedLayerRenderCtrl::PalettedLayerRenderCtrl( wxWindow* parent )
	:PalettedLayerRenderCtrlBase( parent )
{
	_render = nullptr;
	_raster = nullptr;
	_layer = nullptr;

	wxGridCellColorEditor *edtColor = new wxGridCellColorEditor();
	wxGridCellColorRenderer *rndColor = new wxGridCellColorRenderer();

	_grdPallete->RegisterDataType(wxT("Color"), rndColor, edtColor);

	wxGridPaletteTable *tblPalette = new wxGridPaletteTable(nullptr);
	_grdPallete->SetTable(tblPalette, true, wxGrid::wxGridSelectRows);
	_grdPallete->SetColSize(0, 150);
	_grdPallete->SetColSize(1, 105);
	_grdPallete->EnableDragColMove(false);
	_grdPallete->EnableDragColSize(true);

	Bind(wxEVT_SLIDER, &PalettedLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());
	Bind(wxEVT_BUTTON, &PalettedLayerRenderCtrl::_btnRandColor_clicked, this, _btnRandColor->GetId());

}

PalettedLayerRenderCtrl::~PalettedLayerRenderCtrl()
{
	Unbind(wxEVT_SLIDER, &PalettedLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());
	Unbind(wxEVT_BUTTON, &PalettedLayerRenderCtrl::_btnRandColor_clicked, this, _btnRandColor->GetId());

}

const char * PalettedLayerRenderCtrl::GetLayerRenderName()
{
	return "Raster Paletted Render";;
}

bool PalettedLayerRenderCtrl::IsSupportLayerExactly(ILayer * layer)
{
	if (IsSupportLayer(layer))
	{
		ILayerRender* render = layer->GetRender();
		
		if (render == nullptr)
		{
			MyGDALRasterDataset* raster = dynamic_cast<MyGDALRasterDataset*>(layer->GetDataset());
			int bandCount = raster->GetGDALDataset()->GetRasterCount();

			if (bandCount == 1)
				return true;
		}
		else
		{
			if (render->IsTypeOf(RasterPalettedLayerRender::S_GetType()))
				return true;
		}
	}

	return false;
}

bool PalettedLayerRenderCtrl::IsSupportLayer(ILayer * layer)
{
	IDataset* dt = layer->GetDataset();
	if (dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		return true;

	return false;
}

void PalettedLayerRenderCtrl::SetLayer(ILayer * layer)
{
	_layer = layer;
	_raster = (MyGDALRasterDataset*)layer->GetDataset();
	GDALDataset* dt = _raster->GetGDALDataset();
	int layerCount = dt->GetRasterCount();
	for (int i = 1; i <= layerCount; i++)
	{
		wxString bandStr = wxString::Format(wxT("Band %d"), i);
		_choiceBand->Append(bandStr);
	}

	ILayerRender* render = layer->GetRender();
	RasterPalettedLayerRender* trender = dynamic_cast<RasterPalettedLayerRender*>(render);
	if (trender == nullptr)
		SetDataset(_raster);
	else
		SetLayerRender(trender);
}

void PalettedLayerRenderCtrl::UpdateLayerRender()
{
	if (_render == nullptr)
		_render = new RasterPalettedLayerRender(_layer);

	_render->SetBand(_choiceBand->GetSelection() + 1);

	int opacity = _sldOpacity->GetValue();
	_render->SetOpacity(opacity / 255.0f);

	int noDataLogic = 0;
	if (_chkNoDataEQ->IsChecked())
		noDataLogic |= RasterLayerRender::EQ;

	if (_chkNoDataGT->IsChecked())
		noDataLogic |= RasterLayerRender::GT;

	if (_chkNoDataLT->IsChecked())
		noDataLogic |= RasterLayerRender::LT;

	wxString noDataStr = _txtNoData->GetValue();
	double noData = 0;
	noDataStr.ToDouble(&noData);
	_render->SetNoDataValue(noDataLogic, noData);

	wxGridPaletteTable *tblPalette = (wxGridPaletteTable *)_grdPallete->GetTable();
	_render->SetPalette(tblPalette->GetPalette());
}


void PalettedLayerRenderCtrl::SetDataset(MyGDALRasterDataset * raster)
{
	GDALDataset* dt = _raster->GetGDALDataset();
	int layerCount = dt->GetRasterCount();
	GDALRasterBand* band1 = dt->GetRasterBand(1);
	if (band1->GetRasterDataType() == GDT_Byte)
	{
		_chkNoDataEQ->SetValue(false);
		_chkNoDataGT->SetValue(false);
		_chkNoDataLT->SetValue(false);
	}
	else
	{
		double rNoData = 0;
		int rNoDataOK = 0;

		rNoData = band1->GetNoDataValue(&rNoDataOK);

		if (rNoDataOK)
		{
			_chkNoDataEQ->SetValue(true);
			_chkNoDataGT->SetValue(false);
			_chkNoDataLT->SetValue(false);
			_txtNoData->SetValue(wxString::Format(wxT("%f"), rNoData));
		}
	}

	_choiceBand->SetSelection(0);
}

void PalettedLayerRenderCtrl::SetLayerRender(RasterPalettedLayerRender * render)
{
	_render = render;

	_sldOpacity->SetValue((int)(_render->GetOpacity() * 255));

	int noDataLogicR;
	double rNoData;
	_render->GetNoDataValue(&noDataLogicR, &rNoData);
	_txtNoData->SetValue(wxString::Format(wxT("%f"), rNoData));
	_chkNoDataEQ->SetValue(false);
	_chkNoDataGT->SetValue(false);
	_chkNoDataLT->SetValue(false);

	if ((noDataLogicR& RasterLayerRender::EQ) != 0)
	{
		_chkNoDataEQ->SetValue(true);
	}
	if ((noDataLogicR & RasterLayerRender::GT) != 0)
	{
		_chkNoDataGT->SetValue(true);
	}
	if ((noDataLogicR & RasterLayerRender::LT) != 0)
	{
		_chkNoDataLT->SetValue(true);
	}

	_choiceBand->SetSelection(_render->GetBand() - 1);

	Palette* pal = _render->GetPalette();
	if (pal != nullptr)
	{
		pal = pal->Clone();
		wxGridPaletteTable *tblPalette = new wxGridPaletteTable(pal);
		_grdPallete->SetTable(tblPalette, true, wxGrid::wxGridSelectRows);
		pal->Release();

		wxSize sz = _grdPallete->GetSize();
		_grdPallete->SetInitialSize(sz);
		_grdPallete->FitInside();

		this->Layout();
	}
}

void PalettedLayerRenderCtrl::_sldOpacity_scroll(wxCommandEvent & event)
{
	_lblOpacityValue->SetLabel(wxString::Format(wxT("%-3d"), event.GetInt()));
}

void PalettedLayerRenderCtrl::_btnRandColor_clicked(wxCommandEvent & event)
{
	Palette* pal = Palette::CreatePalette(_raster, _choiceBand->GetSelection() + 1);
	wxGridPaletteTable *tblPalette = new wxGridPaletteTable(pal);
	_grdPallete->SetTable(tblPalette, true, wxGrid::wxGridSelectRows);
	pal->Release();

	_grdPallete->Refresh();
}
