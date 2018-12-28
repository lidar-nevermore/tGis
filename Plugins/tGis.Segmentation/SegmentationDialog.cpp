#include "SegmentationDialog.h"
#include "MyGDALDataset.h"
#include <QFileDialog>

using namespace tGis::Core;

SegmentationDialog::SegmentationDialog(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	_initialDir = QDir::homePath();
	connect(ui.btnInputImg, &QPushButton::clicked, this, &SegmentationDialog::on_btnInputImg_clicked);
	connect(ui.btnOutputShp, &QPushButton::clicked, this, &SegmentationDialog::on_btnOutputShp_clicked);
}


SegmentationDialog::~SegmentationDialog()
{
}

const char * SegmentationDialog::GetInputImage()
{
	if (_inputImage.empty())
		return NULL;
	return _inputImage.c_str();
}

const char * SegmentationDialog::GetOutputShape()
{
	if (_outputShape.empty())
		return NULL;
	return _outputShape.c_str();
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
		QFileInfo finfo(qtPath);
		_initialDir = finfo.absolutePath();
		ui.leInputImg->setText(qtPath);
		QString path = QDir::toNativeSeparators(qtPath);
		_inputImage = path.toLocal8Bit().data();
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
		QFileInfo finfo(qtPath);
		_initialDir = finfo.absolutePath();
		ui.leOutputShp->setText(qtPath);
		QString path = QDir::toNativeSeparators(qtPath);
		_outputShape = path.toLocal8Bit().data();
	}
}
