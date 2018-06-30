#include "tGisApp.h"
#include "QDataSourceWidget.h"
#include "QOpenedDatasetWidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include "QLayerWidget.h"
#include "Event.h"

using namespace tGis::Core;

tGisApp::tGisApp(QWidget *parent)
	: QMainWindow(parent)
	, _AfterDatasetOpenEventHandler(this, &tGisApp::AfterDatasetOpen)
	, _BeforeDatasetCloseEventHandler(this, &tGisApp::BeforeDatasetClose)
{
	ui.setupUi(this);

	_selectedObjectSampleDataSource = nullptr;
	_selectedRasterLayer = nullptr;
	_selectedDataset = nullptr;
	_selectedDataSource = nullptr;

	ui.dataSourceWidget->AfterDatasetOpenEvent += &_AfterDatasetOpenEventHandler;
	ui.dataSourceWidget->BeforeDatasetCloseEvent += &_BeforeDatasetCloseEventHandler;
	ui.dataSourceWidget->AfterDataSourceConnectEvent.Add(this,&tGisApp::AfterDataSourceConnect);
	ui.dataSourceWidget->BeforeDataSourceDisconnectEvent.Add(this,&tGisApp::BeforeDataSourceDisconnect);

	_map.LayerAddedEvent.Add(this, &tGisApp::LayerAdded);
	_map.LayerClearedEvent.Add(this, &tGisApp::LayerCleared);
	_map.LayerRemovedEvent.Add(this, &tGisApp::LayerRemoved);

	ui.mapWidget->SetMap(&_map);
	ui.layerWidget->SetMap(&_map);

	SetCurrentMap(&_map);
	SetCurrentMapWidget((IMapWidget*)ui.mapWidget);
	ui.mapWidget->AddMapTool(&_mapPanTool);
	ui.mapWidget->AddMapTool(&_mapZoomTool);
	ui.mapWidget->AddMapTool(&_rectZoomTool);
	_rectZoomTool.SetEnabled(false);

	_takeObjectSampleTool.SetEnabled(false);
	ui.mapWidget->AddMapTool(&_takeObjectSampleTool);

	QDesktopWidget * desktop = QApplication::desktop();
	int curMonitor = desktop->screenNumber(this);
	QRect rect = desktop->screenGeometry(curMonitor);
	SetMaxSurfaceSize(rect.width(), rect.height());

	connect(ui.layerWidget, &QLayerWidget::LayerSelectionChanged, this, &tGisApp::on_layerWidget_LayerSelectionChanged);

	ui.zoomFreeAction->setChecked(true);
	ui.panAction->setChecked(true);
	ui.zoomLayerAction->setEnabled(false);
	ui.zoomOriginalAction->setEnabled(false);
	ui.removeLayerAction->setEnabled(false);
	ui.layerVisibleAction->setEnabled(false);
	ui.layerAttributeAction->setEnabled(false);
	ui.takeObjectSampleAction->setEnabled(false);
	ui.showGridAction->setChecked(ui.mapWidget->GetGridVisible());
}

tGisApp::~tGisApp()
{
	ui.mapWidget->SetMap(nullptr);
}

void tGisApp::AfterDatasetOpen(IDataSourceProvider * provider, IDataset * dt)
{
	if (_selectedDataset == dt)
	{
		ui.closeDatasetAction->setEnabled(true);
		ui.datasetInfoAction->setEnabled(true);
	}
}

void tGisApp::BeforeDatasetClose(IDataSourceProvider * provider, IDataset * dt)
{
	_map.RemoveLayer(dt);
	if (_selectedDataset == dt)
	{
		ui.closeDatasetAction->setEnabled(false);
		ui.datasetInfoAction->setEnabled(false);
	}
}

void tGisApp::AfterDataSourceConnect(IDataSourceProvider * provider, IDataSource * ds)
{
	if (_selectedDataSource == ds)
	{
		ui.refreshDataSourceAction->setEnabled(true);
	}
}

void tGisApp::BeforeDataSourceDisconnect(IDataSourceProvider * provider, IDataSource * ds)
{
}

void tGisApp::LayerAdded(IMapPtr, ILayerPtr)
{
	ui.mapWidget->RepaintMap();
}

