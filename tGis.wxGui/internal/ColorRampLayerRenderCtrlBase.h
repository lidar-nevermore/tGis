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
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/gbsizer.h>

///////////////////////////////////////////////////////////////////////////

#include "tGis_wxGuiCfg.h"

BEGIN_NAME_SPACE(tGis, Gui)
class wxGradientColorWidget;
END_NAME_SPACE(tGis, Gui)
///////////////////////////////////////////////////////////////////////////////
/// Class ColorRampLayerRenderCtrlBase
///////////////////////////////////////////////////////////////////////////////
class ColorRampLayerRenderCtrlBase : public wxPanel
{
	private:

	protected:
		wxStaticText* m_staticText2;
		wxChoice* _choiceBand;
		wxStaticText* m_staticText5;
		wxTextCtrl* _txtMin;
		wxStaticText* m_staticText6;
		wxTextCtrl* _txtMax;
		wxStaticText* m_staticText13;
		wxTextCtrl* _txtNoData;
		wxCheckBox* _chkNoDataEQ;
		wxCheckBox* _chkNoDataGT;
		wxCheckBox* _chkNoDataLT;
		wxStaticText* m_staticText51;
		wxStaticText* _lblOpacityValue;
		wxSlider* _sldOpacity;
		wxStaticText* m_staticText7;
		wxButton* _btnSelGradColor;
		tGis::Gui::wxGradientColorWidget* _wxGradientColor;
		wxCheckBox* _chkApproximate;
		wxButton* _btnComputeStatistics;

	public:

		ColorRampLayerRenderCtrlBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 430,438 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~ColorRampLayerRenderCtrlBase();

};

