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
	static ILayer* CreateRasterBinaryGrayScaleLayer(ILayerProvider*, IDataset*);
	static void RasterBinaryGrayScaleLayerProperty(ILayerProvider*, ILayer*);

private:
	Ui::RasterBinaryGrayScaleLayerPropertyDialog ui;
};

