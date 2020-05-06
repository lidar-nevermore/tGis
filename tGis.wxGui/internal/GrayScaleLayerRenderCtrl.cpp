#include "GrayScaleLayerRenderCtrl.h"
#include <wx/progdlg.h>

GrayScaleLayerRenderCtrl::GrayScaleLayerRenderCtrl( wxWindow* parent )
	:GrayScaleLayerRenderCtrlBase( parent )
{
	_render = nullptr;
	_raster = nullptr;
	_layer = nullptr;

	Bind(wxEVT_SLIDER, &GrayScaleLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());

}

GrayScaleLayerRenderCtrl::~GrayScaleLayerRenderCtrl()
{
	Unbind(wxEVT_SLIDER, &GrayScaleLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());

}

const char * GrayScaleLayerRenderCtrl::GetLayerRenderName()
{
	return "Raster GrayScale Render";;
}

bool GrayScaleLayerRenderCtrl::IsSupportLayerExactly(ILayer * layer)
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
			if (render->IsTypeOf(RasterGrayScaleLayerRender::S_GetType()))
				return true;
		}
	}

	return false;
}

bool GrayScaleLayerRenderCtrl::IsSupportLayer(ILayer * layer)
{
	IDataset* dt = layer->GetDataset();
	if (dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		return true;

	return false;
}

void GrayScaleLayerRenderCtrl::SetLayer(ILayer * layer)
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
	if (render == nullptr)
		SetDataset(_raster);
	else
		SetLayerRender(dynamic_cast<RasterGrayScaleLayerRender*>(render));
}

void GrayScaleLayerRenderCtrl::UpdateLayerRender()
{
	if (_render == nullptr)
		_render = new RasterGrayScaleLayerRender(_layer);

	_render->SetBand(_choiceBand->GetSelection() + 1);

	int opacity = _sldOpacity->GetValue();
	_render->SetOpacity(opacity / 255.0f);

	wxString rMinStr = _txtMin->GetValue();
	wxString rMaxStr = _txtMax->GetValue();
	double minR;
	rMinStr.ToDouble(&minR);
	double maxR;
	rMaxStr.ToDouble(&maxR);
	if (minR < maxR)
		_render->SetMinMax(minR, maxR);

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
}


void GrayScaleLayerRenderCtrl::SetDataset(MyGDALRasterDataset * raster)
{
	GDALDataset* dt = _raster->GetGDALDataset();
	int layerCount = dt->GetRasterCount();
	GDALRasterBand* band1 = dt->GetRasterBand(1);
	if (band1->GetRasterDataType() == GDT_Byte)
	{
		_txtMin->SetValue(wxT("0"));
		_txtMax->SetValue(wxT("255"));

		_chkNoDataEQ->SetValue(false);
		_chkNoDataGT->SetValue(false);
		_chkNoDataLT->SetValue(false);
	}
	else
	{
		double rMin = -1.0;
		double rMax = -1.0;
		double rNoData = 0;
		int rNoDataOK = 0;

		band1->ComputeStatistics(TRUE, &rMin, &rMax, nullptr, nullptr, nullptr, nullptr);
		rNoData = band1->GetNoDataValue(&rNoDataOK);

		_txtMin->SetValue(wxString::Format(wxT("%.3f"), rMin));
		_txtMax->SetValue(wxString::Format(wxT("%.3f"), rMax));

		if (rNoDataOK)
		{
			_chkNoDataEQ->SetValue(true);
			_chkNoDataGT->SetValue(false);
			_chkNoDataLT->SetValue(false);
			_txtNoData->SetValue(wxString::Format(wxT("%.f"), rNoData));
		}
	}

	_choiceBand->SetSelection(0);
}

void GrayScaleLayerRenderCtrl::SetLayerRender(RasterGrayScaleLayerRender * render)
{
	_render = render;

	_sldOpacity->SetValue((int)(_render->GetOpacity() * 255));

	double rMin;
	double rMax;
	_render->GetMinMax(&rMin, &rMax);

	_txtMin->SetValue(wxString::Format(wxT("%.3f"), rMin));
	_txtMax->SetValue(wxString::Format(wxT("%.3f"), rMax));

	int noDataLogicR;
	double rNoData;
	_render->GetNoDataValue(&noDataLogicR, &rNoData);
	_txtNoData->SetValue(wxString::Format(wxT("%.f"), rNoData));
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
}

void GrayScaleLayerRenderCtrl::_sldOpacity_scroll(wxCommandEvent & event)
{
	_lblOpacityValue->SetLabel(wxString::Format(wxT("%-3d"), event.GetInt()));
}


static int CPL_STDCALL ComputeStatisticsPrgFunc(double dfComplete, const char *pszMessage, void *pProgressArg)
{
	wxProgressDialog* prgDlg = (wxProgressDialog*)pProgressArg;

	prgDlg->Update(int(100 * dfComplete));

	return TRUE;
}

void GrayScaleLayerRenderCtrl::_btnComputeStatistics_Clicked(wxCommandEvent & event)
{
	GDALDataset* dt = _raster->GetGDALDataset();
	bool bApproxOK = _chkApproximate->GetValue();

	wxProgressDialog* prgDlg = new wxProgressDialog(wxT("Compute Statistics..."), wxT("Computing Statistics..."), 100, this, wxPD_AUTO_HIDE | wxPD_APP_MODAL);

	int rBandIndex = _choiceBand->GetSelection() + 1;
	GDALRasterBand* rBand = dt->GetRasterBand(rBandIndex);
	double rMin = -1.0;
	double rMax = -1.0;
	rBand->ComputeStatistics(bApproxOK ? TRUE : FALSE, &rMin, &rMax, nullptr, nullptr, ComputeStatisticsPrgFunc, prgDlg);

	_txtMin->SetValue(wxString::Format(wxT("%.3f"), rMin));
	_txtMax->SetValue(wxString::Format(wxT("%.3f"), rMax));

	delete prgDlg;
}
