#include "QtHelper.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>


QtHelper QtHelper::INSTANCE;

QtHelper::QtHelper(QObject *parent)
{
	_mainWindow = nullptr;
	qRegisterMetaType<IMapPtr>("IMapPtr");
	qRegisterMetaType<ILayerPtr>("ILayerPtr");
	qRegisterMetaType<ILayerProviderPtr>("ILayerProviderPtr");
	qRegisterMetaType<IDataSourcePtr>("IDataSourcePtr");
	qRegisterMetaType<IDataSourceProviderPtr>("IDataSourceProviderPtr");
	qRegisterMetaType<IDatasetPtr>("IDatasetPtr");
	qRegisterMetaType<ITGisObjectPtr>("ITGisObjectPtr");
	qRegisterMetaType<IToolPtr>("IToolPtr");
	qRegisterMetaType<ToolKitPtr>("ToolKitPtr");
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

