// tGis.CTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "tEntity.h"
#include "tUtility.h"
#include "TangRuiSegment.h"

using namespace tGis::Core;

int main()
{
	MyGDALFileRasterDataset raster;
	raster.Attach("E:\\ObjectSamples\\test.bmp", GA_ReadOnly,true);
	int band[] = { 1,2,3 };
	double min[] = { 0,0,0 };
	double max[] = { 255,255,255 };
	TangRuiSegment(raster.GetGDALDataset(), band, min, max, 3, 0, 0, 800, 800, 1.0, 7);
	system("pause");
    return 0;
}

