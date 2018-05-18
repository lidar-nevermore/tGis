#include "DataSourceProviderRepository.h"
#include "IDataSourceProvider.h"
#include "FileSystemDataSourceProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

DataSourceProviderRepository DataSourceProviderRepository::INSTANCE;

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
		if (dsp != &FileSystemDataSourceProvider::INSTANCE)
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
	string catagory = dsp->GetCatagory();
	_vecDataSourceProvider.push_back(dsp);	
	_mapDataSourceProvider.insert(map<string, IDataSourceProvider*>::value_type(catagory, dsp));
}

IDataSourceProvider * DataSourceProviderRepository::GetDataSourceProvider(const char* catagory)
{
	map<string, IDataSourceProvider*>::iterator pos = _mapDataSourceProvider.find(catagory);

	if (pos != _mapDataSourceProvider.end())
		return (*pos).second;
	return nullptr;
}

END_NAME_SPACE(tGis, Core)
