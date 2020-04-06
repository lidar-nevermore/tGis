#include "Dataset.h"
#include "DataSource.h"
#include "DataSourceRepository.h"

BEGIN_NAME_SPACE(tGis, Core)

Dataset::Dataset(DataSource * ds)
{
	_parent = ds;
	if (_parent != nullptr)
		_parent->AddDataset(this);
}


Dataset::~Dataset()
{
}

void Dataset::Open()
{
	if(_open)
		return;

	DataSourceRepository::INSTANCE().AddOpenedDataset(this);
}

void Dataset::Close()
{
	if (_open == false)
		return;

	DataSourceRepository::INSTANCE().RemoveOpenedDataset(this);
}

IDataSource * Dataset::GetParent()
{
	return _parent;
}


END_NAME_SPACE(tGis, Core)


