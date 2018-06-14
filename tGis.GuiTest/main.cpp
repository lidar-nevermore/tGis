#include "tGisGuiTest.h"
#include <QtWidgets/QApplication>

#include "tOrganizer.h"

using namespace tGis::Core;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	tGisGuiTest w;
	w.show();
	return a.exec();
}
