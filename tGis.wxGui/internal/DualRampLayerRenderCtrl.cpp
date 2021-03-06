#include <wx/wx.h>
#include "DualRampLayerRenderCtrl.h"
#include <wx/progdlg.h>

DualRampLayerRenderCtrl::DualRampLayerRenderCtrl( wxWindow* parent )
	:DualRampLayerRenderCtrlBase( parent )
{
	_render = nullptr;
	_raster = nullptr;
	_layer = nullptr;

	Bind(wxEVT_SLIDER, &DualRampLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());
	Bind(wxEVT_BUTTON, &DualRampLayerRenderCtrl::_btnComputeStatistics_Clicked, this, _btnComputeStatistics->GetId());

}

DualRampLayerRenderCtrl::~DualRampLayerRenderCtrl()
{
	Unbind(wxEVT_SLIDER, &DualRampLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());
	Unbind(wxEVT_BUTTON, &DualRampLayerRenderCtrl::_btnComputeStatistics_Clicked, this, _btnComputeStatistics->GetId());

}

const char * DualRampLayerRenderCtrl::GetLayerRenderName()
{
	return "Raster DualRamp Render";;
}

bool DualRampLayerRenderCtrl::IsSupportLayerExactly(ILayer * layer)
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
			if (render->IsTypeOf(RasterDualRampLayerRender::S_GetType()))
				return true;
		}
	}

	return false;
}

