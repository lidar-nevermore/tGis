#include "FileSystemDataSource.h"
#include "IDataset.h"
#include "MyGDALFileRasterDataset.h"
#include "MyGDALVectorDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <io.h>

#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const FileSystemDataSource::_type = "9357FB74-8ED4-4666-9D91-8B322208D60A";

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

class FileSystemDataSourceImpl
{
public:
	FileSystemDataSourceImpl(FileSystemDataSource* owner)
	{
		_owner = owner;
	}

	FileSystemDataSource* _owner;
	string _path;
};

FileSystemDataSource::FileSystemDataSource(const char* path)
{
	_impl_ = new FileSystemDataSourceImpl(this);
	_impl_->_path = path;
	string sepstr(TGIS_PATH_SEPARATOR_STR);
	size_t spos = _impl_->_path.size() - sepstr.size();

	if (_impl_->_path.find(sepstr, spos) == spos)
	{
		_impl_->_path = _impl_->_path.substr(0, spos);
	}

	size_t pos = _impl_->_path.find_last_of(TGIS_PATH_SEPARATOR_CHAR);
	if (pos == _impl_->_path.npos)
	{
		SetName(_impl_->_path.c_str());
	}
	else
	{
		SetName(_impl_->_path.substr(pos + 1).c_str());
	}

	_connected = false;
}


FileSystemDataSource::~FileSystemDataSource()
{
	delete _impl_;
}

void FileSystemDataSource::OnTraverseDirConnect(void * usr, const char * dir, const char * name, unsigned int attrib)
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
			IDataSource* ds = new FileSystemDataSource(path);
			fsDataSrc->AddDataSource(ds);
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

void FileSystemDataSource::OnTraverseDirRefresh(void * usr, const char * dir, const char * name, unsigned int attrib)
{
	if ((attrib&_TGIS_A_HIDDEN)
		|| (attrib&_TGIS_A_SYSTEM))
		return;

	FileSystemDataSource* fsDataSrc = (FileSystemDataSource*)usr;

	char path[TGIS_MAX_PATH] = { 0 };
	strcpy(path, dir);
	strcat(path, TGIS_PATH_SEPARATOR_STR);
	strcat(path, name);

	if (attrib&_TGIS_A_SUBDIR)
	{
		bool contain = fsDataSrc->ContainDataSource(name);

		if (false == contain)
		{
			IDataSource* ds = new FileSystemDataSource(path);
			fsDataSrc->AddDataSource(ds);
		}
	}
	else
	{
		size_t pos = _tgis_find_last_of(name, TGIS_EXT_SEPARATOR_STR, 0);
		if (pos != -1)
		{
			bool contain = fsDataSrc->ContainDataset(name);
			if (contain) return;

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

void FileSystemDataSource::Connect()
{
	if (_connected)
		return;

	_connected = true;

	_tgis_traverse_dir(_impl_->_path.c_str(),"*", this, OnTraverseDirConnect);

	DataSource::Connect();
}

void FileSystemDataSource::Refresh()
{
	if (false == _connected)
		return;

	_tgis_traverse_dir(_impl_->_path.c_str(), "*", this, OnTraverseDirRefresh);
}

const char * FileSystemDataSource::GetPath()
{
	return _impl_->_path.c_str();
}


END_NAME_SPACE(tGis, Core)

