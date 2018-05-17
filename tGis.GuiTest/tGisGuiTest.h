#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tGisGuiTest.h"

class tGisGuiTest : public QMainWindow
{
	Q_OBJECT

public:
	tGisGuiTest(QWidget *parent = Q_NULLPTR);

private:
	Ui::tGisGuiTestClass ui;
};
