
#include "wxSpatialReferenceDialog.h"
#include "wxTGisApplication.h"

BEGIN_NAME_SPACE(tGis, Gui)

wxSpatialReferenceDialog::wxSpatialReferenceDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) 
	: wxDialog( parent, id, title, pos, size, style )
{
	if (parent == nullptr)
		SetParent(((wxTGisApplication*)tGisApplication::INSTANCE())->GetMainFrame());

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	_txtSpatialReference = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	bSizer2->Add( _txtSpatialReference, 1, wxALL|wxEXPAND, 5 );

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

wxSpatialReferenceDialog::~wxSpatialReferenceDialog()
{
}

static std::string _str_format(const char* format, va_list args)
{
	int count = _vsnprintf(NULL, 0, format, args);
	size_t bufsz = count * sizeof(wchar_t);
	char* buff = (char*)malloc(bufsz);
	_vsnprintf(buff, bufsz, format, args);

	std::string str(buff, count);
	free(buff);
	return str;
}

static void Concat(wxString& osRet, bool bStdoutOutput,
	const char* pszFormat, ...)
{
	va_list args;
	va_start(args, pszFormat);

	if (bStdoutOutput)
	{
		vfprintf(stdout, pszFormat, args);
	}
	else
	{
		try
		{
			wxString osTarget(_str_format(pszFormat, args).c_str());

			osRet += osTarget;
		}
		catch (const std::bad_alloc&)
		{
			CPLError(CE_Failure, CPLE_OutOfMemory, "Out of memory");
		}
	}

	va_end(args);
}


void wxSpatialReferenceDialog::SetSpatialReference(const OGRSpatialReference * spatialRef)
{
	if (spatialRef == nullptr)
		return;

	wxString osStr;
	OGRSpatialReferenceH hSRS = const_cast<OGRSpatialReference *>(spatialRef);

	char *pszPrettyWkt = NULL;
	OSRExportToPrettyWkt(hSRS, &pszPrettyWkt, FALSE);

	Concat(osStr, false,
		"Coordinate System is:\n%s\n",
		pszPrettyWkt);

	CPLFree(pszPrettyWkt);

	char *pszProj4 = NULL;
	OSRExportToProj4(hSRS, &pszProj4);

	Concat(osStr, false,
		"PROJ.4 string is:\n\'%s\'\n", pszProj4);

	CPLFree(pszProj4);

	_txtSpatialReference->SetValue(osStr);
}

END_NAME_SPACE(tGis, Gui)
