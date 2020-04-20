#include "BuildPyramids.h"
#include "MyGDALDataset.h"
#include <string>

#pragma warning(disable: 4996)

BEGIN_NAME_SPACE(tGis, Core)

#define MAX_PRG_VALUE 100

int CPL_STDCALL BuildPyramidsPrgFunc(double dfComplete, const char *pszMessage, void *pProgressArg)
{
	ProgressEvent* prgEvent = (ProgressEvent*)pProgressArg;
	if (prgEvent != nullptr)
	{
		int totalValue = int(MAX_PRG_VALUE * dfComplete);
		if (totalValue > MAX_PRG_VALUE)
			totalValue = MAX_PRG_VALUE;

		Progress prg(totalValue,
			totalValue,
			MAX_PRG_VALUE,
			"正在创建金字塔");

		prgEvent->Raise(prg);
	}
	return TRUE;
}


TGIS_CORE_API bool NeedBuildPyramids(GDALDataset * raster, int sizeThreshold, int topLevelSize)
{
	GDALDriver* pDriver = raster->GetDriver();
	const char* drName = GDALGetDriverShortName(pDriver);

	if (EQUAL("MEM", drName))
		return false; //内存数据集不需要金字塔
	
	if (EQUAL("ECW", drName))
		return false; //ECW不需要金字塔

	GDALDataset* pDataset = raster;

	int iWidth = pDataset->GetRasterXSize();
	int iHeight = pDataset->GetRasterYSize();

	int iCurNum = max(iWidth, iHeight);

	int anLevels[1024] = { 0 };
	int nLevelCount = 0;                //金字塔级数

	do    //计算金字塔级数，从第二级到顶层
	{
		iCurNum /= 2;
		anLevels[nLevelCount] = static_cast<int>(pow(2.0, nLevelCount + 2));
		nLevelCount++;
	} while (iCurNum > topLevelSize);

	GDALRasterBand* band = pDataset->GetRasterBand(1);
	if (band->GetOverviewCount() < nLevelCount)
		return true;

	return false;
}

TGIS_CORE_API void BuildPyramids(GDALDataset * raster,
	ProgressEvent * progressEvent,
	int sizeThreshold,
	int topLevelSize)
{
	CPLSetConfigOption("USE_RRD", "YES");
	CPLSetConfigOption("TIFF_USE_OVR", "YES");
	GDALDataset* pDataset = raster;
	/* -------------------------------------------------------------------- */
	/*      Get File basic infomation                                       */
	/* -------------------------------------------------------------------- */
	int iWidth = pDataset->GetRasterXSize();
	int iHeight = pDataset->GetRasterYSize();

	int iCurNum = max(iWidth, iHeight);

	int anLevels[1024] = { 0 };
	int nLevelCount = 0;                //金字塔级数

	do    //计算金字塔级数，从第二级到顶层
	{
		iCurNum /= 2;
		anLevels[nLevelCount] = static_cast<int>(pow(2.0, nLevelCount + 2));
		nLevelCount++;
	} while (iCurNum > topLevelSize);

	/* -------------------------------------------------------------------- */
	/*      Generate overviews.                                             */
	/* -------------------------------------------------------------------- */

	GDALRasterBand* band = pDataset->GetRasterBand(1);
	if (band->GetOverviewCount() < nLevelCount)
	{
		const char      *pszResampling = "NEAREST"; //采样方式
		GDALProgressFunc pfnProgress = BuildPyramidsPrgFunc;//进度条

		CPLErr ret = pDataset->BuildOverviews(pszResampling, nLevelCount, anLevels, 0, NULL, pfnProgress, progressEvent);

		if (ret != CE_None)
		{
			CPLErrorNum errNum = CPLGetLastErrorNo();
			if(errNum != CPLE_NotSupported)
				BuildPyramidsPrgFunc(1.0, "金字塔创建失败！", progressEvent);
		}
		else
		{
			BuildPyramidsPrgFunc(1.0, "金字塔创建成功。", progressEvent);
		}
	}
}

END_NAME_SPACE(tGis, Core)

