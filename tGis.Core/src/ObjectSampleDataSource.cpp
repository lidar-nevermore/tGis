#include "ObjectSampleDataSource.h"
#include "MyGDALRasterDataset.h"
#include <io.h>
#include <stdio.h>

BEGIN_NAME_SPACE(tGis, Core)

const char* const ObjectSampleDataSource::_type = "B97F1F35-B223-48EB-8AAA-4BF7D448ABF5";


ObjectSampleDataSource::ObjectSampleDataSource(const char* path_)
	:FileSystemDataSource(path_)
{
}


ObjectSampleDataSource::~ObjectSampleDataSource()
{
	Disconnect();
}

const char * ObjectSampleDataSource::GetType()
{
	return _type;
}

const char * ObjectSampleDataSource::S_GetType()
{
	return _type;
}

void ObjectSampleDataSource::Connect()
{
	const int len = 524288; // 512k
	char buffer[len];
	string path = _path;
	path.append(TGIS_PATH_SEPARATOR_STR);
	path.append("tgis.sample");

	FILE *fp = fopen(path.c_str(), "rb");
	if (fp == NULL)
	{
		throw exception("样本库元数据文件打开失败！");
	}
	size_t readSize = 0;
	size_t remainSize = 0;
	while (readSize = fread(buffer + remainSize, len, 1, fp) > 0)
	{
		char* line = buffer;
		size_t totalSize = readSize + remainSize;
		remainSize = totalSize;
		for (size_t i = 0; i < totalSize; i++)
		{
			if (buffer[i] == '\n')
			{
				buffer[i] = '\0';
				vector<string> fileds;
				_tgis_str_split(line, " ", fileds);
				if (fileds.size() > 7)
				{
					ObjectSampleMetadata meta;
					memset(&meta, 0, sizeof(ObjectSampleMetadata));
					int nameLen = fileds[0].length();
					if (nameLen > ObjectSampleNameLength)
						nameLen = ObjectSampleNameLength;
					memcpy(meta.Name, fileds[0].data(), nameLen);
					meta.Label = std::stoi(fileds[1]);
					meta.MinPixelSize = std::stof(fileds[2]);
					meta.MaxPixelSize = std::stof(fileds[3]);
					meta.MinImageWidth = std::stof(fileds[4]);
					meta.MaxImageWidth = std::stof(fileds[5]);
					meta.MinImageHeight = std::stof(fileds[6]);
					meta.MaxImageHeight = std::stof(fileds[7]);
				}

				line = buffer + i + 1;
				remainSize = totalSize - i - 1;
			}
		}
		if (remainSize > 0 && remainSize < totalSize)
		{
			memcpy(buffer, buffer + (totalSize - remainSize), remainSize);
		}
	}

	FileSystemDataSource::Connect();
}

void ObjectSampleDataSource::Disconnect()
{
	for (vector<ObjectSampleMetadata*>::iterator it = _vecObjectSampleMetadata.begin(); it != _vecObjectSampleMetadata.end(); it++)
	{
		delete (*it);
	}
	_vecObjectSampleMetadata.clear();
	_mapObjectSampleMetadata.clear();
	FileSystemDataSource::Disconnect();
}

int ObjectSampleDataSource::GetObjectSampleMetadataCount()
{
	return _vecObjectSampleMetadata.size();
}

ObjectSampleMetadata * ObjectSampleDataSource::GetObjectSampleMetadata(int pos)
{
	return _vecObjectSampleMetadata.at(pos);
}

ObjectSampleMetadata * ObjectSampleDataSource::GetObjectSampleMetadataByLabel(int label)
{
	map<int, ObjectSampleMetadata*>::iterator pos = _mapObjectSampleMetadata.find(label);

	if (pos != _mapObjectSampleMetadata.end())
		return (*pos).second;
	return nullptr;
}

ObjectSampleMetadata* ObjectSampleDataSource::AddObjectSampleMetadata(ObjectSampleMetadata * meta_)
{
	map<int, ObjectSampleMetadata*>::iterator pos = _mapObjectSampleMetadata.find(meta_->Label);

	if (pos != _mapObjectSampleMetadata.end())
		throw exception("Already exists!");

	ObjectSampleMetadata* meta = new ObjectSampleMetadata;
	memcpy(meta, meta_, sizeof(ObjectSampleMetadata));
	_vecObjectSampleMetadata.push_back(meta);
	_mapObjectSampleMetadata.insert(map<int, ObjectSampleMetadata*>::value_type(meta->Label, meta));

	return meta;
}

END_NAME_SPACE(tGis, Core)
