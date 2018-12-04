#pragma once


#include <QDialog>

#include "ui_RasterGrayScaleLayerPropertyDialog.h"


#include "tEntity.h"
#include "tOrganizer.h"

using namespace tGis::Core;

class RasterGrayScaleLayerPropertyDialog : public QDialog
{
	Q_OBJECT
public:
	explicit RasterGrayScaleLayerPropertyDialog(QWidget *parent = 0);
	~RasterGrayScaleLayerPropertyDialog();

public:
	void SetDataset(MyGDALRasterDataset* dataset, int band);

public:
	static ILayer* CreateRasterGrayScaleLayer(ILayerProvider*, IDataset*);
	static void RasterGrayScaleLayerProperty(ILayerProvider*, ILayer*);

private:
	Ui::RasterGrayScaleLayerPropertyDialog ui;
};

