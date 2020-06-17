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
#include <wx/sizer.h>
#include <wx/gbsizer.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class RgbLayerRenderCtrlBase
///////////////////////////////////////////////////////////////////////////////
class RgbLayerRenderCtrlBase : public wxPanel
{
	private:

	protected:
		wxStaticText* m_staticText2;
		wxChoice* _choiceRBand;
		wxStaticText* m_staticText5;
		wxTextCtrl* _txtRMin;
		wxStaticText* m_staticText6;
		wxTextCtrl* _txtRMax;
		wxStaticText* m_staticText13;
		wxTextCtrl* _txtRNoData;
		wxCheckBox* _chkRNoDataEQ;
		wxCheckBox* _chkRNoDataGT;
		wxCheckBox* _chkRNoDataLT;
		wxStaticText* m_staticText7;
		wxChoice* _choiceGBand;
		wxStaticText* m_staticText8;
		wxTextCtrl* _txtGMin;
		wxStaticText* m_staticText9;
		wxTextCtrl* _txtGMax;
		wxStaticText* m_staticText111;
		wxTextCtrl* _txtGNoData;
		wxCheckBox* _chkGNoDataEQ;
		wxCheckBox* _chkGNoDataGT;
		wxCheckBox* _chkGNoDataLT;
		wxStaticText* m_staticText10;
		wxChoice* _choiceBBand;
		wxStaticText* m_staticText11;
		wxTextCtrl* _txtBMin;
		wxStaticText* m_staticText12;
		wxTextCtrl* _txtBMax;
		wxStaticText* m_staticText121;
		wxTextCtrl* _txtBNoData;
		wxCheckBox* _chkBNoDataEQ;
		wxCheckBox* _chkBNoDataGT;
		wxCheckBox* _chkBNoDataLT;
		wxStaticText* m_staticText131;
		wxStaticText* _lblOpacityValue;
		wxSlider* _sldOpacity;
		wxCheckBox* _chkApproximate ;
		wxButton* _btnComputeStatistics;

	public:

		RgbLayerRenderCtrlBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 430,438 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~RgbLayerRenderCtrlBase();

};

