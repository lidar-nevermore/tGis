#include "DataSourceProviderRepository.h"
#include "IDataSourceProvider.h"
#include "FileSystemDataSourceProvider.h"
#include "ObjectSampleDataSourceProvider.h"
#include "MemoryDataSourceProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

DataSourceProviderRepository* DataSourceProviderRepository::_instance = nullptr;


DataSourceProviderRepository & DataSourceProviderRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new DataSourceProviderRepository();
		static PtrDestructor<DataSourceProviderRepository> shit(_instance);
		_instance->AddDataSourceProvider(&FileSystemDataSourceProvider::INSTANCE());
		_instance->AddDataSourceProvider(&MemoryDataSourceProvider::INSTANCE());
	}

	return *_instance;
}

DataSourceProviderRepository::DataSourceProviderRepository()
{

}


DataSourceProviderRepository::~DataSourceProviderRepository()
{
	for (vector<IDataSourceProvider*>::iterator it = _vecDataSourceProvider.begin(); it != _vecDataSourceProvider.end(); it++)
	{
		IDataSourceProvider* dsp = *it;

		//FileSystemDataSourceProvider����ջ�ϣ������˳�ʱ��������DataSourceProviderRepository����
		//�����Ѿ������Ķ���ĳ�Ա�����ǲ��Ե�
		//
		//������������������ж��ᵼ��FileSystemDataSourceProviderһ����DataSourceProviderRepository������
		if (dsp != &FileSystemDataSourceProvider::INSTANCE()
			&& dsp != &ObjectSampleDataSourceProvider::INSTANCE()
			&& dsp != &MemoryDataSourceProvider::INSTANCE())
			dsp->Release();
	}
}

int DataSourceProviderRepository::GetDataSourceProviderCount()
{
	return _vecDataSourceProvider.size();
}

IDataSourceProvider * DataSourceProviderRepository::GetDataSourceProvider(int pos)
{
	return _vecDataSourceProvider.at(pos);
}

void DataSourceProviderRepository::AddDataSourceProvider(IDataSourceProvider* dsp)
{
	string dataSourceType = dsp->GetSupportedDataSourceType();
	_vecDataSourceProvider.insert(_vecDataSourceProvider.begin(), dsp);
	_mapDataSourceProvider.insert(map<string, IDataSourceProvider*>::value_type(dataSourceType, dsp));
}

IDataSourceProvider * DataSourceProviderRepository::GetDataSourceProvider(const char* dataSourceType)
{
	map<string, IDataSourceProvider*>::iterator pos = _mapDataSourceProvider.find(dataSourceType);

	if (pos != _mapDataSourceProvider.end())
		return (*pos).second;
	return nullptr;
}

END_NAME_SPACE(tGis, Core)
