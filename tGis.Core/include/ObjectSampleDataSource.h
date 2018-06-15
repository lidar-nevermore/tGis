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

//Ŀ¼�����ļ�tgis.sample��˵�����Ŀ¼��Ŀ��ʶ������������Դ
//tgis.sample ��һ��utf8��bom�ı��ļ���һ��һ����𣬻��з���\n
//һ���ÿո�ָ�����ֶ� �ֱ��� ����� ��ǩֵ ��С��Ԫ�ֱ���(��) �����Ԫ�ֱ���(��) ��С���(��) �����(��) ��С�߶�(��) ���߶�(��)
//Ԫ������� ��С���(��) �����(��) ��С�߶�(��) ���߶�(��) ����������Ч����Ĵ�С
//�洢������ͼƬ��С��ʵ��������Ч����Ŀ�����򣩶Խ��ߵĳ��ȣ�Ŀ����Ϊ�˶�����������ת����
//����ͼƬ��������ʽΪ ��Ч����_��Ч����_��������.tif
//��Ч����һ��������ͼƬ������
//��������������洢���������ͼƬ��Ŀ¼��
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


