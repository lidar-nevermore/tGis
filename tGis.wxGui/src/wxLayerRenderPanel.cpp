#include "wxLayerRenderPanel.h"
#include "ILayerRenderCtrl.h"
#include "RgbLayerRenderCtrl.h"
#include "GrayScaleLayerRenderCtrl.h"
#include "ColorRampLayerRenderCtrl.h"

BEGIN_NAME_SPACE(tGis, Gui)

class wxLayerRenderPanelImpl
{
public:
	wxLayerRenderPanelImpl(wxLayerRenderPanel* owner)
	{
		_owner = owner;
	}

	wxLayerRenderPanel* _owner;
	vector<ILayerRenderCtrl*> _vecLayerRenderCtrl;
};


wxLayerRenderPanel::wxLayerRenderPanel(ILayer* layer, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name )
	: wxPanel( parent, id, pos, size, style, name )
{
	_impl_ = new wxLayerRenderPanelImpl(this);
	_layer = layer;
	_dataset = layer->GetDataset();
	wxBoxSizer* _bszMain;
	_bszMain = new wxBoxSizer( wxVERTICAL );

	wxArrayString _choiceRanderChoices;
	_choiceRander = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, _choiceRanderChoices, 0 );
	
	_bszMain->Add(_choiceRander, 0, wxALL | wxEXPAND, 10);

	_rgbRenderCtrl = new RgbLayerRenderCtrl(this);
	_bszMain->Add(_rgbRenderCtrl, 1, wxEXPAND | wxALL, 5 );
	_rgbRenderCtrl->Hide();
	_impl_->_vecLayerRenderCtrl.push_back(_rgbRenderCtrl);

	_grayScaleRenderCtrl = new GrayScaleLayerRenderCtrl(this);
	_bszMain->Add(_grayScaleRenderCtrl, 1, wxEXPAND | wxALL, 5);
	_grayScaleRenderCtrl->Hide();
	_impl_->_vecLayerRenderCtrl.push_back(_grayScaleRenderCtrl);

	_colorRampRenderCtrl = new ColorRampLayerRenderCtrl(this);
	_bszMain->Add(_colorRampRenderCtrl, 1, wxEXPAND | wxALL, 5);
	_colorRampRenderCtrl->Hide();
	_impl_->_vecLayerRenderCtrl.push_back(_colorRampRenderCtrl);

	bool selected = false;
	selected = SelectRenderCtrl(_rgbRenderCtrl, selected);
	selected = SelectRenderCtrl(_grayScaleRenderCtrl, selected);
	selected = SelectRenderCtrl(_colorRampRenderCtrl, selected);


	this->SetSizer( _bszMain );
	this->Layout();

	Bind(wxEVT_CHOICE, &wxLayerRenderPanel::_choiceRander_selected, this, _choiceRander->GetId());

}

wxLayerRenderPanel::~wxLayerRenderPanel()
{
	Unbind(wxEVT_CHOICE, &wxLayerRenderPanel::_choiceRander_selected, this, _choiceRander->GetId());

	delete _impl_;
}

void wxLayerRenderPanel::UpdateLayerRender()
{
	if (_selLayerRenderCtrl != nullptr)
		_selLayerRenderCtrl->UpdateLayerRender();
}

bool wxLayerRenderPanel::SelectRenderCtrl(ILayerRenderCtrl* renderCtrl, bool selected)
{
	int itemIndex = -1;

	if (renderCtrl->IsSupportLayer(_layer))
		itemIndex = _choiceRander->Append(wxString(renderCtrl->GetLayerRenderName()), renderCtrl);

	if (itemIndex >= 0 && selected == false 
		&& renderCtrl->IsSupportLayerExactly(_layer))
	{
		(dynamic_cast<wxPanel*>(renderCtrl))->Show();
		_selLayerRenderCtrl = renderCtrl;
		renderCtrl->SetLayer(_layer);
		_choiceRander->SetSelection(itemIndex);

		return true;
	}

	return false;
}

void wxLayerRenderPanel::_choiceRander_selected(wxCommandEvent & event)
{
	int sel = event.GetSelection();
	_selLayerRenderCtrl = (ILayerRenderCtrl*)_choiceRander->GetClientData(sel);
	_selLayerRenderCtrl->SetLayer(_layer);
	(dynamic_cast<wxPanel*>(_selLayerRenderCtrl))->Show();

	for (auto it = _impl_->_vecLayerRenderCtrl.begin(); it != _impl_->_vecLayerRenderCtrl.end(); it++)
	{
		ILayerRenderCtrl* ctrl = *it;
		if (ctrl != _selLayerRenderCtrl)
			(dynamic_cast<wxPanel*>(ctrl))->Hide();
	}

	this->Layout();
}

END_NAME_SPACE(tGis, Gui)
