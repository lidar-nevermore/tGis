#include "tGisApp.h"
#include "QDataSourceWidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include "QLayerWidget.h"

tGisApp::tGisApp(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.mapWidget->SetMap(&_map);
	ui.layerWidget->SetMap(&_map);
	ui.layerWidget->SetMapWidget(ui.mapWidget);
	SetCurrentMap(&_map);
	SetCurrentMapWidget((IMapWidget*)ui.mapWidget);
	ui.mapWidget->AddMapTool(&_mapPanTool);
	ui.mapWidget->AddMapTool(&_mapZoomTool);
	ui.mapWidget->AddMapTool(&_rectZoomTool);
	_rectZoomTool.SetEnabled(false);

	QDesktopWidget * desktop = QApplication::desktop();
	int curMonitor = desktop->screenNumber(this);
	QRect rect = desktop->screenGeometry(curMonitor);
	SetMaxSurfaceSize(rect.width(), rect.height());

	connect(ui.dataSourceWidget, &QDataSourceWidget::LayerAdded, ui.layerWidget,&QLayerWidget::LayerAdded);
	connect(ui.layerWidget, &QLayerWidget::LayerSelectionChanged, this, &tGisApp::on_layerWidget_LayerSelectionChanged);

	ui.zoomFreeAction->setChecked(true);
	ui.panAction->setChecked(true);
	ui.zoomLayerAction->setEnabled(false);
	ui.zoomOriginalAction->setEnabled(false);
	ui.removeLayerAction->setEnabled(false);
	ui.layerVisibleAction->setEnabled(false);
	ui.layerAttributeAction->setEnabled(false);
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
	ui.panAction->setChecked(!checked);
}

void tGisApp::on_panAction_toggled(bool checked)
{
	_mapPanTool.SetEnabled(checked);
	ui.zoomRectAction->setChecked(!checked);
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
		if (strcmp(MyGDALRasterDataset::S_GetType(), dataset->GetType()) == 0)
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
	ui.mapWidget->RepaintMap();
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
	ui.mapWidget->RepaintMap();
}

void tGisApp::on_layerWidget_LayerSelectionChanged(IMapPtr map, ILayerPtr layer, ILayerProviderPtr provider)
{
	ui.zoomLayerAction->setEnabled(layer != nullptr);
	ui.removeLayerAction->setEnabled(layer != nullptr);
	ui.layerVisibleAction->setEnabled(layer != nullptr);
	ui.layerAttributeAction->setEnabled(layer != nullptr);
	if (layer != nullptr)
	{
		ui.layerVisibleAction->setChecked(layer->GetVisible());
		IDataset* dataset = layer->GetDataset();
		if (strcmp(MyGDALRasterDataset::S_GetType(), dataset->GetType()) == 0)
		{
			ui.zoomOriginalAction->setEnabled(true);
		}
	}
	else
	{
		ui.zoomOriginalAction->setEnabled(layer != nullptr);
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
