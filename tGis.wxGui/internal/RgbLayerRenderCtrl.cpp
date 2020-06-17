#include <wx/wx.h>
#include "RgbLayerRenderCtrl.h"
#include <wx/progdlg.h>

RgbLayerRenderCtrl::RgbLayerRenderCtrl( wxWindow* parent )
	:RgbLayerRenderCtrlBase( parent )
{
	_render = nullptr;
	_raster = nullptr;
	_layer = nullptr;

	Bind(wxEVT_SLIDER, &RgbLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());
	Bind(wxEVT_BUTTON, &RgbLayerRenderCtrl::_btnComputeStatistics_Clicked, this, _btnComputeStatistics->GetId());

}

RgbLayerRenderCtrl::~RgbLayerRenderCtrl()
{
	Unbind(wxEVT_SLIDER, &RgbLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());
	Unbind(wxEVT_BUTTON, &RgbLayerRenderCtrl::_btnComputeStatistics_Clicked, this, _btnComputeStatistics->GetId());

}

const char * RgbLayerRenderCtrl::GetLayerRenderName()
{
	return "Raster RGB Render";
}

bool RgbLayerRenderCtrl::IsSupportLayerExactly(ILayer * layer)
{
	if (IsSupportLayer(layer))
	{
		ILayerRender* render = layer->GetRender();

		if (render == nullptr)
			return true;
		else if (render->IsTypeOf(RasterRgbLayerRender::S_GetType()))
			return true;
	}

	return false;
}

