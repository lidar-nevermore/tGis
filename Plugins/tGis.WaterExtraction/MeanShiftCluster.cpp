#include "MeanShiftCluster.h"

#include "tGisCore.h"

using namespace tGis::Core;

#include "ogrsf_frmts.h"
#include <algorithm>
#include <vector>

using namespace std;

MeanShiftCluster::MeanShiftCluster(GDALDataset* raster, int band, GDALDataset* aoiRaster, const int aoiBand)
{
	_raster = raster;
	_band = raster->GetRasterBand(band);

	_aoiRaster = aoiRaster;
	_aoiBand = aoiBand;
}


MeanShiftCluster::~MeanShiftCluster()
{
}

int MeanShiftCluster::GetClusterCount()
{
	return _clusters.size();
}

void MeanShiftCluster::GetClusterInfo(int cluster, double * low, double * center, double * heigh)
{
	_cluster_t & c = _clusters.at(cluster);
	if (low != nullptr)
		*low = c.Min;
	if (center != nullptr)
		*center = c.Cen;
	if (heigh != nullptr)
		*heigh = c.Max;
}

struct PixelValueStater
{
	double aoiNoDataValue;
	double minPixValue;
	double maxPixValue;
	double sumPixValue;
	int totalPixCount;
	std::function<bool(double pix)> func;

	double avgPixValue;
	double sumPixVariance;
	double pixRange;
	int entryCount;
	int entryMax;
	int* pixCounts;
};

void PixelValueStat1(void* user, GDALRasterBand* band, double pix, int x, int y, void* orgPix, StorageBlockBuffer* block, int xPosIB, int yPosIB)
{
	PixelValueStater* stater = (PixelValueStater*)user;

	if (pix != stater->aoiNoDataValue && stater->func(pix))
	{
		stater->totalPixCount++;
		stater->sumPixValue += pix;
		if (pix < stater->minPixValue)
			stater->minPixValue = pix;
		if (pix > stater->maxPixValue)
			stater->maxPixValue = pix;
	}
}

void PixelValueStat2(void* user, GDALRasterBand* band, double pix, int x, int y, void* orgPix, StorageBlockBuffer* block, int xPosIB, int yPosIB)
{
	PixelValueStater* stater = (PixelValueStater*)user;

	if (pix != stater->aoiNoDataValue && stater->func(pix))
	{
		int pos = (int)(stater->entryCount*(pix - stater->minPixValue) / stater->pixRange);
		if (pos < 0)
			pos = 0;
		else if (pos > stater->entryMax)
			pos = stater->entryMax;
		stater->pixCounts[pos]++;

		stater->sumPixVariance += (pix - stater->avgPixValue)*(pix - stater->avgPixValue);
	}
}


