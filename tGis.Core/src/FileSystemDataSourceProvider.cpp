#include "FileSystemDataSourceProvider.h"
#include "FileSystemDataSource.h"

#include <cassert>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


const char* const FileSystemDataSourceProvider::_name = "FileSystem";
const char* const FileSystemDataSourceProvider::_type = "13D0E005-C5CD-4210-A5D3-FDD57AB12990";

FileSystemDataSourceProvider* FileSystemDataSourceProvider::_instance = nullptr;

static PtrDestructor<FileSystemDataSourceProvider> shit(FileSystemDataSourceProvider::_instance);


FileSystemDataSourceProvider & FileSystemDataSourceProvider::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new FileSystemDataSourceProvider();
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
	return FileSystemDataSource::_type;
}

bool FileSystemDataSourceProvider::IsRoot()
{
	return true;
}

const char * FileSystemDataSourceProvider::GetName()
{
	return FileSystemDataSourceProvider::_name;
}

const char * FileSystemDataSourceProvider::GetType()
{
	return _type;
}

IDataSource * FileSystemDataSourceProvider::CreateDataSourceNoHost(const char * path)
{
	string strPath(path);
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(strPath);

	if (pos != _mapDataSource.end())
	{
		IDataSource *ds = (*pos).second;
		const char* t = ds->GetType();
		if (strcmp(t, FileSystemDataSource::S_GetType()) != 0)
		{
			throw exception("Already connected as DataSource of different Type!");
		}
		return ds;
	}

	FileSystemDataSource* ds = new FileSystemDataSource(path);

	_mapDataSource.insert(map<string, IDataSource*>::value_type(strPath, ds));

	return ds;
}


IDataSource * FileSystemDataSourceProvider::CreateDataSource(const char * path)
{
	bool exist = false;
	IDataSource* ds = CreateDataSourceNoHost(path);
	for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); ++it)
	{
		if (*it == ds)
		{
			exist = true;
			break;
		}
	}
	if(exist == false)
		_vecDataSource.push_back(ds);
	return ds;
}

END_NAME_SPACE(tGis, Core)

