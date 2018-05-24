#include "FileSystemDataSource.h"
#include "IDataset.h"
#include "FileSystemDataSourceProvider.h"
#include "MyGDALRasterDataset.h"
#include "MyGDALVectorDataset.h"

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


	wchar_t sepc[2] = { fs::path::preferred_separator, 0 };
	wstring sep;
	sep.append(sepc);
	fs::path sepp(sep);
	string sepstr = sepp.string();

	size_t spos = _path.size() - sepstr.size();

	if (_path.find(sepstr, spos) == spos)
	{
		string seppath = _path.substr(0, spos);
		fs::path dir(seppath);
		_name = dir.filename().string();
	}
	else
	{
		fs::path dir(_path);
		_name = dir.filename().string();
	}

	map<string, IDataSource*>::iterator pos = FileSystemDataSourceProvider::INSTANCE._mapDataSource.find(_path);
	if (pos != FileSystemDataSourceProvider::INSTANCE._mapDataSource.end())
	{
		FileSystemDataSource* ds = (FileSystemDataSource*)(*pos).second;

		_vecDataSource = ds->_vecDataSource;
		_mapDataSource = ds->_mapDataSource;

		_vecDataset = ds->_vecDataset;
		_mapDataset = ds->_mapDataset;

		_connected = ds->IsConnected();
	}
	else
	{
		_connected = false;
	}
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

	fs::path dir(_path);
	fs::directory_iterator end_iter;
	for (fs::directory_iterator dir_itr(dir); dir_itr != end_iter; ++dir_itr)
	{
		//fs::file_status status = fs::detail::status(*dir_itr);
		//fs::perms perm = status.permissions();
		//if (perm&(fs::perms::owner_read | fs::perms::group_read | fs::perms::others_read) == 0)
		//	continue;
		fs::path subdir = (*dir_itr).path();
		if (subdir.filename_is_dot() || subdir.filename_is_dot_dot())
			continue;

		if (fs::is_directory(*dir_itr))
		{
			string path = subdir.string();
			map<string, IDataSource*>::iterator pos = FileSystemDataSourceProvider::INSTANCE._mapDataSource.find(path);

			IDataSource* ds = nullptr;
			if (pos != FileSystemDataSourceProvider::INSTANCE._mapDataSource.end())
			{
				ds = (*pos).second;
				_vecDataSource.push_back(ds);
				map<string, IDataSource*>::value_type v(path, ds);
				_mapDataSource.insert(v);
			}
			else
			{
				ds = new FileSystemDataSource(path.c_str());
				_vecDataSource.push_back(ds);
				map<string, IDataSource*>::value_type v(path, ds);
				_mapDataSource.insert(v);
				FileSystemDataSourceProvider::INSTANCE._mapDataSource.insert(v);
			}			
		}
		else
		{
			if (fs::is_regular_file(*dir_itr) && (*dir_itr).path().has_extension())
			{
				string path = subdir.string();
				string ext = subdir.extension().string().substr(1);
				if (MyGDALFileDataset::IsSupportedRasterFormatExt(ext.c_str()))
				{
					MyGDALRasterDataset* dt = new MyGDALRasterDataset(path.c_str());
					dt->_dataSource = this;
					_vecDataset.push_back(dt);
					_mapDataset.insert(map<string, IDataset*>::value_type(path, dt));
				}
				else if (MyGDALFileDataset::IsSupportedVectorFormatExt(ext.c_str()))
				{
					MyGDALVectorDataset* dt = new MyGDALVectorDataset(path.c_str());
					dt->_dataSource = this;
					_vecDataset.push_back(dt);
					_mapDataset.insert(map<string, IDataset*>::value_type(path, dt));
				}
			}
		}
	}

	_connected = true;
}

void FileSystemDataSource::Disconnect()
{
	_connected = false;
	for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); it++)
	{
		FileSystemDataSourceProvider::INSTANCE._mapDataSource.erase((*it)->GetCreationString());
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

