#pragma once

#ifndef __wxGradientColorEditorWidget_H__
#define __wxGradientColorEditorWidget_H__

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
#include <wx/dataview.h>

#include "tGis_wxGuiCfg.h"


using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Core)

class GradientColor;

END_NAME_SPACE(tGis, Core)

BEGIN_NAME_SPACE(tGis, Gui)

class wxGradientColorWidget;
class wxGradientColorEditorWidgetImpl;

class TGIS_GUI_API wxGradientColorEditorWidget : public wxPanel
{
public:
	wxGradientColorEditorWidget(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 334), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxGradientColorEditorWidget();

public:
	void SetGradientColor(GradientColor* color);
	GradientColor* GetGradientColor();

protected:
	wxButton* _btnAdd;
	wxButton* _btnEdit;
	wxButton* _btnDelete;
	wxDataViewListCtrl* _dvlKeyColor;
	wxGradientColorWidget* _wxGradientColor;

private:
	wxGradientColorEditorWidgetImpl* _impl_;
	GradientColor* _grdColor;

private:
	void _btnAdd_clicked(wxCommandEvent& event);
	void _btnEdit_clicked(wxCommandEvent& event);
	void _btnDelete_clicked(wxCommandEvent& event);
};


END_NAME_SPACE(tGis, Gui)

#endif

