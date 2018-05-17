
#include "MyGDALRasterDataset.h"
#include <algorithm>
#include <string>
#include <vector>
#include "boost/filesystem.hpp" 

using namespace std;

namespace fs = boost::filesystem;

BEGIN_NAME_SPACE(tGis, Core)

MyGDALRasterDataset::GDALInit MyGDALRasterDataset::_init;// = IndexRaster::RasterInit();

vector<string> g_SupportedFileFormatExt;
vector<string> g_SupportedFileFormatName;
vector<int> g_SupportedFileFormatCreatable;

MyGDALRasterDataset::GDALInit::GDALInit()
{
    GDALAllRegister();          //GDAL所有操作都需要先注册格式
	OGRRegisterAll();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //支持中文路径

	fs::path exePath = fs::initial_path<boost::filesystem::path>();

	fs::path dataPath(exePath);
	dataPath.append("gdal-data");

	fs::path pluginPath(exePath);
	pluginPath.append("gdalplugins");


	CPLSetConfigOption("GDAL_DATA", dataPath.string().c_str());
    CPLSetConfigOption("GEOTIFF_CSV", dataPath.string().c_str());
	CPLSetConfigOption("GDAL_DRIVER_PATH", pluginPath.string().c_str());

	GDALDriverManager* dm = GetGDALDriverManager();
	int drc = dm->GetDriverCount();
	for (int i = 0; i < drc; i++)
	{
		GDALDriver* drv = dm->GetDriver(i);
		const char* ext = drv->GetMetadataItem(GDAL_DMD_EXTENSIONS);
		if (ext != nullptr && strcmp(ext, "") != 0)
		{
			const char* name = drv->GetMetadataItem(GDAL_DMD_LONGNAME);
			const char* creatable = drv->GetMetadataItem(GDAL_DCAP_CREATE);
			int bc = (creatable==nullptr || strcmp(creatable, "YES") != 0) ? 0 : 1;

			g_SupportedFileFormatExt.push_back(ext);
			if(name == nullptr)
				g_SupportedFileFormatName.push_back("");
			else
				g_SupportedFileFormatName.push_back(name);
			g_SupportedFileFormatCreatable.push_back(bc);
		}
	}
}

const char* const MyGDALRasterDataset::_type = "AB56EFC6-4940-4CF8-AC48-01F830DA8C0D";

MyGDALRasterDataset::MyGDALRasterDataset()
{
	_dataset = nullptr;
	_autoClose = false;
	_eAccess = GA_ReadOnly;
}

MyGDALRasterDataset::MyGDALRasterDataset(char* path, bool delayOpen, GDALAccess eAccess)
{
	_eAccess = eAccess;
	_openStr = path;
	fs::path dir(path);
	_name = dir.filename().string();
	if (delayOpen)
	{
		_dataset = nullptr;
		_autoClose = true;
	}
	else
	{
		Attach(path, eAccess);
	}
}

MyGDALRasterDataset::~MyGDALRasterDataset()
{
	if (_autoClose && _dataset != nullptr)
	{
		GDALClose(_dataset);
	}
}

int MyGDALRasterDataset::GetSupportedFileFormatCount()
{
	return g_SupportedFileFormatExt.size();
}

const char * MyGDALRasterDataset::GetSupportedFileFormatExt(int pos)
{
	return g_SupportedFileFormatExt.at(pos).c_str();
}

const char * MyGDALRasterDataset::GetSupportedFileFormatName(int pos)
{
	return g_SupportedFileFormatName.at(pos).c_str();
}

bool MyGDALRasterDataset::GetSupportedFileFormatCreatable(int pos)
{
	return g_SupportedFileFormatCreatable.at(pos);
}


void MyGDALRasterDataset::Attach(GDALDataset* dataset, bool autoClose)
{
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
		_geoTransform[3] = yRasterSize;
	}

	double x1, y1, x2, y2, x3, y3;
	this->Pixel2Spatial(0, yRasterSize, &x1, &y1);
	this->Pixel2Spatial(xRasterSize, yRasterSize, &x2, &y2);
	this->Pixel2Spatial(xRasterSize, 0, &x3, &y3);

	_envelope.MinX = min(_geoTransform[0], min(x1, min(x2, x3)));
	_envelope.MaxX = max(_geoTransform[0], max(x1, max(x2, x3)));
	_envelope.MinY = min(_geoTransform[3], min(y1, min(y2, y3)));
	_envelope.MaxY = max(_geoTransform[3], max(y1, max(y2, y3)));
}


