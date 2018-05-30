#include "MyGDALVectorDataset.h"
#include "ogrsf_frmts.h"

#include <string>


using namespace std;


BEGIN_NAME_SPACE(tGis, Core)

const char* const MyGDALVectorDataset::_type = "09DBE03B-7228-49A1-89B5-8970AE3E16C8";


const char * MyGDALVectorDataset::GetType()
{
	return _type;
}

const char * MyGDALVectorDataset::S_GetType()
{
	return _type;
}

MyGDALVectorDataset::MyGDALVectorDataset()
{
}

MyGDALVectorDataset::MyGDALVectorDataset(const char * path, GDALAccess eAccess, bool delayOpen, bool autoClose)
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
		_name = _path.substr(pos+1);
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


MyGDALVectorDataset::~MyGDALVectorDataset()
{
}

void MyGDALVectorDataset::Attach(const char * file, GDALAccess eAccess, bool autoClose)
{
	_eAccess = eAccess;
	_path = file;
	size_t pos = _path.find_last_of(TGIS_PATH_SEPARATOR_CHAR);
	if (pos == _path.npos)
	{
		_name = _path;
	}
	else
	{
		_name = _path.substr(pos + 1);
	}
	GDALDataset *dataset = (GDALDataset*)GDALOpenEx(file, eAccess, nullptr, nullptr, nullptr);
	if (dataset != nullptr)
	{
		Attach(dataset, autoClose);
	}
}

void MyGDALVectorDataset::Attach(GDALDataset * dataset, bool autoClose)
{
	_dataset = dataset;
	_autoClose = autoClose;
	_spatialRef = nullptr;

	_envelope.MinX = 0.0;
	_envelope.MaxX = 0.0;
	_envelope.MinY = 0.0;
	_envelope.MaxY = 0.0;
	int layerCount = _dataset->GetLayerCount();
	for (int i = 0; i < layerCount; i++)
	{
		OGRLayer* layer = _dataset->GetLayer(i);
		OGREnvelope e;
		OGRErr err = layer->GetExtent(&e);
		if (err != OGRERR_FAILURE)
		{
			_envelope.Merge(e);
		}
	}
}



END_NAME_SPACE(tGis, Core)


