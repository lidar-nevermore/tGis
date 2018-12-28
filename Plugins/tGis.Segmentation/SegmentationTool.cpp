#include "SegmentationTool.h"
#include "SegmentationDialog.h"
#include "QtHelper.h"
#include "tGisCore.h"
#include "TangRuiSegment.h"
#include <QMessageBox>

using namespace tGis::Core;

SegmentationTool SegmentationTool::INSTANCE;


SegmentationTool::SegmentationTool()
{
}


SegmentationTool::~SegmentationTool()
{
}

const char * SegmentationTool::GetName()
{
	return "Dam Breaking";
}

void SegmentationTool::Execute()
{
	SegmentationDialog dlg((QWidget*)QtHelper::INSTANCE.GetMainWindow());
	if (dlg.exec() == QDialog::Accepted)
	{
		if (dlg.GetInputImage() == NULL)
			return;

		if (dlg.GetOutputShape() == NULL)
			return;

		try
		{
			MyGDALFileRasterDataset raster;
			raster.Attach(dlg.GetInputImage(), GA_ReadOnly, true);
			GDALDataset *dataset = raster.GetGDALDataset();
			int band[] = { 1,2,3 };
			double min[] = { 0,0,0 };
			double max[] = { 255,255,255 };
			int bandCount = min(3,dataset->GetRasterCount());
			bool valid = true;
			for (int i = 0; i < bandCount; i++)
			{
				GDALRasterBand* band = dataset->GetRasterBand(i + 1);
				valid = band->GetRasterDataType() == GDT_Byte;
			}

			if (valid)
			{
				int xSize = dataset->GetRasterXSize();
				int ySize = dataset->GetRasterYSize();

				TangRuiSegment(dataset, dlg.GetOutputShape(),
					band, min, max, bandCount,
					0, 0, min(1000, xSize), min(1000, ySize), 1.0, 7);

				QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
					QStringLiteral("消息"),
					QStringLiteral("分割完成。"),
					QMessageBox::Yes, QMessageBox::Yes);
			}
			else
			{
				QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
					QStringLiteral("错误"),
					QStringLiteral("暂只支持8位影像"),
					QMessageBox::Yes, QMessageBox::Yes);
			}
		}
		catch (exception &ex)
		{
			QString msg;
			msg.arg(QStringLiteral("异常：%s"), ex.what());
			QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
				QStringLiteral("错误"),
				msg,
				QMessageBox::Yes, QMessageBox::Yes);
		}

	}
}
