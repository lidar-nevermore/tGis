#include "MyGDALFileRasterDataset.h"

#include <algorithm>
#include <string>
#include <vector>


using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const MyGDALFileRasterDataset::_type = "CCD78B6B-39DC-42A1-824E-F3C1E14AAC36";


const char * MyGDALFileRasterDataset::GetType()
{
	return _type;
}

const char * MyGDALFileRasterDataset::S_GetType()
{
	return _type;
}

bool MyGDALFileRasterDataset::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return MyGDALRasterDataset::IsTypeOf(type);
}

bool MyGDALFileRasterDataset::IsTypeOf(ITGisObject * object)
{
	if (strcmp(object->GetType(), _type) == 0)
		return true;
	return MyGDALRasterDataset::IsTypeOf(object);
}

MyGDALFileRasterDataset::MyGDALFileRasterDataset()
	:MyGDALRasterDataset(nullptr)
{
}

MyGDALFileRasterDataset::MyGDALFileRasterDataset(IDataSource* ds, const char* path, GDALAccess eAccess, bool delayOpen, bool autoClose)
	:MyGDALRasterDataset(ds)
{
	_eAccess = eAccess;
	_path = path;
	size_t pos = _path.find_last_of(TGIS_PATH_SEPARATOR_CHAR);
	if (pos == _path.npos)
	{
		_name = _path;
	}
	else
	{
		_name = _path.substr(pos + 1);
	}
	if (delayOpen)
	{
		_dataset = nullptr;
		_autoClose = true;
	}
	else
	{
		Attach(path, eAccess, autoClose);
	}
}

MyGDALFileRasterDataset::~MyGDALFileRasterDataset()
{
}

void MyGDALFileRasterDataset::Open()
{
	if (_dataset == nullptr)
	{
		Attach(_path.c_str(), _eAccess);
	}
	if (_dataset != nullptr)
	{
		MyGDALRasterDataset::Open();
	}
}

void MyGDALFileRasterDataset::Attach(const char* file, GDALAccess eAccess, bool autoClose, double noDataValue)
{
	MyGDALDataset::Attach(file, eAccess, autoClose);
	MyGDALRasterDataset::Attach(_dataset, autoClose, noDataValue);
}

void MyGDALFileRasterDataset::AttachHDF(const char* file, GDALAccess eAccess, const int subdataset, bool autoClose, double noDataValue)
{
	_eAccess = eAccess;
	GDALDataset* pd = (GDALDataset*)GDALOpen(file, GA_ReadOnly);

	if (pd == NULL)
		return;

	char ** papszSUBDATASETS = GDALGetMetadata((GDALDatasetH)pd, "SUBDATASETS");

	if (papszSUBDATASETS != NULL)
	{
		int iCount = CSLCount(papszSUBDATASETS);
		if (iCount > 0)
		{
			for (int i = 0; papszSUBDATASETS[i] != NULL; i++)
			{
				std::string tmpstr = std::string(papszSUBDATASETS[i]);
				size_t espos = tmpstr.find_first_of("=");
				std::string subitem = tmpstr.substr(0, espos);
				transform(subitem.begin(), subitem.end(), subitem.begin(), ::toupper);

				if (subitem.rfind("NAME") == espos - 4)
				{
					size_t uspos = subitem.find_last_of("_");
					//SUBDATASET_num_NAME
					std::string num = subitem.substr(11, uspos - 11);
					int inum = atoi(num.c_str());

					if (inum == subdataset)
					{
						std::string subname = tmpstr.substr(tmpstr.find_first_of("=") + 1);
						size_t infpos = tmpstr.rfind("\":");
						std::string inf = tmpstr.substr(infpos + 2);
						GDALDataset *dataset = (GDALDataset*)GDALOpen(subname.c_str(), eAccess);
						Attach(dataset, autoClose);
					}
				}
			}//end for  
		}
	}

	GDALClose(pd);
}


END_NAME_SPACE(tGis, Core)

