#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tGisApp.h"

#include "tVisualize.h"

#include "MapPanTool.h"
#include "MapZoomTool.h"
#include "MyMap.h"
#include "DrawRectTool.h"
#include "RectZoomTool.h"

using namespace tGis::Core;
using namespace tGis::Utility;

class tGisApp : public QMainWindow
{
	Q_OBJECT

public:
	tGisApp(QWidget *parent = Q_NULLPTR);

private:
	MyMap _map;
	MapPanTool _mapPanTool;
	MapZoomTool _mapZoomTool;
	RectZoomTool _rectZoomTool;

private slots:
	void on_zoomInAction_triggered(bool checked);
	void on_zoomOutAction_triggered(bool checked);
	void on_entireMapAction_triggered(bool checked);
	void on_refreshMapAction_triggered(bool checked);
	void on_zoomFreeAction_toggled(bool checked);
	void on_zoomRectAction_toggled(bool checked);
	void on_panAction_toggled(bool checked);

private:
	Ui::tGisAppClass ui;
};
