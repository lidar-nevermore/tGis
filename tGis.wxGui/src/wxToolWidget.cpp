#include "wxToolWidget.h"

#define _TOOL_PNG(file_name) \
wxBitmap(wxString(tGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxToolWidget/tool/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

#define _TREE_PNG(file_name) \
wxBitmap(wxString(tGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxToolWidget/tree/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

BEGIN_NAME_SPACE(tGis, Gui)

wxToolWidget::wxToolWidget( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	_toolAddTool = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("AddTool"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolRemoveTool = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("RemoveTool"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);


	_srchCtrl = new wxSearchCtrl(_toolBar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
#ifndef __WXMAC__
	_srchCtrl->ShowSearchButton(true);
#endif
	_srchCtrl->ShowCancelButton(true);
	_toolSrch  = _toolBar->AddControl(_srchCtrl);
	//_toolEndFind = _toolBar->AddTool(wxID_ANY, wxEmptyString, _TOOL_PNG("EndFind"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	_toolBar->Realize();

	bSizer1->Add( _toolBar, 0, wxEXPAND, 5 );

	_treeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer1->Add( _treeCtrl, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
}

wxToolWidget::~wxToolWidget()
{
}

END_NAME_SPACE(tGis, Gui)

