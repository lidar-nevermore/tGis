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
	wxToolBarToolBase* _toolPan;
	wxToolBarToolBase* _toolZoomFree;
	wxToolBarToolBase* _toolZoomIn;
	wxToolBarToolBase* _toolZoomOut;
	wxToolBarToolBase* _toolZoomRect;
	wxToolBarToolBase* _toolEntire;
	wxToolBarToolBase* _toolZoomLayer;
	wxToolBarToolBase* _toolZoomOriginal;
	wxToolBarToolBase* _toolGrid;
	wxToolBarToolBase* _toolMapSpatialRef;

private:
    wxAuiManager    _mgr;
	wxGLMapWidget *_mapWidget;
	Map _map;
	MapPanTool _mapPanTool;
	MapZoomTool _mapZoomTool;
	RectZoomTool _rectZoomTool;

	wxPanel* _toolWidget;

	wxPanel* _eagleEyeWidget;

private:
	void OnSize(wxSizeEvent& event);
	void OnExit(wxCommandEvent& WXUNUSED(event));
	void _toolTestOnToolClicked(wxCommandEvent& event);

	DECLARE_EVENT_TABLE();
};
