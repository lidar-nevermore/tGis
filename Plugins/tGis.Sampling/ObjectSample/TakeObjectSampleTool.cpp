#include "TakeObjectSampleTool.h"
#include "ObjectSampleDialog.h"

#include "tUtility.h"
#include "tOrganizer.h"

#include <QMessageBox>
#include <QDir>

#include "QtHelper.h"

using namespace tGis::Core;



TakeObjectSampleTool::TakeObjectSampleTool()
{
}


TakeObjectSampleTool::~TakeObjectSampleTool()
{
}

void TakeObjectSampleTool::SetRasterLayer(ILayer * layer)
{
	_layer = layer;
	_dataset = (MyGDALRasterDataset*)layer->GetDataset();
}

void TakeObjectSampleTool::SetObjectSampleDataSource(ObjectSampleDataSource * samples)
{
	_samples = samples;
}

void TakeObjectSampleTool::MouseUp(void *ev)
{
	DrawRectTool::MouseUp(ev);
	GeoViewPort* viewPort = _mapWidget->GetViewPort();
	double spatialLeft;
	double spatialTop;
	double spatialRight;
	double spatialBottom;
	viewPort->Surface2Spatial(_rect._left, _rect._top, &spatialLeft, &spatialTop);
	viewPort->Surface2Spatial(_rect._right, _rect._bottom, &spatialRight, &spatialBottom);

	double pixLeftd;
	double pixTopd;
	double pixRightd;
	double pixBottomd;

	_dataset->Spatial2Pixel(spatialLeft, spatialTop, &pixLeftd, &pixTopd);
	_dataset->Spatial2Pixel(spatialRight, spatialBottom, &pixRightd, &pixBottomd);

	int sampleCenterX = (int)((pixLeftd + pixRightd) / 2);
	int sampleCenterY = (int)((pixBottomd + pixTopd) / 2);
	int sampleSizeX = ((int)abs(pixRightd - pixLeftd)) | 1;
	int sampleSizeY = ((int)abs(pixBottomd - pixTopd)) | 1;
	int sampleSize = ((int)sqrt(sampleSizeX*sampleSizeX + sampleSizeY*sampleSizeY)) | 1;

	int pixLeft = sampleCenterX - sampleSize / 2;
	int pixRight = sampleCenterX + sampleSize / 2;
	int pixTop = sampleCenterY - sampleSize / 2;
	int pixBottom = sampleCenterY + sampleSize / 2;

	int rasterXSize = _dataset->GetGDALDataset()->GetRasterXSize();
	int rasterYSize = _dataset->GetGDALDataset()->GetRasterYSize();

	if (pixLeft < 0 || pixRight >= rasterXSize || pixTop < 0 || pixBottom >= rasterYSize)
	{
		QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
			QStringLiteral("Warning"),
			QStringLiteral("即将扣取的样本范围超过文件范围！"),
			QMessageBox::Yes, QMessageBox::Yes);
		return;
	}

	//TODO: 创建一个内存数据集，然后显示到加标签的对话框
	MyGDALMemRasterDataset* memDataset = MemoryDataSourceProvider::INSTANCE().CreateMemRasterDataset(_dataset, pixLeft, pixTop, sampleSize, sampleSize, -1);

	ObjectSampleDialog dlg((QWidget*)QtHelper::INSTANCE.GetMainWindow());
	dlg.SetObjectSampleDataSource(_samples);
	ILayer* layer = _layer->Clone(memDataset);
	dlg.SetLayer(layer);
	double sampleLeft;
	double sampleTop;
	double sampleRight;
	double sampleBottom;
	memDataset->Pixel2Spatial((sampleSize - sampleSizeX) / 2, (sampleSize - sampleSizeY) / 2, &sampleLeft, &sampleTop);
	memDataset->Pixel2Spatial((sampleSize + sampleSizeX) / 2, (sampleSize + sampleSizeY) / 2, &sampleRight, &sampleBottom);
	dlg.SetSampleArea(sampleLeft, sampleTop, sampleRight, sampleBottom);

	if (dlg.exec() == QDialog::Accepted)
	{
		ObjectSampleMetadata* osm = dlg.GetObjectSampleMetadata();

		QString sampleSavePath = QString::fromLocal8Bit(_samples->GetCreationString());
		sampleSavePath.append(TGIS_PATH_SEPARATOR_CHAR);
		sampleSavePath.append(QString::fromUtf8(osm->Name));
		QDir dir(sampleSavePath);
		if (!dir.exists())
			dir.mkdir(sampleSavePath);
		time_t tmt;
		struct tm tms;
		time(&tmt);
		_tgis_localtime(&tmt, &tms);
		char buffer[100] = { 0 };
		sprintf(buffer, "%d_%d_%d_%04d%02d%02d%02d%02d%02d.tif", osm->Label, sampleSizeX, sampleSizeY, tms.tm_year, tms.tm_mon, tms.tm_mday, tms.tm_hour, tms.tm_min, tms.tm_sec);
		sampleSavePath.append(TGIS_PATH_SEPARATOR_CHAR);
		sampleSavePath.append(QString::fromLocal8Bit(buffer));

		GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
		GDALDataset* fileDataset = poDriver->CreateCopy(sampleSavePath.toLocal8Bit(), memDataset->GetGDALDataset(), TRUE, NULL, NULL, NULL);
		GDALClose(fileDataset);
	}

	MemoryDataSourceProvider::INSTANCE().ReleaseMemRasterDataset(memDataset);
}






