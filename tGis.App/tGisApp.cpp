#include "tGisApp.h"

tGisApp::tGisApp(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.mapWidget->SetMap(&_map);
	SetCurrentMap(&_map);
	SetCurrentMapWidget((IMapWidget*)ui.mapWidget);
}
