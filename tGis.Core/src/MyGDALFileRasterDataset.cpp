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
		MyGDALDataset::Open();
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
				if (i % 2 != 0)
					continue;

				if (i == 2 * subdataset)
				{
					std::string tmpstr = std::string(papszSUBDATASETS[i]);
					_path = tmpstr.substr(tmpstr.find_first_of("=") + 1);

					char subset[32] = { 0 };
					strcpy(subset, ":SUBDATASET_");
					_itoa(subdataset, subset + 12, 10);
					string file_path(file);
					size_t pos = file_path.find_last_of(TGIS_PATH_SEPARATOR_CHAR);
					if (pos == _path.npos)
					{
						_name = file_path + subset;
					}
					else
					{
						_name = file_path.substr(pos + 1) + subset;
					}


					GDALDataset *dataset = (GDALDataset*)GDALOpen(_path.c_str(), eAccess);
					Attach(dataset, autoClose,noDataValue);
					break;
				}
			}//end for  
		}
	}

	GDALClose(pd);
}


END_NAME_SPACE(tGis, Core)

