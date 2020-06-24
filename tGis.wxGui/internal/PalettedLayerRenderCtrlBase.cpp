///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "PalettedLayerRenderCtrlBase.h"

///////////////////////////////////////////////////////////////////////////

PalettedLayerRenderCtrlBase::PalettedLayerRenderCtrlBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Band"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gbSizer1->Add( m_staticText2, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString _choiceBandChoices;
	_choiceBand = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, _choiceBandChoices, 0 );
	_choiceBand->SetSelection( 0 );
	gbSizer1->Add( _choiceBand, wxGBPosition( 0, 1 ), wxGBSpan( 1, 7 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("NoData"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	gbSizer1->Add( m_staticText13, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtNoData = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtNoData, wxGBPosition( 1, 1 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	_chkNoDataEQ = new wxCheckBox( this, wxID_ANY, wxT("EQ"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkNoDataEQ, wxGBPosition( 1, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_chkNoDataGT = new wxCheckBox( this, wxID_ANY, wxT("GT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkNoDataGT, wxGBPosition( 1, 5 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_chkNoDataLT = new wxCheckBox( this, wxID_ANY, wxT("LT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkNoDataLT, wxGBPosition( 1, 6 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText51 = new wxStaticText( this, wxID_ANY, wxT("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	gbSizer1->Add( m_staticText51, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_lblOpacityValue = new wxStaticText( this, wxID_ANY, wxT("255"), wxDefaultPosition, wxDefaultSize, 0 );
	_lblOpacityValue->Wrap( -1 );
	gbSizer1->Add( _lblOpacityValue, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_sldOpacity = new wxSlider( this, wxID_ANY, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	gbSizer1->Add( _sldOpacity, wxGBPosition( 2, 2 ), wxGBSpan( 1, 6 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	_dvPalette = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_MULTIPLE );
	bSizer2->Add( _dvPalette, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );


	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );

	_btnRandColor = new wxButton( this, wxID_ANY, wxT("Rand Color"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( _btnRandColor, 0, wxALL, 5 );

	_btnGradColor = new wxButton( this, wxID_ANY, wxT("Grad Color"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( _btnGradColor, 0, wxALL, 5 );

	_btnAdd = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	_btnAdd->SetMaxSize( wxSize( 35,-1 ) );

	bSizer3->Add( _btnAdd, 0, wxALL, 5 );

	_btnEdit = new wxButton( this, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	_btnEdit->SetMaxSize( wxSize( 35,-1 ) );

	bSizer3->Add( _btnEdit, 0, wxALL, 5 );

	_btnDelete = new wxButton( this, wxID_ANY, wxT("Del"), wxDefaultPosition, wxDefaultSize, 0 );
	_btnDelete->SetMaxSize( wxSize( 35,-1 ) );

	bSizer3->Add( _btnDelete, 0, wxALL, 5 );

	_btnClear = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	_btnClear->SetMaxSize( wxSize( 50,-1 ) );

	bSizer3->Add( _btnClear, 0, wxALL, 5 );


	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );


	gbSizer1->Add( bSizer2, wxGBPosition( 3, 0 ), wxGBSpan( 1, 8 ), wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	gbSizer1->AddGrowableCol( 2 );
	gbSizer1->AddGrowableCol( 7 );
	gbSizer1->AddGrowableRow( 3 );

	this->SetSizer( gbSizer1 );
	this->Layout();
}

PalettedLayerRenderCtrlBase::~PalettedLayerRenderCtrlBase()
{
}
