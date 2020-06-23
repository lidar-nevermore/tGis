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
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/slider.h>
#include <wx/grid.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/gbsizer.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PalettedLayerRenderCtrlBase
///////////////////////////////////////////////////////////////////////////////
class PalettedLayerRenderCtrlBase : public wxPanel
{
	private:

	protected:
		wxStaticText* m_staticText2;
		wxChoice* _choiceBand;
		wxStaticText* m_staticText13;
		wxTextCtrl* _txtNoData;
		wxCheckBox* _chkNoDataEQ;
		wxCheckBox* _chkNoDataGT;
		wxCheckBox* _chkNoDataLT;
		wxStaticText* m_staticText51;
		wxStaticText* _lblOpacityValue;
		wxSlider* _sldOpacity;
		wxGrid* _grdPallete;
		wxButton* _btnRandColor;
		wxButton* _btnGradColor;
		wxButton* _btnAdd;
		wxButton* _btnDelete;
		wxButton* _btnClear;

	public:

		PalettedLayerRenderCtrlBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 418,496 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~PalettedLayerRenderCtrlBase();

};

