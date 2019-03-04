#include "SegmentationDialog.h"
#include "MyGDALDataset.h"
#include <QFileDialog>

using namespace tGis::Core;

QString SegmentationDialog::_initialDir = QDir::homePath();

SegmentationDialog::SegmentationDialog(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	_gaussianCoef = 0.9;
	_texturePeriod = 7;
	_lowPercentage = 0.53;
	_highPercentage = 0.23;
	_damStrength = 0.3;
	_poolStrength = 0.03;
	_useTexture = true;

	connect(ui.btnInputImg, &QPushButton::clicked, this, &SegmentationDialog::on_btnInputImg_clicked);
	connect(ui.btnOutputShp, &QPushButton::clicked, this, &SegmentationDialog::on_btnOutputShp_clicked);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &SegmentationDialog::on_accepted);
}


SegmentationDialog::~SegmentationDialog()
{
}

void SegmentationDialog::on_btnInputImg_clicked(bool checked)
{
	QFileDialog fileDlg((QWidget*)this);
	fileDlg.setFileMode(QFileDialog::ExistingFile);
	fileDlg.setAcceptMode(QFileDialog::AcceptOpen);
	fileDlg.setViewMode(QFileDialog::List);
	fileDlg.setDirectory(_initialDir);
	QStringList filters;

	filters << QStringLiteral("常用格式 (*.jpg *.jpeg *.png *.bmp *.tif *.tiff *.img *.pix)");

	size_t c = MyGDALDataset::GetSupportedRasterFormatCount();
	
	for (size_t i = 0; i < c; i++)
	{
		QString filter = QString::fromLocal8Bit(MyGDALDataset::GetSupportedRasterFormatName(i));
		filter.append(" (*.");
		filter.append(QString::fromLocal8Bit(MyGDALDataset::GetSupportedRasterFormatExt(i).at(0).c_str()));
		filter.append(")");
		filters << filter;
	}

	fileDlg.setNameFilters(filters);

	if (fileDlg.exec() == QDialog::Accepted)
	{
		QString qtPath = fileDlg.selectedFiles().at(0);
		QString nqtPath = QDir::toNativeSeparators(qtPath);
		QFileInfo finfo(qtPath);
		_initialDir = finfo.absolutePath();
		ui.leInputImg->setText(qtPath);		
		_inputImage = nqtPath.toLocal8Bit().data();
		_outputShape.clear();
		ui.leOutputShp->setText(QStringLiteral(""));
	}
}

void SegmentationDialog::on_btnOutputShp_clicked(bool checked)
{
	QFileDialog fileDlg((QWidget*)this);
	fileDlg.setFileMode(QFileDialog::AnyFile);
	fileDlg.setAcceptMode(QFileDialog::AcceptSave);
	fileDlg.setViewMode(QFileDialog::List);
	fileDlg.setDirectory(_initialDir);
	QStringList filters;
	filters << QStringLiteral("ESRI Shapefile (*.shp)");
	fileDlg.setNameFilters(filters);
	if (fileDlg.exec() == QDialog::Accepted)
	{
		QString qtPath = fileDlg.selectedFiles().at(0);
		QString nqtPath = QDir::toNativeSeparators(qtPath);
		QFileInfo finfo(qtPath);
		_initialDir = finfo.absolutePath();
		ui.leOutputShp->setText(nqtPath);
		_outputShape = nqtPath.toLocal8Bit().data();
	}
}

void SegmentationDialog::on_accepted()
{
	QString strGaussianCoef = ui.leGauss->text();
	_gaussianCoef = strGaussianCoef.toDouble();

	QString strTexturePeriod = ui.leTexturePeriod->text();
	_texturePeriod = strTexturePeriod.toInt();

	QString strLowPercentage = ui.leLowPercentage->text();
	_lowPercentage = strLowPercentage.toDouble();

	QString strHighPercentage = ui.leHighPercentage->text();
	_highPercentage = strHighPercentage.toDouble();

	QString strDamStrength = ui.leDamStrength->text();
	_damStrength = strDamStrength.toDouble();

	QString strPoolStrength = ui.lePoolStrength->text();
	_poolStrength = strPoolStrength.toDouble();

	_useTexture = ui.chkUseTexture->isChecked();
}
