#include "MyGDALFileDataset.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct GDALInit
{
	vector<vector<string>> _SupportedVectorFormatExt;
	vector<string> _SupportedVectorFormatName;
	vector<int> _SupportedVectorFormatCreatable;

	vector<vector<string>> _SupportedRasterFormatExt;
	vector<string> _SupportedRasterFormatName;
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

	GDALInit()
	{
		GDALAllRegister();          //GDAL所有操作都需要先注册格式
		OGRRegisterAll();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //支持中文路径

		char exePathBuffer[TGIS_MAX_PATH];
		my_getcwd(exePathBuffer, TGIS_MAX_PATH);

		string dataPath(exePathBuffer);
		dataPath.append(TGIS_PATH_SEPARATOR_STR);
		dataPath.append("gdal_data");

		string pluginPath(exePathBuffer);
		pluginPath.append(TGIS_PATH_SEPARATOR_STR);
		pluginPath.append("gdalplugins");


		CPLSetConfigOption("GDAL_DATA", dataPath.c_str());
		CPLSetConfigOption("GEOTIFF_CSV", dataPath.c_str());
		CPLSetConfigOption("GDAL_DRIVER_PATH", pluginPath.c_str());

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
				str_split(const_cast<char*>(strext.c_str()), " ,.|/\\", dstext);
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

char* GDALInit::_excludeExt[] = { "xml", "json", "txt", "aoi", "pdf", "\0" };

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
	if (GDALInit::IsExcludeExt(ext))
		return false;

	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedRasterFormatExt.begin(); it != _GDALInit->_SupportedRasterFormatExt.end(); it++)
	{
		//if (_stricmp((*it).at(0).c_str(), ext) == 0)
		//	return true;
		for (vector<string>::iterator itt = (*it).begin(); itt != (*it).end(); itt++)
		{
			if (_stricmp((*itt).c_str(), ext) == 0)
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
	if (GDALInit::IsExcludeExt(ext))
		return false;

	for (vector<vector<string>>::iterator it = _GDALInit->_SupportedVectorFormatExt.begin(); it != _GDALInit->_SupportedVectorFormatExt.end(); it++)
	{
		if (_stricmp((*it).at(0).c_str(), ext) == 0)
			return true;
		//for (vector<string>::iterator itt = (*it).begin(); itt != (*it).end(); itt++)
		//{
		//	if (stricmp((*itt).c_str(), ext) == 0)
		//		return true;
		//}
	}
	return false;
}


MyGDALFileDataset::MyGDALFileDataset()
{
	_dataset = nullptr;
	_autoClose = false;
	_eAccess = GA_ReadOnly;
}

MyGDALFileDataset::MyGDALFileDataset(bool isReadOnly)
{
	_dataset = nullptr;
	_autoClose = false;
	_eAccess = isReadOnly ? GA_ReadOnly : GA_Update;
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

const char * MyGDALFileDataset::GetCreationString()
{
	return _path.c_str();
}

bool MyGDALFileDataset::IsReadOnly()
{
	return _eAccess == GA_ReadOnly;;
}

bool MyGDALFileDataset::IsOpened()
{
	return _dataset != nullptr;
}

void MyGDALFileDataset::Open()
{
	if (_dataset == nullptr)
	{
		Attach(_path.c_str(), _eAccess);
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



