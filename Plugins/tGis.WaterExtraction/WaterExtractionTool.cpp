#include "WaterExtractionTool.h"
#include "WaterExtrationDialog.h"
#include "QtHelper.h"
#include <QMessageBox>
#include "tGisCore.h"
#include "RasterBinarize.h"
#include "RasterSubset.h"
#include "MeanShiftCluster.h"
#include <exception>

using namespace tGis::Core;

WaterExtractionTool WaterExtractionTool::INSTANCE;

WaterExtractionTool::WaterExtractionTool()
{
	MyGDALDataset::GDALInit();
}


WaterExtractionTool::~WaterExtractionTool()
{
}

const char * WaterExtractionTool::GetName()
{
	return "Water Extraction";
}

void WaterExtractionTool::Execute()
{
	WaterExtrationDialog dlg((QWidget*)QtHelper::INSTANCE.GetMainWindow());
	if (dlg.exec() == QDialog::Accepted)
	{
		try
		{
			WaterExtract(dlg._inputImage,
				dlg._aoiImage,
				dlg._outputDir,
				dlg._subDataset,
				dlg._band,
				dlg._imageType,
				dlg._expThreshold,
				dlg._lowRange,
				dlg._highRange,
				dlg._histBins,
				dlg._radiusBins,
				dlg._maxIteration);

			QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
				QStringLiteral("消息"),
				QStringLiteral("水体提取已经完成！"),
				QMessageBox::Yes, QMessageBox::Yes);
		}
		catch (std::exception & ex)
		{
			QMessageBox::information((QWidget*)QtHelper::INSTANCE.GetMainWindow(),
				QStringLiteral("错误"),
				QString::fromLocal8Bit(ex.what()),
				QMessageBox::Yes, QMessageBox::Yes);
		}
	}
}

void WaterExtractionTool::WaterExtract(
	string& inputImage,
	string& aoiImage,
	string& outputDir,
	int subDataset,
	int band,
	int imgType,
	double expThreshold,
	double lowRange,
	double highRange,
	int histBins,
	int radiusBins,
	int maxIteration) 
{
	string&  imgPath = inputImage;
	int pathSepPos = imgPath.find_last_of(TGIS_PATH_SEPARATOR_CHAR);
	int extSepPos = imgPath.find_last_of(TGIS_EXT_SEPARATOR_CHAR);
	string  imgFile = imgPath.substr(pathSepPos + 1, extSepPos - pathSepPos - 1);
	string  imgExt = imgPath.substr(extSepPos + 1, imgPath.length() - extSepPos - 1);
	string&  outDir = outputDir;
	if (outDir.at(outDir.length() - 1) != TGIS_PATH_SEPARATOR_CHAR)
		outDir.append(TGIS_PATH_SEPARATOR_STR);

	string subPath(outDir);
	subPath.append(imgFile);
	subPath.append("_sub.tif");

	string outPath(outDir);
	outPath.append(imgFile);
	outPath.append("_ext.tif");

	MyGDALFileRasterDataset inRaster;
	if (stricmp(imgExt.c_str(), "hdf") == 0)
	{
		inRaster.AttachHDF(imgPath.c_str(), GA_ReadOnly, subDataset, true);
	}
	else
	{
		inRaster.Attach(imgPath.c_str(), GA_ReadOnly, true);
	}

	int xOffset;
	int yOffset;
	int xSize;
	int ySize;

	MyGDALFileRasterDataset aoi;
	if (aoiImage.empty())
	{
		xOffset = 0;
		yOffset = 0;
		xSize = inRaster.GetGDALDataset()->GetRasterXSize();
		ySize = inRaster.GetGDALDataset()->GetRasterYSize();
	}
	else
	{
		aoi.Attach(aoiImage.c_str(), GA_ReadOnly, true);

		RasterBandSeqPixelReader reader(inRaster.GetGDALDataset(), band, aoi.GetGDALDataset(), 1);

		xOffset = reader.GetXOffset();
		yOffset = reader.GetYOffset();
		xSize = reader.GetXSize();
		ySize = reader.GetYSize();
	}

	MeanShiftCluster meanshift(inRaster.GetGDALDataset(), band, aoi.GetGDALDataset(), 1);

	meanshift.Process(histBins, radiusBins,
		[lowRange, highRange]
	(double pix)->bool
	{
		if (pix<highRange && pix>lowRange)
			return true;
		return false;
	},
		maxIteration);

	int ccount = meanshift.GetClusterCount();
	double th = 0;

	//NDWI
	if (imgType == 0)
	{
		for (int c = 0; c < ccount; c++)
		{
			double low;
			double center;
			double heigh;
			meanshift.GetClusterInfo(c, &low, &center, &heigh);
			if (center > expThreshold)
			{
				th = low;
				break;
			}
		}
	}
	else if (imgType == 1) //NDVI
	{
		for (int c = ccount - 1; c >= 0; c--)
		{
			double low;
			double center;
			double heigh;
			meanshift.GetClusterInfo(c, &low, &center, &heigh);
			if (center < expThreshold)
			{
				th = heigh;
				break;
			}
		}
	}
	else //反射率
	{
		for (int c = 0; c < ccount; c++)
		{
			double low;
			double center;
			double heigh;
			meanshift.GetClusterInfo(c, &low, &center, &heigh);
			if (center > expThreshold)
			{
				th = heigh;
				break;
			}
		}
	}

	RasterBinarize binarize(inRaster.GetGDALDataset(), band, aoi.GetGDALDataset(), 1);

	int pixCount = binarize.Process(outPath.c_str(), 255,
		[imgType, th, lowRange, highRange]
	(double pix)->bool
	{
		//NDWI
		if (imgType == 0)
		{
			if (pix < highRange && pix >= th)
			{
				return true;
			}
		}
		else //NDVI或者反射率
		{
			if (pix > lowRange && pix <= th)
			{
				return true;
			}
		}
		return false;
	});

	RasterSubset subset(inRaster.GetGDALDataset(), xOffset, yOffset, xSize, ySize);
	subset.Process(subPath.c_str());
}
