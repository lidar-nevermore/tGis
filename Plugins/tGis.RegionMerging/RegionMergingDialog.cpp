#include "RegionMergingDialog.h"
#include "MyGDALDataset.h"
#include <QFileDialog>

using namespace tGis::Core;

QString RegionMergingDialog::_initialDir = QDir::homePath();

RegionMergingDialog::RegionMergingDialog(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	_colorWeight = 0.4;
	_compactWeight = 0.5;
	_smoothWeight = 0.5;
	_scale = 100;

	connect(ui.btnInputImg, &QPushButton::clicked, this, &RegionMergingDialog::on_btnInputImg_clicked);
	connect(ui.btnOutputShp, &QPushButton::clicked, this, &RegionMergingDialog::on_btnOutputShp_clicked);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &RegionMergingDialog::on_accepted);
}


RegionMergingDialog::~RegionMergingDialog()
{
}

void RegionMergingDialog::on_btnInputImg_clicked(bool checked)
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

void RegionMergingDialog::on_btnOutputShp_clicked(bool checked)
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

void RegionMergingDialog::on_accepted()
{
	QString strColorWeight = ui.leColor->text();
	_colorWeight = strColorWeight.toDouble();

	QString strCompactWeight = ui.leCompact->text();
	_compactWeight = strCompactWeight.toDouble();

	QString strSmoothWeight = ui.leSmooth->text();
	_smoothWeight = strSmoothWeight.toDouble();

	QString strScale = ui.leScale->text();
	_scale = strScale.toDouble();
}
