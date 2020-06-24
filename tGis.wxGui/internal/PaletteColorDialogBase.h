///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/clrpicker.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PaletteColorDialogBase
///////////////////////////////////////////////////////////////////////////////
class PaletteColorDialogBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText4;
		wxTextCtrl* _txtValue;
		wxStaticText* m_staticText2;
		wxColourPickerCtrl* _cpColor;
		wxStdDialogButtonSizer* _sdbSz;
		wxButton* _sdbSzOK;
		wxButton* _sdbSzCancel;

	public:

		PaletteColorDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Palette Color"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 346,211 ), long style = wxDEFAULT_DIALOG_STYLE );
		~PaletteColorDialogBase();

};

