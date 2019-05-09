#include "RegionMergingTool.h"
#include "RegionMergingDialog.h"
#include "QtHelper.h"
#include "tGisCore.h"
#include "algo/imgseg.h"
#include <QMessageBox>

using namespace tGis::Core;

RegionMergingTool RegionMergingTool::INSTANCE;


RegionMergingTool::RegionMergingTool()
{
}


RegionMergingTool::~RegionMergingTool()
{
}

const char * RegionMergingTool::GetName()
{
	return "Region Merging";
}

void RegionMergingTool::Execute()
{
	RegionMergingDialog dlg((QWidget*)QtHelper::INSTANCE.GetMainWindow());
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
			int bandCount = dataset->GetRasterCount();
			bool valid = true;
			for (int i = 0; i < bandCount; i++)
			{
				GDALRasterBand* band = dataset->GetRasterBand(i + 1);
				valid = band->GetRasterDataType() == GDT_Byte;
			}
			raster.Detach();

			if (valid)
			{
				int xSize = dataset->GetRasterXSize();
				int ySize = dataset->GetRasterYSize();

				ortho_algo::CImgSeg Seg;
				Seg.Init(dlg._colorWeight, dlg._compactWeight, dlg._smoothWeight, dlg._scale);
				Seg.RunSeg(dlg._inputImage.c_str(), dlg._outputShape.c_str());

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