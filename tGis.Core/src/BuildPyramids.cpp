#include "BuildPyramids.h"
#include "MyGDALDataset.h"
#include <string>

#pragma warning(disable: 4996)

BEGIN_NAME_SPACE(tGis, Core)

#define MAX_PRG_VALUE 100

int CPL_STDCALL BuildPyramidsPrgFunc(double dfComplete, const char *pszMessage, void *pProgressArg)
{
	ProgressEventHandler* prgHandler = (ProgressEventHandler*)pProgressArg;
	if (prgHandler != nullptr)
	{
		int totalValue = int(MAX_PRG_VALUE * dfComplete);
		if (totalValue > MAX_PRG_VALUE)
			totalValue = MAX_PRG_VALUE;

		Progress prg(totalValue,
			totalValue,
			MAX_PRG_VALUE,
			"���ڴ���������");

		(*prgHandler)(prg);
	}
	return TRUE;
}


TGIS_CORE_API bool NeedBuildPyramids(GDALDataset * raster, int sizeThreshold, int topLevelSize)
{
	GDALDriver* pDriver = raster->GetDriver();
	const char* drName = GDALGetDriverShortName(pDriver);

	if (EQUAL("MEM", drName))
		return false; //�ڴ����ݼ�����Ҫ������
	
	if (EQUAL("ECW", drName))
		return false; //ECW����Ҫ������

	GDALDataset* pDataset = raster;

	int iWidth = pDataset->GetRasterXSize();
	int iHeight = pDataset->GetRasterYSize();

	if (_tgis_max(iWidth, iHeight) < sizeThreshold)
		return false;

	sizeThreshold = topLevelSize*topLevelSize;

	int iPixelNum = iWidth * iHeight;    //ͼ���е�����Ԫ����
	int iCurNum = iPixelNum;

	int anLevels[1024] = { 0 };
	int nLevelCount = 0;                //����������

	do    //����������������ӵڶ���������
	{
		iCurNum /= 4;
		anLevels[nLevelCount] = static_cast<int>(pow(2.0, nLevelCount + 2));
		nLevelCount++;
	} while (iCurNum > sizeThreshold);

	GDALRasterBand* band = pDataset->GetRasterBand(1);
	if (band->GetOverviewCount() < nLevelCount)
		return true;

	return false;
}

TGIS_CORE_API void BuildPyramids(GDALDataset * raster,
	ProgressEventHandler * progressHandler,
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

	if (_tgis_max(iWidth, iHeight) < sizeThreshold)
		return;

	sizeThreshold = topLevelSize*topLevelSize;

	int iPixelNum = iWidth * iHeight;    //ͼ���е�����Ԫ����
	int iCurNum = iPixelNum;

	int anLevels[1024] = { 0 };
	int nLevelCount = 0;                //����������

	do    //����������������ӵڶ���������
	{
		iCurNum /= 4;
		anLevels[nLevelCount] = static_cast<int>(pow(2.0, nLevelCount + 2));
		nLevelCount++;
	} while (iCurNum > sizeThreshold);

	/* -------------------------------------------------------------------- */
	/*      Generate overviews.                                             */
	/* -------------------------------------------------------------------- */

	GDALRasterBand* band = pDataset->GetRasterBand(1);
	if (band->GetOverviewCount() < nLevelCount)
	{
		const char      *pszResampling = "NEAREST"; //������ʽ
		GDALProgressFunc pfnProgress = BuildPyramidsPrgFunc;//������

		CPLErr ret = pDataset->BuildOverviews(pszResampling, nLevelCount, anLevels, 0, NULL, pfnProgress, progressHandler);

		if (ret != CE_None)
		{
			CPLErrorNum errNum = CPLGetLastErrorNo();
			if(errNum != CPLE_NotSupported)
				BuildPyramidsPrgFunc(1.0, "����������ʧ�ܣ�", progressHandler);
		}
		else
		{
			BuildPyramidsPrgFunc(1.0, "�����������ɹ���", progressHandler);
		}
	}
}

END_NAME_SPACE(tGis, Core)

