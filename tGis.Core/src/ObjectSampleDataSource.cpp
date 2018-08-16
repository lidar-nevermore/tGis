#include "ObjectSampleDataSource.h"
#include "ObjectSampleDataSourceProvider.h"
#include "MyGDALRasterDataset.h"
#include <io.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

BEGIN_NAME_SPACE(tGis, Core)

const char* const ObjectSampleDataSource::_type = "B97F1F35-B223-48EB-8AAA-4BF7D448ABF5";


ObjectSampleDataSource::ObjectSampleDataSource(const char* path_)
	:FileSystemDataSource(path_, &ObjectSampleDataSourceProvider::INSTANCE())
	, _mapObjectSampleMetadata()
	, _vecObjectSampleMetadata()
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


bool ObjectSampleDataSource::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return FileSystemDataSource::IsTypeOf(type);
}

bool ObjectSampleDataSource::IsTypeOf(ITGisObject * object)
{
	if (strcmp(object->GetType(), _type) == 0)
		return true;
	return FileSystemDataSource::IsTypeOf(object);;
}

void ObjectSampleDataSource::Connect()
{
	if (_connected)
		return;

	const size_t len = 524288; // 512k
	char buffer[len] = { 0 };
	string path = _path;
	path.append(TGIS_PATH_SEPARATOR_STR);
	path.append("tgis.sample");

	std::ifstream file;
	file.open(path.c_str(), ios::binary);

	try
	{
		size_t readSize = 0;
		size_t remainSize = 0;
		do
		{
			file.read(buffer + remainSize, len);
			readSize = file.gcount();
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
						size_t nameLen = fileds[0].length();
						if (nameLen > ObjectSampleNameLength)
							nameLen = ObjectSampleNameLength;
						memcpy(meta.Name, fileds[0].data(), nameLen);
						meta.Label = std::stoi(fileds[1]);
						meta.MinPixelSize = std::stof(fileds[2]);
						meta.MaxPixelSize = std::stof(fileds[3]);
						meta.MinObjectWidth = std::stof(fileds[4]);
						meta.MaxObjectWidth = std::stof(fileds[5]);
						meta.MinObjectHeight = std::stof(fileds[6]);
						meta.MaxObjectHeight = std::stof(fileds[7]);
						AddObjectSampleMetadata(&meta);
					}

					line = buffer + i + 1;
					remainSize = totalSize - i - 1;
				}
			}
			if (remainSize > 0 && remainSize < totalSize)
			{
				memcpy(buffer, buffer + (totalSize - remainSize), remainSize);
			}
		} while (readSize == len);
	}
	catch (exception &ex)
	{
		file.close();
		throw ex;
	}
	file.close();

	FileSystemDataSource::Connect();
}

void ObjectSampleDataSource::Disconnect()
{
	if (_connected)
	{
		for (vector<ObjectSampleMetadata*>::iterator it = _vecObjectSampleMetadata.begin(); it != _vecObjectSampleMetadata.end(); it++)
		{
			delete (*it);
		}
		_vecObjectSampleMetadata.clear();
		_mapObjectSampleMetadata.clear();
		FileSystemDataSource::Disconnect();
	}
}

size_t ObjectSampleDataSource::GetObjectSampleMetadataCount()
{
	return _vecObjectSampleMetadata.size();
}

ObjectSampleMetadata * ObjectSampleDataSource::GetObjectSampleMetadata(size_t pos)
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

void ObjectSampleDataSource::SaveObjectSampleMetadata()
{
	char buffer[20];
	string path = _path;
	path.append(TGIS_PATH_SEPARATOR_STR);
	path.append("tgis.sample");

	FILE *fp = fopen(path.c_str(), "wb");
	if (fp == NULL)
	{
		throw exception("样本库元数据文件写入失败！");
	}
	for (vector<ObjectSampleMetadata*>::iterator it = _vecObjectSampleMetadata.begin(); it != _vecObjectSampleMetadata.end(); it++)
	{
		ObjectSampleMetadata* meta = *it;
		size_t len = strlen(meta->Name);
		fwrite(meta->Name, len, 1, fp);
		fwrite("   ", 3, 1, fp);

		sprintf(buffer, "%d", meta->Label);
		len = strlen(buffer);
		fwrite(buffer, len, 1, fp);
		fwrite("   ", 3, 1, fp);

		sprintf(buffer, "%.3f", meta->MinPixelSize);
		len = strlen(buffer);
		fwrite(buffer, len, 1, fp);
		fwrite("   ", 3, 1, fp);

		sprintf(buffer, "%.3f", meta->MaxPixelSize);
		len = strlen(buffer);
		fwrite(buffer, len, 1, fp);
		fwrite("   ", 3, 1, fp);

		sprintf(buffer, "%.3f", meta->MinObjectWidth);
		len = strlen(buffer);
		fwrite(buffer, len, 1, fp);
		fwrite("   ", 3, 1, fp);

		sprintf(buffer, "%.3f", meta->MaxObjectWidth);
		len = strlen(buffer);
		fwrite(buffer, len, 1, fp);
		fwrite("   ", 3, 1, fp);

		sprintf(buffer, "%.3f", meta->MinObjectHeight);
		len = strlen(buffer);
		fwrite(buffer, len, 1, fp);
		fwrite("   ", 3, 1, fp);

		sprintf(buffer, "%.3f", meta->MaxObjectHeight);
		len = strlen(buffer);
		fwrite(buffer, len, 1, fp);
		fwrite("  \n", 3, 1, fp);
	}

	::fclose(fp);
}

END_NAME_SPACE(tGis, Core)
