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
	static ILayer* CreateRasterRgbLayer(RasterRgbLayerProvider*, IDataset*);
	static void RasterRgbLayerProperty(RasterRgbLayerProvider*, ILayer*);

public:
	void SetDataset(MyGDALRasterDataset* dataset);
	int GetBandIndexR();
	int GetBandIndexG();
	int GetBandIndexB();

private:
	Ui::RasterRgbLayerPropertyDialog ui;
};

