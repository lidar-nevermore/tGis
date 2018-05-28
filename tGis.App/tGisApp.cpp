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

	ui.zoomFreeAction->setChecked(true);
	ui.panAction->setChecked(true);
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