bool RgbLayerRenderCtrl::IsSupportLayer(ILayer * layer)
{
	IDataset* dt = layer->GetDataset();
	if (false == dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		return false;

	MyGDALRasterDataset* raster = (MyGDALRasterDataset*)dt;
	GDALDataset* gdalDt = raster->GetGDALDataset();
	int bandCount = gdalDt->GetRasterCount();

	if (1 == bandCount)
		return false;

	return true;
}

void RgbLayerRenderCtrl::SetLayer(ILayer * layer)
{
	_layer = layer;
	_raster = (MyGDALRasterDataset*)layer->GetDataset();
	GDALDataset* dt = _raster->GetGDALDataset();
	int layerCount = dt->GetRasterCount();
	for (int i = 1; i <= layerCount; i++)
	{
		wxString bandStr = wxString::Format(wxT("Band %d"), i);
		_choiceRBand->Append(bandStr);
		_choiceGBand->Append(bandStr);
		_choiceBBand->Append(bandStr);
	}

	ILayerRender* render = layer->GetRender();
	RasterRgbLayerRender* trender = dynamic_cast<RasterRgbLayerRender*>(render);
	if (trender == nullptr)
		SetDataset(_raster);
	else
		SetLayerRender(trender);
}

void RgbLayerRenderCtrl::UpdateLayerRender()
{
	if (_render == nullptr)
		_render = new RasterRgbLayerRender(_layer);

	_render->SetBandR(_choiceRBand->GetSelection() + 1);
	_render->SetBandG(_choiceGBand->GetSelection() + 1);
	_render->SetBandB(_choiceBBand->GetSelection() + 1);

	int opacity = _sldOpacity->GetValue();
	_render->SetOpacity(opacity / 255.0f);

	wxString rMinStr = _txtRMin->GetValue();
	wxString rMaxStr = _txtRMax->GetValue();
	double minR;
	rMinStr.ToDouble(&minR);
	double maxR;
	rMaxStr.ToDouble(&maxR);
	if (minR < maxR)
		_render->SetMinMaxR(minR, maxR);

	wxString gMinStr = _txtGMin->GetValue();
	wxString gMaxStr = _txtGMax->GetValue();
	double minG;
	gMinStr.ToDouble(&minG);
	double maxG;
	gMaxStr.ToDouble(&maxG);
	if (minG < maxG)
		_render->SetMinMaxG(minG, maxG);

	wxString bMinStr = _txtBMin->GetValue();
	wxString bMaxStr = _txtBMax->GetValue();
	double minB;
	bMinStr.ToDouble(&minB);
	double maxB;
	bMaxStr.ToDouble(&maxB);
	if (minB < maxB)
		_render->SetMinMaxB(minB, maxB);

	int noDataLogicR = 0;
	if (_chkRNoDataEQ->IsChecked())
		noDataLogicR |= RasterLayerRender::EQ;

	if (_chkRNoDataGT->IsChecked())
		noDataLogicR |= RasterLayerRender::GT;

	if (_chkRNoDataLT->IsChecked())
		noDataLogicR |= RasterLayerRender::LT;

	wxString noDataStrR = _txtRNoData->GetValue();
	double noDataR = 0;
	noDataStrR.ToDouble(&noDataR);
	_render->SetNoDataValueR(noDataLogicR, noDataR);

	int noDataLogicG = 0;
	if (_chkGNoDataEQ->IsChecked())
		noDataLogicG |= RasterLayerRender::EQ;

	if (_chkGNoDataGT->IsChecked())
		noDataLogicG |= RasterLayerRender::GT;

	if (_chkGNoDataLT->IsChecked())
		noDataLogicG |= RasterLayerRender::LT;

	wxString noDataStrG = _txtGNoData->GetValue();
	double noDataG = 0;
	noDataStrG.ToDouble(&noDataG);
	_render->SetNoDataValueG(noDataLogicG, noDataG);

	int noDataLogicB = 0;
	if (_chkBNoDataEQ->IsChecked())
		noDataLogicB |= RasterLayerRender::EQ;

	if (_chkBNoDataGT->IsChecked())
		noDataLogicB |= RasterLayerRender::GT;

	if (_chkBNoDataLT->IsChecked())
		noDataLogicB |= RasterLayerRender::LT;

	wxString noDataStrB = _txtBNoData->GetValue();
	double noDataB = 0;
	noDataStrB.ToDouble(&noDataB);
	_render->SetNoDataValueB(noDataLogicB, noDataB);
}


void RgbLayerRenderCtrl::SetDataset(MyGDALRasterDataset * raster)
{
	GDALDataset* dt = _raster->GetGDALDataset();
	int layerCount = dt->GetRasterCount();
	GDALRasterBand* band1 = dt->GetRasterBand(1);
	if (band1->GetRasterDataType() == GDT_Byte)
	{
		_choiceRBand->SetSelection(0);
		_choiceGBand->SetSelection(1);
		_choiceBBand->SetSelection(2);
		_txtRMin->SetValue(wxT("0"));
		_txtGMin->SetValue(wxT("0"));
		_txtBMin->SetValue(wxT("0"));
		_txtRMax->SetValue(wxT("255"));
		_txtGMax->SetValue(wxT("255"));
		_txtBMax->SetValue(wxT("255"));

		_chkRNoDataEQ->SetValue(false);
		_chkRNoDataGT->SetValue(false);
		_chkRNoDataLT->SetValue(false);

		_chkGNoDataEQ->SetValue(false);
		_chkGNoDataGT->SetValue(false);
		_chkGNoDataLT->SetValue(false);

		_chkBNoDataEQ->SetValue(false);
		_chkBNoDataGT->SetValue(false);
		_chkBNoDataLT->SetValue(false);
	}
	else
	{
		double rMin = -1.0;
		double rMax = -1.0;
		double rNoData = 0;
		int rNoDataOK = 0;
		double gMin = -1.0;
		double gMax = -1.0;
		double gNoData = 0;
		int gNoDataOK = 0;
		double bMin = -1.0;
		double bMax = -1.0;
		double bNoData = 0;
		int bNoDataOK = 0;

		if (layerCount == 1)
		{
			_choiceRBand->SetSelection(0);
			_choiceGBand->SetSelection(0);
			_choiceBBand->SetSelection(0);

			band1->ComputeStatistics(TRUE, &bMin, &bMax, nullptr, nullptr, nullptr, nullptr);
			bNoData = band1->GetNoDataValue(&bNoDataOK);

			rMin = bMin;
			rMax = bMax;
			rNoData = bNoData;
			rNoDataOK = bNoDataOK;

			gMin = bMin;
			gMax = bMax;
			gNoData = bNoData;
			gNoDataOK = bNoDataOK;
		}
		else if (layerCount == 2)
		{
			_choiceRBand->SetSelection(1);
			_choiceGBand->SetSelection(1);
			_choiceBBand->SetSelection(0);

			GDALRasterBand* band2 = dt->GetRasterBand(2);

			band2->ComputeStatistics(TRUE, &gMin, &gMax, nullptr, nullptr, nullptr, nullptr);
			gNoData = band2->GetNoDataValue(&gNoDataOK);

			rMin = gMin;
			rMax = gMax;
			rNoData = gNoData;
			rNoDataOK = gNoDataOK;

			band1->ComputeStatistics(TRUE, &bMin, &bMax, nullptr, nullptr, nullptr, nullptr);
			bNoData = band1->GetNoDataValue(&bNoDataOK);
		}
		else if (layerCount == 3)
		{
			_choiceRBand->SetSelection(2);
			_choiceGBand->SetSelection(1);
			_choiceBBand->SetSelection(0);

			GDALRasterBand* band2 = dt->GetRasterBand(2);
			GDALRasterBand* band3 = dt->GetRasterBand(3);

			band3->ComputeStatistics(TRUE, &rMin, &rMax, nullptr, nullptr, nullptr, nullptr);
			rNoData = band3->GetNoDataValue(&rNoDataOK);
			//char szValue[100];
			//CPLsnprintf(szValue, sizeof(szValue), "%.14g", rMin);
			//band3->SetMetadataItem("STATISTICS_MINIMUM", szValue);

			band2->ComputeStatistics(TRUE, &gMin, &gMax, nullptr, nullptr, nullptr, nullptr);
			gNoData = band2->GetNoDataValue(&gNoDataOK);

			band1->ComputeStatistics(TRUE, &bMin, &bMax, nullptr, nullptr, nullptr, nullptr);
			bNoData = band1->GetNoDataValue(&bNoDataOK);
		}
		else
		{
			_choiceRBand->SetSelection(3);
			_choiceGBand->SetSelection(1);
			_choiceBBand->SetSelection(0);

			GDALRasterBand* band2 = dt->GetRasterBand(2);
			GDALRasterBand* band4 = dt->GetRasterBand(4);

			band4->ComputeStatistics(TRUE, &rMin, &rMax, nullptr, nullptr, nullptr, nullptr);
			rNoData = band4->GetNoDataValue(&rNoDataOK);
			//char szValue[100];
			//CPLsnprintf(szValue, sizeof(szValue), "%.14g", rMin);
			//band4->SetMetadataItem("STATISTICS_MINIMUM", szValue);

			band2->ComputeStatistics(TRUE, &gMin, &gMax, nullptr, nullptr, nullptr, nullptr);
			gNoData = band2->GetNoDataValue(&gNoDataOK);

			band1->ComputeStatistics(TRUE, &bMin, &bMax, nullptr, nullptr, nullptr, nullptr);
			bNoData = band1->GetNoDataValue(&bNoDataOK);
		}

		_txtRMin->SetValue(wxString::Format(wxT("%.3f"), rMin));
		_txtGMin->SetValue(wxString::Format(wxT("%.3f"), gMin));
		_txtBMin->SetValue(wxString::Format(wxT("%.3f"), bMin));
		_txtRMax->SetValue(wxString::Format(wxT("%.3f"), rMax));
		_txtGMax->SetValue(wxString::Format(wxT("%.3f"), gMax));
		_txtBMax->SetValue(wxString::Format(wxT("%.3f"), bMax));
		if (rNoDataOK)
		{
			_chkRNoDataEQ->SetValue(true);
			_chkRNoDataGT->SetValue(false);
			_chkRNoDataLT->SetValue(false);
			_txtRNoData->SetValue(wxString::Format(wxT("%f"), rNoData));
		}
		if (gNoDataOK)
		{
			_chkGNoDataEQ->SetValue(true);
			_chkGNoDataGT->SetValue(false);
			_chkGNoDataLT->SetValue(false);
			_txtGNoData->SetValue(wxString::Format(wxT("%f"), gNoData));
		}
		if (bNoDataOK)
		{
			_chkBNoDataEQ->SetValue(true);
			_chkBNoDataGT->SetValue(false);
			_chkBNoDataLT->SetValue(false);
			_txtBNoData->SetValue(wxString::Format(wxT("%f"), bNoData));
		}
	}
}

void RgbLayerRenderCtrl::SetLayerRender(RasterRgbLayerRender * render)
{
	_render = render;

	_sldOpacity->SetValue((int)(_render->GetOpacity() * 255));

	double rMin;
	double rMax;
	_render->GetMinMaxR(&rMin, &rMax);

	double gMin;
	double gMax;
	_render->GetMinMaxG(&gMin, &gMax);

	double bMin;
	double bMax;
	_render->GetMinMaxB(&bMin, &bMax);

	_txtRMin->SetValue(wxString::Format(wxT("%.3f"), rMin));
	_txtGMin->SetValue(wxString::Format(wxT("%.3f"), gMin));
	_txtBMin->SetValue(wxString::Format(wxT("%.3f"), bMin));
	_txtRMax->SetValue(wxString::Format(wxT("%.3f"), rMax));
	_txtGMax->SetValue(wxString::Format(wxT("%.3f"), gMax));
	_txtBMax->SetValue(wxString::Format(wxT("%.3f"), bMax));

	int noDataLogicR;
	double rNoData;
	_render->GetNoDataValueR(&noDataLogicR, &rNoData);
	_txtRNoData->SetValue(wxString::Format(wxT("%f"), rNoData));
	_chkRNoDataEQ->SetValue(false);
	_chkRNoDataGT->SetValue(false);
	_chkRNoDataLT->SetValue(false);

	if ((noDataLogicR& RasterLayerRender::EQ) != 0)
	{
		_chkRNoDataEQ->SetValue(true);
	}
	if ((noDataLogicR & RasterLayerRender::GT) != 0)
	{
		_chkRNoDataGT->SetValue(true);
	}
	if ((noDataLogicR & RasterLayerRender::LT) != 0)
	{
		_chkRNoDataLT->SetValue(true);
	}

	int noDataLogicG;
	double gNoData;
	_render->GetNoDataValueG(&noDataLogicG, &gNoData);
	_txtGNoData->SetValue(wxString::Format(wxT("%f"), gNoData));
	_chkGNoDataEQ->SetValue(false);
	_chkGNoDataGT->SetValue(false);
	_chkGNoDataLT->SetValue(false);

	if ((noDataLogicG& RasterLayerRender::EQ) != 0)
	{
		_chkGNoDataEQ->SetValue(true);
	}
	if ((noDataLogicG & RasterLayerRender::GT) != 0)
	{
		_chkGNoDataGT->SetValue(true);
	}
	if ((noDataLogicG & RasterLayerRender::LT) != 0)
	{
		_chkGNoDataLT->SetValue(true);
	}

	int noDataLogicB;
	double bNoData;
	_render->GetNoDataValueB(&noDataLogicB, &bNoData);
	_txtBNoData->SetValue(wxString::Format(wxT("%f"), bNoData));
	_chkBNoDataEQ->SetValue(false);
	_chkBNoDataGT->SetValue(false);
	_chkBNoDataLT->SetValue(false);
	if ((noDataLogicB& RasterLayerRender::EQ) != 0)
	{
		_chkBNoDataEQ->SetValue(true);
	}
	if ((noDataLogicB & RasterLayerRender::GT) != 0)
	{
		_chkBNoDataGT->SetValue(true);
	}
	if ((noDataLogicB & RasterLayerRender::LT) != 0)
	{
		_chkBNoDataLT->SetValue(true);
	}

	_choiceRBand->SetSelection(_render->GetBandR() - 1);
	_choiceGBand->SetSelection(_render->GetBandG() - 1);
	_choiceBBand->SetSelection(_render->GetBandB() - 1);
}

void RgbLayerRenderCtrl::_sldOpacity_scroll(wxCommandEvent & event)
{
	_lblOpacityValue->SetLabel(wxString::Format(wxT("%-3d"), event.GetInt()));
}

static int CPL_STDCALL ComputeStatisticsPrgFunc(double dfComplete, const char *pszMessage, void *pProgressArg)
{
	void** progData = (void**)pProgressArg;
	int prog = *((int*)progData[0]);
	wxProgressDialog* prgDlg = (wxProgressDialog*)progData[1];

	prgDlg->Update(prog + int(100 * dfComplete));

	return TRUE;
}

void RgbLayerRenderCtrl::_btnComputeStatistics_Clicked(wxCommandEvent & event)
{
	GDALDataset* dt = _raster->GetGDALDataset();
	bool bApproxOK = _chkApproximate->GetValue();

	int prog = 0;
	wxProgressDialog* prgDlg = new wxProgressDialog(wxT("Compute Min and Max..."), wxT("Computing Min and Max..."), 300, this, wxPD_AUTO_HIDE | wxPD_APP_MODAL);
	void* progData[2] = { &prog, prgDlg };

	int rBandIndex = _choiceRBand->GetSelection() + 1;
	GDALRasterBand* rBand = dt->GetRasterBand(rBandIndex);
	double rMin = -1.0;
	double rMax = -1.0;
	rBand->ComputeStatistics(bApproxOK ? TRUE : FALSE, &rMin, &rMax, nullptr, nullptr, ComputeStatisticsPrgFunc, progData);

	int gBandIndex = _choiceGBand->GetSelection() + 1;
	GDALRasterBand* gBand = dt->GetRasterBand(gBandIndex);
	double gMin = -1.0;
	double gMax = -1.0;
	prog = 100;
	gBand->ComputeStatistics(bApproxOK ? TRUE : FALSE, &gMin, &gMax, nullptr, nullptr, ComputeStatisticsPrgFunc, progData);

	int bBandIndex = _choiceBBand->GetSelection() + 1;
	GDALRasterBand* bBand = dt->GetRasterBand(bBandIndex);
	double bMin = -1.0;
	double bMax = -1.0;
	prog = 200;
	bBand->ComputeStatistics(bApproxOK ? TRUE : FALSE, &bMin, &bMax, nullptr, nullptr, ComputeStatisticsPrgFunc, progData);

	_txtRMin->SetValue(wxString::Format(wxT("%.3f"), rMin));
	_txtGMin->SetValue(wxString::Format(wxT("%.3f"), gMin));
	_txtBMin->SetValue(wxString::Format(wxT("%.3f"), bMin));
	_txtRMax->SetValue(wxString::Format(wxT("%.3f"), rMax));
	_txtGMax->SetValue(wxString::Format(wxT("%.3f"), gMax));
	_txtBMax->SetValue(wxString::Format(wxT("%.3f"), bMax));

	delete prgDlg;
}
