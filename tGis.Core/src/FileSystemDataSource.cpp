#include "FileSystemDataSource.h"
#include "IDataset.h"
#include "FileSystemDataSourceProvider.h"
#include "ObjectSampleDataSourceProvider.h"
#include "MyGDALFileRasterDataset.h"
#include "MyGDALVectorDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <io.h>


BEGIN_NAME_SPACE(tGis, Core)


const char* const FileSystemDataSource::_type = "9357FB74-8ED4-4666-9D91-8B322208D60A";

FileSystemDataSource::FileSystemDataSource(const char* path)
	:FileSystemDataSource(path,&FileSystemDataSourceProvider::INSTANCE())
{
	_refCount = 1;
}

FileSystemDataSource::FileSystemDataSource(const char * path, IDataSourceProvider * provider)
	:DataSource(provider)
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
	Disconnect(false);
}

const char * FileSystemDataSource::GetType()
{
	return FileSystemDataSource::_type;
}

const char * FileSystemDataSource::S_GetType()
{
	return _type;
}

bool FileSystemDataSource::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

bool FileSystemDataSource::IsTypeOf(ITGisObject * object)
{
	if (strcmp(object->GetType(), _type) == 0)
		return true;
	return false;
}

const char * FileSystemDataSource::GetCreationString()
{
	return _path.c_str();
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
				IDataSource* ds = FileSystemDataSourceProvider::INSTANCE().CreateDataSource(subpath.c_str());
				_vecDataSource.push_back(ds);
			}
			else
			{
				size_t pos = subpath.find_last_of(TGIS_EXT_SEPARATOR_CHAR);
				if (pos != subpath.npos)
				{
					string ext = subpath.substr(pos+1);
					GDALAccess eAccess = (file.attrib&_TGIS_A_RDONLY) == 0 ? GA_Update : GA_ReadOnly;

					if (MyGDALDataset::IsSupportedRasterFormatExt(ext.c_str()))
					{
						MyGDALFileRasterDataset* dt = new MyGDALFileRasterDataset(this,subpath.c_str(), eAccess);
						_vecDataset.push_back(dt);
					}
					else if (MyGDALDataset::IsSupportedVectorFormatExt(ext.c_str()))
					{
						MyGDALVectorDataset* dt = new MyGDALVectorDataset(this,subpath.c_str(), eAccess);
						_vecDataset.push_back(dt);
					}
				}
			}
		}

		flag = _tgis_findnext(handle, &file);
	};
	_tgis_findclose(handle);

	DataSource::Connect();
}

void FileSystemDataSource::Disconnect(bool raiseEvent)
{
	if (_connected)
	{
		if (raiseEvent)
		{
			DataSource::Disconnect();
		}

		_connected = false;
		for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); it++)
		{
			IDataSourceProvider* provider = (*it)->GetProvider();
			provider->ReleaseDataSource(*it);
		}
		_vecDataSource.clear();
		for (vector<IDataset*>::iterator it = _vecDataset.begin(); it != _vecDataset.end(); it++)
		{
			IDataset* dt = *it;
			dt->Close();
			delete dt;
		}
		_vecDataset.clear();
	}
}


void FileSystemDataSource::Disconnect()
{
	Disconnect(true);
}


END_NAME_SPACE(tGis, Core)

