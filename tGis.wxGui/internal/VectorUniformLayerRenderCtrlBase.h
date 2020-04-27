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
#include <wx/slider.h>
#include <wx/notebook.h>
#include <wx/gbsizer.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class VectorUniformLayerRenderCtrlBase
///////////////////////////////////////////////////////////////////////////////
class VectorUniformLayerRenderCtrlBase : public wxPanel
{
	private:

	protected:
		wxStaticText* m_staticText2;
		wxChoice* _choiceLayer;
		wxStaticText* m_staticText51;
		wxStaticText* _lblOpacityValue;
		wxSlider* _sldOpacity;
		wxNotebook* _tabSymPicker;

	public:

		VectorUniformLayerRenderCtrlBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 430,438 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~VectorUniformLayerRenderCtrlBase();

};

