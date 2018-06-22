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

void MyGDALMemRasterDataset::Open()
{
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

END_NAME_SPACE(tGis, Core)

