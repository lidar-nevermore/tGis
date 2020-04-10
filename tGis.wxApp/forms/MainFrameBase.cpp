///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MainFrameBase.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	_menuBar = new wxMenuBar( 0 );
	_menuProject = new wxMenu();
	_menuTest = new wxMenu();
	wxMenuItem* _menuTestItem = new wxMenuItem( _menuProject, wxID_ANY, wxT("MyMenu"), wxEmptyString, wxITEM_NORMAL, _menuTest );
	_menuProject->Append( _menuTestItem );

	_menuBar->Append( _menuProject, wxT("Project") );

	this->SetMenuBar( _menuBar );

	_toolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	_toolTest = _toolBar->AddTool( wxID_OPEN_FILE, wxT("tool"), wxArtProvider::GetBitmap( wxART_NEW, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	_toolBar->Realize();

	_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( _toolTest->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::_toolTestOnToolClicked ) );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( _toolTest->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::_toolTestOnToolClicked ) );

}
