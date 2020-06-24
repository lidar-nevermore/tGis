#include <wx/wx.h>
#include "PalettedLayerRenderCtrl.h"
#include <wx/progdlg.h>

#include "wxDataViewColorRenderer.h"

using namespace tGis::Gui;

static void __stdcall ForEachColor(int* pos, unsigned char *r, unsigned char *g, unsigned char *b, void* ud)
{
	wxDataViewListCtrl* dvPal = (wxDataViewListCtrl*)ud;

	wxVector<wxVariant> data;
	data.push_back(wxVariant(*pos));
	//wxColour clr;
	//clr.Set(*r, *g, *b);
	//wxVariant var;
	//var << clr;
	//data.push_back(var);
	long clr = *r | *g << 8 | *b << 16;
	data.push_back(wxVariant(clr));
	dvPal->AppendItem(data, (wxUIntPtr)(*pos));
}

PalettedLayerRenderCtrl::PalettedLayerRenderCtrl( wxWindow* parent )
	:PalettedLayerRenderCtrlBase( parent )
{
	_render = nullptr;
	_raster = nullptr;
	_layer = nullptr;
	_palette = nullptr;

	wxDataViewColumn* col0 = _dvPalette->AppendTextColumn(wxT("Value"));
	col0->SetSortable(true);
	wxDataViewColorRenderer* render = new wxDataViewColorRenderer();
	wxDataViewColumn* col1 = new wxDataViewColumn("Color", render, 1);
	_dvPalette->AppendColumn(col1);
	//_dvPalette->AppendTextColumn(wxT("Color"));

	Bind(wxEVT_SLIDER, &PalettedLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());
	Bind(wxEVT_BUTTON, &PalettedLayerRenderCtrl::_btnRandColor_clicked, this, _btnRandColor->GetId());
	Bind(wxEVT_BUTTON, &PalettedLayerRenderCtrl::_btnClear_clicked, this, _btnClear->GetId());
	Bind(wxEVT_BUTTON, &PalettedLayerRenderCtrl::_btnDelete_clicked, this, _btnDelete->GetId());

}

PalettedLayerRenderCtrl::~PalettedLayerRenderCtrl()
{
	Unbind(wxEVT_SLIDER, &PalettedLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());
	Unbind(wxEVT_BUTTON, &PalettedLayerRenderCtrl::_btnRandColor_clicked, this, _btnRandColor->GetId());
	Unbind(wxEVT_BUTTON, &PalettedLayerRenderCtrl::_btnClear_clicked, this, _btnClear->GetId());
	Unbind(wxEVT_BUTTON, &PalettedLayerRenderCtrl::_btnDelete_clicked, this, _btnDelete->GetId());


	if (_palette != nullptr)
		_palette->Release();
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
	_render->SetPalette(_palette);
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
		_palette = pal->Clone();
		_palette->ForEachColor(ForEachColor, _dvPalette);

		//this->Layout();
	}
}

void PalettedLayerRenderCtrl::_sldOpacity_scroll(wxCommandEvent & event)
{
	_lblOpacityValue->SetLabel(wxString::Format(wxT("%-3d"), event.GetInt()));
}

void PalettedLayerRenderCtrl::_btnRandColor_clicked(wxCommandEvent & event)
{
	_palette = Palette::CreatePalette(_raster, _choiceBand->GetSelection() + 1);
	_palette->ForEachColor(ForEachColor, _dvPalette);
}

void PalettedLayerRenderCtrl::_btnGradColor_clicked(wxCommandEvent & event)
{
}

void PalettedLayerRenderCtrl::_btnAdd_clicked(wxCommandEvent & event)
{
}

static int CompareInt(int* first, int *second)
{
	if (*first > *second)
		return -1;
	else if (*first == *second)
		return 0;
	else return 1;
}

void PalettedLayerRenderCtrl::_btnDelete_clicked(wxCommandEvent & event)
{
	wxArrayInt rows;
	wxDataViewItemArray sels;
	_dvPalette->GetSelections(sels);
	for (wxDataViewItemArray::iterator it = sels.begin(); it != sels.end(); it++)
	{
		wxDataViewItem item = *it;
		int row = _dvPalette->ItemToRow(item);
		rows.push_back(row);
		int ent = (int)(_dvPalette->GetItemData(item));
		_palette->RemoveColor(ent);
	}

	rows.Sort(CompareInt);

	for (wxArrayInt::iterator it = rows.begin(); it != rows.end(); it++)
	{
		int row = *it;
		_dvPalette->DeleteItem(row);
	}
}

void PalettedLayerRenderCtrl::_btnClear_clicked(wxCommandEvent & event)
{
	_dvPalette->DeleteAllItems();
	_palette->ClearColor();
}