void tGisApp::LayerRemoved(IMapPtr, ILayerPtr)
{
	ui.mapWidget->RepaintMap();
}

void tGisApp::LayerCleared(IMapPtr)
{
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_zoomInAction_triggered(bool checked)
{
	IGeoSurface* surface = ui.mapWidget->GetGeoSurface();
	double resolution;
	surface->GetViewPort(nullptr, nullptr, nullptr, nullptr, &resolution);
	resolution *= 0.96;
	surface->SetViewResolution(resolution);
	ui.mapWidget->PresentSurface();
}

void tGisApp::on_zoomOutAction_triggered(bool checked)
{
	IGeoSurface* surface = ui.mapWidget->GetGeoSurface();
	double resolution;
	surface->GetViewPort(nullptr, nullptr, nullptr, nullptr, &resolution);
	resolution *= 1.042;
	surface->SetViewResolution(resolution);
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_entireMapAction_triggered(bool checked)
{
	IGeoSurface* surface = ui.mapWidget->GetGeoSurface();
	surface->IncludeEnvelope(_map.GetEnvelope());
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_refreshMapAction_triggered(bool checked)
{
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_zoomFreeAction_toggled(bool checked)
{
	_mapZoomTool.SetEnabled(checked);
}

void tGisApp::on_zoomRectAction_toggled(bool checked)
{
	_rectZoomTool.SetEnabled(checked);
	if (checked)
	{
		ui.panAction->setChecked(false);
		ui.takeObjectSampleAction->setChecked(false);
	}
}

void tGisApp::on_panAction_toggled(bool checked)
{
	_mapPanTool.SetEnabled(checked);
	if (checked)
	{
		ui.zoomRectAction->setChecked(false);
		ui.takeObjectSampleAction->setChecked(false);
	}
}

void tGisApp::on_zoomLayerAction_triggered(bool checked)
{
	ILayer* layer = ui.layerWidget->GetSelectedLayer();
	if (layer != nullptr)
	{
		IGeoSurface* surface = ui.mapWidget->GetGeoSurface();
		surface->IncludeEnvelope(layer->GetEnvelope());
		ui.mapWidget->RepaintMap();
	}
}

void tGisApp::on_zoomOriginalAction_triggered(bool checked)
{
	ILayer* layer = ui.layerWidget->GetSelectedLayer();
	if (layer != nullptr)
	{
		IDataset* dataset = layer->GetDataset();
		if (dataset->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		{
			MyGDALRasterDataset* gdalDataset = (MyGDALRasterDataset*)dataset;
			double res = gdalDataset->GetGeoTransform()[1];
			IGeoSurface* surface = ui.mapWidget->GetGeoSurface();
			surface->SetViewResolution(res);
			ui.mapWidget->RepaintMap();
		}
	}
}

void tGisApp::on_removeLayerAction_triggered(bool checked)
{
	ui.layerWidget->RemoveSelectedLayer();
}

void tGisApp::on_layerVisibleAction_toggled(bool checked)
{
	ui.layerWidget->SetSelectedLayerVisible(checked);
}

void tGisApp::on_layerAttributeAction_triggered(bool checked)
{
	ui.layerWidget->SelectedLayerPropertyUI();
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_removeAllLayersAction_triggered(bool checked)
{
	ui.layerWidget->RemoveAllLayers();
}

void tGisApp::on_layerUpAction_triggered(bool checked)
{
	ui.layerWidget->MoveSelectedLayerUp();
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_layerTopAction_triggered(bool checked)
{
	ui.layerWidget->MoveSelectedLayerTop();
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_layerDownAction_triggered(bool checked)
{
	ui.layerWidget->MoveSelectedLayerDown();
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_layerBottomAction_triggered(bool checked)
{
	ui.layerWidget->MoveSelectedLayerBottom();
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_takeObjectSampleAction_toggled(bool checked)
{
	_takeObjectSampleTool.SetEnabled(checked);
	if (checked)
	{
		ui.panAction->setChecked(false);
		ui.zoomRectAction->setChecked(false);
	}
}

void tGisApp::on_showGridAction_toggled(bool checked)
{
	ui.mapWidget->SetGridVisible(checked);
	ui.mapWidget->PresentSurface();
}

void tGisApp::on_closeDatasetAction_triggered(bool checked)
{
	if (_selectedDataset != nullptr)
	{
		_selectedDataset->Close();
	}
}

void tGisApp::on_refreshDataSourceAction_triggered(bool checked)
{
	ui.dataSourceWidget->RefreshSelectedDataSource();
}

void tGisApp::on_layerWidget_LayerSelectionChanged(IMapPtr map, ILayerPtr layer, ILayerProviderPtr provider)
{
	_selectedRasterLayer = nullptr;
	ui.zoomLayerAction->setEnabled(layer != nullptr);
	ui.removeLayerAction->setEnabled(layer != nullptr);
	ui.layerVisibleAction->setEnabled(layer != nullptr);
	ui.layerAttributeAction->setEnabled(layer != nullptr);
	if (layer != nullptr)
	{
		ui.layerUpAction->setEnabled(ui.layerWidget->CanMoveSelectedLayerUp());
		ui.layerTopAction->setEnabled(ui.layerWidget->CanMoveSelectedLayerUp());
		ui.layerDownAction->setEnabled(ui.layerWidget->CanMoveSelectedLayerDown());
		ui.layerBottomAction->setEnabled(ui.layerWidget->CanMoveSelectedLayerDown());
		ui.layerVisibleAction->setChecked(layer->GetVisible());
		IDataset* dataset = layer->GetDataset();
		if (dataset->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		{
			_selectedRasterLayer = (RasterLayer*)layer;
			_takeObjectSampleTool.SetRasterLayer(_selectedRasterLayer);
			ui.zoomOriginalAction->setEnabled(true);
			ui.takeObjectSampleAction->setEnabled(_selectedObjectSampleDataSource != nullptr&&_selectedObjectSampleDataSource->IsConnected());
		}
		else
		{
			ui.takeObjectSampleAction->setEnabled(false);
		}
	}
	else
	{
		ui.zoomOriginalAction->setEnabled(false);
		ui.layerUpAction->setEnabled(false);
		ui.layerTopAction->setEnabled(false);
		ui.layerDownAction->setEnabled(false);
		ui.layerBottomAction->setEnabled(false);
		ui.takeObjectSampleAction->setEnabled(false);
	}
}

void tGisApp::on_layerWidget_LayerVisibleChanged(IMapPtr map, ILayerPtr layer, ILayerProviderPtr provider)
{
	ILayer* sellayer = ui.layerWidget->GetSelectedLayer();
	ui.mapWidget->RepaintMap();
	if (sellayer == layer)
	{
		ui.layerVisibleAction->setChecked(layer->GetVisible());
	}
}

void tGisApp::on_dataSourceWidget_SelectionChanged(IDataSourcePtr ds, IDatasetPtr dt, IDataSourceProviderPtr provider)
{
	_selectedObjectSampleDataSource = nullptr;
	_selectedDataSource = ds;
	_selectedDataset = dt;
	if (dt != nullptr && dt->IsOpened())
	{
		ui.closeDatasetAction->setEnabled(true);
		ui.datasetInfoAction->setEnabled(true);
	}
	else
	{
		ui.closeDatasetAction->setEnabled(false);
		ui.datasetInfoAction->setEnabled(false);
	}
	
	ui.refreshDataSourceAction->setEnabled(ds != nullptr && ds->IsConnected());
	
	if (ds != nullptr && ds->IsTypeOf(ObjectSampleDataSource::S_GetType()))
	{
		_selectedObjectSampleDataSource = reinterpret_cast<tGis::Core::ObjectSampleDataSource*>(ds);
		_takeObjectSampleTool.SetObjectSampleDataSource(_selectedObjectSampleDataSource);
		ui.takeObjectSampleAction->setEnabled(_selectedRasterLayer != nullptr&&_selectedObjectSampleDataSource->IsConnected());
	}
	else
	{
		ui.takeObjectSampleAction->setEnabled(false);
	}
}

void tGisApp::on_openedDatasetWidget_SelectionChanged(IDataSourcePtr ds, IDatasetPtr dt, IDataSourceProviderPtr provider)
{
	on_dataSourceWidget_SelectionChanged(_selectedDataSource, dt, provider);
}
