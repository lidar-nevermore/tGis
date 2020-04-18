#include "Dataset.h"
#include "DataSource.h"
#include "DataSourceRepository.h"

BEGIN_NAME_SPACE(tGis, Core)

Dataset::Dataset(DataSource * ds)
{
	_parent = ds;
}


Dataset::~Dataset()
{
}

void Dataset::Open()
{
	if(_opened)
		return;

	_opened = true;

	DataSourceRepository::INSTANCE().AddOpenedDataset(this);
}

bool Dataset::IsOpened()
{
	return _opened;
}

void Dataset::Close()
{
	if (_opened == false)
		return;

	_opened = false;

	DataSourceRepository::INSTANCE().RemoveOpenedDataset(this);
}

IDataSource * Dataset::GetParent()
{
	return _parent;
}


END_NAME_SPACE(tGis, Core)


