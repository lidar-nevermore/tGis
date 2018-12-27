#pragma once

#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "Helper.h"
#include "tGisCoreCfg.h"

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API Statistics
{
	double max;
	double min;
	double mean;
	double sum;
	bool   calcmean;//是否计算和值
	bool   calcvar;//是否计算方差
	double sumvar;//和方差
	double meanvar;//均方差
	double stddev;//标准差
	size_t count;//数量

	//初始化统计信息计算迭代
	void InitRecursive(double x,bool calcMean = false, bool calcVar = false);

	//一次迭代计算时 和方差 均方差 标准差 都是粗略计算的近似值
	void Recursive(double x);

	//初始化 和方差 均方差 标准差 计算迭代
	void InitRecursiveVariance();

	//若要计算出准确的 和方差 均方差 标准差，用此方法再迭代一次所有数据
	void RecursiveVariance(double x);

	void FinishRecursiveVariance();
};


END_NAME_SPACE(tGis, Core)

#endif

