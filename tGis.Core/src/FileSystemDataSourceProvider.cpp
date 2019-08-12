#include "FileSystemDataSourceProvider.h"
#include "ObjectSampleDataSourceProvider.h"
#include "FileSystemDataSource.h"
#include <memory>
#include <cassert>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


const char* const FileSystemDataSourceProvider::_name = "FileSystem";
const char* const FileSystemDataSourceProvider::_type = "13D0E005-C5CD-4210-A5D3-FDD57AB12990";

FileSystemDataSourceProvider* FileSystemDataSourceProvider::_instance = nullptr;


FileSystemDataSourceProvider & FileSystemDataSourceProvider::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new FileSystemDataSourceProvider();
		static unique_ptr<FileSystemDataSourceProvider> shit(_instance);
		_instance->AddSubProvider(&ObjectSampleDataSourceProvider::INSTANCE());
	}

	return *_instance;
}

FileSystemDataSourceProvider::FileSystemDataSourceProvider()
{
}

FileSystemDataSourceProvider::~FileSystemDataSourceProvider()
{
}

const char * FileSystemDataSourceProvider::GetSupportedDataSourceType()
{
	return FileSystemDataSource::S_GetType();
}

const char * FileSystemDataSourceProvider::GetName()
{
	return FileSystemDataSourceProvider::_name;
}

const char * FileSystemDataSourceProvider::GetType()
{
	return _type;
}

const char * FileSystemDataSourceProvider::S_GetType()
{
	return _type;
}


bool FileSystemDataSourceProvider::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

bool FileSystemDataSourceProvider::IsTypeOf(ITGisObject * object)
{
	if (strcmp(object->GetType(), _type) == 0)
		return true;
	return false;
}

IDataSource * FileSystemDataSourceProvider::CreateDataSource(const char * path)
{
	IDataSource *ds = nullptr;
	string strPath(path);
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(strPath);

	if (pos != _mapDataSource.end())
	{
		ds = (*pos).second;
		if (!ds->IsTypeOf(FileSystemDataSource::S_GetType()))
		{
			throw exception("Already connected as DataSource of different Type!");
		}
		FileSystemDataSource* fds = (FileSystemDataSource*)ds;
		fds->_refCount++;
		return ds;
	}

	ds = DataSourceProvider::CreateDataSource(path);
	if (ds != nullptr)
		return ds;

	ds = new FileSystemDataSource(path);

	_mapDataSource.insert(map<string, IDataSource*>::value_type(strPath, ds));

	return ds;
}

void FileSystemDataSourceProvider::ReleaseDataSource(IDataSource * ds)
{
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(ds->GetCreationString());

	if (pos != _mapDataSource.end())
	{
		FileSystemDataSource* fds = (FileSystemDataSource*)ds;
		fds->_refCount--;
		if (fds->_refCount == 0)
		{
			ds->Disconnect();
			delete ds;
		}
	
		_mapDataSource.erase(pos);
	}
}

END_NAME_SPACE(tGis, Core)

