#include "tGisApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	tGisApp w;
	w.show();
	return a.exec();
}
