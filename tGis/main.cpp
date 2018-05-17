#include "tGis.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	tGis w;
	w.show();
	return a.exec();
}
