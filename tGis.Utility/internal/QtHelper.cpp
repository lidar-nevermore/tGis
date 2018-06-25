#include "QtHelper.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>

BEGIN_NAME_SPACE(tGis, Utility)

QtHelper _QtHelper;

static QMainWindow* g_MainWindow = nullptr;

QMainWindow* GetMainWindow()
{
	if (g_MainWindow == nullptr)
	{
		foreach(QWidget *w, qApp->topLevelWidgets())
			if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
				g_MainWindow = mainWin;
	}
	return g_MainWindow;
}

QtHelper::QtHelper(QObject *parent)
{
	qRegisterMetaType<ObjectSampleMetadataPtr>("ObjectSampleMetadataPtr");
}


QtHelper::~QtHelper()
{
}

END_NAME_SPACE(tGis, Utility)

