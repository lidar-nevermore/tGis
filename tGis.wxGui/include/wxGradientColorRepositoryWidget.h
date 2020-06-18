#pragma once

#ifndef __wxGradientColorRepositoryWidget_H__
#define __wxGradientColorRepositoryWidget_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/scrolbar.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include "tGis_wxGuiCfg.h"

using namespace tGis::Core;


BEGIN_NAME_SPACE(tGis, Gui)

class wxGLMapWidget;
class wxGradientColorRepositoryWidgetImpl;

class TGIS_GUI_API wxGradientColorRepositoryWidget : public wxPanel
{
public:
	wxGradientColorRepositoryWidget(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(581, 376), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~wxGradientColorRepositoryWidget();

public:
	GradientColor* GetSelGradientColor();
	void SetSelGradientColor(GradientColor* color);

private:
	wxGLMapWidget* _gcrWidget;
	wxScrollBar* _scrollBar;
	wxGradientColorRepositoryWidgetImpl* _impl_;

private:
	void _scrollBar_scroll(wxScrollEvent& event);

	wxDECLARE_EVENT_TABLE();
};

END_NAME_SPACE(tGis, Gui)

#endif

