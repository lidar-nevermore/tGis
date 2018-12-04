#include <QDialog>

#include "ui_RasterRgbLayerPropertyDialog.h"


#include "tEntity.h"
#include "tOrganizer.h"

using namespace tGis::Core;

class RasterRgbLayerPropertyDialog : public QDialog
{
	Q_OBJECT
public:
	RasterRgbLayerPropertyDialog(QWidget *parent = 0);
	~RasterRgbLayerPropertyDialog();

public:
	static ILayer* CreateRasterRgbLayer(ILayerProvider*, IDataset*);
	static void RasterRgbLayerProperty(ILayerProvider*, ILayer*);

public:
	void SetDataset(MyGDALRasterDataset* dataset, int r, int g, int b);

private:
	Ui::RasterRgbLayerPropertyDialog ui;
};

