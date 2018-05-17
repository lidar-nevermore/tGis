#include "FileSystemDataSource.h"
#include "IDataset.h"

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
	for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); it++)
	{
		delete (*it);
	}

	for (vector<IDataset*>::iterator it = _vecDataset.begin(); it != _vecDataset.end(); it++)
	{
		delete (*it);
	}
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
	return;
}

int FileSystemDataSource::GetDatasetCount()
{
	return 0;
}

IDataset * FileSystemDataSource::GetDataset(int)
{
	return nullptr;
}

IDataset * FileSystemDataSource::GetDataset(char *)
{
	return nullptr;
}

int FileSystemDataSource::GetDataSourceCount()
{
	return 0;
}

IDataSource * FileSystemDataSource::GetDataSource(int)
{
	return nullptr;
}

IDataSource * FileSystemDataSource::GetDataSource(char *)
{
	return nullptr;
}

END_NAME_SPACE(tGis, Core)

