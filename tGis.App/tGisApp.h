#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tGisApp.h"

#include "tUtility.h"

using namespace tGis::Core;

class tGisApp : public QMainWindow
{
	Q_OBJECT

public:
	tGisApp(QWidget *parent = Q_NULLPTR);

private:
	Map _map;

private:
	Ui::tGisAppClass ui;
};
