#include "RasterBinaryGrayScaleLayerPropertyDialog.h"
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include "tGisMetaType.h"


RasterBinaryGrayScaleLayerPropertyDialog::RasterBinaryGrayScaleLayerPropertyDialog(QWidget * parent)
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
	ui.lePivot->setValidator(minMaxValidator);
	ui.pteData->setReadOnly(true);
}

RasterBinaryGrayScaleLayerPropertyDialog::~RasterBinaryGrayScaleLayerPropertyDialog()
{
}

void RasterBinaryGrayScaleLayerPropertyDialog::SetDataset(MyGDALRasterDataset * dataset, int band)
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

ILayer * RasterBinaryGrayScaleLayerPropertyDialog::CreateRasterBinaryGrayScaleLayer(ILayerProvider* provider_,IDataset* dataset_)
{
	RasterBinaryGrayScaleLayerProvider * provider = (RasterBinaryGrayScaleLayerProvider*)provider_;
	MyGDALRasterDataset * dataset = (MyGDALRasterDataset*)dataset_;
	RasterBinaryGrayScaleLayerPropertyDialog dlg((QWidget*)GetMainWindow());
	dlg.SetDataset(dataset, 1);
	dlg.ui.pteData->setPlainText(QString::fromLocal8Bit(dataset->GetCreationString()));
	if (QDialog::Accepted == dlg.exec())
	{
		int band = dlg.ui.cboBand->currentIndex() + 1;
		RasterBinaryGrayScaleLayer* layer = (RasterBinaryGrayScaleLayer*)provider->CreateLayer(dataset, band);
		QString minStr = dlg.ui.leMin->text();
		QString maxStr = dlg.ui.leMax->text();
		QString pivotStr = dlg.ui.lePivot->text();
		unsigned char opacity = (unsigned char)dlg.ui.sdOpacity->value();
		double min = minStr.toDouble();
		double max = maxStr.toDouble();
		double pivot = pivotStr.toDouble();
		if (min < pivot && pivot < max)
		{
			layer->SetMinPivotMax(min, pivot, max);
		}
		layer->SetOpacity(opacity / 255.0);
		bool leftRChannel = dlg.ui.chkLeftR->isChecked();
		bool leftGChannel = dlg.ui.chkLeftG->isChecked();
		bool leftBChannel = dlg.ui.chkLeftB->isChecked();
		bool rightRChannel = dlg.ui.chkRightR->isChecked();
		bool rightGChannel = dlg.ui.chkRightG->isChecked();
		bool rightBChannel = dlg.ui.chkRightB->isChecked();
		layer->SetLeftChannel(leftRChannel, leftGChannel, leftBChannel);
		layer->SetRightChannel(rightRChannel, rightBChannel, rightBChannel);

		return layer;
	}

	return nullptr;
}

void RasterBinaryGrayScaleLayerPropertyDialog::RasterBinaryGrayScaleLayerProperty(ILayerProvider *provider_, ILayer *layer_)
{
	RasterBinaryGrayScaleLayerProvider *provider = (RasterBinaryGrayScaleLayerProvider*)provider_;
	RasterBinaryGrayScaleLayer *layer = (RasterBinaryGrayScaleLayer*)layer_;
	RasterBinaryGrayScaleLayerPropertyDialog dlg((QWidget*)GetMainWindow());
	dlg.SetDataset((MyGDALRasterDataset*)layer->GetDataset(), layer->GetBand());
	dlg.ui.pteData->setPlainText(QString::fromLocal8Bit(layer->GetDataset()->GetCreationString()));
	double min;
	double max;
	double pivot;
	layer->GetMinPivotMax(&min,&pivot, &max);
	dlg.ui.leMin->setText(QString::number(min));
	dlg.ui.leMax->setText(QString::number(max));
	dlg.ui.lePivot->setText(QString::number(pivot));
	dlg.ui.sdOpacity->setValue((int)(layer->GetOpacity() * 255));
	bool leftRChannel;
	bool leftGChannel;
	bool leftBChannel;
	bool rightRChannel;
	bool rightGChannel;
	bool rightBChannel;
	layer->GetLeftChannel(&leftRChannel, &leftGChannel, &leftBChannel);
	layer->GetRightChannel(&rightRChannel, &rightGChannel, &rightBChannel);
	dlg.ui.chkLeftR->setChecked(leftRChannel);
	dlg.ui.chkLeftG->setChecked(leftGChannel);
	dlg.ui.chkLeftB->setChecked(leftBChannel);
	dlg.ui.chkRightR->setChecked(rightRChannel);
	dlg.ui.chkRightG->setChecked(rightGChannel);
	dlg.ui.chkRightB->setChecked(rightBChannel);

	if (QDialog::Accepted == dlg.exec())
	{
		int band = dlg.ui.cboBand->currentIndex() + 1;
		layer->SetDataset((MyGDALRasterDataset*)layer->GetDataset(), band);

		QString minStr = dlg.ui.leMin->text();
		QString maxStr = dlg.ui.leMax->text();
		QString pivotStr = dlg.ui.lePivot->text();
		unsigned char opacity = (unsigned char)dlg.ui.sdOpacity->value();
		min = minStr.toDouble();
		max = maxStr.toDouble();
		pivot = pivotStr.toDouble();
		if (min < pivot && pivot < max)
		{
			layer->SetMinPivotMax(min, pivot, max);
		}
		layer->SetOpacity(opacity / 255.0);
		leftRChannel = dlg.ui.chkLeftR->isChecked();
		leftGChannel = dlg.ui.chkLeftG->isChecked();
		leftBChannel = dlg.ui.chkLeftB->isChecked();
		rightRChannel = dlg.ui.chkRightR->isChecked();
		rightGChannel = dlg.ui.chkRightG->isChecked();
		rightBChannel = dlg.ui.chkRightB->isChecked();
		layer->SetLeftChannel(leftRChannel, leftGChannel, leftBChannel);
		layer->SetRightChannel(rightRChannel, rightGChannel, rightBChannel);
	}
}
