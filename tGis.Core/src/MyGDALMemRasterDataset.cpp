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


//调用MyGDALRasterDataset的构造函数时delayOpen必须传入true
//为得是避免在子类中将name当作文件存储路径打开
MyGDALMemRasterDataset::MyGDALMemRasterDataset(DataSource* ds,
	MyGDALRasterDataset* raster,
	int left, int top, int width, int height,
	int *bandIndexs, int bandCount,
	const char* name)
	:MyGDALRasterDataset(ds,name,GA_Update,true,true)
{
	//TODO: 从raster中指定区域创建内存数据集并为_dataset赋值

	
	Dataset::Open();
}

MyGDALMemRasterDataset::~MyGDALMemRasterDataset()
{
}

END_NAME_SPACE(tGis, Core)

