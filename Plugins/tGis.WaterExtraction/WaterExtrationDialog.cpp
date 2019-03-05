#include "WaterExtrationDialog.h"

#include <QFileDialog>
#include <QMessageBox>

#include "QtHelper.h"

#include "tGisCore.h"

using namespace tGis::Core;

#include <algorithm>
using namespace std;

QString WaterExtrationDialog::_initialDir = QDir::homePath();

WaterExtrationDialog::WaterExtrationDialog(QWidget *parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	ui.cboInputType->addItem(QStringLiteral("NDWI"));
	ui.cboInputType->addItem(QStringLiteral("NDVI"));
	ui.cboInputType->addItem(QStringLiteral("REF"));

	connect(ui.btnInputImage, &QPushButton::clicked, this, &WaterExtrationDialog::on_btnInputImage_clicked);
	connect(ui.btnAoiImage, &QPushButton::clicked, this, &WaterExtrationDialog::on_btnAoiImage_clicked);
	connect(ui.btnOutputDir, &QPushButton::clicked, this, &WaterExtrationDialog::on_btnOutputDir_clicked);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &WaterExtrationDialog::on_accepted);
}


WaterExtrationDialog::~WaterExtrationDialog()
{
}


void WaterExtrationDialog::on_btnInputImage_clicked(bool checked)
{
	QFileDialog fileDlg((QWidget*)this);
	fileDlg.setFileMode(QFileDialog::ExistingFile);
	fileDlg.setAcceptMode(QFileDialog::AcceptOpen);
	fileDlg.setViewMode(QFileDialog::List);
	fileDlg.setDirectory(_initialDir);

	QStringList filters;
	filters << QStringLiteral("常用格式 (*.tif *.tiff *.img *.pix *.hdf *.hdf5)");

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
		string strPath = nqtPath.toLocal8Bit().data();
		GDALDataset* pd = (GDALDataset*)GDALOpen(strPath.c_str(), GA_ReadOnly);

		if (pd == NULL)
		{
			QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
				QStringLiteral("错误"),
				QStringLiteral("无法打开，不是有效的影像文件!"),
				QMessageBox::Yes, QMessageBox::Yes);

			return;
		}

		if (qtPath.endsWith(QStringLiteral("hdf"), Qt::CaseInsensitive))
		{
			char ** papszSUBDATASETS = GDALGetMetadata((GDALDatasetH)pd, "SUBDATASETS");
			int iCount = papszSUBDATASETS == NULL ? 0 : CSLCount(papszSUBDATASETS);
			if (iCount == 0)
			{
				QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
					QStringLiteral("错误"),
					QStringLiteral("没有子数据集，不是有效的HDF文件!"),
					QMessageBox::Yes, QMessageBox::Yes);

				GDALClose(pd);
				return;
			}

			for (int i = 0; i < iCount; i++)
			{
				std::string tmpstr = std::string(papszSUBDATASETS[i]);
				size_t espos = tmpstr.find_first_of("=");
				std::string subitem = tmpstr.substr(0, espos);
				transform(subitem.begin(), subitem.end(), subitem.begin(), ::toupper);

				if (subitem.rfind("NAME") == espos - 4)
				{
					size_t uspos = subitem.find_last_of("_");
					//SUBDATASET_num_NAME
					std::string num = subitem.substr(11, uspos - 11);
					int subnum = atoi(num.c_str());

					size_t infpos = tmpstr.rfind("\":");
					std::string inf = tmpstr.substr(infpos + 2);
					QString qstrItem = QString::fromLocal8Bit(num.data());
					qstrItem.append(QStringLiteral(":"));
					qstrItem.append(QString::fromLocal8Bit(inf.data()));
					ui.cboSubDataset->addItem(qstrItem);
				}
			}
		}
		else
		{
			int bandCount = pd->GetRasterCount();

			for (int i = 1; i <= bandCount; i++)
			{
				ui.cboInputBand->addItem(QString::asprintf("Band %d", i));
			}
		}

		GDALClose(pd);

		QFileInfo finfo(qtPath);
		_initialDir = finfo.absolutePath();
		ui.leInputImage->setText(nqtPath);
		_inputImage = strPath;
	}
}


void WaterExtrationDialog::on_btnAoiImage_clicked(bool checked)
{
	QFileDialog fileDlg((QWidget*)this);
	fileDlg.setFileMode(QFileDialog::ExistingFile);
	fileDlg.setAcceptMode(QFileDialog::AcceptOpen);
	fileDlg.setViewMode(QFileDialog::List);
	fileDlg.setDirectory(_initialDir);

	QStringList filters;
	filters << QStringLiteral("GeoTiff (*.tif *.tiff)");

	fileDlg.setNameFilters(filters);

	if (fileDlg.exec() == QDialog::Accepted)
	{
		QString qtPath = fileDlg.selectedFiles().at(0);
		QString nqtPath = QDir::toNativeSeparators(qtPath);
		string strPath = nqtPath.toLocal8Bit().data();
		GDALDataset* pd = (GDALDataset*)GDALOpen(strPath.c_str(), GA_ReadOnly);

		if (pd == NULL)
		{
			QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
				QStringLiteral("错误"),
				QStringLiteral("无法打开，不是有效的影像文件!"),
				QMessageBox::Yes, QMessageBox::Yes);

			return;
		}

		GDALRasterBand* aoiBand = pd->GetRasterBand(1);

		if (aoiBand == NULL || aoiBand->GetRasterDataType() != GDT_Byte)
		{
			QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
				QStringLiteral("错误"),
				QStringLiteral("不是有效的AOI影像！AOI影像的第一波段是AOI，且类型必须是UINT8。"),
				QMessageBox::Yes, QMessageBox::Yes);

			GDALClose(pd);
			return;
		}

		QFileInfo finfo(qtPath);
		_initialDir = finfo.absolutePath();
		ui.leAoiImage->setText(nqtPath);
		_aoiImage = strPath;
	}
}

void WaterExtrationDialog::on_btnOutputDir_clicked(bool checked)
{
	QFileDialog fileDlg((QWidget*)this);
	fileDlg.setFileMode(QFileDialog::DirectoryOnly);
	fileDlg.setAcceptMode(QFileDialog::AcceptOpen);
	fileDlg.setViewMode(QFileDialog::List);
	fileDlg.setDirectory(_initialDir);

	if (fileDlg.exec() == QDialog::Accepted)
	{
		QString qtPath = fileDlg.selectedFiles().at(0);
		QString nqtPath = QDir::toNativeSeparators(qtPath);
		ui.leOutputDir->setText(nqtPath);
		_outputDir = nqtPath.toLocal8Bit().data();
	}
}

void WaterExtrationDialog::on_accepted()
{
	_subDataset = ui.cboSubDataset->currentIndex() + 1;
	_band = ui.cboInputBand->currentIndex() + 1;
	_imageType = ui.cboInputType->currentIndex();

	QString strExpThreshold = ui.leExpThreshold->text();
	_expThreshold = strExpThreshold.toDouble();

	QString strLowRange = ui.leLowRange->text();
	_lowRange = strLowRange.toDouble();

	QString strHighRange = ui.leHighRange->text();
	_highRange = strHighRange.toDouble();

	QString strHistBins = ui.leHistogramBins->text();
	_histBins = strHistBins.toInt();

	QString strRadiusBins = ui.leRadiusBins->text();
	_radiusBins = strRadiusBins.toInt();

	QString strIteration = ui.leIteration->text();
	_maxIteration = strIteration.toInt();
}


