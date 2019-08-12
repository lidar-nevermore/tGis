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

void FileSystemDataSource::OnTraverseDir(void * usr, const char * dir, const char * name, unsigned int attrib)
{
	FileSystemDataSource* fsDataSrc = (FileSystemDataSource*)usr;

	if (!(attrib&_TGIS_A_HIDDEN)
		&& !(attrib&_TGIS_A_SYSTEM))
	{
		char path[TGIS_MAX_PATH] = { 0 };
		strcpy(path, dir);
		strcat(path, TGIS_PATH_SEPARATOR_STR);
		strcat(path, name);

		if (attrib&_TGIS_A_SUBDIR)
		{
			IDataSource* ds = FileSystemDataSourceProvider::INSTANCE().CreateDataSource(path);
			fsDataSrc->_vecDataSource.push_back(ds);
		}
		else
		{
			size_t pos = _tgis_find_last_of(name, TGIS_EXT_SEPARATOR_STR, 0);
			if (pos != -1)
			{
				const char* ext = name + pos + 1;
				GDALAccess eAccess = (attrib&_TGIS_A_RDONLY) == 0 ? GA_Update : GA_ReadOnly;

				if (MyGDALDataset::IsSupportedRasterFormatFirstExt(ext))
				{
					MyGDALFileRasterDataset* dt = new MyGDALFileRasterDataset(fsDataSrc, path, eAccess);
				}
				else if (MyGDALDataset::IsSupportedVectorFormatFirstExt(ext))
				{
					MyGDALVectorDataset* dt = new MyGDALVectorDataset(fsDataSrc, path, eAccess);
				}
			}
		}
	}
}


void FileSystemDataSource::Connect()
{
	if (_connected)
		return;

	_connected = true;

	_tgis_traverse_dir(_path.c_str(),"*", this, OnTraverseDir);

	DataSource::Connect();
}

void FileSystemDataSource::Connect(const char * creationString, IDataset ** dtOut)
{
	string path(creationString);
	size_t pos = path.find_last_of(TGIS_EXT_SEPARATOR_CHAR);
	if (pos != path.npos)
	{
		string ext = path.substr(pos + 1);
		GDALAccess eAccess = GA_ReadOnly;
		if (_tgis_access(path.c_str(), _TGIS_OK_WRITE) == _TGIS_OK_ACCESS)
			eAccess = GA_Update;
		if (MyGDALDataset::IsSupportedRasterFormatFirstExt(ext.c_str()))
		{
			MyGDALFileRasterDataset* dt = new MyGDALFileRasterDataset(this, path.c_str(), eAccess);
			if (*dtOut != nullptr)
				*dtOut = dt;
		}
		else if (MyGDALDataset::IsSupportedVectorFormatFirstExt(ext.c_str()))
		{
			MyGDALVectorDataset* dt = new MyGDALVectorDataset(this, path.c_str(), eAccess);
			if (*dtOut != nullptr)
				*dtOut = dt;
		}
	}
	if (!_connected)
	{
		_connected = true;
		DataSource::Connect();
	}
}

void FileSystemDataSource::Connect(const char * creationString, IDataSource ** dsOut)
{
	IDataSource* ds = FileSystemDataSourceProvider::INSTANCE().CreateDataSource(creationString);
	if (*dsOut != nullptr)
		*dsOut = ds;
	_vecDataSource.push_back(ds);

	if (!_connected)
	{
		_connected = true;
		DataSource::Connect();
	}
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

