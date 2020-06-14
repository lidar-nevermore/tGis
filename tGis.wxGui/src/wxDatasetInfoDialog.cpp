#include "wxDatasetInfoDialog.h"
#include "wxDatasetInfoWidget.h"
#include "wxTGisApplication.h"

BEGIN_NAME_SPACE(tGis, Gui)

wxDatasetInfoDialog::wxDatasetInfoDialog(IDataset* dataset, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) 
	: wxDialog( parent, id, title, pos, size, style )
{
	if (parent == nullptr)
		SetParent(((wxTGisApplication*)tGisApplication::INSTANCE())->GetMainFrame());

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	_txtDtInfo = new wxDatasetInfoWidget(dataset, this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP | wxTE_MULTILINE | wxTE_READONLY);
	bSizer2->Add( _txtDtInfo, 1, wxALL|wxEXPAND, 5 );

	_sdbSz = new wxStdDialogButtonSizer();
	_sdbSzOK = new wxButton( this, wxID_OK );
	_sdbSz->AddButton( _sdbSzOK );
	_sdbSz->Realize();
	_sdbSz->SetMinSize( wxSize( -1,55 ) );

	bSizer2->Add( _sdbSz, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );
}

wxDatasetInfoDialog::~wxDatasetInfoDialog()
{
}

END_NAME_SPACE(tGis, Gui)
