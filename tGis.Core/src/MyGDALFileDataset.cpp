#include "MyGDALFileDataset.h"

#include <algorithm>
#include <string>
#include <vector>
#include "boost/filesystem.hpp" 
#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/split.hpp"

using namespace std;

namespace fs = boost::filesystem;

BEGIN_NAME_SPACE(tGis, Core)

struct GDALInit
{
	vector<vector<string>> _SupportedVectorFormatExt;
	vector<string> _SupportedVectorFormatName;
	vector<int> _SupportedVectorFormatCreatable;

	vector<vector<string>> _SupportedRasterFormatExt;
	vector<string> _SupportedRasterFormatName;
	vector<int> _SupportedRasterFormatCreatable;

	GDALInit()
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
				vector<string> dstext;
				string strext = ext;
				boost::split(dstext, strext, boost::is_any_of(" ,.|/\\"), boost::token_compress_on);
				for (vector<string>::iterator it = dstext.begin(); it != dstext.end();)
				{
					if ((*it).empty())
						it = dstext.erase(it);
					else
						++it;
				}
				if (dstext.size() > 0)
				{
					const char* raster = drv->GetMetadataItem(GDAL_DCAP_RASTER);
					if (raster != nullptr && strcmp(raster, "YES") == 0)
					{
						_SupportedRasterFormatExt.push_back(dstext);

						const char* name = drv->GetMetadataItem(GDAL_DMD_LONGNAME);
						const char* creatable = drv->GetMetadataItem(GDAL_DCAP_CREATE);
						int bc = (creatable == nullptr || strcmp(creatable, "YES") != 0) ? 0 : 1;

						if (name == nullptr)
							_SupportedRasterFormatName.push_back("");
						else
							_SupportedRasterFormatName.push_back(name);
						_SupportedRasterFormatCreatable.push_back(bc);
					}
					else
					{
						const char* vector = drv->GetMetadataItem(GDAL_DCAP_VECTOR);
						if (vector != nullptr && strcmp(vector, "YES") == 0)
						{
							_SupportedVectorFormatExt.push_back(dstext);

							const char* name = drv->GetMetadataItem(GDAL_DMD_LONGNAME);
							const char* creatable = drv->GetMetadataItem(GDAL_DCAP_CREATE);
							int bc = (creatable == nullptr || strcmp(creatable, "YES") != 0) ? 0 : 1;

							if (name == nullptr)
								_SupportedVectorFormatName.push_back("");
							else
								_SupportedVectorFormatName.push_back(name);
							_SupportedVectorFormatCreatable.push_back(bc);
						}
					}
				}//end of ext not empty
			}//end of ext not null or empty
		}//end of for
	}//end of constructor


};


GDALInit* MyGDALFileDataset::_GDALInit = new GDALInit();



int MyGDALFileDataset::GetSupportedRasterFormatCount()
{
	return _GDALInit->_SupportedRasterFormatExt.size();
}

const vector<string>& MyGDALFileDataset::GetSupportedRasterFormatExt(int pos)
{
	return _GDALInit->_SupportedRasterFormatExt.at(pos);
}

const char * MyGDALFileDataset::GetSupportedRasterFormatName(int pos)
{
	return _GDALInit->_SupportedRasterFormatName.at(pos).c_str();
}

bool MyGDALFileDataset::GetSupportedRasterFormatCreatable(int pos)
{
	return _GDALInit->_SupportedRasterFormatCreatable.at(pos) == 1;
}

bool MyGDALFileDataset::IsSupportedRasterFormatExt(const char * ext)
{
	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedRasterFormatExt.begin(); it != _GDALInit->_SupportedRasterFormatExt.end(); it++)
	{
		for (vector<string>::iterator itt = (*it).begin(); itt != (*it).end(); itt++)
		{
			if (stricmp((*itt).c_str(), ext) == 0)
				return true;
		}
	}
	return false;
}

int MyGDALFileDataset::GetSupportedVectorFormatCount()
{
	return _GDALInit->_SupportedVectorFormatExt.size();
}

const vector<string>& MyGDALFileDataset::GetSupportedVectorFormatExt(int pos)
{
	return _GDALInit->_SupportedVectorFormatExt.at(pos);
}

const char * MyGDALFileDataset::GetSupportedVectorFormatName(int pos)
{
	return _GDALInit->_SupportedVectorFormatName.at(pos).c_str();
}

bool MyGDALFileDataset::GetSupportedVectorFormatCreatable(int pos)
{
	return _GDALInit->_SupportedVectorFormatCreatable.at(pos)==1;
}

bool MyGDALFileDataset::IsSupportedVectorFormatExt(const char * ext)
{
	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedVectorFormatExt.begin(); it != _GDALInit->_SupportedVectorFormatExt.end(); it++)
	{
		for (vector<string>::iterator itt = (*it).begin(); itt != (*it).end(); itt++)
		{
			if (stricmp((*itt).c_str(), ext) == 0)
				return true;
		}
	}
	return false;
}


MyGDALFileDataset::MyGDALFileDataset()
{
	_dataset = nullptr;
	_autoClose = false;
	_eAccess = GA_ReadOnly;
}


MyGDALFileDataset::~MyGDALFileDataset()
{
	Detach();
}

GDALDataset * MyGDALFileDataset::GetGDALDataset()
{
	return _dataset;
}

const char * MyGDALFileDataset::GetName()
{
	return _name.c_str();
}

const char * MyGDALFileDataset::GetOpenString()
{
	return _openStr.c_str();
}

bool MyGDALFileDataset::IsOpened()
{
	return _dataset != nullptr;
}

void MyGDALFileDataset::Open()
{
	if (_dataset == nullptr)
	{
		Attach(_openStr.c_str(), _eAccess);
	}
}

void MyGDALFileDataset::Close()
{
	Detach();
}

IDataSource * MyGDALFileDataset::GetDataSource()
{
	return _dataSource;
}

const OGRSpatialReference * MyGDALFileDataset::GetSpatialReference()
{
	return _spatialRef;
}

const OGREnvelope * MyGDALFileDataset::GetEnvelope()
{
	return &_envelope;
}

void MyGDALFileDataset::Attach(const char * file, GDALAccess eAccess, bool autoClose)
{
	_eAccess = eAccess;
	_openStr = file;
	fs::path dir(file);
	_name = dir.filename().string();
	GDALDataset *dataset = (GDALDataset*)GDALOpenEx(file, eAccess, nullptr, nullptr, nullptr);
	if (dataset != nullptr)
	{
		Attach(dataset, autoClose);
	}
}

void MyGDALFileDataset::Detach()
{
	if (_autoClose && _dataset != nullptr)
	{
		GDALClose(_dataset);
	}
	_dataset = nullptr;
	_autoClose = false;
}

void MyGDALFileDataset::SetAutoClose(bool autoClose)
{
	_autoClose = autoClose;
}

bool MyGDALFileDataset::GetAutoClose()
{
	return _autoClose;
}



END_NAME_SPACE(tGis, Core)



