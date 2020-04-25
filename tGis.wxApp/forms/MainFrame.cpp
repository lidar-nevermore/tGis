#include "MainFrame.h"
#include <wx/artprov.h>
#include <wx/progdlg.h>

#include "tGis_wxAppCfg.h"

MainFrame::MainFrame()
    : MainFrameBase(NULL,wxID_ANY,wxT("tGis Desktop"))
{
	wxIcon icon(wxString(TGisApplication::INSTANCE()->GetExeDir()) + wxString("/wxApp_res/icon.png"), wxBITMAP_TYPE_PNG);
	SetIcon(icon);

	_toolZoomIn = _toolBar->AddTool(wxID_ANY, wxT("Zoom In"), _TOOL_PNG("MapTool", "ZoomIn"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolZoomOut = _toolBar->AddTool(wxID_ANY, wxT("Zoom In"), _TOOL_PNG("MapTool", "ZoomOut"), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	_toolPan = _toolBar->AddTool(wxID_ANY, wxT("Pan"), _TOOL_PNG("MapTool","Pan"), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL);
	_toolZoomFree = _toolBar->AddTool(wxID_ANY, wxT("Zoom Free"), _TOOL_PNG("MapTool", "ZoomFree"), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL);
	_toolZoomRect = _toolBar->AddTool(wxID_ANY, wxT("Zoom In"), _TOOL_PNG("MapTool", "ZoomRect"), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL);
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
	_mapWidget->AddMapTool(&_rectZoomTool);
	_toolBar->ToggleTool(_toolPan->GetId(), true);
	_toolBar->ToggleTool(_toolZoomFree->GetId(), true);
	_rectZoomTool.SetEnabled(false);

	_mgr.AddPane(_mapWidget, wxAuiPaneInfo().
		Name(wxT("map")).Caption(wxT("map")).
		Center().Layer(0).Position(0).
		CloseButton(false).MaximizeButton(true));

	_dataSourceWidget = new wxDataSourceWidget(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize);

	_dataSourceWidget->AfterDatasetActivatedEvent.Add<MainFrame>(this, &MainFrame::OnDatasetActivated);

	_mgr.AddPane(_dataSourceWidget, wxAuiPaneInfo().
		Name(wxT("DataSource")).Caption(wxT("DataSource")).
		Left().Layer(0).Position(0).
		MinSize(wxSize(270, 150)).
		CloseButton(false).MaximizeButton(false));

	_layerWidget = new wxLayerWidget(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize);
	_layerWidget->SetMap(&_map, _mapWidget);
	_toolBar->EnableTool(_toolZoomLayer->GetId(), false);
	_toolBar->EnableTool(_toolZoomOriginal->GetId(), false);
	_layerWidget->LayerSelChangedEvent.Add(this, &MainFrame::OnLayerSelChanged);

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

	_eagleEyeWidget = new wxGLEagleEyeWidget(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize);

	_eagleEyeWidget->SetMapWidget(_mapWidget);

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

	Bind(wxEVT_TOOL, &MainFrame::_toolPan_Clicked, this, _toolPan->GetId());
	Bind(wxEVT_TOOL, &MainFrame::_toolZoomFree_Clicked, this, _toolZoomFree->GetId());
	Bind(wxEVT_TOOL, &MainFrame::_toolZoomIn_Clicked, this, _toolZoomIn->GetId());
	Bind(wxEVT_TOOL, &MainFrame::_toolZoomOut_Clicked, this, _toolZoomOut->GetId());
	Bind(wxEVT_TOOL, &MainFrame::_toolZoomRect_Clicked, this, _toolZoomRect->GetId());
	Bind(wxEVT_TOOL, &MainFrame::_toolEntire_Clicked, this, _toolEntire->GetId());
	Bind(wxEVT_TOOL, &MainFrame::_toolZoomLayer_Clicked, this, _toolZoomLayer->GetId());
	Bind(wxEVT_TOOL, &MainFrame::_toolZoomOriginal_Clicked, this, _toolZoomOriginal->GetId());
	Bind(wxEVT_TOOL, &MainFrame::_toolGrid_Clicked, this, _toolGrid->GetId());
	Bind(wxEVT_TOOL, &MainFrame::_toolMapSpatialRef_Clicked, this, _toolMapSpatialRef->GetId());

}


MainFrame::~MainFrame()
{
	Unbind(wxEVT_TOOL, &MainFrame::_toolPan_Clicked, this, _toolPan->GetId());
	Unbind(wxEVT_TOOL, &MainFrame::_toolZoomFree_Clicked, this, _toolZoomFree->GetId());
	Unbind(wxEVT_TOOL, &MainFrame::_toolZoomIn_Clicked, this, _toolZoomIn->GetId());
	Unbind(wxEVT_TOOL, &MainFrame::_toolZoomOut_Clicked, this, _toolZoomOut->GetId());
	Unbind(wxEVT_TOOL, &MainFrame::_toolZoomRect_Clicked, this, _toolZoomRect->GetId());
	Unbind(wxEVT_TOOL, &MainFrame::_toolEntire_Clicked, this, _toolEntire->GetId());
	Unbind(wxEVT_TOOL, &MainFrame::_toolZoomLayer_Clicked, this, _toolZoomLayer->GetId());
	Unbind(wxEVT_TOOL, &MainFrame::_toolZoomOriginal_Clicked, this, _toolZoomOriginal->GetId());
	Unbind(wxEVT_TOOL, &MainFrame::_toolGrid_Clicked, this, _toolGrid->GetId());
	Unbind(wxEVT_TOOL, &MainFrame::_toolMapSpatialRef_Clicked, this, _toolMapSpatialRef->GetId());

	_eagleEyeWidget->SetMapWidget(nullptr);
	_mapWidget->SetMap(nullptr);
	_layerWidget->SetMap(nullptr, nullptr);
	_mgr.UnInit();
}

void MainFrame::OnDatasetActivated(IDataset * dt)
{
	if (dt->IsOpened() == false)
		return;

	if (dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
	{
		MyGDALRasterDataset* dataset = (MyGDALRasterDataset*)dt;
		if (NeedBuildPyramids(dataset->GetGDALDataset()))
		{
			wxProgressDialog* prgDlg = new wxProgressDialog(wxT("����ͼ�������..."), wxT("����ͼ�������..."), 100, this, wxPD_AUTO_HIDE|wxPD_APP_MODAL);
			ProgressEvent pyrPrgEvent;
			FunctorEventHandler<const Progress&> handler(
				[prgDlg](const Progress& prg)->void
			{
				prgDlg->Update(prg.StepValue, prg.Message);
			});
			pyrPrgEvent.Add(&handler);

			BuildPyramids(dataset->GetGDALDataset(), &pyrPrgEvent);

			delete prgDlg;
		}
	}

	Layer* layer = new Layer(dt);
	wxLayerPropertyDialog lpDlg(layer);
	if (lpDlg.ShowModal() != wxID_OK)
	{
		delete layer;
		return;
	}

	_map.AddLayer(layer);
}

void MainFrame::OnLayerSelChanged(IMapPtr, ILayerPtr layer, size_t)
{
	if (layer == nullptr)
	{
		_toolBar->EnableTool(_toolZoomLayer->GetId(), false);
		_toolBar->EnableTool(_toolZoomOriginal->GetId(), false);
	}
	else
	{
		_toolBar->EnableTool(_toolZoomLayer->GetId(), true);
		_toolBar->EnableTool(_toolZoomOriginal->GetId(), true);
	}
}

void MainFrame::OnSize(wxSizeEvent & event)
{

}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MainFrame::_toolPan_Clicked(wxCommandEvent & event)
{
	if (event.IsChecked())
	{
		_mapPanTool.SetEnabled(true);
		_rectZoomTool.SetEnabled(false);
		//_mapWidget->AddMapTool(&_mapPanTool);
		//_mapWidget->RemoveMapTool(&_rectZoomTool);
		_toolBar->ToggleTool(_toolZoomRect->GetId(), false);
	}
	else
	{
		_mapPanTool.SetEnabled(false);
		//_mapWidget->RemoveMapTool(&_mapPanTool);
	}
}

void MainFrame::_toolZoomFree_Clicked(wxCommandEvent & event)
{
	if (event.IsChecked())
		_mapZoomTool.SetEnabled(true);
	else
		_mapZoomTool.SetEnabled(false);
}

void MainFrame::_toolZoomIn_Clicked(wxCommandEvent & event)
{
	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	double scale;
	viewPort->GetViewScale(&scale);
	scale *= 0.96; //360/375
	viewPort->SetViewScale(scale);
	_mapWidget->PresentMap();
}

void MainFrame::_toolZoomOut_Clicked(wxCommandEvent & event)
{
	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	double scale;
	viewPort->GetViewScale(&scale);
	scale *= 1.042; //375/360
	viewPort->SetViewScale(scale);
	_mapWidget->RepaintMap();
}

void MainFrame::_toolZoomRect_Clicked(wxCommandEvent & event)
{
	if (event.IsChecked())
	{
		_mapPanTool.SetEnabled(false);
		_rectZoomTool.SetEnabled(true);
		_toolBar->ToggleTool(_toolPan->GetId(), false);
	}
	else
	{
		_rectZoomTool.SetEnabled(false);
	}
}

void MainFrame::_toolEntire_Clicked(wxCommandEvent & event)
{
	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	viewPort->IncludeEnvelope(_map.GetEnvelope());
	_mapWidget->RepaintMap();
}

void MainFrame::_toolZoomLayer_Clicked(wxCommandEvent & event)
{
	ILayer* layer = _layerWidget->GetSelLayer();
	if (layer != nullptr)
	{
		GeoViewPort* viewPort = _mapWidget->GetViewPort();
		viewPort->IncludeEnvelope(layer->GetEnvelope());
		_mapWidget->RepaintMap();
	}
}

void MainFrame::_toolZoomOriginal_Clicked(wxCommandEvent & event)
{
	ILayer* layer = _layerWidget->GetSelLayer();
	if (layer != nullptr)
	{
		IDataset* dataset = layer->GetDataset();
		if (dataset->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		{
			MyGDALRasterDataset* gdalDataset = (MyGDALRasterDataset*)dataset;
			double res = gdalDataset->GetGeoTransform()[1];
			GeoViewPort* viewPort = _mapWidget->GetViewPort();
			viewPort->SetViewScale(res);
			_mapWidget->RepaintMap();
		}
	}
}

void MainFrame::_toolGrid_Clicked(wxCommandEvent & event)
{
}

void MainFrame::_toolMapSpatialRef_Clicked(wxCommandEvent & event)
{
}

BEGIN_EVENT_TABLE(MainFrame, MainFrameBase)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_SIZE(MainFrame::OnSize)
END_EVENT_TABLE()