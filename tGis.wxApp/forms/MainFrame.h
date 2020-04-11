#pragma once

#include <wx/wx.h>
#include "wx/aui/aui.h"

#include "tGis_Core.h"
#include "tGis_wxGui.h"

using namespace tGis::Gui;

#include "MainFrameBase.h"


class MainFrame : public MainFrameBase
{
public:
	MainFrame();
    ~MainFrame();

private:
    wxAuiManager    _mgr;
	wxGLMapWidget *_mapWidget;
	Map _map;
	MapPanTool _mapPanTool;
	MapZoomTool _mapZoomTool;
	RectZoomTool _rectZoomTool;

private:
	void OnExit(wxCommandEvent& WXUNUSED(event));
	void _toolTestOnToolClicked(wxCommandEvent& event);

	DECLARE_EVENT_TABLE();
};
