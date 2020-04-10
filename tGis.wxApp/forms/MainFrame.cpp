#include "MainFrame.h"
#include <wx/artprov.h>


MainFrame::MainFrame()
    : MainFrameBase(NULL,wxID_ANY,wxT("tGis Desktop"))
{
    _mgr.SetManagedWindow(this);

	_mapWidget = new wxGLMapWidget(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize, wxNO_BORDER);
	_mapWidget->SetMap(&_map);

	_mgr.AddPane(_mapWidget, wxAuiPaneInfo().
		Name(wxT("map")).Caption(wxT("map")).
		Center().Layer(0).Position(0).
		CloseButton(false).MaximizeButton(true));

	wxPanel* pDataSource = new wxPanel(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize);

	_mgr.AddPane(pDataSource, wxAuiPaneInfo().
		Name(wxT("DataSource")).Caption(wxT("DataSource")).
		Left().Layer(0).Position(0).
		MinSize(wxSize(270, 150)).
		CloseButton(false).MaximizeButton(false));

	wxPanel* pLayer = new wxPanel(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize);

	_mgr.AddPane(pLayer, wxAuiPaneInfo().
		Name(wxT("Layer")).Caption(wxT("Layer")).
		Left().Layer(0).Position(1).
		MinSize(wxSize(270, 150)).
		CloseButton(false).MaximizeButton(false));

	wxPanel* pTool = new wxPanel(this, wxID_ANY,
		wxPoint(0, 0), wxSize(270, 350));

	_mgr.AddPane(pTool, wxAuiPaneInfo().
		Name(wxT("Tool")).Caption(wxT("Tool")).
		Right().Layer(0).Position(0).
		MinSize(wxSize(270, 100)).
		CloseButton(false).MaximizeButton(false));

	wxPanel* pEagleEye = new wxPanel(this, wxID_ANY,
		wxPoint(0, 0), wxDefaultSize);

	_mgr.AddPane(pEagleEye, wxAuiPaneInfo().
		Name(wxT("EagleEye")).Caption(wxT("EagleEye")).
		Right().Layer(0).Position(1).
		MinSize(wxSize(270, 270)).
		CloseButton(false).MaximizeButton(false));

    _mgr.Update();

	SetPosition(wxPoint(100, 100));
	SetSize(wxSize(900, 700));
}


MainFrame::~MainFrame()
{
	_mgr.UnInit();
}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MainFrame::_toolTestOnToolClicked(wxCommandEvent & event)
{
	MyGDALFileRasterDataset* _dataset = new MyGDALFileRasterDataset(nullptr, "E:\\SegTest\\seg_result_sub.tif", GA_ReadOnly, false);
	Layer* layer = new Layer(_dataset);
	int bandCount = ((MyGDALFileRasterDataset*)_dataset)->GetGDALDataset()->GetRasterCount();
	ILayerRender* layerRender = nullptr;
	if (bandCount < 3)
		layerRender = new RasterGrayScaleLayerRender(layer, 1);
	else
		layerRender = new RasterRgbLayerRender(layer, 1, 2, 3);
	_map.AddLayer(layer);
}


BEGIN_EVENT_TABLE(MainFrame, MainFrameBase)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
END_EVENT_TABLE()
