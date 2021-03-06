#pragma once

#ifndef __WX_LAYERPROPERTYDIALOG_H__
#define __WX_LAYERPROPERTYDIALOG_H__

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
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>

#include "tGis_wxGuiCfg.h"


using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class wxLayerRenderWidget;
class wxDatasetInfoWidget;

class TGIS_GUI_API wxLayerPropertyDialog : public wxDialog
{
public:
	wxLayerPropertyDialog(ILayer* layer, wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxT("图层属性"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(670, 685), long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	~wxLayerPropertyDialog();

private:
	ILayer* _layer;

protected:
	wxToolBar* _toolBar;
	wxToolBarToolBase* _toolLayerStyle;
	wxToolBarToolBase* _toolDatasetInfo;
	wxLayerRenderWidget* _pnlLayerRender;
	wxDatasetInfoWidget* _txtDatasetInfo;
	wxStdDialogButtonSizer* _sdbSz;
	wxButton* _sdbSzOK;
	wxButton* _sdbSzCancel;

private:
	void _tool_Clicked(wxCommandEvent& event);
	void _sdbSzOK_Clicked(wxCommandEvent& event);
};

END_NAME_SPACE(tGis, Gui)

#endif

