#include "tGisApp.h"
#include <QtWidgets/QApplication>

#include "tGisMetaType.h"
#include "RasterGrayScaleLayerPropertyDialog.h"
#include "RasterRgbLayerPropertyDialog.h"
#include "RasterBinaryGrayScaleLayerPropertyDialog.h"

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
	RasterGrayScaleLayerProvider::INSTANCE.SetPropertyUI(&RasterGrayScaleLayerPropertyDialog::RasterGrayScaleLayerProperty);
	RasterBinaryGrayScaleLayerProvider::INSTANCE.SetCreationUI(&RasterBinaryGrayScaleLayerPropertyDialog::CreateRasterBinaryGrayScaleLayer);
	RasterBinaryGrayScaleLayerProvider::INSTANCE.SetPropertyUI(&RasterBinaryGrayScaleLayerPropertyDialog::RasterBinaryGrayScaleLayerProperty);
	RasterRgbLayerProvider::INSTANCE.SetCreationUI(&RasterRgbLayerPropertyDialog::CreateRasterRgbLayer);
	RasterRgbLayerProvider::INSTANCE.SetPropertyUI(&RasterRgbLayerPropertyDialog::RasterRgbLayerProperty);
	qRegisterMetaType<IMapPtr>("IMapPtr");
	qRegisterMetaType<ILayerPtr>("ILayerPtr");
	qRegisterMetaType<ILayerProviderPtr>("ILayerProviderPtr");
	qRegisterMetaType<IDataSourcePtr>("IDataSourcePtr");
	qRegisterMetaType<IDataSourceProviderPtr>("IDataSourceProviderPtr");
	qRegisterMetaType<IDatasetPtr>("IDatasetPtr");
	qRegisterMetaType<ITGisObjectPtr>("ITGisObjectPtr");

	QApplication a(argc, argv);
	tGisApp w;
	w.show();
	return a.exec();
}
