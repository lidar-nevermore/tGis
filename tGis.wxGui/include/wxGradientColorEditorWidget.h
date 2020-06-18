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
#include <wx/clrpicker.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/grid.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class wxGradientColorEditorWidget
///////////////////////////////////////////////////////////////////////////////
class wxGradientColorEditorWidget : public wxPanel
{
	private:

	protected:
		wxStaticText* m_staticText1;
		wxColourPickerCtrl* _cpAdd;
		wxButton* _btnDelete;
		wxGrid* _gridKeyColor;
		wxPanel* _wxGradientColor;

	public:

		wxGradientColorEditorWidget( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,334 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~wxGradientColorEditorWidget();

};

