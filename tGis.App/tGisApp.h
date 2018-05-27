#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tGisApp.h"

#include "tVisualize.h"

#include "MapPanTool.h"
#include "MapZoomTool.h"
#include "MyMap.h"

using namespace tGis::Core;

class tGisApp : public QMainWindow
{
	Q_OBJECT

public:
	tGisApp(QWidget *parent = Q_NULLPTR);

private:
	MyMap _map;
	MapPanTool _mapPanTool;
	MapZoomTool _mapZoomTool;

private:
	Ui::tGisAppClass ui;
};
