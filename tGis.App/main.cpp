#include "tGisApp.h"
#include <QtWidgets/QApplication>

#include "tGisMetaType.h"

int main(int argc, char *argv[])
{
	qRegisterMetaType<IMapPtr>("IMapPtr");
	qRegisterMetaType<ILayerPtr>("ILayerPtr");

	QApplication a(argc, argv);
	tGisApp w;
	w.show();
	return a.exec();
}
