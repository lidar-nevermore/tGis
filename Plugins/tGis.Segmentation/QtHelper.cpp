#include "QtHelper.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>



QtHelper QtHelper::INSTANCE;

QtHelper::QtHelper(QObject *parent)
{
	_mainWindow = nullptr;
	qRegisterMetaType<ObjectSampleMetadataPtr>("ObjectSampleMetadataPtr");
	qRegisterMetaType<IDataSourcePtr>("IDataSourcePtr");
	qRegisterMetaType<ILayerPtr>("ILayerPtr");
}


QtHelper::~QtHelper()
{
}

QMainWindow * QtHelper::GetMainWindow()
{
	if (_mainWindow == nullptr)
	{
		foreach(QWidget *w, qApp->topLevelWidgets())
			if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
				_mainWindow = mainWin;
	}
	return _mainWindow;
}



