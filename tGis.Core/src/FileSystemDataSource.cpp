#include "FileSystemDataSource.h"
#include "IDataset.h"
#include "FileSystemDataSourceProvider.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include "boost/filesystem.hpp" 

namespace fs = boost::filesystem;

BEGIN_NAME_SPACE(tGis, Core)


const char* const FileSystemDataSource::_type = "9357FB74-8ED4-4666-9D91-8B322208D60A";

FileSystemDataSource::FileSystemDataSource(const char* path)
{
	_path = path;
	fs::path dir(path);
	_name = dir.filename().string();
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

const char * FileSystemDataSource::GetName()
{
	return _name.c_str();
}

const char * FileSystemDataSource::GetConnectionString()
{
	return _path.c_str();
}

bool FileSystemDataSource::IsConnected()
{
	return _connected;
}

void FileSystemDataSource::Connect()
{
	return;
}

void FileSystemDataSource::Disconnect()
{
	_connected = false;
	for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); it++)
	{
		FileSystemDataSourceProvider::INSTANCE._mapDataSource.erase((*it)->GetConnectionString());
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

IDataset * FileSystemDataSource::GetDataset(char * name)
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

IDataSource * FileSystemDataSource::GetDataSource(char * path)
{
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(path);

	if (pos != _mapDataSource.end())
		return (*pos).second;
	return nullptr;
}

END_NAME_SPACE(tGis, Core)

