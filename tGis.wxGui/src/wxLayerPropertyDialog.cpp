#include "wxLayerPropertyDialog.h"
#include "wxLayerRenderWidget.h"
#include "wxTGisApplication.h"
#include "wxDatasetInfoWidget.h"


#define _TOOL_PNG(file_name) \
wxBitmap(wxString(tGisApplication::INSTANCE()->GetExeDir()) \
+ wxString("/wxGui_res/wxLayerPropertyDialog/tool/"##file_name##".png"), \
wxBITMAP_TYPE_PNG )

BEGIN_NAME_SPACE(tGis, Gui)

enum
{
	wxID_LAYER_STYLE = wxID_HIGHEST + 1,
	wxID_DATASET_INFO = wxID_HIGHEST + 2,
};

wxLayerPropertyDialog::wxLayerPropertyDialog(ILayer* layer, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) 
	: wxDialog( parent, id, title, pos, size, style )
{
	if(parent == nullptr)
		SetParent(((wxTGisApplication*)tGisApplication::INSTANCE())->GetMainFrame());
	
	_layer = layer;
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_DEFAULT_STYLE|wxTB_FLAT|wxTB_TEXT|wxTB_VERTICAL );
	_toolBar->SetToolBitmapSize( wxSize( 48,48 ) );
	_toolBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );

	_toolLayerStyle = _toolBar->AddTool(wxID_LAYER_STYLE, wxT("图层样式"), _TOOL_PNG("LayerStyle"), wxNullBitmap, wxITEM_RADIO, wxEmptyString, wxEmptyString, NULL );

	_toolDatasetInfo = _toolBar->AddTool(wxID_DATASET_INFO, wxT("数据集信息"), _TOOL_PNG("DatasetInfo"), wxNullBitmap, wxITEM_RADIO, wxEmptyString, wxEmptyString, NULL );

	_toolBar->Realize();

	bSizer1->Add( _toolBar, 0, wxEXPAND, 5 );

	wxBoxSizer* _bszMain;
	_bszMain = new wxBoxSizer( wxVERTICAL );

	_pnlLayerRender = new wxLayerRenderWidget(layer, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	_bszMain->Add(_pnlLayerRender, 1, wxEXPAND | wxALL, 5 );

	_txtDatasetInfo = new wxDatasetInfoWidget(layer->GetDataset(), this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP | wxTE_MULTILINE | wxTE_READONLY);
	_bszMain->Add(_txtDatasetInfo, 1, wxALL | wxEXPAND, 5);

	_sdbSz = new wxStdDialogButtonSizer();
	_sdbSzOK = new wxButton( this, wxID_OK );
	_sdbSz->AddButton( _sdbSzOK );
	_sdbSzCancel = new wxButton( this, wxID_CANCEL );
	_sdbSz->AddButton( _sdbSzCancel );
	_sdbSz->Realize();
	_sdbSz->SetMinSize( wxSize( 50,50 ) );

	_bszMain->Add( _sdbSz, 0, wxEXPAND, 5 );


	bSizer1->Add( _bszMain, 1, wxEXPAND, 5 );

	_toolBar->ToggleTool(_toolLayerStyle->GetId(), true);
	_txtDatasetInfo->Hide();

	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	Bind(wxEVT_TOOL, &wxLayerPropertyDialog::_tool_Clicked, this, _toolLayerStyle->GetId());
	Bind(wxEVT_TOOL, &wxLayerPropertyDialog::_tool_Clicked, this, _toolDatasetInfo->GetId());
	Bind(wxEVT_BUTTON, &wxLayerPropertyDialog::_sdbSzOK_Clicked, this, _sdbSzOK->GetId());

}

wxLayerPropertyDialog::~wxLayerPropertyDialog()
{
	Unbind(wxEVT_TOOL, &wxLayerPropertyDialog::_tool_Clicked, this, _toolLayerStyle->GetId());
	Unbind(wxEVT_TOOL, &wxLayerPropertyDialog::_tool_Clicked, this, _toolDatasetInfo->GetId());
	Unbind(wxEVT_BUTTON, &wxLayerPropertyDialog::_sdbSzOK_Clicked, this, _sdbSzOK->GetId());

}

void wxLayerPropertyDialog::_tool_Clicked(wxCommandEvent & event)
{
	int toolId = event.GetId();
	switch (toolId)
	{
	case wxID_LAYER_STYLE:
		_pnlLayerRender->Show();
		_txtDatasetInfo->Hide();
		break;
	case wxID_DATASET_INFO:
		_pnlLayerRender->Hide();
		_txtDatasetInfo->Show();
		break;
	default:
		break;
	}

	this->Layout();
}

void wxLayerPropertyDialog::_sdbSzOK_Clicked(wxCommandEvent & event)
{
	_pnlLayerRender->UpdateLayerRender();
	event.SetId(wxID_OK);
	event.Skip();
}


END_NAME_SPACE(tGis, Gui)
