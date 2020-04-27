#pragma once

#ifndef __wxSymbolPickerWidget_H__
#define __wxSymbolPickerWidget_H__


#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/treectrl.h>
#include <wx/clrpicker.h>
#include <wx/textctrl.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/gbsizer.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class wxSymbolLibraryWidget;
class wxSymbolPickerWidgetImpl;

class TGIS_GUI_API wxSymbolPickerWidget : public wxPanel
{
public:
	wxSymbolPickerWidget(SymbolLibraryType defaultSymLibType, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(430, 438), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxSymbolPickerWidget();

public:
	void AddSymbolLibrary(ISymbolLibrary* symLib);
	ISymbolLibrary* GetSelSymbolLibrary();
	ISymbol* GetSymbol();
	void SetSymbol(ISymbol* sym);

private:
	ISymbol* _selSym;
	ISymbolLibrary* _selSymLib;
	wxSymbolPickerWidgetImpl* _impl_;

private:
	wxStaticText* m_staticText2;
	wxChoice* _choiceSymLib;
	wxSymbolLibraryWidget* _symLibWidget;
	wxPanel* _markerSymPanel;
	wxStaticText* m_staticText2111;
	wxColourPickerCtrl* _cpMarker;
	wxStaticText* m_staticText311;
	wxTextCtrl* _txtMarkerRed;
	wxStaticText* m_staticText411;
	wxTextCtrl* _txtMarkerGreen;
	wxStaticText* m_staticText511;
	wxTextCtrl* _txtMarkerBlue;
	wxStaticText* m_staticText611;
	wxSlider* _sldMarkerAlpha;
	wxStaticText* m_staticText35;
	wxTextCtrl* _txtMarkerAlpha;
	wxStaticText* m_staticText171;
	wxTextCtrl* _txtMarkerWidth;
	wxStaticText* m_staticText30;
	wxTextCtrl* _txtMarkerHeight;
	wxStaticText* m_staticText312;
	wxTextCtrl* _txtMarkerLineWidth;
	wxStaticText* m_staticText32;
	wxSpinCtrl* _spMarkerXOff;
	wxStaticText* m_staticText33;
	wxSpinCtrl* _spMarkerYOff;
	wxPanel* _lineSymPanel;
	wxStaticText* m_staticText21111;
	wxColourPickerCtrl* _cpLine;
	wxStaticText* m_staticText3111;
	wxTextCtrl* _txtLineRed;
	wxStaticText* m_staticText4111;
	wxTextCtrl* _txtLineGreen;
	wxStaticText* m_staticText5111;
	wxTextCtrl* _txtLineBlue;
	wxStaticText* m_staticText6111;
	wxSlider* _sldLineAlpha;
	wxStaticText* m_staticText351;
	wxTextCtrl* _txtLineAlpha;
	wxStaticText* m_staticText1711;
	wxTextCtrl* _txtLineWidth;
	wxPanel* _fillSymPanel;
	wxStaticText* m_staticText21112;
	wxColourPickerCtrl* _cpFill;
	wxStaticText* m_staticText3112;
	wxTextCtrl* _txtFillRed;
	wxStaticText* m_staticText4112;
	wxTextCtrl* _txtFillGreen;
	wxStaticText* m_staticText5112;
	wxTextCtrl* _txtFillBlue;
	wxStaticText* m_staticText6112;
	wxSlider* _sldFillAlpha;
	wxStaticText* m_staticText352;
	wxTextCtrl* _txtFillAlpha;

private:
	void _choiceSymLib_selected(wxCommandEvent& event);
	void _sldAlpha_scroll(wxCommandEvent& event);
	void _colourPicker_changed(wxColourPickerEvent& event);

};

END_NAME_SPACE(tGis, Gui)

#endif

