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

bool MyGDALVectorDataset::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

const char * MyGDALVectorDataset::S_GetType()
{
	return _type;
}

MyGDALVectorDataset::MyGDALVectorDataset()
	:MyGDALDataset()
{
}

MyGDALVectorDataset::MyGDALVectorDataset(DataSource* ds, const char * path, GDALAccess eAccess, bool delayOpen, bool autoClose)
	:MyGDALDataset((DataSource*)ds, path, eAccess, delayOpen, autoClose)
{
	if (_dataset != nullptr)
	{
		try
		{
			Attach(_dataset, _autoClose);
		}
		catch (...)
		{
			Detach();
			throw;
		}
	}
	if (_dataset != nullptr)
	{
		Dataset::Open();
	}
}


MyGDALVectorDataset::~MyGDALVectorDataset()
{
}


void MyGDALVectorDataset::Open()
{
	MyGDALDataset::Attach();
	if (_dataset != nullptr)
	{
		try
		{
			Attach(_dataset, _autoClose);
		}
		catch (...)
		{
			Detach();
			throw;
		}
	}
	if (_dataset != nullptr)
	{
		Dataset::Open();
	}
}

void MyGDALVectorDataset::Close()
{
	Dataset::Close();
	Detach();
}

void MyGDALVectorDataset::Attach(const char * file, GDALAccess eAccess, bool autoClose)
{
	MyGDALDataset::Attach(file, eAccess, autoClose);
	if (_dataset != nullptr)
	{
		try 
		{
			Attach(_dataset, autoClose);
		}
		catch (...)
		{
			Detach();
			throw;
		}
	}	
}

void MyGDALVectorDataset::Attach(GDALDataset * dataset, bool autoClose)
{
	int layerCount = _dataset->GetLayerCount();
	if(layerCount == 0)
		throw std::exception("不是有效的矢量数据集！");

	_dataset = dataset;
	_autoClose = autoClose;
	_spatialRef = nullptr;

	_envelope.MinX = 0.0;
	_envelope.MaxX = 0.0;
	_envelope.MinY = 0.0;
	_envelope.MaxY = 0.0;
	
	//TODO: 如果矢量数据集的不同图层有不同的投影 这里就是错的
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


