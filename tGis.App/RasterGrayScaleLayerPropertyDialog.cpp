#include "RasterGrayScaleLayerPropertyDialog.h"
#include <QPushButton>
#include <QLineEdit>
#include "tGisMetaType.h"


RasterGrayScaleLayerPropertyDialog::RasterGrayScaleLayerPropertyDialog(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	button->setEnabled(false);
	QRegExp regx("^-?([1-9]\\d*\\.\\d*|0\\.\\d*[1-9]\\d*|0?\\.0+|0)$");
	QValidator* minMaxValidator = new QRegExpValidator(regx, this);
	ui.leMax->setValidator(minMaxValidator);
	ui.leMin->setValidator(minMaxValidator);
}


RasterGrayScaleLayerPropertyDialog::~RasterGrayScaleLayerPropertyDialog()
{

}


void RasterGrayScaleLayerPropertyDialog::SetDataset(MyGDALRasterDataset * dataset, int band)
{
	GDALDataset* gdt = dataset->GetGDALDataset();
	int bandCount = gdt->GetRasterCount();
	for (int i = 0; i < bandCount; i++)
	{
		ui.cboBand->addItem(QString::asprintf("band %d", i + 1));
	}

	if (bandCount >= band)
	{
		ui.cboBand->setCurrentIndex(band - 1);
		QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
		button->setEnabled(true);
	}
}

ILayer * RasterGrayScaleLayerPropertyDialog::CreateRasterGrayScaleLayer(RasterGrayScaleLayerProvider* provider, MyGDALRasterDataset * dataset)
{
	RasterGrayScaleLayerPropertyDialog dlg((QWidget*)GetMainWindow());
	dlg.SetDataset(dataset, 1);
	if (QDialog::Accepted == dlg.exec())
	{
		int band = dlg.ui.cboBand->currentIndex() + 1;
		RasterGrayScaleLayer* layer = (RasterGrayScaleLayer*)provider->CreateLayer((MyGDALRasterDataset*)dataset, band);
		QString minStr = dlg.ui.leMin->text();
		QString maxStr = dlg.ui.leMax->text();
		unsigned char opacity = (unsigned char)dlg.ui.sdOpacity->value();
		double min = minStr.toDouble();
		double max = maxStr.toDouble();
		if (min < max)
		{
			layer->SetMinMax(min, max);
		}
		layer->SetOpacity(opacity / 255.0);

		return layer;
	}

	return nullptr;
}

void RasterGrayScaleLayerPropertyDialog::RasterGrayScaleLayerProperty(RasterGrayScaleLayerProvider* provider, RasterGrayScaleLayer * layer)
{
	RasterGrayScaleLayerPropertyDialog dlg((QWidget*)GetMainWindow());
	dlg.SetDataset((MyGDALRasterDataset*)layer->GetDataset(), layer->GetBand());
	double min;
	double max;
	layer->GetMinMax(&min, &max);
	dlg.ui.leMin->setText(QString::number(min));
	dlg.ui.leMax->setText(QString::number(max));
	dlg.ui.sdOpacity->setValue((int)(layer->GetOpacity() * 255));
	if (QDialog::Accepted == dlg.exec())
	{
		int band = dlg.ui.cboBand->currentIndex() + 1;
		layer->SetDataset((MyGDALRasterDataset*)layer->GetDataset(), band);

		QString minStr = dlg.ui.leMin->text();
		QString maxStr = dlg.ui.leMax->text();
		unsigned char opacity = (unsigned char)dlg.ui.sdOpacity->value();
		min = minStr.toDouble();
		max = maxStr.toDouble();
		if (min < max)
		{
			layer->SetMinMax(min, max);
		}
		layer->SetOpacity(opacity / 255.0);
	}
}

