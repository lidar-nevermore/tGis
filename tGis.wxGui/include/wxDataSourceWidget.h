#pragma once

#ifndef __WX_DATASOURCEWIDGET_H__
#define __WX_DATASOURCEWIDGET_H__

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

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API wxDataSourceWidget : public wxPanel
{
public:
	wxDataSourceWidget(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxDataSourceWidget();

protected:
	wxToolBar* _toolBar;
	wxTreeCtrl* _treeCtrl;

};

END_NAME_SPACE(tGis, Gui)

#endif
