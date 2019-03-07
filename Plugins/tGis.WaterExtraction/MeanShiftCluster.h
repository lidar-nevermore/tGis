#pragma once
#include "gdal.h"
#include "cpl_conv.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <functional>
#include <vector>

using namespace std;

class MeanShiftCluster
{
private:
	GDALDataset* _raster;
	GDALRasterBand* _band;

	GDALDataset* _aoiRaster;
	int _aoiBand;

private:
	struct _cluster_t
	{
		double Min;
		double Cen;
		double Max;
		int PixCount;
		int CenPixCount;
	};
	vector<_cluster_t> _clusters;
	double _initialThreshold;

public:
	MeanShiftCluster(GDALDataset* raster, int band, GDALDataset* aoiRaster, const int aoiBand);
	~MeanShiftCluster();

	int GetClusterCount();

	void GetClusterInfo(int cluster, double *low, double *center, double *heigh);

	double GetInitialThreshold() { return _initialThreshold;  }

	bool Process(int entryCount, 
		int radius, 
		bool positive,
		const std::function<bool(double pix)>& func,
		bool isBalanced,
		int maxIteration = 1000);
};

