///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxGradientColorEditorWidget.h"

///////////////////////////////////////////////////////////////////////////

wxGradientColorEditorWidget::wxGradientColorEditorWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );


	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Add Key Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer2->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_cpAdd = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer2->Add( _cpAdd, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	_btnDelete = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( _btnDelete, 0, wxALL, 5 );


	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );

	_gridKeyColor = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	_gridKeyColor->CreateGrid( 5, 5 );
	_gridKeyColor->EnableEditing( true );
	_gridKeyColor->EnableGridLines( true );
	_gridKeyColor->EnableDragGridSize( false );
	_gridKeyColor->SetMargins( 0, 0 );

	// Columns
	_gridKeyColor->EnableDragColMove( false );
	_gridKeyColor->EnableDragColSize( true );
	_gridKeyColor->SetColLabelSize( 30 );
	_gridKeyColor->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	_gridKeyColor->EnableDragRowSize( true );
	_gridKeyColor->SetRowLabelSize( 80 );
	_gridKeyColor->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	_gridKeyColor->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer1->Add( _gridKeyColor, 1, wxALL|wxEXPAND, 5 );

	_wxGradientColor = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	_wxGradientColor->SetMinSize( wxSize( -1,20 ) );
	_wxGradientColor->SetMaxSize( wxSize( -1,20 ) );

	bSizer1->Add( _wxGradientColor, 0, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
}

wxGradientColorEditorWidget::~wxGradientColorEditorWidget()
{
}
