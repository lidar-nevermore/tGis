#pragma once

#include <QDialog>

#include "ui_RasterBinaryGrayScaleLayerPropertyDialog.h"


#include "tEntity.h"
#include "tOrganizer.h"

using namespace tGis::Core;


class RasterBinaryGrayScaleLayerPropertyDialog : public QDialog
{
public:
	RasterBinaryGrayScaleLayerPropertyDialog(QWidget *parent = 0);
	~RasterBinaryGrayScaleLayerPropertyDialog();

public:
	void SetDataset(MyGDALRasterDataset* dataset, int band);

public:
	static ILayer* CreateRasterBinaryGrayScaleLayer(RasterBinaryGrayScaleLayerProvider*, MyGDALRasterDataset*);
	static void RasterBinaryGrayScaleLayerProperty(RasterBinaryGrayScaleLayerProvider*, RasterBinaryGrayScaleLayer*);

private:
	Ui::RasterBinaryGrayScaleLayerPropertyDialog ui;
};

