#include "Dataset.h"
#include "DataSource.h"

BEGIN_NAME_SPACE(tGis, Core)

Dataset::Dataset(IDataSource * ds)
{
	_dataSource = ds;
	if (_dataSource != nullptr)
		((DataSource*)_dataSource)->_vecDataset.push_back(this);
}


Dataset::~Dataset()
{
}

void Dataset::Open()
{
	if(_dataSource != nullptr)
	    ((DataSource*)_dataSource)->AddOpenedDataset(this);
}

void Dataset::Close()
{
	if (_dataSource != nullptr)
	    ((DataSource*)_dataSource)->RemoveOpenedDataset(this);
}


END_NAME_SPACE(tGis, Core)


