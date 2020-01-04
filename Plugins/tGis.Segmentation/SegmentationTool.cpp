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
	return "Dam Razing";
}

void SegmentationTool::Execute()
{
	SegmentationDialog dlg((QWidget*)QtHelper::INSTANCE.GetMainWindow());
	if (dlg.exec() == QDialog::Accepted)
	{
		if (dlg._inputImage.empty())
			return;

		if (dlg._outputShape.empty())
			return;

		try
		{
			MyGDALFileRasterDataset raster;
			raster.Attach(dlg._inputImage.c_str(), GA_ReadOnly, true);
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

				TangRuiSegment(dataset, dlg._outputShape.c_str(),
					band, min, max, bandCount,
					0, 0, min(1000, xSize), min(1000, ySize),
					dlg._gaussianCoef,
					dlg._texturePeriod,
					2,
					1000,
					0.03,
					0.3,
					0.23,
					1.00,
					dlg._lowPercentage,
					dlg._highPercentage
					);

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
