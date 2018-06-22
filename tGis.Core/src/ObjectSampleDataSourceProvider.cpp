#include "ObjectSampleDataSourceProvider.h"
#include "ObjectSampleDataSource.h"


#include <cassert>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const ObjectSampleDataSourceProvider::_name = "ObjectSample";
const char* const ObjectSampleDataSourceProvider::_type = "230160E1-A1D5-4D41-B3A3-1595BDF0147D";

ObjectSampleDataSourceProvider* ObjectSampleDataSourceProvider::_instance = nullptr;

static PtrDestructor<ObjectSampleDataSourceProvider> shit(ObjectSampleDataSourceProvider::_instance);

ObjectSampleDataSourceProvider & ObjectSampleDataSourceProvider::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new ObjectSampleDataSourceProvider();
	}

	return *_instance;
}

ObjectSampleDataSourceProvider::ObjectSampleDataSourceProvider()
{
}

ObjectSampleDataSourceProvider::~ObjectSampleDataSourceProvider()
{
}


bool ObjectSampleDataSourceProvider::IsObjectSampleDataSource(const char * path_)
{
	string path = path_;
	string sepstr(TGIS_PATH_SEPARATOR_STR);
	size_t spos = path.size() - sepstr.size();

	if (path.find(sepstr, spos) != spos)
	{
		path.append(TGIS_PATH_SEPARATOR_STR);
	}
	path.append("tgis.sample");
	if (_tgis_access(path.c_str(), _TGIS_A_READ_WRITE) == _TGIS_R_OK)
	{
		return true;
	}
	return false;
}

const char * ObjectSampleDataSourceProvider::GetSupportedDataSourceType()
{
	return ObjectSampleDataSource::S_GetType();
}

const char * ObjectSampleDataSourceProvider::GetName()
{
	return _name;
}

const char * ObjectSampleDataSourceProvider::GetType()
{
	return _type;
}

bool ObjectSampleDataSourceProvider::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return FileSystemDataSourceProvider::IsTypeOf(type);
}

bool ObjectSampleDataSourceProvider::IsTypeOf(ITGisObject * object)
{
	if (strcmp(object->GetType(), _type) == 0)
		return true;
	return FileSystemDataSourceProvider::IsTypeOf(object);
}

IDataSource * ObjectSampleDataSourceProvider::CreateDataSource(const char * path)
{
	IDataSource *ds = nullptr;
	string strPath(path);
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(strPath);

	if (pos != _mapDataSource.end())
	{
		ds = (*pos).second;
		if (ds->IsTypeOf(ObjectSampleDataSource::S_GetType()))
		{
			throw exception("Already connected as DataSource of different Type!");
		}
		return ds;
	}

	ds = DataSourceProvider::CreateDataSource(path);
	if (ds != nullptr)
		return ds;

	if (IsObjectSampleDataSource(path))
	{
		ds = new ObjectSampleDataSource(path);
		_mapDataSource.insert(map<string, IDataSource*>::value_type(strPath, ds));
	}

	return ds;
}

END_NAME_SPACE(tGis, Core)

