#pragma once

#ifndef __wxSymbolLibraryWidget_H__
#define __wxSymbolLibraryWidget_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/scrolbar.h>
#include <wx/sizer.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;


BEGIN_NAME_SPACE(tGis, Gui)

class wxGLMapWidget;
class wxSymbolLibraryWidgetImpl;

class TGIS_GUI_API wxSymbolLibraryWidget : public wxPanel
{
public:
	wxSymbolLibraryWidget(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxSymbolLibraryWidget();

public:
	void SetSymbolLibrary(ISymbolLibrary* symLib);
	int GetSelSymbolId();
	void SetSelSymbolId(int symId);

private:
	wxGLMapWidget* _symWidget;
	wxScrollBar* _scrollBar;
	Map* _map;
	SymbolLibraryRender* _render;
	wxSymbolLibraryWidgetImpl* _impl_;
};

END_NAME_SPACE(tGis, Gui)

#endif
