#include "tGisApp.h"
#include <QtWidgets/QApplication>

#include "RasterGrayScaleLayerPropertyDialog.h"
#include "RasterRgbLayerPropertyDialog.h"
#include "RasterBinaryGrayScaleLayerPropertyDialog.h"

#include "tOrganizer.h"
#include "tUtility.h"
#include "tGis.Utility.h"

#include "QtHelper.h"

using namespace tGis::Core;

int main(int argc, char *argv[])
{
	tGis::Utility::InitializeModule();

	RasterGrayScaleLayerProvider::INSTANCE.SetCreationUI(&RasterGrayScaleLayerPropertyDialog::CreateRasterGrayScaleLayer);
	RasterGrayScaleLayerProvider::INSTANCE.SetPropertyUI(&RasterGrayScaleLayerPropertyDialog::RasterGrayScaleLayerProperty);
	RasterBinaryGrayScaleLayerProvider::INSTANCE.SetCreationUI(&RasterBinaryGrayScaleLayerPropertyDialog::CreateRasterBinaryGrayScaleLayer);
	RasterBinaryGrayScaleLayerProvider::INSTANCE.SetPropertyUI(&RasterBinaryGrayScaleLayerPropertyDialog::RasterBinaryGrayScaleLayerProperty);
	RasterRgbLayerProvider::INSTANCE.SetCreationUI(&RasterRgbLayerPropertyDialog::CreateRasterRgbLayer);
	RasterRgbLayerProvider::INSTANCE.SetPropertyUI(&RasterRgbLayerPropertyDialog::RasterRgbLayerProperty);

	QApplication a(argc, argv);
	tGisApp w;
	QtHelper::INSTANCE.setObjectName(QStringLiteral("QtHelper"));
	w.show();
	return a.exec();
}
