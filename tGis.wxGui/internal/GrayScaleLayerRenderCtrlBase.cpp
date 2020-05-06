///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GrayScaleLayerRenderCtrlBase.h"

///////////////////////////////////////////////////////////////////////////

GrayScaleLayerRenderCtrlBase::GrayScaleLayerRenderCtrlBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
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

	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	gbSizer1->Add( m_staticText5, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtMin = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtMin, wxGBPosition( 1, 1 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	gbSizer1->Add( m_staticText6, wxGBPosition( 1, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtMax = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtMax, wxGBPosition( 1, 5 ), wxGBSpan( 1, 3 ), wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("NoData"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	gbSizer1->Add( m_staticText13, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtNoData = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtNoData, wxGBPosition( 2, 1 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	_chkNoDataEQ = new wxCheckBox( this, wxID_ANY, wxT("EQ"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkNoDataEQ, wxGBPosition( 2, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_chkNoDataGT = new wxCheckBox( this, wxID_ANY, wxT("GT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkNoDataGT, wxGBPosition( 2, 5 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_chkNoDataLT = new wxCheckBox( this, wxID_ANY, wxT("LT"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _chkNoDataLT, wxGBPosition( 2, 6 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText51 = new wxStaticText( this, wxID_ANY, wxT("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	gbSizer1->Add( m_staticText51, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	_lblOpacityValue = new wxStaticText( this, wxID_ANY, wxT("255"), wxDefaultPosition, wxDefaultSize, 0 );
	_lblOpacityValue->Wrap( -1 );
	gbSizer1->Add( _lblOpacityValue, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	_sldOpacity = new wxSlider( this, wxID_ANY, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	gbSizer1->Add( _sldOpacity, wxGBPosition( 3, 2 ), wxGBSpan( 1, 6 ), wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	_btnComputeStatistics = new wxButton( this, wxID_ANY, wxT("Compute Statistics"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( _btnComputeStatistics, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_chkApproximate = new wxCheckBox( this, wxID_ANY, wxT("approximate"), wxDefaultPosition, wxDefaultSize, 0 );
	_chkApproximate->SetValue(true);
	bSizer1->Add( _chkApproximate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	gbSizer1->Add( bSizer1, wxGBPosition( 4, 0 ), wxGBSpan( 1, 8 ), wxEXPAND, 5 );


	gbSizer1->AddGrowableCol( 2 );
	gbSizer1->AddGrowableCol( 7 );

	this->SetSizer( gbSizer1 );
	this->Layout();
}

GrayScaleLayerRenderCtrlBase::~GrayScaleLayerRenderCtrlBase()
{
}
