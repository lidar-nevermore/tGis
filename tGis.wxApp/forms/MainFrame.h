#pragma once

#include <wx/wx.h>
#include "wx/aui/aui.h"

#include "tGis_Core.h"
#include "tGis_wxGui.h"

using namespace tGis::Gui;

#include "MainFrameBase.h"


class MainFrame : public MainFrameBase
{
	friend class tGisWxApp;
public:
	MainFrame();
    ~MainFrame();

private:
	Map _map;
	wxGLMapWidget *_mapWidget;
	
	MapPanTool _mapPanTool;
	MapZoomTool _mapZoomTool;
	RectZoomTool _rectZoomTool;

	wxDataSourceWidget* _dataSourceWidget;

	wxLayerWidget* _layerWidget;

	wxToolWidget* _toolWidget;

	wxGLEagleEyeWidget* _eagleEyeWidget;
	VirtualDataSource _outerDtContainer;

private:
	wxAuiManager    _mgr;

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
	void OnDatasetActivated(IDataset* dt);
	void OnLayerSelChanged(IMapPtr, ILayerPtr, size_t);
	void OnMapScaleChanged(GeoViewPort*);
	void OnMapCenterChanged(GeoViewPort*);
	void OnMouseMove(wxGLMapWidget*, wxMouseEvent*);
	void OnDataSelChanged(IDataSource*, IDataset*);
	void OnMapToolChanged(IMapWidget*, IMapTool*);

private:
	void OnSize(wxSizeEvent& event);
	void OnExit(wxCommandEvent& WXUNUSED(event));
	void OnLayerWidgetActivated(wxAuiManagerEvent& event);
	void OnDataSourceWidgetActivated(wxAuiManagerEvent& event);
	void OnOpenRasterDataset(wxCommandEvent& event);
	void _toolPan_Clicked(wxCommandEvent& event);
	void _toolZoomFree_Clicked(wxCommandEvent& event);
	void _toolZoomIn_Clicked(wxCommandEvent& event);
	void _toolZoomOut_Clicked(wxCommandEvent& event);
	void _toolZoomRect_Clicked(wxCommandEvent& event);
	void _toolEntire_Clicked(wxCommandEvent& event);
	void _toolZoomLayer_Clicked(wxCommandEvent& event);
	void _toolZoomOriginal_Clicked(wxCommandEvent& event);
	void _toolGrid_Clicked(wxCommandEvent& event);
	void _toolMapSpatialRef_Clicked(wxCommandEvent& event);
	
	DECLARE_EVENT_TABLE();
};
