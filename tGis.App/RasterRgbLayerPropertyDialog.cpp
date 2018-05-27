#include "RasterRgbLayerPropertyDialog.h"
#include <QPushButton>
#include "tGisMetaType.h"


RasterRgbLayerPropertyDialog::RasterRgbLayerPropertyDialog(QWidget *parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	button->setEnabled(false);
}


RasterRgbLayerPropertyDialog::~RasterRgbLayerPropertyDialog()
{
}

ILayer * RasterRgbLayerPropertyDialog::CreateRasterRgbLayer(RasterRgbLayerProvider * provider, IDataset * dataset)
{
	RasterRgbLayerPropertyDialog dlg((QWidget*)GetMainWindow());
	dlg.SetDataset((MyGDALRasterDataset*)dataset);
	if (QDialog::Accepted == dlg.exec())
	{
		return provider->CreateLayer((MyGDALRasterDataset*)dataset, dlg.GetBandIndexR(), dlg.GetBandIndexG(), dlg.GetBandIndexB());
	}

	return nullptr;
}

void RasterRgbLayerPropertyDialog::RasterRgbLayerProperty(RasterRgbLayerProvider *, ILayer *)
{
}

void RasterRgbLayerPropertyDialog::SetDataset(MyGDALRasterDataset * dataset)
{
	GDALDataset* gdt = dataset->GetGDALDataset();
	int bandCount = gdt->GetRasterCount();
	for (int i = 0; i < bandCount; i++)
	{
		ui.cboBandR->addItem(QString::asprintf("band %d", i + 1));
		ui.cboBandG->addItem(QString::asprintf("band %d", i + 1));
		ui.cboBandB->addItem(QString::asprintf("band %d", i + 1));
	}

	if (bandCount > 0)
	{
		QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
		button->setEnabled(true);
		int bandIndex = 0;
		ui.cboBandR->setCurrentIndex(bandIndex);
		bandIndex++;
		bandIndex = bandIndex >= bandCount ? bandCount - 1 : bandIndex;
		ui.cboBandG->setCurrentIndex(bandIndex);
		bandIndex++;
		bandIndex = bandIndex >= bandCount ? bandCount - 1 : bandIndex;
		ui.cboBandB->setCurrentIndex(bandIndex);
	}
}

int RasterRgbLayerPropertyDialog::GetBandIndexR()
{
	return ui.cboBandR->currentIndex() + 1;
}

int RasterRgbLayerPropertyDialog::GetBandIndexG()
{
	return ui.cboBandG->currentIndex() + 1;
}

int RasterRgbLayerPropertyDialog::GetBandIndexB()
{
	return ui.cboBandB->currentIndex() + 1;
}
