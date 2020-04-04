#include "FileSystemDataSource.h"
#include "IDataset.h"
#include "MyGDALFileRasterDataset.h"
#include "MyGDALVectorDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <io.h>


BEGIN_NAME_SPACE(tGis, Core)


const char* const FileSystemDataSource::_type = "9357FB74-8ED4-4666-9D91-8B322208D60A";

FileSystemDataSource::FileSystemDataSource(const char* path)
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
			IDataSource* ds = new FileSystemDataSource(path);
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


END_NAME_SPACE(tGis, Core)

