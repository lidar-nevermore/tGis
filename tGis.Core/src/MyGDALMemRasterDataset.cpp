#include "MyGDALMemRasterDataset.h"



BEGIN_NAME_SPACE(tGis, Core)

const char* const MyGDALMemRasterDataset::_type = "A251D25A-292C-4B7C-A5F1-616A585A5C13";


const char * MyGDALMemRasterDataset::GetType()
{
	return _type;
}

const char * MyGDALMemRasterDataset::S_GetType()
{
	return _type;
}

bool MyGDALMemRasterDataset::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return MyGDALRasterDataset::IsTypeOf(type);
}


MyGDALMemRasterDataset::MyGDALMemRasterDataset(IDataSource* ds, const char* name)
	:MyGDALRasterDataset(ds)
{
	if(name != nullptr)
		_name = name;
	_name.append(".mem");
}


MyGDALMemRasterDataset::~MyGDALMemRasterDataset()
{
}

void MyGDALMemRasterDataset::Open()
{
}

void MyGDALMemRasterDataset::Attach(GDALDataset * dataset, bool autoClose, double noDataValue)
{
	const char* drName = dataset->GetDriverName();
	if (memcmp("MEM", drName, 3) == 0)
	{
		MyGDALRasterDataset::Attach(dataset, autoClose, noDataValue);
	}
	else
	{
		throw std::exception("MyGDALMemRasterDataset中不可以附加非内存数据集！！");
	}
}

END_NAME_SPACE(tGis, Core)

