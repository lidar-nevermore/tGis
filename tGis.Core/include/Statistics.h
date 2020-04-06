#pragma once

#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "Helper.h"


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

	Statistics(bool calcMean = false, bool calcVar = false);

	//此接口迭代计算时 和方差 均方差 标准差 都是粗略计算的近似值
	//此接口随用随取
	void Recursive(double x);

	//初始化 和方差 均方差 标准差 计算迭代
	void InitRecursiveVariance();

	//该接口迭代计算出准确的 和方差 均方差 标准差
	//该接口需要和InitRecursiveVariance及FinishRecursiveVariance配合使用
	void RecursiveVariance(double x);

	void FinishRecursiveVariance();
};


END_NAME_SPACE(tGis, Core)

#endif

