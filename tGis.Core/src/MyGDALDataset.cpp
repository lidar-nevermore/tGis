#include "MyGDALDataset.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct GDALInitializer
{
	vector<vector<string>> _SupportedVectorFormatExt;
	vector<string> _SupportedVectorFormatName;
	vector<int> _SupportedVectorFormatDriverIndex;
	vector<int> _SupportedVectorFormatCreatable;

	vector<vector<string>> _SupportedRasterFormatExt;
	vector<string> _SupportedRasterFormatName;
	vector<int> _SupportedRasterFormatDriverIndex;
	vector<int> _SupportedRasterFormatCreatable;

	static char*  _excludeExt[];

	static bool IsExcludeExt(const char* ext)
	{
		int i = 0;
		char* e = _excludeExt[i];
		while(strcmp(e, "\0") != 0)
		{
			if (_stricmp(e, ext) == 0)
				return true;

			i++;
			e = _excludeExt[i];
		}
		return false;
	}

	GDALInitializer()
	{
		char exePathBuffer[TGIS_MAX_PATH];
		_tgis_get_exe_dir(exePathBuffer, nullptr);

		string dataPath(exePathBuffer);
		dataPath.append(TGIS_PATH_SEPARATOR_STR);
		dataPath.append("gdal_data");

		string pluginPath(exePathBuffer);
		pluginPath.append(TGIS_PATH_SEPARATOR_STR);
		pluginPath.append("gdalplugins");

		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //支持中文路径
		CPLSetConfigOption("GDAL_DATA", dataPath.c_str());
		CPLSetConfigOption("GEOTIFF_CSV", dataPath.c_str());
		CPLSetConfigOption("GDAL_DRIVER_PATH", pluginPath.c_str());

		GDALAllRegister();          //GDAL所有操作都需要先注册格式
		OGRRegisterAll();

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
				_tgis_str_split(const_cast<char*>(strext.c_str()), " ,.|/\\", dstext);
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
						_SupportedRasterFormatDriverIndex.push_back(i);
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
							_SupportedVectorFormatDriverIndex.push_back(i);
						}
					}
				}//end of ext not empty
			}//end of ext not null or empty
		}//end of for
	}//end of constructor
};

char* GDALInitializer::_excludeExt[] = { "xml", "json", "txt", "aoi", "pdf", "\0" };

GDALInitializer* MyGDALDataset::_GDALInit = new GDALInitializer();

bool MyGDALDataset::GDALInit()
{
	//该方法始终会返回true
	//尝试使用_GDALInit之前_GDALInit就会实例化
	//_GDALInit实例化时会初始化GDAL
	return _GDALInit != NULL;
}


size_t MyGDALDataset::GetSupportedRasterFormatCount()
{
	return _GDALInit->_SupportedRasterFormatExt.size();
}

const vector<string>& MyGDALDataset::GetSupportedRasterFormatExt(size_t pos)
{
	return _GDALInit->_SupportedRasterFormatExt.at(pos);
}

const char * MyGDALDataset::GetSupportedRasterFormatName(size_t pos)
{
	return _GDALInit->_SupportedRasterFormatName.at(pos).c_str();
}

bool MyGDALDataset::GetSupportedRasterFormatCreatable(size_t pos)
{
	return _GDALInit->_SupportedRasterFormatCreatable.at(pos) == 1;
}

size_t MyGDALDataset::GetSupportedRasterFormatPos(const char * ext, bool * supported)
{
	if (supported != nullptr)
		*supported = false;

	if (GDALInitializer::IsExcludeExt(ext))
		return _GDALInit->_SupportedRasterFormatExt.size();

	size_t pos = 0;
	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedRasterFormatExt.begin(); it != _GDALInit->_SupportedRasterFormatExt.end(); it++)
	{
		for (vector<string>::iterator itt = (*it).begin(); itt != (*it).end(); itt++)
		{
			if (_stricmp((*itt).c_str(), ext) == 0)
			{
				if (supported != nullptr)
					*supported = true;
				return pos;
			}
		}

		pos++;
	}
	return _GDALInit->_SupportedRasterFormatExt.size();
}

int MyGDALDataset::GetSupportedRasterFormatDriverIndex(size_t pos)
{
	return _GDALInit->_SupportedRasterFormatDriverIndex.at(pos);
}

bool MyGDALDataset::IsSupportedRasterFormatExt(const char * ext)
{
	if (GDALInitializer::IsExcludeExt(ext))
		return false;

	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedRasterFormatExt.begin(); it != _GDALInit->_SupportedRasterFormatExt.end(); it++)
	{
		for (vector<string>::iterator itt = (*it).begin(); itt != (*it).end(); itt++)
		{
			if (_stricmp((*itt).c_str(), ext) == 0)
				return true;
		}
	}
	return false;
}

bool MyGDALDataset::IsSupportedRasterFormatFirstExt(const char * ext)
{
	if (GDALInitializer::IsExcludeExt(ext))
		return false;

	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedRasterFormatExt.begin(); it != _GDALInit->_SupportedRasterFormatExt.end(); it++)
	{
		if (_stricmp((*it).at(0).c_str(), ext) == 0)
			return true;
	}
	return false;
}

