#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tGis.h"

class tGis : public QMainWindow
{
	Q_OBJECT

public:
	tGis(QWidget *parent = Q_NULLPTR);

private:
	Ui::tGisClass ui;
};
