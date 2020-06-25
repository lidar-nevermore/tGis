#include "wxGradientColorPickerDialog.h"
#include "wxGradientColorRepositoryWidget.h"
#include "wxGradientColorEditorWidget.h"

#include "wxTGisApplication.h"

BEGIN_NAME_SPACE(tGis, Gui)

wxGradientColorPickerDialog::wxGradientColorPickerDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	if (parent == nullptr)
		SetParent(((wxTGisApplication*)tGisApplication::INSTANCE())->GetMainFrame());

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	_tabGrdColor = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	_gcrWidget = new wxGradientColorRepositoryWidget(_tabGrdColor);
	_tabGrdColor->AddPage(_gcrWidget, wxT("Predefined Gradient Color"));

	_gcEditor = new wxGradientColorEditorWidget(_tabGrdColor);
	_tabGrdColor->AddPage(_gcEditor, wxT("Edit Gradient Color"));

	bSizer1->Add( _tabGrdColor, 1, wxEXPAND | wxALL, 3 );

	_sdbSz = new wxStdDialogButtonSizer();
	_sdbSzOK = new wxButton( this, wxID_OK );
	_sdbSz->AddButton( _sdbSzOK );
	_sdbSzCancel = new wxButton( this, wxID_CANCEL );
	_sdbSz->AddButton( _sdbSzCancel );
	_sdbSz->Realize();
	_sdbSz->SetMinSize( wxSize( -1,55 ) );

	bSizer1->Add( _sdbSz, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

wxGradientColorPickerDialog::~wxGradientColorPickerDialog()
{
}

GradientColor * wxGradientColorPickerDialog::GetGradientColor()
{
	if(_tabGrdColor->GetSelection() == 0)
		return _gcrWidget->GetSelGradientColor();

	return _gcEditor->GetGradientColor();
}

void wxGradientColorPickerDialog::SetGradientColor(GradientColor * color)
{
	_gcrWidget->SetSelGradientColor(color);
	GradientColor * c = color->Clone();
	_gcEditor->SetGradientColor(c);
	c->Release();
}


END_NAME_SPACE(tGis, Gui)
