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
	void SetDataset(MyGDALRasterDataset* dataset, int band);

public:
	static ILayer* CreateRasterGrayScaleLayer(RasterGrayScaleLayerProvider*, MyGDALRasterDataset*);
	static void RasterGrayScaleLayerProperty(RasterGrayScaleLayerProvider*, RasterGrayScaleLayer*);

private:
	Ui::RasterGrayScaleLayerPropertyDialog ui;
};

