///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "VectorUniformLayerRenderCtrlBase.h"

///////////////////////////////////////////////////////////////////////////

VectorUniformLayerRenderCtrlBase::VectorUniformLayerRenderCtrlBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("OGR Layer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gbSizer1->Add( m_staticText2, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString _choiceLayerChoices;
	_choiceLayer = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, _choiceLayerChoices, 0 );
	_choiceLayer->SetSelection( 0 );
	gbSizer1->Add( _choiceLayer, wxGBPosition( 0, 1 ), wxGBSpan( 1, 7 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText51 = new wxStaticText( this, wxID_ANY, wxT("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	gbSizer1->Add( m_staticText51, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	_lblOpacityValue = new wxStaticText( this, wxID_ANY, wxT("255"), wxDefaultPosition, wxDefaultSize, 0 );
	_lblOpacityValue->Wrap( -1 );
	gbSizer1->Add( _lblOpacityValue, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	_sldOpacity = new wxSlider( this, wxID_ANY, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	gbSizer1->Add( _sldOpacity, wxGBPosition( 1, 2 ), wxGBSpan( 1, 6 ), wxALL|wxEXPAND, 5 );

	_tabSymPicker = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	gbSizer1->Add( _tabSymPicker, wxGBPosition( 2, 0 ), wxGBSpan( 1, 8 ), wxEXPAND | wxALL, 5 );


	gbSizer1->AddGrowableCol( 2 );
	gbSizer1->AddGrowableCol( 7 );
	gbSizer1->AddGrowableRow( 2 );

	this->SetSizer( gbSizer1 );
	this->Layout();
}

VectorUniformLayerRenderCtrlBase::~VectorUniformLayerRenderCtrlBase()
{
}
