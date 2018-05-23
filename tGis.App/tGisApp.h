#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tGisApp.h"

class tGisApp : public QMainWindow
{
	Q_OBJECT

public:
	tGisApp(QWidget *parent = Q_NULLPTR);

private:
	Ui::tGisAppClass ui;
};
