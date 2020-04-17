#include "MainFrame.h"
#include <wx/artprov.h>

#include "tGis_wxAppCfg.h"

MainFrame::MainFrame()
    : MainFrameBase(NULL,wxID_ANY,wxT("tGis Desktop"))
{
	wxIcon icon(wxString(TGisApplication::INSTANCE()->GetExeDir()) + wxString("/wxApp_res/icon.png"), wxBITMAP_TYPE_PNG);
	SetIcon(icon);

	_toolPan = _toolBar->AddTool(wxID_ANY, wxT("Pan"), _TOOL_PNG("MapTool","Pan"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolZoomFree = _toolBar->AddTool(wxID_ANY, wxT("Zoom Free"), _TOOL_PNG("MapTool", "ZoomFree"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolZoomIn = _toolBar->AddTool(wxID_ANY, wxT("Zoom In"), _TOOL_PNG("MapTool", "ZoomIn"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolZoomOut = _toolBar->AddTool(wxID_ANY, wxT("Zoom In"), _TOOL_PNG("MapTool", "ZoomOut"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolZoomRect = _toolBar->AddTool(wxID_ANY, wxT("Zoom In"), _TOOL_PNG("MapTool", "ZoomRect"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolEntire = _toolBar->AddTool(wxID_ANY, wxT("Entire Map"), _TOOL_PNG("MapTool", "Entire"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolZoomLayer = _toolBar->AddTool(wxID_ANY, wxT("Zoom to layer"), _TOOL_PNG("MapTool", "ZoomLayer"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolZoomOriginal = _toolBar->AddTool(wxID_ANY, wxT("Zoom Original"), _TOOL_PNG("MapTool", "ZoomOriginal"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolGrid = _toolBar->AddTool(wxID_ANY, wxT("Show Grid"), _TOOL_PNG("MapTool", "Grid"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolMapSpatialRef = _toolBar->AddTool(wxID_ANY, wxT("Map SpatialRefrence"), _TOOL_PNG("MapTool", "MapSpatialRef"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	_toolBar->Realize();

    _mgr.SetManagedWindow(this);

	_mapWidget = new wxGLMapWidget(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize, wxNO_BORDER);
	_mapWidget->SetMap(&_map);
	_mapWidget->AddMapTool(&_mapPanTool);
	_mapWidget->AddMapTool(&_mapZoomTool);	

	_mgr.AddPane(_mapWidget, wxAuiPaneInfo().
		Name(wxT("map")).Caption(wxT("map")).
		Center().Layer(0).Position(0).
		CloseButton(false).MaximizeButton(true));

	_dataSourceWidget = new wxDataSourceWidget(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize);

	_dataSourceWidget->AfterDatasetActivatedEvent.Add<MainFrame>(this, &MainFrame::OnDatasetOpen);

	_mgr.AddPane(_dataSourceWidget, wxAuiPaneInfo().
		Name(wxT("DataSource")).Caption(wxT("DataSource")).
		Left().Layer(0).Position(0).
		MinSize(wxSize(270, 150)).
		CloseButton(false).MaximizeButton(false));

	_layerWidget = new wxLayerWidget(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize);
	_layerWidget->SetMap(&_map, _mapWidget);

	_mgr.AddPane(_layerWidget, wxAuiPaneInfo().
		Name(wxT("Layer")).Caption(wxT("Layer")).
		Left().Layer(0).Position(1).
		MinSize(wxSize(270, 150)).
		CloseButton(false).MaximizeButton(false));

	_toolWidget = new wxToolWidget(this, wxID_ANY,
		wxPoint(0, 0), wxSize(270, 350));

	_mgr.AddPane(_toolWidget, wxAuiPaneInfo().
		Name(wxT("Tool")).Caption(wxT("Tool")).
		Right().Layer(0).Position(0).
		MinSize(wxSize(270, 100)).
		CloseButton(false).MaximizeButton(false));

	_eagleEyeWidget = new wxPanel(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize);

	_mgr.AddPane(_eagleEyeWidget, wxAuiPaneInfo().
		Name(wxT("EagleEye")).Caption(wxT("EagleEye")).
		Right().Layer(0).Position(1).
		MaxSize(5000,355).
		MinSize(wxSize(270, 270)).
		CloseButton(false).MaximizeButton(false));

    _mgr.Update();

	SetPosition(wxPoint(100, 100));
	SetSize(wxSize(1080, 700));

	wxAuiPaneInfo pane = _mgr.GetPane(_eagleEyeWidget);
	pane.dock_proportion = 20;
	_mgr.GetPane(_eagleEyeWidget).SafeSet(pane);
	_mgr.Update();
}


MainFrame::~MainFrame()
{
	_mapWidget->SetMap(nullptr);
	_layerWidget->SetMap(nullptr, nullptr);
	_mgr.UnInit();
}

void MainFrame::OnDatasetOpen(IDataset * dt)
{
	if (dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
	{
		MyGDALRasterDataset* dataset = (MyGDALRasterDataset*)dt;
		Layer* layer = new Layer(dataset);
		int bandCount = dataset->GetGDALDataset()->GetRasterCount();
		ILayerRender* layerRender = nullptr;
		if (bandCount < 3)
			layerRender = new RasterGrayScaleLayerRender(layer, 1);
		else
			layerRender = new RasterRgbLayerRender(layer, 1, 2, 3);
		_map.AddLayer(layer);
	}
}

void MainFrame::OnSize(wxSizeEvent & event)
{

}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

BEGIN_EVENT_TABLE(MainFrame, MainFrameBase)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_SIZE(MainFrame::OnSize)
END_EVENT_TABLE()
