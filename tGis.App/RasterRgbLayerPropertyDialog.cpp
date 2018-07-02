#include "RasterRgbLayerPropertyDialog.h"
#include <QPushButton>
#include "QtHelper.h"


RasterRgbLayerPropertyDialog::RasterRgbLayerPropertyDialog(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	QPushButton* button = ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	button->setEnabled(false);
	QRegExp regx("^-?([1-9]\\d*\\.\\d*|0\\.\\d*[1-9]\\d*|0?\\.0+|0)$");
	QValidator* minMaxValidator = new QRegExpValidator(regx, this);
	ui.leMaxR->setValidator(minMaxValidator);
	ui.leMinR->setValidator(minMaxValidator);
	ui.leMaxG->setValidator(minMaxValidator);
	ui.leMinG->setValidator(minMaxValidator);
	ui.leMaxB->setValidator(minMaxValidator);
	ui.leMinB->setValidator(minMaxValidator);
	ui.pteData->setReadOnly(true);
}


RasterRgbLayerPropertyDialog::~RasterRgbLayerPropertyDialog()
{
}

ILayer * RasterRgbLayerPropertyDialog::CreateRasterRgbLayer(ILayerProvider * provider_,IDataset* dataset_)
{
	RasterRgbLayerProvider * provider = (RasterRgbLayerProvider*)provider_;
	MyGDALRasterDataset* dataset = (MyGDALRasterDataset*)dataset_;

	RasterRgbLayerPropertyDialog dlg((QWidget*)QtHelper::INSTANCE.GetMainWindow());
	GDALDataset* gdt = dataset->GetGDALDataset();
	if (gdt == nullptr)
		return nullptr;
	if (gdt->GetRasterCount() == 0)
		return nullptr;
	GDALRasterBand* rBand = gdt->GetRasterBand(1);
	GDALDataType rDataType = rBand->GetRasterDataType();
	if(rDataType == GDT_Byte)
		dlg.SetDataset(dataset, 1, 2, 3);
	else
		dlg.SetDataset(dataset, 3, 2, 1);
	dlg.ui.pteData->setPlainText(QString::fromLocal8Bit(dataset->GetCreationString()));
	if (QDialog::Accepted == dlg.exec())
	{
		int rBand = dlg.ui.cboBandR->currentIndex() + 1;
		int gBand = dlg.ui.cboBandG->currentIndex() + 1;
		int bBand = dlg.ui.cboBandB->currentIndex() + 1;
		RasterRgbLayer* layer = (RasterRgbLayer*)provider->CreateLayer(dataset, rBand, gBand, bBand);

		unsigned char opacity = (unsigned char)dlg.ui.sdOpacity->value();
		layer->SetOpacity(opacity / 255.0);

		QString rMinStr = dlg.ui.leMinR->text();
		QString rMaxStr = dlg.ui.leMaxR->text();
		double minR = rMinStr.toDouble();
		double maxR = rMaxStr.toDouble();
		if (minR < maxR)
		{
			layer->SetMinMaxR(minR, maxR);
		}
		
		QString gMinStr = dlg.ui.leMinG->text();
		QString gMaxStr = dlg.ui.leMaxG->text();
		double minG = gMinStr.toDouble();
		double maxG = gMaxStr.toDouble();
		if (minG < maxG)
		{
			layer->SetMinMaxG(minG, maxG);
		}

		QString bMinStr = dlg.ui.leMinB->text();
		QString bMaxStr = dlg.ui.leMaxB->text();
		double minB = bMinStr.toDouble();
		double maxB = bMaxStr.toDouble();
		if (minB < maxB)
		{
			layer->SetMinMaxG(minB, maxB);
		}

		return layer;
	}

	return nullptr;
}

void RasterRgbLayerPropertyDialog::RasterRgbLayerProperty(ILayerProvider *provider_, ILayer *layer_)
{
	RasterRgbLayerProvider* provider = (RasterRgbLayerProvider*)provider_;
	RasterRgbLayer* layer = (RasterRgbLayer*)layer_;
	RasterRgbLayerPropertyDialog dlg((QWidget*)QtHelper::INSTANCE.GetMainWindow());
	dlg.SetDataset((MyGDALRasterDataset*)layer->GetDataset(), layer->GetBandR(), layer->GetBandG(), layer->GetBandB());
	dlg.ui.pteData->setPlainText(QString::fromLocal8Bit(layer->GetDataset()->GetCreationString()));
	dlg.ui.sdOpacity->setValue((int)(layer->GetOpacity() * 255));

	double minR;
	double maxR;
	layer->GetMinMaxR(&minR, &maxR);
	dlg.ui.leMinR->setText(QString::number(minR));
	dlg.ui.leMaxR->setText(QString::number(maxR));

	double minG;
	double maxG;
	layer->GetMinMaxG(&minG, &maxG);
	dlg.ui.leMinG->setText(QString::number(minG));
	dlg.ui.leMaxG->setText(QString::number(maxG));

	double minB;
	double maxB;
	layer->GetMinMaxB(&minB, &maxB);
	dlg.ui.leMinB->setText(QString::number(minB));
	dlg.ui.leMaxB->setText(QString::number(maxB));

	if (QDialog::Accepted == dlg.exec())
	{
		int rBand = dlg.ui.cboBandR->currentIndex() + 1;
		int gBand = dlg.ui.cboBandG->currentIndex() + 1;
		int bBand = dlg.ui.cboBandB->currentIndex() + 1;

		layer->SetDataset((MyGDALRasterDataset*)layer->GetDataset(), rBand, gBand, bBand);

		unsigned char opacity = (unsigned char)dlg.ui.sdOpacity->value();
		layer->SetOpacity(opacity / 255.0);

		QString rMinStr = dlg.ui.leMinR->text();
		QString rMaxStr = dlg.ui.leMaxR->text();
		double minR = rMinStr.toDouble();
		double maxR = rMaxStr.toDouble();
		if (minR < maxR)
		{
			layer->SetMinMaxR(minR, maxR);
		}

		QString gMinStr = dlg.ui.leMinG->text();
		QString gMaxStr = dlg.ui.leMaxG->text();
		double minG = gMinStr.toDouble();
		double maxG = gMaxStr.toDouble();
		if (minG < maxG)
		{
			layer->SetMinMaxG(minG, maxG);
		}

		QString bMinStr = dlg.ui.leMinB->text();
		QString bMaxStr = dlg.ui.leMaxB->text();
		double minB = bMinStr.toDouble();
		double maxB = bMaxStr.toDouble();
		if (minB < maxB)
		{
			layer->SetMinMaxG(minB, maxB);
		}
	}
}

void RasterRgbLayerPropertyDialog::SetDataset(MyGDALRasterDataset * dataset,int r, int g, int b)
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

		r = r > bandCount ? bandCount: r;
		ui.cboBandR->setCurrentIndex(r-1);

		b = b > bandCount ? bandCount: b;
		ui.cboBandB->setCurrentIndex(b-1);
		
		g = g > bandCount ? bandCount : g;
		ui.cboBandG->setCurrentIndex(g-1);	
	}
}

