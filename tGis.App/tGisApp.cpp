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
	QDesktopWidget * desktop = QApplication::desktop();
	int curMonitor = desktop->screenNumber(this);
	QRect rect = desktop->screenGeometry(curMonitor);
	SetMaxSurfaceSize(rect.width(), rect.height());
	connect(ui.dataSourceWidget, &QDataSourceWidget::LayerAdded, ui.layerWidget,&QLayerWidget::LayerAdded);
}
