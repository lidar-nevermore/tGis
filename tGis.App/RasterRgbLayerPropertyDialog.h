#include <QDialog>

#include "ui_RasterRgbLayerPropertyDialog.h"


#include "tEntity.h"
#include "tOrganizer.h"

using namespace tGis::Core;

class RasterRgbLayerPropertyDialog : public QDialog
{
public:
	RasterRgbLayerPropertyDialog(QWidget *parent = 0);
	~RasterRgbLayerPropertyDialog();

public:
	static ILayer* CreateRasterRgbLayer(RasterRgbLayerProvider*, MyGDALRasterDataset*);
	static void RasterRgbLayerProperty(RasterRgbLayerProvider*, RasterRgbLayer*);

public:
	void SetDataset(MyGDALRasterDataset* dataset, int r, int g, int b);

private:
	Ui::RasterRgbLayerPropertyDialog ui;
};