bool MeanShiftCluster::Process(int entryCount, int radius, const std::function<bool(double pix)>& func, int maxIt)
{
	//直方图中心节点的像素值
	double *centerPixValue = nullptr;
	//直方图的某个格是否是聚类中心
	int* clusterFlags = nullptr;
	//直方图上每个格子漂移至的格子索引，格子中的值等于其索引则收敛
	int* convergeFlags = nullptr;
	int* pixCounts = nullptr;
	int entryMax = entryCount - 1;

	try
	{
		centerPixValue = new double[entryCount];
		clusterFlags = new int[entryCount];
		convergeFlags = new int[entryCount];
		pixCounts = new int[entryCount]();
	}
	catch (std::bad_alloc& e)
	{
		if (centerPixValue != nullptr)
			delete[] centerPixValue;
		if (clusterFlags != nullptr)
			delete[] clusterFlags;
		if (convergeFlags != nullptr)
			delete[] convergeFlags;
		if (pixCounts != nullptr)
			delete[] pixCounts;

		return false;
	}

	//找到最大最小值，然后统计直方图
	double minPixValue = DBL_MAX;
	double maxPixValue = -DBL_MAX;
	double sumPixValue = 0;
	double avgPixValue = 0;
	double sumPixVariance = 0;
	int totalPixCount = 0;

	double aoiNoDataValue;
	int hasNoDataValue = 0;
	aoiNoDataValue = _band->GetNoDataValue(&hasNoDataValue);
	if (hasNoDataValue == 0)
	{
		aoiNoDataValue = INT_MIN;
	}

	PixelValueStater stater;
	stater.aoiNoDataValue = aoiNoDataValue;
	stater.minPixValue = minPixValue;
	stater.maxPixValue = maxPixValue;
	stater.sumPixValue = sumPixValue;
	stater.totalPixCount = totalPixCount;
	stater.func = func;

	RasterBandSeqPixelReader reader(_raster, _band, _aoiRaster, _aoiBand);
	reader.ForEachPixel(PixelValueStat1, &stater);

	aoiNoDataValue = stater.aoiNoDataValue;
	minPixValue = stater.minPixValue;
	maxPixValue = stater.maxPixValue;
	sumPixValue = stater.sumPixValue;
	totalPixCount = stater.totalPixCount;

	if (totalPixCount == 0)
	{
		delete[] centerPixValue;
		delete[] clusterFlags;
		delete[] convergeFlags;
		delete[] pixCounts;

		return false;
	}

	avgPixValue = sumPixValue / totalPixCount;

	double pixRange = maxPixValue - minPixValue;
	double halfPixStep = pixRange / (2 * entryCount);

	for (int j = 0; j < entryCount; j++)
	{
		centerPixValue[j] = j*pixRange / entryCount + minPixValue + halfPixStep;
	}

	stater.avgPixValue = avgPixValue;
	stater.sumPixVariance = sumPixVariance;
	stater.pixRange = pixRange;
	stater.entryCount = entryCount;
	stater.entryMax = entryMax;
	stater.pixCounts = pixCounts;

	reader.ResetPixelReading();
	reader.ForEachPixel(PixelValueStat2, &stater);
	sumPixVariance = stater.sumPixVariance;

	//对直方图进行均值滤波
	for (int j = 0; j < entryCount; j++)
	{
		clusterFlags[j] = 1;

		int lowPos = j - radius;
		if (lowPos < 0)
			lowPos = 0;
		int heighPos = j + radius;
		if (heighPos > entryMax)
			heighPos = entryMax;

		double sumSpanPixCount = 0;

		for (int k = lowPos; k <= heighPos; k++)
		{
			sumSpanPixCount += pixCounts[k];
		}

		convergeFlags[j] = (int)(sumSpanPixCount / (heighPos - lowPos + 1));
	}

	//找直方图上的极大值点
	vector<int> not_peak;
	for (int j = 0; j < entryCount; j++)
	{
		int n_3[3] = { -1,-1,-1 };
		int p_3[3] = { 0,1,0 };//0 表示肯定不是局部峰值 1 表示不确定是否是局部峰值 2 表示是局部峰值 
		if (j > 0)
		{
			n_3[0] = convergeFlags[j - 1];
			p_3[0] = clusterFlags[j - 1];
		}
		n_3[1] = convergeFlags[j];
		p_3[1] = clusterFlags[j];
		if (j + 1 < entryCount)
		{
			n_3[2] = convergeFlags[j + 1];
			p_3[2] = clusterFlags[j + 1];
		}

		int n_max = max(n_3[0], max(n_3[1], n_3[2]));
		int may_peak_around = 0; // 2领域中可能是极值点的点数
		int	pro_peak_around = 0; // 2领域中支持中心点是极值点的点数
		int	con_peak_around = 0; // 2领域中反对中心点是极值点的点数

		for (int k = 0; k < 3; k++)
		{
			if (k == 1)
				continue;
			if (p_3[k] == 1)
				may_peak_around += 1;
			if (n_3[1] == n_max)
			{
				if (n_3[k] == n_max)
				{
					if (p_3[k] == 2)
						pro_peak_around += 1;
					else if (p_3[k] == 0)
						con_peak_around += 1;
				}
				else
					pro_peak_around += 1;
			}
			else
				con_peak_around += 1;
		}

		if (con_peak_around > 0)
		{
			clusterFlags[j] = 0;
			if (may_peak_around > 0)
				not_peak.push_back(j);
		}
		else if(pro_peak_around == 2)
			clusterFlags[j] = 2;
		else
			clusterFlags[j] = 1;
	}

	while (not_peak.size() > 0)
	{
		int npeak = not_peak.back();
		not_peak.pop_back();
		int ns = npeak - 1;
		int	ne = npeak + 2;
		if (ns < 0)
			ns = npeak;
		if (ne > entryCount)
			ne = entryCount;
		for (int k = ns; k < ne; k++)
		{
			if (k != npeak)
			{
				if (clusterFlags[k] == 1 && convergeFlags[k] <= convergeFlags[npeak])
				{
					clusterFlags[k] = 0;
					not_peak.push_back(k);
				}
			}
		}
	}
	
	//过滤局部极大值
	int maxIteration = maxIt;
	bool converged = false;
	for (int i = 0; converged == false && i < maxIteration; i++)
	{
		converged = true;

		for (int j = radius; j <= entryCount; j++)
		{
			int clusterCount = 0;
			int maxPos = j - radius;
			int maxCount = pixCounts[maxPos];

			for (int k = maxPos; k < j; k++)
			{
				if (clusterFlags[k] != 0)
				{
					clusterCount++;
					if (pixCounts[k] > maxCount)
					{
						maxCount = pixCounts[k];
						maxPos = k;
					}
				}
			}

			converged &= clusterCount <= 1;
			if (clusterCount > 1)
			{
				for (int k = j - radius; k < j; k++)
				{
					clusterFlags[k] = 0;
				}
				clusterFlags[maxPos] = 2;
			}
		}
	}

	vector<int> cluster_centers;
	//找聚类中心
	for (int j = 0; j < entryCount; j++)
	{
		if (clusterFlags[j] > 0)
		{
			cluster_centers.push_back(j);
		}
	}

	//判定类别的范围
	_clusters.clear();
	double lowPixValue = minPixValue;
	for (int i = 0; i < cluster_centers.size() - 1; i++)
	{
		double totalPixCount = 0;
		int thresholdPos = 0;
		int pos_s = cluster_centers[i];
		int pos_e = cluster_centers[i + 1];
		for (int j = pos_s; j <= pos_e; j++)
		{
			totalPixCount += pixCounts[j];
		}

		if (totalPixCount == 0 || (pos_s+1) == pos_e)
		{
			_cluster_t c;
			c.Min = lowPixValue;
			c.Cen = centerPixValue[pos_s];
			c.Max = (centerPixValue[pos_s] + centerPixValue[pos_e]) / 2;
			lowPixValue = c.Max;
			_clusters.push_back(c);
			continue;
		}

		double w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
		for (int j = pos_s; j <= pos_e; j++)
		{
			w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
			for (int k = pos_s; k <= pos_e; k++)
			{
				if (k <= j)   //背景部分  
				{
					w0 += pixCounts[k];
					u0tmp += centerPixValue[k] * pixCounts[k];
				}
				else   //前景部分  
				{
					w1 += pixCounts[k];
					u1tmp += centerPixValue[k] * pixCounts[k];
				}
			}

			u0 = u0tmp / w0;
			u1 = u1tmp / w1;
			u = (u0tmp + u1tmp) / totalPixCount;
			deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
			if (deltaTmp > deltaMax)
			{
				deltaMax = deltaTmp;
				thresholdPos = j;
			}
		}

		_cluster_t c;
		c.Min = lowPixValue;
		c.Cen = centerPixValue[pos_s];
		c.Max = centerPixValue[thresholdPos];
		lowPixValue = c.Max;
		_clusters.push_back(c);
	}

	_cluster_t c;
	c.Min = lowPixValue;
	c.Cen = centerPixValue[cluster_centers[cluster_centers.size()-1]];
	c.Max = centerPixValue[entryMax];
	_clusters.push_back(c);

	delete[] clusterFlags;
	delete[] centerPixValue;
	delete[] convergeFlags;
	delete[] pixCounts;

	return true;
}
