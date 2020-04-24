///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "RgbLayerRenderCtrlBase.h"

///////////////////////////////////////////////////////////////////////////

RgbLayerRenderCtrlBase::RgbLayerRenderCtrlBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("R Band"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gbSizer1->Add( m_staticText2, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString _choiceRBandChoices;
	_choiceRBand = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, _choiceRBandChoices, 0 );
	_choiceRBand->SetSelection( 0 );
	gbSizer1->Add( _choiceRBand, wxGBPosition( 0, 1 ), wxGBSpan( 1, 7 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("R Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	gbSizer1->Add( m_staticText5, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtRMin = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtRMin, wxGBPosition( 1, 1 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("R Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	gbSizer1->Add( m_staticText6, wxGBPosition( 1, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtRMax = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtRMax, wxGBPosition( 1, 5 ), wxGBSpan( 1, 3 ), wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("R NoData"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	gbSizer1->Add( m_staticText13, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	_txtRNoData = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtRNoData, wxGBPosition( 2, 1 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	_chkRNoDataEQ = new wxCheckBox( this, wxID_ANY, wxT("EQ"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkRNoDataEQ, wxGBPosition( 2, 4 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	_chkRNoDataGT = new wxCheckBox( this, wxID_ANY, wxT("GT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkRNoDataGT, wxGBPosition( 2, 5 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	_chkRNoDataLT = new wxCheckBox( this, wxID_ANY, wxT("LT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkRNoDataLT, wxGBPosition( 2, 6 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("G Band"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gbSizer1->Add( m_staticText7, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString _choiceGBandChoices;
	_choiceGBand = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, _choiceGBandChoices, 0 );
	_choiceGBand->SetSelection( 0 );
	gbSizer1->Add( _choiceGBand, wxGBPosition( 3, 1 ), wxGBSpan( 1, 7 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("G Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	gbSizer1->Add( m_staticText8, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtGMin = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtGMin, wxGBPosition( 4, 1 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("G Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	gbSizer1->Add( m_staticText9, wxGBPosition( 4, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtGMax = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtGMax, wxGBPosition( 4, 5 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText111 = new wxStaticText( this, wxID_ANY, wxT("G NoData"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	gbSizer1->Add( m_staticText111, wxGBPosition( 5, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	_txtGNoData = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtGNoData, wxGBPosition( 5, 1 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	_chkGNoDataEQ = new wxCheckBox( this, wxID_ANY, wxT("EQ"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkGNoDataEQ, wxGBPosition( 5, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_chkGNoDataGT = new wxCheckBox( this, wxID_ANY, wxT("GT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkGNoDataGT, wxGBPosition( 5, 5 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_chkGNoDataLT = new wxCheckBox( this, wxID_ANY, wxT("LT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkGNoDataLT, wxGBPosition( 5, 6 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("B Band"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	gbSizer1->Add( m_staticText10, wxGBPosition( 6, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString _choiceBBandChoices;
	_choiceBBand = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, _choiceBBandChoices, 0 );
	_choiceBBand->SetSelection( 0 );
	gbSizer1->Add( _choiceBBand, wxGBPosition( 6, 1 ), wxGBSpan( 1, 7 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("B Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	gbSizer1->Add( m_staticText11, wxGBPosition( 7, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtBMin = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtBMin, wxGBPosition( 7, 1 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("B Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	gbSizer1->Add( m_staticText12, wxGBPosition( 7, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtBMax = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtBMax, wxGBPosition( 7, 5 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText121 = new wxStaticText( this, wxID_ANY, wxT("B NoData"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	gbSizer1->Add( m_staticText121, wxGBPosition( 8, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtBNoData = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtBNoData, wxGBPosition( 8, 1 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	_chkBNoDataEQ = new wxCheckBox( this, wxID_ANY, wxT("EQ"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkBNoDataEQ, wxGBPosition( 8, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_chkBNoDataGT = new wxCheckBox( this, wxID_ANY, wxT("GT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkBNoDataGT, wxGBPosition( 8, 5 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_chkBNoDataLT = new wxCheckBox( this, wxID_ANY, wxT("LT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkBNoDataLT, wxGBPosition( 8, 6 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText131 = new wxStaticText( this, wxID_ANY, wxT("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	gbSizer1->Add( m_staticText131, wxGBPosition( 9, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_lblOpacityValue = new wxStaticText( this, wxID_ANY, wxT("255"), wxDefaultPosition, wxDefaultSize, 0 );
	_lblOpacityValue->Wrap( -1 );
	gbSizer1->Add( _lblOpacityValue, wxGBPosition( 9, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	_sldOpacity = new wxSlider( this, wxID_ANY, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	gbSizer1->Add( _sldOpacity, wxGBPosition( 9, 2 ), wxGBSpan( 1, 6 ), wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	_btnComputeStatistics = new wxButton( this, wxID_ANY, wxT("Compute Statistics"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( _btnComputeStatistics, 0, wxALL, 5 );

	_gaugeStatistics = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	_gaugeStatistics->SetValue( 0 );
	bSizer1->Add( _gaugeStatistics, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	gbSizer1->Add( bSizer1, wxGBPosition( 10, 0 ), wxGBSpan( 1, 8 ), wxEXPAND, 5 );


	gbSizer1->AddGrowableCol( 2 );
	gbSizer1->AddGrowableCol( 7 );

	this->SetSizer( gbSizer1 );
	this->Layout();
}

RgbLayerRenderCtrlBase::~RgbLayerRenderCtrlBase()
{
}
