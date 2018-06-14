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

ObjectSampleDataSourceProvider::ObjectSampleDataSourceProvider()
{
}


ObjectSampleDataSourceProvider::~ObjectSampleDataSourceProvider()
{
}

const char * ObjectSampleDataSourceProvider::GetSupportedDataSourceType()
{
	return ObjectSampleDataSource::S_GetType();
}

bool ObjectSampleDataSourceProvider::IsRoot()
{
	return false;
}

const char * ObjectSampleDataSourceProvider::GetName()
{
	return _name;
}

const char * ObjectSampleDataSourceProvider::GetType()
{
	return _type;
}

void ObjectSampleDataSourceProvider::SetCreationUI(const CreationUI ui)
{
	_uiCreation = ui;
}

IDataSource * ObjectSampleDataSourceProvider::UI_CreateDataSource()
{
	assert(_uiCreation != nullptr);

	return _uiCreation(this);
}

void ObjectSampleDataSourceProvider::SetPropertyUI(const PropertyUI ui)
{
	_uiProperty = ui;
}

void ObjectSampleDataSourceProvider::UI_DataSourceProperty(IDataSource *ds, IDataset *dt)
{
	assert(_uiProperty != nullptr);

	_uiProperty(this, ds, dt);
}

IDataSource * ObjectSampleDataSourceProvider::CreateDataSourceNoHost(const char * path)
{
	string strPath(path);
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(strPath);

	if (pos != _mapDataSource.end())
	{
		IDataSource *ds = (*pos).second;
		const char* t = ds->GetType();
		if (strcmp(t, ObjectSampleDataSource::S_GetType()) != 0)
		{
			throw exception("Already connected as DataSource of different Type!");
		}
		return ds;
	}

	ObjectSampleDataSource* ds = new ObjectSampleDataSource(path);
	_mapDataSource.insert(map<string, IDataSource*>::value_type(strPath, ds));

	return ds;
}

IDataSource * ObjectSampleDataSourceProvider::CreateDataSource(const char * path)
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
	if (exist == false)
		_vecDataSource.push_back(ds);
	return ds;
}

END_NAME_SPACE(tGis, Core)

