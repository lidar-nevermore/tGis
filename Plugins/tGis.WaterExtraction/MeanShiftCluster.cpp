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


bool MeanShiftCluster::Process(
	int entryCount,
	int radius,
	bool positive,
	const std::function<bool(double pix)>& func,
	bool isBalanced,
	int maxIt)
{
	//ֱ��ͼ���Ľڵ������ֵ
	double *centerPixValue = nullptr;
	//ֱ��ͼ��ĳ�����Ƿ��Ǿ�������
	int* clusterFlags = nullptr;
	//ֱ��ͼ��ÿ������Ư�����ĸ��������������е�ֵ����������������
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

	//�ҵ������Сֵ��Ȼ��ͳ��ֱ��ͼ
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

	//��ֱ��ͼ���о�ֵ�˲�
	for (int j = 0; j < entryCount; j++)
	{
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

	//��ֱ��ͼ�ϵļ���ֵ��
	vector<int> not_peak;
	for (int j = 0; j < entryCount; j++)
	{
		int n_3[3] = { -1,-1,-1 };
		int p_3[3] = { 0,1,0 };//0 ��ʾ�϶����Ǿֲ���ֵ 1 ��ʾ��ȷ���Ƿ��Ǿֲ���ֵ 2 ��ʾ�Ǿֲ���ֵ 
		if (j > 0)
		{
			p_3[0] = clusterFlags[j - 1];
			n_3[0] = convergeFlags[j - 1];
		}
		n_3[1] = convergeFlags[j];
		if (j + 1 < entryCount)
			n_3[2] = convergeFlags[j + 1];

		int n_max = max(n_3[0], max(n_3[1], n_3[2]));

		if (n_3[1] == n_max && j + 1 < entryCount)
		{
			if (n_3[2] < n_max)
			{
				p_3[2] = 0;
				clusterFlags[j + 1] = 0;
			}
			else if (n_3[2] == n_max)
			{
				p_3[1] = 1;
				clusterFlags[j] = 1;
				p_3[2] = 1;
				clusterFlags[j + 1] = 1;
			}
		}

		int may_peak_around = 0; // 2�����п����Ǽ�ֵ��ĵ���
		int	pro_peak_around = 0; // 2������֧�����ĵ��Ǽ�ֵ��ĵ���
		int	con_peak_around = 0; // 2�����з������ĵ��Ǽ�ֵ��ĵ���

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
	
	//���˾ֲ�����ֵ
	int diameter = radius + radius + 1;
	int maxIteration = maxIt;
	bool converged = false;
	for (int i = 0; converged == false && i < maxIteration; i++)
	{
		converged = true;

		for (int j = diameter; j <= entryCount; j++)
		{
			int clusterCount = 0;
			int maxPos = -1;
			int maxCount = -1;

			for (int k = j - diameter; k < j; k++)
			{
				if (clusterFlags[k] != 0)
				{
					clusterCount++;
					if (convergeFlags[k] > maxCount)
					{
						maxCount = convergeFlags[k];
						maxPos = k;
					}
				}
			}

			converged &= clusterCount <= 1;
			if (clusterCount > 1)
			{
				for (int k = j - diameter; k < j; k++)
				{
					clusterFlags[k] = 0;
				}
				clusterFlags[maxPos] = 2;
			}
		}
	}

	vector<int> cluster_centers;

	if(clusterFlags[0] == 0)
		cluster_centers.push_back(0);
	//�Ҿ�������
	for (int j = 0; j < entryCount; j++)
	{
		if (clusterFlags[j] > 0)
		{
			cluster_centers.push_back(j);
		}
	}
	if (clusterFlags[entryMax] == 0)
		cluster_centers.push_back(entryMax);

	//�ж����ķ�Χ
	_clusters.clear();
	int topPos = 0;
	int topCenPixCount = 0;
	double lowPixValue = minPixValue;
	for (int i = 0; i < cluster_centers.size() - 1; i++)
	{
		int pos_s = cluster_centers[i];
		int pos_e = cluster_centers[i + 1];
		int thresholdPos = (pos_s + pos_e)/2;

		_cluster_t c;
		c.Min = lowPixValue;
		c.Cen = centerPixValue[pos_s];
		c.CenPixCount = convergeFlags[pos_s];

		if ((pos_s+1) == pos_e)
		{
			c.Max = (centerPixValue[pos_s] + centerPixValue[pos_e]) / 2;
			lowPixValue = c.Max;
			_clusters.push_back(c);
		}
		else
		{			
			double w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
			for (int j = pos_s; j <= pos_e; j++)
			{
				w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
				for (int k = pos_s; k <= pos_e; k++)
				{
					if (k <= j)   //��������  
					{
						w0 += pixCounts[k];
						u0tmp += centerPixValue[k] * pixCounts[k];
					}
					else   //ǰ������  
					{
						w1 += pixCounts[k];
						u1tmp += centerPixValue[k] * pixCounts[k];
					}
				}

				double sum = w0 + w1;
				if (sum <= 0)
					break;
				u0 = u0tmp / w0;
				u1 = u1tmp / w1;
				u = (u0tmp + u1tmp) / sum;
				deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
				if (deltaTmp > deltaMax)
				{
					deltaMax = deltaTmp;
					thresholdPos = j;
				}
			}

			c.Max = centerPixValue[thresholdPos];
			lowPixValue = c.Max;
			_clusters.push_back(c);
		}

		if (topCenPixCount < c.CenPixCount
			|| (positive && topCenPixCount == c.CenPixCount))
		{
			topCenPixCount = c.CenPixCount;
			topPos = thresholdPos;
		}
	}

	int lastCen = cluster_centers.at(cluster_centers.size() - 1);
	_cluster_t c;
	c.Min = lowPixValue;
	c.Cen = centerPixValue[lastCen];
	c.Max = maxPixValue;
	c.CenPixCount = convergeFlags[lastCen];
	_clusters.push_back(c);


	//�����ʼ��ֵ
	int thresholdPos = 0;
	int pos_s = 0;
	int pos_e = entryMax;
	if (isBalanced == false)
	{
		pos_s = positive ? topPos : 0;
		pos_e = positive ? entryMax : topPos;
	}
	double w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (int j = pos_s; j <= pos_e; j++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (int k = pos_s; k <= pos_e; k++)
		{
			if (k <= j)   //��������  
			{
				w0 += pixCounts[k];
				u0tmp += centerPixValue[k] * pixCounts[k];
			}
			else   //ǰ������  
			{
				w1 += pixCounts[k];
				u1tmp += centerPixValue[k] * pixCounts[k];
			}
		}

		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = (u0tmp + u1tmp) / (w0 + w1);
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			thresholdPos = j;
		}
	}
	
	_initialThreshold = centerPixValue[thresholdPos];

	delete[] clusterFlags;
	delete[] centerPixValue;
	delete[] convergeFlags;
	delete[] pixCounts;

	return true;
}