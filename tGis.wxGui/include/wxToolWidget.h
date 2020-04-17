#pragma once

#ifndef __WX_TOOLWIDGET_H__
#define __WX_TOOLWIDGET_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API wxToolWidget : public wxPanel
{
private:

protected:
	wxToolBar* _toolBar;
	wxToolBarToolBase* m_tool1;
	wxToolBarToolBase* m_tool2;
	wxTreeCtrl* _treeCtrl;

public:

	wxToolWidget(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxToolWidget();

};

END_NAME_SPACE(tGis, Gui)

#endif
