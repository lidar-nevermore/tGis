#include "tGisApp.h"
#include <QtWidgets/QApplication>

#include "tGisMetaType.h"
#include "RasterGrayScaleLayerPropertyDialog.h"
#include "RasterRgbLayerPropertyDialog.h"

#include "tOrganizer.h"

using namespace tGis::Core;

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


int main(int argc, char *argv[])
{
	RasterGrayScaleLayerProvider::INSTANCE.SetCreationUI(&RasterGrayScaleLayerPropertyDialog::CreateRasterGrayScaleLayer);
	RasterRgbLayerProvider::INSTANCE.SetCreationUI(&RasterRgbLayerPropertyDialog::CreateRasterRgbLayer);
	qRegisterMetaType<IMapPtr>("IMapPtr");
	qRegisterMetaType<ILayerPtr>("ILayerPtr");
	qRegisterMetaType<ILayerProviderPtr>("ILayerProviderPtr");
	qRegisterMetaType<IDataSourcePtr>("IDataSourcePtr");
	qRegisterMetaType<IDataSourceProviderPtr>("IDataSourceProviderPtr");
	qRegisterMetaType<IDatasetPtr>("IDatasetPtr");

	QApplication a(argc, argv);
	tGisApp w;
	w.show();
	return a.exec();
}
