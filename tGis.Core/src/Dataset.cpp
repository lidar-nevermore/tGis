#include "Dataset.h"
#include "DataSource.h"

BEGIN_NAME_SPACE(tGis, Core)


Dataset::Dataset()
{
}

Dataset::Dataset(IDataSource * ds)
{
	_dataSource = ds;
}


Dataset::~Dataset()
{
}

void Dataset::Open()
{
	((DataSource*)_dataSource)->AddOpenedDataset(this);
}

void Dataset::Close()
{
	((DataSource*)_dataSource)->RemoveOpenedDataset(this);
}


END_NAME_SPACE(tGis, Core)


