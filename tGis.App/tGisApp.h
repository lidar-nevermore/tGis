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

class tGisApp : public QMainWindow
{
	Q_OBJECT

public:
	tGisApp(QWidget *parent = Q_NULLPTR);

private:
	MyMap _map;
	MapPanTool _mapPanTool;
	MapZoomTool _mapZoomTool;
	DrawRectTool _drawRectTool;
	RectZoomTool _rectZoomTool;

private:
	Ui::tGisAppClass ui;
};
