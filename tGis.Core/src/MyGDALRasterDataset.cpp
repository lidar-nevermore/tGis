
#include "MyGDALRasterDataset.h"
#include <algorithm>
#include <string>
#include <vector>


using namespace std;


BEGIN_NAME_SPACE(tGis, Core)

const char* const MyGDALRasterDataset::_type = "AB56EFC6-4940-4CF8-AC48-01F830DA8C0D";


const char * MyGDALRasterDataset::GetType()
{
	return _type;
}

bool MyGDALRasterDataset::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

const char * MyGDALRasterDataset::S_GetType()
{
	return _type;
}

MyGDALRasterDataset::MyGDALRasterDataset(IDataSource * ds)
	:MyGDALDataset(ds)
{
}

MyGDALRasterDataset::MyGDALRasterDataset()
{
}

MyGDALRasterDataset::~MyGDALRasterDataset()
{
}

void MyGDALRasterDataset::Attach(GDALDataset* dataset, bool autoClose, double noDataValue)
{
	if (dataset->GetRasterCount() == 0)
		throw std::exception("不是有效的栅格数据集！");

	_dataset = dataset;
	_autoClose = autoClose;
	_dataset->GetGeoTransform(_geoTransform);
	int xRasterSize = _dataset->GetRasterXSize();
	int yRasterSize = _dataset->GetRasterYSize();

	_spatialRef = (OGRSpatialReference*)OSRNewSpatialReference(nullptr);
	char* _spatialRefStr = (char*)_dataset->GetProjectionRef();
	OGRErr err = _spatialRef->importFromWkt(&_spatialRefStr);
	if (OGRERR_NONE != err)
	{
		OSRDestroySpatialReference(_spatialRef);
		_spatialRef = nullptr;
		_geoTransform[5] = -abs(_geoTransform[5]);
	}

	double x1, y1, x2, y2, x3, y3;
	this->Pixel2Spatial(0, yRasterSize, &x1, &y1);
	this->Pixel2Spatial(xRasterSize, yRasterSize, &x2, &y2);
	this->Pixel2Spatial(xRasterSize, 0, &x3, &y3);

	_envelope.MinX = min(_geoTransform[0], min(x1, min(x2, x3)));
	_envelope.MaxX = max(_geoTransform[0], max(x1, max(x2, x3)));
	_envelope.MinY = min(_geoTransform[3], min(y1, min(y2, y3)));
	_envelope.MaxY = max(_geoTransform[3], max(y1, max(y2, y3)));

	if (!isnan(noDataValue))
	{
		int bandCount = _dataset->GetRasterCount();
		for (int i = 1; i <= bandCount; i++)
		{
			GDALRasterBand* band = _dataset->GetRasterBand(i);
			band->SetNoDataValue(noDataValue);
		}
	}
}

void MyGDALRasterDataset::Detach()
{
	if (_spatialRef != nullptr)
	{
		OSRDestroySpatialReference(_spatialRef);
		_spatialRef = nullptr;
	}
	MyGDALDataset::Detach();
}

void MyGDALRasterDataset::Close()
{
	if (_spatialRef != nullptr)
	{
		OSRDestroySpatialReference(_spatialRef);
		_spatialRef = nullptr;
	}
	MyGDALDataset::Close();
}

const double* MyGDALRasterDataset::GetGeoTransform()
{
	return _geoTransform;
}

bool MyGDALRasterDataset::IsNorthUp()
{
	return _geoTransform[2] == 0 && _geoTransform[4] == 0;
}

//Spatial position of the upper left corner of the pixel.
void MyGDALRasterDataset::Pixel2Spatial(int pixX,int pixY,double *projX, double *projY)
{
	*projX = _geoTransform[0] + pixX*_geoTransform[1] + pixY*_geoTransform[2];
	*projY = _geoTransform[3] + pixX*_geoTransform[4] + pixY*_geoTransform[5];
}

void MyGDALRasterDataset::Spatial2Pixel(double projX,double projY,double *pixX, double *pixY)
{
	double t0 = _geoTransform[0];
	double t1 = _geoTransform[1];
	double t2 = _geoTransform[2];
	double t3 = _geoTransform[3];
	double t4 = _geoTransform[4];
	double t5 = _geoTransform[5];
	
	//padfTransform[1] is the x position increment of each pixel column, padfTransform[2] is the x position increment of each pixel row.
	//padfTransform[4] is the y position increment of each pixel column, padfTransform[5] is the y position increment of each pixel row.
	//In a north up image, padfTransform[1] is the pixel width, and padfTransform[5] is the pixel height. 
	//The upper left corner of the upper left pixel is at position (padfTransform[0],padfTransform[3]).
	*pixX = (t5*projX-t2*projY+t2*t3-t5*t0)/(t5*t1-t2*t4+DBL_EPSILON);
	*pixY = (t4*projX-t1*projY+t1*t3-t4*t0)/(t4*t2-t1*t5+DBL_EPSILON);
}


END_NAME_SPACE(tGis, Core)