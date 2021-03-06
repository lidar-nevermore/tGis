#include "wxLayerRenderWidget.h"
#include "ILayerRenderCtrl.h"
#include "RgbLayerRenderCtrl.h"
#include "GrayScaleLayerRenderCtrl.h"
#include "DualRampLayerRenderCtrl.h"
#include "VectorUniformLayerRenderCtrl.h"
#include "ColorRampLayerRenderCtrl.h"
#include "PalettedLayerRenderCtrl.h"

#include "tGis_Symbol.h"

BEGIN_NAME_SPACE(tGis, Gui)

class wxLayerRenderWidgetImpl
{
public:
	wxLayerRenderWidgetImpl(wxLayerRenderWidget* owner)
	{
		_owner = owner;
	}

	wxLayerRenderWidget* _owner;
	vector<ILayerRenderCtrl*> _vecLayerRenderCtrl;
};


wxLayerRenderWidget::wxLayerRenderWidget(ILayer* layer, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name )
	: wxPanel( parent, id, pos, size, style, name )
{
	_impl_ = new wxLayerRenderWidgetImpl(this);
	_selLayerRenderCtrl = nullptr;
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

	_dualRampRenderCtrl = new DualRampLayerRenderCtrl(this);
	_bszMain->Add(_dualRampRenderCtrl, 1, wxEXPAND | wxALL, 5);
	_dualRampRenderCtrl->Hide();
	_impl_->_vecLayerRenderCtrl.push_back(_dualRampRenderCtrl);

	_vectorUniformLayerRenderCtrl = new VectorUniformLayerRenderCtrl(this);
	_bszMain->Add(_vectorUniformLayerRenderCtrl, 1, wxEXPAND | wxALL, 5);
	_vectorUniformLayerRenderCtrl->Hide();
	_impl_->_vecLayerRenderCtrl.push_back(_vectorUniformLayerRenderCtrl);

	_colorRampRenderCtrl = new ColorRampLayerRenderCtrl(this);
	_bszMain->Add(_colorRampRenderCtrl, 1, wxEXPAND | wxALL, 5);
	_colorRampRenderCtrl->Hide();
	_impl_->_vecLayerRenderCtrl.push_back(_colorRampRenderCtrl);

	_palettedLayerRenderCtrl = new PalettedLayerRenderCtrl(this);
	_bszMain->Add(_palettedLayerRenderCtrl, 1, wxEXPAND | wxALL, 5);
	_palettedLayerRenderCtrl->Hide();
	_impl_->_vecLayerRenderCtrl.push_back(_palettedLayerRenderCtrl);

	this->SetSizer(_bszMain);
	this->Layout();

	bool selected = false;
	selected = SelectRenderCtrl(_rgbRenderCtrl, selected);
	selected = SelectRenderCtrl(_grayScaleRenderCtrl, selected);
	selected = SelectRenderCtrl(_dualRampRenderCtrl, selected);
	selected = SelectRenderCtrl(_colorRampRenderCtrl, selected);
	selected = SelectRenderCtrl(_palettedLayerRenderCtrl, selected);

	selected = SelectRenderCtrl(_vectorUniformLayerRenderCtrl, selected);
	
	Bind(wxEVT_CHOICE, &wxLayerRenderWidget::_choiceRander_selected, this, _choiceRander->GetId());

}

wxLayerRenderWidget::~wxLayerRenderWidget()
{
	Unbind(wxEVT_CHOICE, &wxLayerRenderWidget::_choiceRander_selected, this, _choiceRander->GetId());

	delete _impl_;
}

void wxLayerRenderWidget::UpdateLayerRender()
{
	if (_selLayerRenderCtrl != nullptr)
		_selLayerRenderCtrl->UpdateLayerRender();
}

bool wxLayerRenderWidget::SelectRenderCtrl(ILayerRenderCtrl* renderCtrl, bool selected)
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

	return selected;
}

void wxLayerRenderWidget::_choiceRander_selected(wxCommandEvent & event)
{
	int sel = event.GetSelection();
	if (sel < 0)
		return;

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
