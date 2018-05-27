#include "MyGDALVectorDataset.h"
#include "ogrsf_frmts.h"

#include <string>
#include "boost/filesystem.hpp" 
#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/split.hpp"

using namespace std;

namespace fs = boost::filesystem;

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

MyGDALVectorDataset::MyGDALVectorDataset(const char * path, bool delayOpen, GDALAccess eAccess, bool autoClose)
{
	_eAccess = eAccess;
	_path = path;
	fs::path dir(path);
	_name = dir.filename().string();
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
	fs::path dir(file);
	_name = dir.filename().string();
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


