#include "TakeObjectSampleTool.h"
#include "tOrganizer.h"

#include <QMessageBox>

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

TakeObjectSampleTool::TakeObjectSampleTool()
{
}


TakeObjectSampleTool::~TakeObjectSampleTool()
{
}

void TakeObjectSampleTool::SetRasterDataset(MyGDALRasterDataset * dataset)
{
	_dataset = dataset;
}

void TakeObjectSampleTool::SetObjectSampleDataSource(ObjectSampleDataSource * samples)
{
	_samples = samples;
}

void TakeObjectSampleTool::MouseUp(void *ev)
{
	DrawRectTool::MouseUp(ev);
	IGeoSurface* surf = _mapWidget->GetGeoSurface();
	double spatialLeft;
	double spatialTop;
	double spatialRight;
	double spatialBottom;
	surf->Surface2Spatial(_rect._left, _rect._top, &spatialLeft, &spatialTop);
	surf->Surface2Spatial(_rect._right, _rect._bottom, &spatialRight, &spatialBottom);

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
		QMessageBox::information(nullptr,
			QStringLiteral("Warning"),
			QStringLiteral("������ȡ��������Χ�����ļ���Χ��"),
			QMessageBox::Yes, QMessageBox::Yes);
		return;
	}

	//TODO: ����һ���ڴ����ݼ���Ȼ����ʾ���ӱ�ǩ�ĶԻ���
	MyGDALMemRasterDataset* memDataset = MemoryDataSourceProvider::INSTANCE().CreateMemRasterDataset(_dataset, pixLeft, pixTop, sampleSize, sampleSize, -1, nullptr, true);
}


END_NAME_SPACE(tGis, Utility)



