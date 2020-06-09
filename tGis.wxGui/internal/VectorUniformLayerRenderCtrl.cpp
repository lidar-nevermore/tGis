#include <wx/wx.h>
#include "VectorUniformLayerRenderCtrl.h"
#include "wxSymbolPickerWidget.h"
#include "SymbolLibraryRepository.h"

VectorUniformLayerRenderCtrl::VectorUniformLayerRenderCtrl( wxWindow* parent )
	: VectorUniformLayerRenderCtrlBase( parent )
{
	_render = nullptr;
	_vector = nullptr;
	_layer = nullptr;

	_lineSymPicker = new wxSymbolPickerWidget(SymbolLibraryType::Line, _tabSymPicker);
	_tabSymPicker->AddPage(_lineSymPicker, wxT("Line Symbol"));
	size_t lineSymLibCount = SymbolLibraryRepository::INSTANCE()->GetLineSymbolLibraryCount();
	for (size_t i = 0; i < lineSymLibCount; i++)
	{
		_lineSymPicker->AddSymbolLibrary(SymbolLibraryRepository::INSTANCE()->GetLineSymbolLibrary(i));
	}

	_fillSymPicker = new wxSymbolPickerWidget(SymbolLibraryType::Fill, _tabSymPicker);
	_tabSymPicker->AddPage(_fillSymPicker, wxT("Fill Symbol"));
	size_t fillSymLibCount = SymbolLibraryRepository::INSTANCE()->GetFillSymbolLibraryCount();
	for (size_t i = 0; i < fillSymLibCount; i++)
	{
		_fillSymPicker->AddSymbolLibrary(SymbolLibraryRepository::INSTANCE()->GetFillSymbolLibrary(i));
	}

	_markSymPicker = new wxSymbolPickerWidget(SymbolLibraryType::Marker, _tabSymPicker);
	_tabSymPicker->AddPage(_markSymPicker, wxT("Marker Symbol"));
	size_t markSymLibCount = SymbolLibraryRepository::INSTANCE()->GetMarkerSymbolLibraryCount();
	for (size_t i = 0; i < markSymLibCount; i++)
	{
		_markSymPicker->AddSymbolLibrary(SymbolLibraryRepository::INSTANCE()->GetMarkerSymbolLibrary(i));
	}

	_tabSymPicker->Layout();

	Bind(wxEVT_SLIDER, &VectorUniformLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());

}

VectorUniformLayerRenderCtrl::~VectorUniformLayerRenderCtrl()
{
	Unbind(wxEVT_SLIDER, &VectorUniformLayerRenderCtrl::_sldOpacity_scroll, this, _sldOpacity->GetId());

}

const char * VectorUniformLayerRenderCtrl::GetLayerRenderName()
{
	return "Vector Uniform Render";
}

bool VectorUniformLayerRenderCtrl::IsSupportLayerExactly(ILayer * layer)
{
	if (IsSupportLayer(layer))
	{
		ILayerRender* render = layer->GetRender();

		if (render == nullptr)
		{
			MyGDALVectorDataset* dt = dynamic_cast<MyGDALVectorDataset*>(layer->GetDataset());
			int layerCount = dt->GetGDALDataset()->GetLayerCount();
			if(layerCount > 0)
				return true;
		}
		else
		{
			if (render->IsTypeOf(VectorUniformLayerRender::S_GetType()))
				return true;
		}
	}

	return false;
}

bool VectorUniformLayerRenderCtrl::IsSupportLayer(ILayer * layer)
{
	IDataset* dt = layer->GetDataset();
	if (dt->IsTypeOf(MyGDALVectorDataset::S_GetType()))
		return true;

	return false;
}

void VectorUniformLayerRenderCtrl::SetLayer(ILayer * layer)
{
	_layer = layer;
	_vector = (MyGDALVectorDataset*)layer->GetDataset();
	GDALDataset* dt = _vector->GetGDALDataset();
	int layerCount = dt->GetLayerCount();
	for (int i = 1; i <= layerCount; i++)
	{
		wxString bandStr = wxString::Format(wxT("Layer %d"), i);
		_choiceLayer->Append(bandStr);
	}

	ILayerRender* render = layer->GetRender();
	VectorUniformLayerRender* trender = dynamic_cast<VectorUniformLayerRender*>(render);
	if (trender == nullptr)
		_choiceLayer->SetSelection(0);
	else
		SetLayerRender(trender);

}

void VectorUniformLayerRenderCtrl::UpdateLayerRender()
{
	if (_render == nullptr)
		_render = new VectorUniformLayerRender(_layer, _choiceLayer->GetSelection());

	_render->SetOGRLayer(_choiceLayer->GetSelection());

	int opacity = _sldOpacity->GetValue();
	_render->SetOpacity(opacity / 255.0f);
	_render->SetMarkerSymbol(dynamic_cast<IMarkerSymbol*>(_markSymPicker->GetSymbol()));
	_render->SetLineSymbol(dynamic_cast<ILineSymbol*>(_lineSymPicker->GetSymbol()));
	_render->SetFillSymbol(dynamic_cast<IFillSymbol*>(_fillSymPicker->GetSymbol()));
}

void VectorUniformLayerRenderCtrl::SetLayerRender(VectorUniformLayerRender * render)
{
	_render = render;

	_sldOpacity->SetValue((int)(_render->GetOpacity() * 255));
	_choiceLayer->SetSelection(_render->GetOGRLayer());

	_markSymPicker->SetSymbol(_render->GetMarkerSymbol());
	_lineSymPicker->SetSymbol(_render->GetLineSymbol());
	_fillSymPicker->SetSymbol(_render->GetFillSymbol());
}

void VectorUniformLayerRenderCtrl::_sldOpacity_scroll(wxCommandEvent & event)
{
	_lblOpacityValue->SetLabel(wxString::Format(wxT("%-3d"), event.GetInt()));
}
