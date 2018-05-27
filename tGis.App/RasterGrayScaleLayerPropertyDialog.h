#pragma once


#include <QDialog>

#include "ui_RasterGrayScaleLayerPropertyDialog.h"


#include "tEntity.h"
#include "tOrganizer.h"

using namespace tGis::Core;

class RasterGrayScaleLayerPropertyDialog : public QDialog
{
public:
	explicit RasterGrayScaleLayerPropertyDialog(QWidget *parent = 0);
	~RasterGrayScaleLayerPropertyDialog();

public:
	static ILayer* CreateRasterGrayScaleLayer(RasterGrayScaleLayerProvider*, IDataset*);
	static void RasterGrayScaleLayerProperty(RasterGrayScaleLayerProvider*, ILayer*);

public:
	void SetDataset(MyGDALRasterDataset* dataset);
	int GetBandIndex();

private:
	Ui::RasterGrayScaleLayerPropertyDialog ui;
};

