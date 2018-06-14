#include "FileSystemDataSource.h"
#include "IDataset.h"
#include "FileSystemDataSourceProvider.h"
#include "ObjectSampleDataSourceProvider.h"
#include "MyGDALRasterDataset.h"
#include "MyGDALVectorDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <io.h>


BEGIN_NAME_SPACE(tGis, Core)


const char* const FileSystemDataSource::_type = "9357FB74-8ED4-4666-9D91-8B322208D60A";

FileSystemDataSource::FileSystemDataSource(const char* path)
{
	_path = path;
	string sepstr(TGIS_PATH_SEPARATOR_STR);
	size_t spos = _path.size() - sepstr.size();

	if (_path.find(sepstr, spos) == spos)
	{
		_path = _path.substr(0, spos);
	}

	size_t pos = _path.find_last_of(TGIS_PATH_SEPARATOR_CHAR);
	if (pos == _path.npos)
	{
		_name = _path;
	}
	else
	{
		_name = _path.substr(pos + 1);
	}

	_connected = false;
}


FileSystemDataSource::~FileSystemDataSource()
{
	Disconnect();
}

const char * FileSystemDataSource::GetType()
{
	return FileSystemDataSource::_type;
}

const char * FileSystemDataSource::S_GetType()
{
	return _type;
}

const char * FileSystemDataSource::GetName()
{
	return _name.c_str();
}

const char * FileSystemDataSource::GetCreationString()
{
	return _path.c_str();
}

bool FileSystemDataSource::IsConnected()
{
	return _connected;
}

void FileSystemDataSource::Connect()
{
	if (_connected)
		return;

	_connected = true;
	string find_path = _path + TGIS_PATH_SEPARATOR_STR + "*";
	_tgis_finddata_t file;
	intptr_t flag;
	intptr_t handle;
	flag = handle = _tgis_findfirst(find_path.c_str(), &file);
	while (flag != -1)
	{
		string subpath = _path + TGIS_PATH_SEPARATOR_STR + file.name;

		if (strcmp(file.name, ".") != 0 
			&& strcmp(file.name, "..") != 0 
			&& !(file.attrib&_TGIS_A_HIDDEN)
			&& !(file.attrib&_TGIS_A_SYSTEM)
			)
		{
			if (file.attrib&_TGIS_A_SUBDIR)
			{
				IDataSource* ds = nullptr;
				if (ObjectSampleDataSourceProvider::IsObjectSampleDataSource(subpath.c_str()))
					ds = ObjectSampleDataSourceProvider::INSTANCE().CreateDataSourceNoHost(subpath.c_str());
				else
					ds = FileSystemDataSourceProvider::INSTANCE().CreateDataSourceNoHost(subpath.c_str());
				_vecDataSource.push_back(ds);
				map<string, IDataSource*>::value_type v(subpath, ds);
				_mapDataSource.insert(v);
			}
			else
			{
				size_t pos = subpath.find_last_of(TGIS_EXT_SEPARATOR_CHAR);
				if (pos != subpath.npos)
				{
					string ext = subpath.substr(pos+1);
					GDALAccess eAccess = (file.attrib&_TGIS_A_RDONLY) == 0 ? GA_Update : GA_ReadOnly;

					if (MyGDALFileDataset::IsSupportedRasterFormatExt(ext.c_str()))
					{
						MyGDALRasterDataset* dt = new MyGDALRasterDataset(subpath.c_str(), eAccess);
						dt->_dataSource = this;
						_vecDataset.push_back(dt);
						_mapDataset.insert(map<string, IDataset*>::value_type(subpath, dt));
					}
					else if (MyGDALFileDataset::IsSupportedVectorFormatExt(ext.c_str()))
					{
						MyGDALVectorDataset* dt = new MyGDALVectorDataset(subpath.c_str(), eAccess);
						dt->_dataSource = this;
						_vecDataset.push_back(dt);
						_mapDataset.insert(map<string, IDataset*>::value_type(subpath, dt));
					}
				}
			}
		}

		flag = _tgis_findnext(handle, &file);
	};
	_tgis_findclose(handle);
}

void FileSystemDataSource::Disconnect()
{
	_connected = false;
	for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); it++)
	{
		FileSystemDataSourceProvider::INSTANCE()._mapDataSource.erase((*it)->GetCreationString());
		delete (*it);
	}
	_vecDataSource.clear();
	_mapDataSource.clear();
	for (vector<IDataset*>::iterator it = _vecDataset.begin(); it != _vecDataset.end(); it++)
	{
		delete (*it);
	}
	_vecDataset.clear();
	_mapDataset.clear();
}

int FileSystemDataSource::GetDatasetCount()
{
	return _vecDataset.size();
}

IDataset * FileSystemDataSource::GetDataset(int pos)
{
	return _vecDataset.at(pos);
}

IDataset * FileSystemDataSource::GetDataset(const char * name)
{
	map<string, IDataset*>::iterator pos = _mapDataset.find(name);

	if (pos != _mapDataset.end())
		return (*pos).second;
	return nullptr;
}

int FileSystemDataSource::GetDataSourceCount()
{
	return _vecDataSource.size();
}

IDataSource * FileSystemDataSource::GetDataSource(int pos)
{
	return _vecDataSource.at(pos);
}

IDataSource * FileSystemDataSource::GetDataSource(const char * path)
{
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(path);

	if (pos != _mapDataSource.end())
		return (*pos).second;
	return nullptr;
}

END_NAME_SPACE(tGis, Core)

