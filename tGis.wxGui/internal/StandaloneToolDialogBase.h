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
#include <wx/filepicker.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/statbox.h>
#include <wx/gbsizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class StandaloneToolDialogBase
///////////////////////////////////////////////////////////////////////////////
class StandaloneToolDialogBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText1;
		wxTextCtrl* _txtBelong;
		wxStaticText* m_staticText3;
		wxTextCtrl* _txtName;
		wxStaticText* m_staticText2;
		wxFilePickerCtrl* _fpExeFile;
		wxTextCtrl* _txtParam;
		wxButton* _btnAddParam;
		wxButton* _btnDelParam;
		wxListBox* _lstParam;
		wxStdDialogButtonSizer* _sdbSz;
		wxButton* _sdbSzOK;
		wxButton* _sdbSzCancel;

	public:

		StandaloneToolDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Add Standalone Application Tool"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 677,412 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~StandaloneToolDialogBase();

};

