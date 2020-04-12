///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxLayerWidget.h"

///////////////////////////////////////////////////////////////////////////

wxLayerWidget::wxLayerWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_tool1 = _toolBar->AddTool( wxID_ANY, wxT("tool"), wxArtProvider::GetBitmap( wxART_NEW, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_tool2 = _toolBar->AddTool( wxID_ANY, wxT("tool"), wxArtProvider::GetBitmap( wxART_NEW, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	_toolBar->Realize();

	bSizer1->Add( _toolBar, 0, wxEXPAND, 5 );

	_treeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer1->Add( _treeCtrl, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
}

wxLayerWidget::~wxLayerWidget()
{
}
