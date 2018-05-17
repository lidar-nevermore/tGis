#include "FileSystemDataSourceProvider.h"
#include "FileSystemDataSource.h"

#include <cassert>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


const char* const FileSystemDataSourceProvider::_catagory = "FileSystem";
FileSystemDataSourceProvider FileSystemDataSourceProvider::INSTANCE;// = new FileSystemDataSourceProvider();


FileSystemDataSourceProvider::FileSystemDataSourceProvider()
{

}

FileSystemDataSourceProvider::~FileSystemDataSourceProvider()
{
	for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); it++)
	{
		delete (*it);
	}
}

const char * FileSystemDataSourceProvider::GetSupportedDataSourceType()
{
	return FileSystemDataSource::_type;
}

const char * FileSystemDataSourceProvider::GetCatagory()
{
	return FileSystemDataSourceProvider::_catagory;
}

void FileSystemDataSourceProvider::SetCreateDataSourceUI(UI ui)
{
	_ui = ui;
}

IDataSource * FileSystemDataSourceProvider::UI_CreateDataSource()
{
	assert(_ui != nullptr);

	return CreateDataSource(_ui());
}

IDataSource * FileSystemDataSourceProvider::CreateDataSource(const char * path)
{
	map<string,int>::iterator pos = _mapDataSource.find(path);

	if (pos != _mapDataSource.end())
		return _vecDataSource.at(pos->second);

	FileSystemDataSource* ds = new FileSystemDataSource(path);
	_vecDataSource.insert(_vecDataSource.end(),ds);
	_mapDataSource.insert(map<string,int>::value_type(path, _vecDataSource.size() - 1));

	return ds;
}

int FileSystemDataSourceProvider::GetDataSourceCount()
{
	return _vecDataSource.size();
}

IDataSource * FileSystemDataSourceProvider::GetDataSource(int pos)
{
	return _vecDataSource.at(pos);
}

END_NAME_SPACE(tGis, Core)