size_t MyGDALDataset::GetSupportedVectorFormatCount()
{
	return _GDALInit->_SupportedVectorFormatExt.size();
}

const vector<string>& MyGDALDataset::GetSupportedVectorFormatExt(size_t pos)
{
	return _GDALInit->_SupportedVectorFormatExt.at(pos);
}

const char * MyGDALDataset::GetSupportedVectorFormatName(size_t pos)
{
	return _GDALInit->_SupportedVectorFormatName.at(pos).c_str();
}

bool MyGDALDataset::GetSupportedVectorFormatCreatable(size_t pos)
{
	return _GDALInit->_SupportedVectorFormatCreatable.at(pos)==1;
}

size_t MyGDALDataset::GetSupportedVectorFormatPos(const char * ext, bool * supported)
{
	if (supported != nullptr)
		*supported = false;

	if (GDALInitializer::IsExcludeExt(ext))
		return _GDALInit->_SupportedVectorFormatExt.size();

	size_t pos = 0;
	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedVectorFormatExt.begin(); it != _GDALInit->_SupportedVectorFormatExt.end(); it++)
	{
		for (vector<string>::iterator itt = (*it).begin(); itt != (*it).end(); itt++)
		{
			if (_stricmp((*itt).c_str(), ext) == 0)
			{
				if (supported != nullptr)
					*supported = true;
				return pos;
			}
		}

		pos++;
	}
	return _GDALInit->_SupportedVectorFormatExt.size();
}

int MyGDALDataset::GetSupportedVectorFormatDriverIndex(size_t pos)
{
	return _GDALInit->_SupportedVectorFormatDriverIndex.at(pos);
}

bool MyGDALDataset::IsSupportedVectorFormatExt(const char * ext)
{
	if (GDALInitializer::IsExcludeExt(ext))
		return false;

	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedVectorFormatExt.begin(); it != _GDALInit->_SupportedVectorFormatExt.end(); it++)
	{
		for (vector<string>::iterator itt = (*it).begin(); itt != (*it).end(); itt++)
		{
			if (_stricmp((*itt).c_str(), ext) == 0)
				return true;
		}
	}
	return false;
}

bool MyGDALDataset::IsSupportedVectorFormatFirstExt(const char * ext)
{
	if (GDALInitializer::IsExcludeExt(ext))
		return false;

	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedVectorFormatExt.begin(); it != _GDALInit->_SupportedVectorFormatExt.end(); it++)
	{
		if (_stricmp((*it).at(0).c_str(), ext) == 0)
			return true;
	}
	return false;
}

MyGDALDataset::MyGDALDataset(IDataSource * ds)
	:Dataset(ds)
{
	_dataset = nullptr;
	_spatialRef = nullptr;
}

MyGDALDataset::MyGDALDataset()
	: Dataset(nullptr)
{
	_dataset = nullptr;
	_spatialRef = nullptr;
}

MyGDALDataset::~MyGDALDataset()
{
	Detach();
}

GDALDataset * MyGDALDataset::GetGDALDataset()
{
	return _dataset;
}

void MyGDALDataset::Attach(const char * file, GDALAccess eAccess, bool autoClose)
{
	_eAccess = eAccess;
	_path = file;
	size_t pos = _path.find_last_of(TGIS_PATH_SEPARATOR_CHAR);
	if (pos == _path.npos)
	{
		_name = _path;
	}
	else
	{
		_name = _path.substr(pos + 1);
	}
	GDALDataset *dataset = (GDALDataset*)GDALOpenEx(file, _eAccess, nullptr, nullptr, nullptr);//(GDALDataset*)GDALOpen(file, eAccess);
	if (dataset == nullptr && _eAccess == GA_Update)
	{
		_eAccess = GA_ReadOnly;
		dataset = (GDALDataset*)GDALOpenEx(file, _eAccess, nullptr, nullptr, nullptr);
	}
	if (dataset != nullptr)
	{
		_dataset = dataset;
		_autoClose = autoClose;
	}
}

const char * MyGDALDataset::GetName()
{
	return _name.c_str();
}

bool MyGDALDataset::IsReadOnly()
{
	return _eAccess == GA_ReadOnly;;
}

bool MyGDALDataset::IsOpened()
{
	return _dataset != nullptr;
}

void MyGDALDataset::Close()
{
	if (_autoClose && _dataset != nullptr)
	{
		Dataset::Close();
	}
	Detach();
}

IDataSource * MyGDALDataset::GetDataSource()
{
	return _dataSource;
}

const OGRSpatialReference * MyGDALDataset::GetSpatialReference()
{
	return _spatialRef;
}

const OGREnvelope * MyGDALDataset::GetEnvelope()
{
	return &_envelope;
}

void MyGDALDataset::Detach()
{
	if (_autoClose && _dataset != nullptr)
	{
		GDALClose(_dataset);
	}
	_dataset = nullptr;
	_autoClose = false;
}

void MyGDALDataset::SetAutoClose(bool autoClose)
{
	_autoClose = autoClose;
}

bool MyGDALDataset::GetAutoClose()
{
	return _autoClose;
}



END_NAME_SPACE(tGis, Core)



