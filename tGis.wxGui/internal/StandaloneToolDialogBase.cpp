///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "StandaloneToolDialogBase.h"

///////////////////////////////////////////////////////////////////////////

StandaloneToolDialogBase::StandaloneToolDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Belong"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	gbSizer1->Add( m_staticText1, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtBelong = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtBelong, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	gbSizer1->Add( m_staticText3, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_txtName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( _txtName, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Exe File"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gbSizer1->Add( m_staticText2, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_fpExeFile = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	gbSizer1->Add( _fpExeFile, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Parameters") ), wxVERTICAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	_txtParam = new wxTextCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( _txtParam, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_btnAddParam = new wxButton( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( _btnAddParam, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_btnDelParam = new wxButton( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Del"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( _btnDelParam, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	sbSizer1->Add( bSizer2, 0, wxEXPAND, 5 );

	_lstParam = new wxListBox( sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_EXTENDED|wxLB_HSCROLL|wxLB_NEEDED_SB );
	sbSizer1->Add( _lstParam, 1, wxALL|wxEXPAND, 5 );


	gbSizer1->Add( sbSizer1, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 9 );

	_sdbSz = new wxStdDialogButtonSizer();
	_sdbSzOK = new wxButton( this, wxID_OK );
	_sdbSz->AddButton( _sdbSzOK );
	_sdbSzCancel = new wxButton( this, wxID_CANCEL );
	_sdbSz->AddButton( _sdbSzCancel );
	_sdbSz->Realize();
	_sdbSz->SetMinSize( wxSize( -1,50 ) );

	gbSizer1->Add( _sdbSz, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );


	gbSizer1->AddGrowableCol( 1 );
	gbSizer1->AddGrowableRow( 3 );

	this->SetSizer( gbSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

StandaloneToolDialogBase::~StandaloneToolDialogBase()
{
}
