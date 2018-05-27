#include "RasterGrayScaleLayerPropertyDialog.h"
#include <QPushButton>
#include "tGisMetaType.h"


RasterGrayScaleLayerPropertyDialog::RasterGrayScaleLayerPropertyDialog(QWidget *parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	button->setEnabled(false);
}


RasterGrayScaleLayerPropertyDialog::~RasterGrayScaleLayerPropertyDialog()
{
}

ILayer * RasterGrayScaleLayerPropertyDialog::CreateRasterGrayScaleLayer(RasterGrayScaleLayerProvider* provider, IDataset * dataset)
{
	RasterGrayScaleLayerPropertyDialog dlg((QWidget*)GetMainWindow());
	dlg.SetDataset((MyGDALRasterDataset*)dataset);
	if (QDialog::Accepted == dlg.exec())
	{
		return provider->CreateLayer((MyGDALRasterDataset*)dataset, dlg.GetBandIndex());
	}

	return nullptr;
}

void RasterGrayScaleLayerPropertyDialog::RasterGrayScaleLayerProperty(RasterGrayScaleLayerProvider*, ILayer *)
{
}

void RasterGrayScaleLayerPropertyDialog::SetDataset(MyGDALRasterDataset * dataset)
{
	GDALDataset* gdt = dataset->GetGDALDataset();
	int bandCount = gdt->GetRasterCount();
	for (int i = 0; i < bandCount; i++)
	{
		ui.cboBand->addItem(QString::asprintf("band %d", i + 1));
	}

	if (bandCount > 0)
	{
		ui.cboBand->setCurrentIndex(0);
		QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
		button->setEnabled(true);
	}
}

int RasterGrayScaleLayerPropertyDialog::GetBandIndex()
{
	return ui.cboBand->currentIndex()+1;
}