bool DualRampLayerRenderCtrl::IsSupportLayer(ILayer * layer)
{
	IDataset* dt = layer->GetDataset();
	if (dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		return true;

	return false;
}

void DualRampLayerRenderCtrl::SetLayer(ILayer * layer)
{
	_layer = layer;
	_raster = (MyGDALRasterDataset*)layer->GetDataset();
	GDALDataset* dt = _raster->GetGDALDataset();
	int layerCount = dt->GetRasterCount();
	_choiceBand->Clear();
	for (int i = 1; i <= layerCount; i++)
	{
		wxString bandStr = wxString::Format(wxT("Band %d"), i);
		_choiceBand->Append(bandStr);
	}

	ILayerRender* render = layer->GetRender();
	RasterDualRampLayerRender* trender = dynamic_cast<RasterDualRampLayerRender*>(render);
	if (trender == nullptr)
		SetDataset(_raster);
	else
		SetLayerRender(trender);
}

void DualRampLayerRenderCtrl::UpdateLayerRender()
{
	if (_render == nullptr)
		_render = new RasterDualRampLayerRender(_layer);

	_render->SetBand(_choiceBand->GetSelection() + 1);

	int opacity = _sldOpacity->GetValue();
	_render->SetOpacity(opacity / 255.0f);

	wxString rMinStr = _txtMin->GetValue();
	wxString rMaxStr = _txtMax->GetValue();
	wxString pivotStr = _txtPivot->GetValue();
	double minR;
	rMinStr.ToDouble(&minR);
	double maxR;
	rMaxStr.ToDouble(&maxR);
	double pivot;
	pivotStr.ToDouble(&pivot);
	if (minR < maxR && pivot <= maxR && pivot >= minR)
		_render->SetMinPivotMax(minR, pivot, maxR);

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

	bool leftRChannel;
	bool leftGChannel;
	bool leftBChannel;
	bool rightRChannel;
	bool rightGChannel;
	bool rightBChannel;

	leftRChannel = _chkLtPivotR->IsChecked();
	leftGChannel = _chkLtPivotG->IsChecked();
	leftBChannel = _chkLtPivotB->IsChecked();
	rightRChannel = _chkGtPivotR->IsChecked();
	rightGChannel = _chkGtPivotG->IsChecked();
	rightBChannel = _chkGtPivotB->IsChecked();
	_render->SetLeftChannel(leftRChannel, leftGChannel, leftBChannel);
	_render->SetRightChannel(rightRChannel, rightGChannel, rightBChannel);
}


void DualRampLayerRenderCtrl::SetDataset(MyGDALRasterDataset * raster)
{
	GDALDataset* dt = _raster->GetGDALDataset();
	int layerCount = dt->GetRasterCount();
	GDALRasterBand* band1 = dt->GetRasterBand(1);
	if (band1->GetRasterDataType() == GDT_Byte)
	{
		_txtMin->SetValue(wxT("0"));
		_txtMax->SetValue(wxT("255"));
		_txtPivot->SetValue(wxT("127"));

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
		_txtPivot->SetValue(wxString::Format(wxT("%.3f"), (rMin+rMax)/2.0));

		if (rNoDataOK)
		{
			_chkNoDataEQ->SetValue(true);
			_chkNoDataGT->SetValue(false);
			_chkNoDataLT->SetValue(false);
			_txtNoData->SetValue(wxString::Format(wxT("%f"), rNoData));
		}
	}


	_chkLtPivotR->SetValue(false);
	_chkLtPivotG->SetValue(false);
	_chkLtPivotB->SetValue(true);

	_chkGtPivotR->SetValue(true);
	_chkGtPivotG->SetValue(false);
	_chkGtPivotB->SetValue(false);

	_choiceBand->SetSelection(0);
}

void DualRampLayerRenderCtrl::SetLayerRender(RasterDualRampLayerRender * render)
{
	_render = render;

	_sldOpacity->SetValue((int)(_render->GetOpacity() * 255));

	double rMin;
	double rMax;
	double pivot;
	_render->GetMinPivotMax(&rMin, &pivot, &rMax);

	_txtMin->SetValue(wxString::Format(wxT("%.3f"), rMin));
	_txtMax->SetValue(wxString::Format(wxT("%.3f"), rMax));
	_txtPivot->SetValue(wxString::Format(wxT("%.3f"), pivot));

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

	bool leftRChannel;
	bool leftGChannel;
	bool leftBChannel;
	bool rightRChannel;
	bool rightGChannel;
	bool rightBChannel;
	_render->GetLeftChannel(&leftRChannel, &leftGChannel, &leftBChannel);
	_render->GetRightChannel(&rightRChannel, &rightGChannel, &rightBChannel);

	_chkLtPivotR->SetValue(leftRChannel);
	_chkLtPivotG->SetValue(leftGChannel);
	_chkLtPivotB->SetValue(leftBChannel);

	_chkGtPivotR->SetValue(rightRChannel);
	_chkGtPivotG->SetValue(rightGChannel);
	_chkGtPivotB->SetValue(rightBChannel);

	_choiceBand->SetSelection(_render->GetBand() - 1);
}

void DualRampLayerRenderCtrl::_sldOpacity_scroll(wxCommandEvent & event)
{
	_lblOpacityValue->SetLabel(wxString::Format(wxT("%-3d"), event.GetInt()));
}


static int CPL_STDCALL ComputeStatisticsPrgFunc(double dfComplete, const char *pszMessage, void *pProgressArg)
{
	wxProgressDialog* prgDlg = (wxProgressDialog*)pProgressArg;

	prgDlg->Update(int(100 * dfComplete));

	return TRUE;
}

void DualRampLayerRenderCtrl::_btnComputeStatistics_Clicked(wxCommandEvent & event)
{
	GDALDataset* dt = _raster->GetGDALDataset();
	bool bApproxOK = _chkApproximate->GetValue();

	wxProgressDialog* prgDlg = new wxProgressDialog(wxT("Compute Min and Max..."), wxT("Computing Min and Max..."), 100, this, wxPD_AUTO_HIDE | wxPD_APP_MODAL);

	int rBandIndex = _choiceBand->GetSelection() + 1;
	GDALRasterBand* rBand = dt->GetRasterBand(rBandIndex);
	double rMin = -1.0;
	double rMax = -1.0;
	rBand->ComputeStatistics(bApproxOK ? TRUE : FALSE, &rMin, &rMax, nullptr, nullptr, ComputeStatisticsPrgFunc, prgDlg);

	_txtMin->SetValue(wxString::Format(wxT("%.3f"), rMin));
	_txtMax->SetValue(wxString::Format(wxT("%.3f"), rMax));

	delete prgDlg;
}