#pragma once

#include "FileSystemDataSource.h"

#include "Helper.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class MyGDALRasterDataset;

#define ObjectSampleNameLength 128
typedef struct _ObjectSampleMetadata
{
	char Name[ObjectSampleNameLength+8];
	int Label;
	float MinPixelSize;
	float MaxPixelSize;
	float MinObjectWidth;
	float MaxObjectWidth;
	float MinObjectHeight;
	float MaxObjectHeight;
}ObjectSampleMetadata;

//目录下有文件tgis.sample，说明这个目录是目标识别样本库数据源
//tgis.sample 是一个utf8无bom文本文件，一行一个类别，换行符是\n
//一行用空格分隔多个字段 分别是 类别名 标签值 最小像元分辨率(米) 最大像元分辨率(米) 最小宽度(米) 最大宽度(米) 最小高度(米) 最大高度(米)
//元数据里的 最小宽度(米) 最大宽度(米) 最小高度(米) 最大高度(米) 都是样本有效区域的大小
//存储的样本图片大小是实际样本有效区（目标区域）对角线的长度，目的是为了对样本进行旋转重用
//样本图片的命名方式为 有效区宽_有效区高_其它部分.tif
//有效区域一定在样本图片的中央
//类别名就是用来存储此类别样本图片的目录名
class TGIS_API ObjectSampleDataSource : public FileSystemDataSource
{
	friend class ObjectSampleDataSourceProvider;
protected:
	ObjectSampleDataSource(const char* path);

public:
	~ObjectSampleDataSource();

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	static const char* S_GetType();

	virtual void Connect();
	virtual void Disconnect();

	int GetObjectSampleMetadataCount();
	ObjectSampleMetadata* GetObjectSampleMetadata(int pos);
	ObjectSampleMetadata* GetObjectSampleMetadataByLabel(int label);
	ObjectSampleMetadata* AddObjectSampleMetadata(ObjectSampleMetadata* meta);
	void SaveObjectSampleMetadata();

private:
	vector<ObjectSampleMetadata*> _vecObjectSampleMetadata;
	map<int, ObjectSampleMetadata*> _mapObjectSampleMetadata;
};


END_NAME_SPACE(tGis, Core)