void MyGDALRasterDataset::Attach(GDALDataset* dataset, double noDataValue, bool autoClose)
{
	int bandCount = dataset->GetRasterCount();
	for (int i = 1; i <= bandCount; i++)
	{
		GDALRasterBand* band = dataset->GetRasterBand(i);
		band->SetNoDataValue(noDataValue);
	}
	Attach(dataset, autoClose);
}

void MyGDALRasterDataset::Attach(const char* file,GDALAccess eAccess,bool autoClose)
{
	_eAccess = eAccess;
	GDALDataset *dataset = (GDALDataset*)GDALOpen(file, eAccess);
	if (dataset != nullptr)
	{
		Attach(dataset,autoClose);
	}	
}


void MyGDALRasterDataset::Attach(const char* file, GDALAccess eAccess, double noDataVale, bool autoClose)
{
	_eAccess = eAccess;
	GDALDataset *dataset = (GDALDataset*)GDALOpen(file, eAccess);
	if (dataset != nullptr)
	{
		Attach(dataset, noDataVale, autoClose);
	}
}


void MyGDALRasterDataset::AttachHDF(const char* file,GDALAccess eAccess,const int subdataset,bool autoClose)
{
	_eAccess = eAccess;
	GDALDataset* pd = (GDALDataset*)GDALOpen(file, GA_ReadOnly);

	if(pd == NULL)
		return;

	char ** papszSUBDATASETS = GDALGetMetadata( (GDALDatasetH)pd, "SUBDATASETS");

	if ( papszSUBDATASETS != NULL )  
	{
		int iCount = CSLCount(papszSUBDATASETS);  
		if(iCount > 0 )  
		{  
			for(int i=0; papszSUBDATASETS[i] != NULL; i++ )  
			{  
				if(i%2 != 0)  
					continue;  

				if(i == 2*subdataset)
				{
					std::string tmpstr = std::string(papszSUBDATASETS[i]);  
					tmpstr = tmpstr.substr(tmpstr.find_first_of("=") + 1);  
					const char *tmpc_str = tmpstr.c_str();  
					GDALDataset *dataset = (GDALDataset*)GDALOpen(tmpc_str, eAccess); 
					Attach(dataset,autoClose);
					break;
				}
			}//end for  
		}		
	}

	GDALClose(pd);
}

void MyGDALRasterDataset::Detach()
{
	if(_autoClose && _dataset != nullptr)
	{
		GDALClose(_dataset);
	}
	_dataset = nullptr;
	_autoClose = false;
}

GDALDataset* MyGDALRasterDataset::GetGDALDataset()
{
	return _dataset;
}

void MyGDALRasterDataset::SetAutoClose(bool autoClose)
{
	_autoClose = autoClose;
}

bool MyGDALRasterDataset::GetAutoClose()
{
	return _autoClose;
}

const OGREnvelope* MyGDALRasterDataset::GetEnvelope()
{
	return &_envelope;
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

const char * MyGDALRasterDataset::GetType()
{
	return _type;
}

const char * MyGDALRasterDataset::GetName()
{
	return _name.c_str();
}

const char * MyGDALRasterDataset::GetOpenString()
{
	return _openStr.c_str();
}

bool MyGDALRasterDataset::IsOpened()
{
	return _dataset != nullptr;
}

void MyGDALRasterDataset::Open()
{
	if (_dataset == nullptr)
	{
		Attach(_openStr.c_str(), _eAccess);
	}
}

void MyGDALRasterDataset::Close()
{
	Detach();
}

IDataSource * MyGDALRasterDataset::GetDataSource()
{
	return _dataSource;
}

const OGRSpatialReference * MyGDALRasterDataset::GetSpatialReference()
{
	return _spatialRef;
}

END_NAME_SPACE(tGis, Core)