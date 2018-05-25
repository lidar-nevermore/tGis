#include "tGisApp.h"
#include "QDataSourceWidget.h"
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

	connect(ui.dataSourceWidget, &QDataSourceWidget::LayerAdded, ui.layerWidget,&QLayerWidget::LayerAdded);
}
