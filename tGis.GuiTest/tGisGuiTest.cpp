#include "tGisGuiTest.h"
#include "QDesktopWidget"

tGisGuiTest::tGisGuiTest(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QDesktopWidget * desktop = QApplication::desktop();
	int curMonitor = desktop->screenNumber(this);
	QRect rect = desktop->screenGeometry(curMonitor);
	SetMaxSurfaceSize(rect.width(), rect.height());
}
