#include "FileSystemDataSourceProvider.h"
#include "FileSystemDataSource.h"
#include "DataSourceProviderRepository.h"

#include <cassert>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


const char* const FileSystemDataSourceProvider::_name = "FileSystem";
const char* const FileSystemDataSourceProvider::_type = "13D0E005-C5CD-4210-A5D3-FDD57AB12990";
FileSystemDataSourceProvider FileSystemDataSourceProvider::INSTANCE;


FileSystemDataSourceProvider::FileSystemDataSourceProvider()
{
	DataSourceProviderRepository::INSTANCE.AddDataSourceProvider(this);
}

FileSystemDataSourceProvider::~FileSystemDataSourceProvider()
{
	Release();
}

const char * FileSystemDataSourceProvider::GetSupportedDataSourceType()
{
	return FileSystemDataSource::_type;
}

const char * FileSystemDataSourceProvider::GetName()
{
	return FileSystemDataSourceProvider::_name;
}

const char * FileSystemDataSourceProvider::GetType()
{
	return _type;
}

void FileSystemDataSourceProvider::SetCreationUI(UI ui)
{
	_ui = ui;
}

IDataSource * FileSystemDataSourceProvider::UI_CreateDataSource()
{
	assert(_ui != nullptr);

	return _ui();
}

IDataSource * FileSystemDataSourceProvider::CreateDataSource(const char * path)
{
	string strPath(path);
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(strPath);

	if (pos != _mapDataSource.end())
		return (*pos).second;

	FileSystemDataSource* ds = new FileSystemDataSource(path);
	_vecDataSource.push_back(ds);
	_mapDataSource.insert(map<string, IDataSource*>::value_type(strPath, ds));

	return ds;
}

void FileSystemDataSourceProvider::ReleaseDataSource(IDataSource * ds)
{
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(ds->GetCreationString());

	if (pos != _mapDataSource.end())
		delete (*pos).second;
}

int FileSystemDataSourceProvider::GetDataSourceCount()
{
	return _vecDataSource.size();
}

IDataSource * FileSystemDataSourceProvider::GetDataSource(int pos)
{
	return _vecDataSource.at(pos);
}

void FileSystemDataSourceProvider::Release()
{
	for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); it++)
	{
		delete (*it);
	}
	_vecDataSource.clear();
}

END_NAME_SPACE(tGis, Core)

